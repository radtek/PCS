/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYCheckableModelHelper.cpp
 *
 * @example
 *
 * @brief
 *
 * @date 2018-04-01 @author weiheng             @note create this file
 * *****************************************************************************
 */

#include "XYCheckableModelHelper.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QPointer>
#include <QStandardItemModel>

class XYCheckableModelHelperPrivate
{
    Q_DECLARE_PUBLIC(XYCheckableModelHelper)

protected:
    XYCheckableModelHelper *const q_ptr;

public:
    XYCheckableModelHelperPrivate(XYCheckableModelHelper &object);
    ~XYCheckableModelHelperPrivate();

    void init();

    void setIndexCheckState(const QModelIndex &index, Qt::CheckState checkState);

    int indexDepth(const QModelIndex &modelIndex) const;

    void updateCheckState(const QModelIndex &modelIndex);

    void propagateCheckStateToChildren(const QModelIndex &modelIndex);

    Qt::CheckState checkState(const QModelIndex &index, bool *checkable) const;
    void setCheckState(const QModelIndex &index, Qt::CheckState newCheckState);

    void forceCheckability(const QModelIndex &index);

    QPointer<QAbstractItemModel> Model;
    QModelIndex RootIndex;
    Qt::Orientation Orientation;
    bool HeaderIsUpdating;
    bool ItemsAreUpdating;
    bool ForceCheckability;
    int PropagateDepth;
    Qt::CheckState DefaultCheckState;
};

XYCheckableModelHelperPrivate::XYCheckableModelHelperPrivate(XYCheckableModelHelper &object)
    : q_ptr(&object)
{
    this->HeaderIsUpdating = false;
    this->ItemsAreUpdating = false;
    this->ForceCheckability = false;
    this->PropagateDepth = -1;
    this->DefaultCheckState = Qt::Unchecked;
}

XYCheckableModelHelperPrivate::~XYCheckableModelHelperPrivate()
{
}

void XYCheckableModelHelperPrivate::init()
{
}

Qt::CheckState XYCheckableModelHelperPrivate::checkState(
    const QModelIndex &index, bool *checkable) const
{
    Q_Q(const XYCheckableModelHelper);
    if (!q->model())
    {
        qWarning() << "Model has not been set.";
        return q->defaultCheckState();
    }
    QVariant indexCheckState = index != q->rootIndex() ? q->model()->data(index, Qt::CheckStateRole) : q->model()->headerData(0, q->orientation(), Qt::CheckStateRole);
    return static_cast<Qt::CheckState>(indexCheckState.toInt(checkable));
}

void XYCheckableModelHelperPrivate::setCheckState(
    const QModelIndex &modelIndex, Qt::CheckState newCheckState)
{
    Q_Q(XYCheckableModelHelper);
    if (!q->model())
    {
        qWarning() << "Model has not been set.";
        return;
    }
    else if (modelIndex != q->rootIndex())
    {
        q->model()->setData(modelIndex, static_cast<int>(newCheckState),
                            Qt::CheckStateRole);
    }
    else
    {
        q->model()->setHeaderData(0, q->orientation(), static_cast<int>(newCheckState),
                                  Qt::CheckStateRole);
    }
}

void XYCheckableModelHelperPrivate::setIndexCheckState(
    const QModelIndex &index, Qt::CheckState checkState)
{
    bool checkable = false;
    this->checkState(index, &checkable);
    if (!checkable && !this->ForceCheckability)
    {
        return;
    }
    this->setCheckState(index, checkState);
    this->propagateCheckStateToChildren(index);
}

int XYCheckableModelHelperPrivate::indexDepth(const QModelIndex &modelIndex) const
{
    int depth = -1;
    QModelIndex parentIndex = modelIndex;
    while (parentIndex.isValid())
    {
        ++depth;
        parentIndex = parentIndex.parent();
    }
    return depth;
}

void XYCheckableModelHelperPrivate ::updateCheckState(const QModelIndex &modelIndex)
{
    Q_Q(XYCheckableModelHelper);
    bool checkable = false;
    int oldCheckState = this->checkState(modelIndex, &checkable);
    if (!checkable)
    {
        return;
    }

    Qt::CheckState newCheckState = Qt::PartiallyChecked;
    bool firstCheckableChild = true;
    const int rowCount = q->orientation() == Qt::Horizontal ? q->model()->rowCount(modelIndex) : 1;
    const int columnCount = q->orientation() == Qt::Vertical ? q->model()->columnCount(modelIndex) : 1;
    for (int r = 0; r < rowCount; ++r)
    {
        for (int c = 0; c < columnCount; ++c)
        {
            QModelIndex child = q->model()->index(r, c, modelIndex);
            QVariant childCheckState = q->model()->data(child, Qt::CheckStateRole);
            int childState = childCheckState.toInt(&checkable);
            if (!checkable)
            {
                continue;
            }
            if (firstCheckableChild)
            {
                newCheckState = static_cast<Qt::CheckState>(childState);
                firstCheckableChild = false;
            }
            if (newCheckState != childState)
            {
                newCheckState = Qt::PartiallyChecked;
            }
            if (newCheckState == Qt::PartiallyChecked)
            {
                break;
            }
        }
        if (!firstCheckableChild && newCheckState == Qt::PartiallyChecked)
        {
            break;
        }
    }
    if (oldCheckState == newCheckState)
    {
        return;
    }
    this->setCheckState(modelIndex, newCheckState);
    if (modelIndex != q->rootIndex())
    {
        this->updateCheckState(modelIndex.parent());
    }
}

void XYCheckableModelHelperPrivate ::propagateCheckStateToChildren(const QModelIndex &modelIndex)
{
    Q_Q(XYCheckableModelHelper);
    int indexDepth = this->indexDepth(modelIndex);
    if (this->PropagateDepth == 0 ||
        !(indexDepth < this->PropagateDepth || this->PropagateDepth == -1))
    {
        return;
    }

    bool checkable = false;
    Qt::CheckState checkState = this->checkState(modelIndex, &checkable);
    if (!checkable || checkState == Qt::PartiallyChecked)
    {
        return;
    }

    while (this->ForceCheckability && q->model()->canFetchMore(modelIndex))
    {
        q->model()->fetchMore(modelIndex);
    }

    const int rowCount = q->orientation() == Qt::Horizontal ? q->model()->rowCount(modelIndex) : 1;
    const int columnCount = q->orientation() == Qt::Vertical ? q->model()->columnCount(modelIndex) : 1;
    for (int r = 0; r < rowCount; ++r)
    {
        for (int c = 0; c < columnCount; ++c)
        {
            QModelIndex child = q->model()->index(r, c, modelIndex);
            this->setIndexCheckState(child, checkState);
        }
    }
}

void XYCheckableModelHelperPrivate ::forceCheckability(const QModelIndex &modelIndex)
{
    Q_Q(XYCheckableModelHelper);
    if (!this->ForceCheckability)
    {
        return;
    }
    this->setCheckState(modelIndex, this->DefaultCheckState);
    if (qobject_cast<QStandardItemModel *>(q->model()))
    {
        QStandardItem *item = modelIndex != q->rootIndex() ? qobject_cast<QStandardItemModel *>(q->model())->itemFromIndex(modelIndex) : (q->orientation() == Qt::Horizontal ? qobject_cast<QStandardItemModel *>(q->model())->horizontalHeaderItem(0) : qobject_cast<QStandardItemModel *>(q->model())->verticalHeaderItem(0));
        item->setCheckable(true);
    }
}

XYCheckableModelHelper::XYCheckableModelHelper(
    Qt::Orientation orient, QObject *objectParent)
    : QObject(objectParent)
    , d_ptr(new XYCheckableModelHelperPrivate(*this))
{
    Q_D(XYCheckableModelHelper);
    d->Orientation = orient;
    d->init();
}

XYCheckableModelHelper::~XYCheckableModelHelper()
{
}

Qt::Orientation XYCheckableModelHelper::orientation() const
{
    Q_D(const XYCheckableModelHelper);
    return d->Orientation;
}

QAbstractItemModel *XYCheckableModelHelper::model() const
{
    Q_D(const XYCheckableModelHelper);
    return d->Model.isNull() ? 0 : d->Model.data();
}

void XYCheckableModelHelper::setModel(QAbstractItemModel *newModel)
{
    Q_D(XYCheckableModelHelper);
    QAbstractItemModel *current = this->model();
    if (current == newModel)
    {
        return;
    }
    if (current)
    {
        this->disconnect(
            current, SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
            this, SLOT(onHeaderDataChanged(Qt::Orientation, int, int)));
        this->disconnect(
            current, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
        this->disconnect(
            current, SIGNAL(columnsInserted(QModelIndex, int, int)),
            this, SLOT(onColumnsInserted(QModelIndex, int, int)));
        this->disconnect(
            current, SIGNAL(rowsInserted(QModelIndex, int, int)),
            this, SLOT(onRowsInserted(QModelIndex, int, int)));
    }
    d->Model = newModel;
    if (newModel)
    {
        this->connect(
            newModel, SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
            this, SLOT(onHeaderDataChanged(Qt::Orientation, int, int)));
        if (d->PropagateDepth != 0)
        {
            this->connect(
                newModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
        }
        this->connect(
            newModel, SIGNAL(columnsInserted(QModelIndex, int, int)),
            this, SLOT(onColumnsInserted(QModelIndex, int, int)));
        this->connect(
            newModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
            this, SLOT(onRowsInserted(QModelIndex, int, int)));

        if (d->ForceCheckability)
        {
            foreach (QModelIndex index, newModel->match(newModel->index(0, 0), Qt::CheckStateRole, QVariant(), -1, Qt::MatchRecursive))
            {
                d->forceCheckability(index);
            }
            d->forceCheckability(this->rootIndex());
        }
        this->updateHeadersFromItems();
    }
}

QModelIndex XYCheckableModelHelper::rootIndex() const
{
    Q_D(const XYCheckableModelHelper);
    return d->RootIndex;
}

void XYCheckableModelHelper::setRootIndex(const QModelIndex &index)
{
    Q_D(XYCheckableModelHelper);
    d->RootIndex = index;
    if (d->PropagateDepth != 0)
    {
        this->updateHeadersFromItems();
    }
}

void XYCheckableModelHelper::setPropagateDepth(int depth)
{
    Q_D(XYCheckableModelHelper);
    if (d->PropagateDepth == depth)
    {
        return;
    }
    d->PropagateDepth = depth;
    if (!this->model())
    {
        return;
    }
    if (depth != 0)
    {
        this->connect(
            this->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(onDataChanged(QModelIndex, QModelIndex)), Qt::UniqueConnection);
        this->updateHeadersFromItems();
    }
    else
    {
        this->disconnect(
            this->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
    }
}

int XYCheckableModelHelper::propagateDepth() const
{
    Q_D(const XYCheckableModelHelper);
    return d->PropagateDepth;
}

void XYCheckableModelHelper::setForceCheckability(bool force)
{
    Q_D(XYCheckableModelHelper);
    if (d->ForceCheckability == force)
    {
        return;
    }
    d->ForceCheckability = force;
    if (this->model())
    {
        d->propagateCheckStateToChildren(this->rootIndex());
    }
}

bool XYCheckableModelHelper::forceCheckability() const
{
    Q_D(const XYCheckableModelHelper);
    return d->ForceCheckability;
}

void XYCheckableModelHelper::setDefaultCheckState(Qt::CheckState defaultCheckState)
{
    Q_D(XYCheckableModelHelper);
    d->DefaultCheckState = defaultCheckState;
}

Qt::CheckState XYCheckableModelHelper::defaultCheckState() const
{
    Q_D(const XYCheckableModelHelper);
    return d->DefaultCheckState;
}

void XYCheckableModelHelper::setHeaderCheckState(int section, Qt::CheckState checkState)
{
    QAbstractItemModel *current = this->model();
    if (current == 0)
    {
        return;
    }
    current->setHeaderData(section, this->orientation(),
                           static_cast<int>(checkState), Qt::CheckStateRole);
}

void XYCheckableModelHelper::setCheckState(const QModelIndex &index, Qt::CheckState checkState)
{
    QAbstractItemModel *current = this->model();
    if (current == 0)
    {
        return;
    }
    current->setData(index, static_cast<int>(checkState), Qt::CheckStateRole);
}

void XYCheckableModelHelper::toggleCheckState(const QModelIndex &modelIndex)
{

    if (!this->isCheckable(modelIndex))
    {
        return;
    }

    this->setCheckState(modelIndex,
                        this->checkState(modelIndex) == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void XYCheckableModelHelper::toggleHeaderCheckState(int section)
{
    if (!this->isHeaderCheckable(section))
    {
        return;
    }

    this->setHeaderCheckState(
        section, this->headerCheckState(section) == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void XYCheckableModelHelper::onHeaderDataChanged(Qt::Orientation orient,
                                                 int firstSection,
                                                 int lastSection)
{
    Q_D(XYCheckableModelHelper);
    Q_UNUSED(firstSection);
    Q_UNUSED(lastSection);
    if (orient != this->orientation())
    {
        return;
    }
    bool oldItemsAreUpdating = d->ItemsAreUpdating;
    if (!d->ItemsAreUpdating)
    {
        d->ItemsAreUpdating = true;
        d->propagateCheckStateToChildren(this->rootIndex());
    }
    d->ItemsAreUpdating = oldItemsAreUpdating;
}

void XYCheckableModelHelper::updateHeadersFromItems()
{
    Q_D(XYCheckableModelHelper);
    QAbstractItemModel *currentModel = this->model();
    if (!currentModel)
    {
        return;
    }
    d->updateCheckState(QModelIndex());
}

void XYCheckableModelHelper::onDataChanged(const QModelIndex &topLeft,
                                           const QModelIndex &bottomRight)
{
    Q_UNUSED(bottomRight);
    Q_D(XYCheckableModelHelper);
    if (d->ItemsAreUpdating || d->PropagateDepth == 0)
    {
        return;
    }
    bool checkable = false;
    d->checkState(topLeft, &checkable);
    if (!checkable)
    {
        return;
    }
    d->ItemsAreUpdating = true;
    d->propagateCheckStateToChildren(topLeft);
    d->updateCheckState(topLeft.parent());

    d->ItemsAreUpdating = false;
}

void XYCheckableModelHelper::onColumnsInserted(const QModelIndex &parentIndex,
                                               int start, int end)
{
    Q_D(XYCheckableModelHelper);
    if (this->orientation() == Qt::Horizontal)
    {
        if (start == 0)
        {
            this->updateHeadersFromItems();
        }
    }
    else
    {
        if (d->ForceCheckability)
        {
            for (int i = start; i <= end; ++i)
            {
                QModelIndex index = this->model()->index(0, i, parentIndex);
                d->forceCheckability(index);
            }
        }
        this->onDataChanged(this->model()->index(0, start, parentIndex),
                            this->model()->index(0, end, parentIndex));
    }
}

void XYCheckableModelHelper::onRowsInserted(const QModelIndex &parentIndex,
                                            int start, int end)
{
    Q_D(XYCheckableModelHelper);
    if (this->orientation() == Qt::Vertical)
    {
        if (start == 0)
        {
            this->updateHeadersFromItems();
        }
    }
    else
    {
        if (d->ForceCheckability)
        {
            for (int i = start; i <= end; ++i)
            {
                QModelIndex index = this->model()->index(i, 0, parentIndex);
                d->forceCheckability(index);
            }
        }
        this->onDataChanged(this->model()->index(start, 0, parentIndex),
                            this->model()->index(end, 0, parentIndex));
    }
}

bool XYCheckableModelHelper::isHeaderCheckable(int section) const
{
    if (!this->model())
    {
        qWarning() << "xyCheckableModelHelper::isHeaderCheckable : Model has not been set";
        return (this->forceCheckability() && section == 0);
    }
    return !this->model()->headerData(section, this->orientation(), Qt::CheckStateRole).isNull();
}

bool XYCheckableModelHelper::isCheckable(const QModelIndex &index) const
{
    if (!this->model())
    {
        qWarning() << "xyCheckableModelHelper::isCheckable : Model has not been set";
        return (this->forceCheckability() && index.column() == 0);
    }
    return !this->model()->data(index, Qt::CheckStateRole).isNull();
}

Qt::CheckState XYCheckableModelHelper::headerCheckState(int section) const
{
    if (!this->model())
    {
        qWarning() << "xyCheckableModelHelper::headerCheckState : Model has not been set";
        return this->defaultCheckState();
    }
    return static_cast<Qt::CheckState>(
        this->model()->headerData(section, this->orientation(), Qt::CheckStateRole).toInt());
}

Qt::CheckState XYCheckableModelHelper::checkState(const QModelIndex &index) const
{
    if (!this->model())
    {
        qWarning() << "xyCheckableModelHelper::checkState : Model has not been set";
        return this->defaultCheckState();
    }
    return static_cast<Qt::CheckState>(
        this->model()->data(index, Qt::CheckStateRole).toInt());
}

bool XYCheckableModelHelper::headerCheckState(int section, Qt::CheckState &checkState) const
{
    bool checkable = false;
    if (!this->model())
    {
        qWarning() << "xyCheckableModelHelper::headerCheckState : Model has not been set";
        return (this->forceCheckability() && section == 0);
    }
    checkState = static_cast<Qt::CheckState>(
        this->model()->headerData(section, this->orientation(), Qt::CheckStateRole).toInt(&checkable));
    return checkable;
}

bool XYCheckableModelHelper::checkState(const QModelIndex &index, Qt::CheckState &checkState) const
{
    bool checkable = false;
    if (!this->model())
    {
        qWarning() << "xyCheckableModelHelper::checkState : Model has not been set";
        return (this->forceCheckability() && index.column() == 0);
    }
    checkState = static_cast<Qt::CheckState>(
        this->model()->data(index, Qt::CheckStateRole).toInt(&checkable));
    return checkable;
}
