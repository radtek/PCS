/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYSqlQueryModel.cpp
 *
 * @example
 *
 * @brief
 *
 * @date 2018-04-01 @author weiheng             @note create this file
 * *****************************************************************************
 */

#include "XYCheckableComboBox.h"
#include "XYCheckableModelHelper.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QItemDelegate>
#include <QLayout>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QStandardItemModel>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QToolBar>

class XYComboBoxDelegate : public QItemDelegate
{
public:
    XYComboBoxDelegate(QObject *parent, QComboBox *cmb)
        : QItemDelegate(parent)
        , ComboBox(cmb)
    {
    }

    static bool isSeparator(const QModelIndex &index)
    {
        return index.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("separator");
    }

    static void setSeparator(QAbstractItemModel *model, const QModelIndex &index)
    {
        model->setData(index, QString::fromLatin1("separator"), Qt::AccessibleDescriptionRole);
        if (QStandardItemModel *m = qobject_cast<QStandardItemModel *>(model))
        {
            if (QStandardItem *item = m->itemFromIndex(index))
            {
                item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            }
        }
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const
    {
        if (isSeparator(index))
        {
            QRect rect = option.rect;

            if (const QAbstractItemView *view = qobject_cast<const QAbstractItemView *>(option.widget))
            {
                rect.setWidth(view->viewport()->width());
            }

            QStyleOption opt;
            opt.rect = rect;
            this->ComboBox->style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, painter, this->ComboBox);
        }
        else
        {
            QItemDelegate::paint(painter, option, index);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
    {
        if (isSeparator(index))
        {
            int pm = this->ComboBox->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, 0, this->ComboBox);
            return QSize(pm, pm);
        }
        return this->QItemDelegate::sizeHint(option, index);
    }

private:
    QComboBox *ComboBox;
};

class XYCheckableComboBoxPrivate
{
    Q_DECLARE_PUBLIC(XYCheckableComboBox)

protected:
    XYCheckableComboBox *const q_ptr;
    QModelIndexList checkedIndexes() const;
    QModelIndexList uncheckedIndexes() const;

public:
    XYCheckableComboBoxPrivate(XYCheckableComboBox &object);
    void init();

    QModelIndexList cachedCheckedIndexes() const;
    void updateCheckedList();

    XYCheckableModelHelper *CheckableModelHelper;
    bool MouseButtonPressed;

    QString fullString;
    QString emptyString;

private:
    QModelIndexList persistentIndexesToModelIndexes(
        const QList<QPersistentModelIndex> &persistentModels) const;
    QList<QPersistentModelIndex> modelIndexesToPersistentIndexes(
        const QModelIndexList &modelIndexes) const;

    mutable QList<QPersistentModelIndex> CheckedList;
};

XYCheckableComboBoxPrivate::XYCheckableComboBoxPrivate(XYCheckableComboBox &object)
    : q_ptr(&object)
{
    this->CheckableModelHelper = 0;
    this->MouseButtonPressed = false;
}

void XYCheckableComboBoxPrivate::init()
{
    Q_Q(XYCheckableComboBox);
    this->CheckableModelHelper = new XYCheckableModelHelper(Qt::Horizontal, q);
    this->CheckableModelHelper->setForceCheckability(true);

    q->setCheckableModel(q->model());
    q->view()->installEventFilter(q);
    q->view()->viewport()->installEventFilter(q);
    q->setItemDelegate(new XYComboBoxDelegate(q->view(), q));
}

void XYCheckableComboBoxPrivate::updateCheckedList()
{
    Q_Q(XYCheckableComboBox);
    QList<QPersistentModelIndex> newCheckedPersistentList =
        this->modelIndexesToPersistentIndexes(this->checkedIndexes());
    if (newCheckedPersistentList == this->CheckedList)
    {
        return;
    }
    this->CheckedList = newCheckedPersistentList;
    emit q->checkedIndexesChanged();
}

QList<QPersistentModelIndex> XYCheckableComboBoxPrivate ::modelIndexesToPersistentIndexes(const QModelIndexList &indexes) const
{
    QList<QPersistentModelIndex> res;
    foreach (const QModelIndex &index, indexes)
    {
        QPersistentModelIndex persistent(index);
        if (persistent.isValid())
        {
            res << persistent;
        }
    }
    return res;
}

QModelIndexList XYCheckableComboBoxPrivate ::persistentIndexesToModelIndexes(
    const QList<QPersistentModelIndex> &indexes) const
{
    QModelIndexList res;
    foreach (const QPersistentModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            res << index;
        }
    }
    return res;
}

QModelIndexList XYCheckableComboBoxPrivate::cachedCheckedIndexes() const
{
    return this->persistentIndexesToModelIndexes(this->CheckedList);
}

QModelIndexList XYCheckableComboBoxPrivate::checkedIndexes() const
{
    Q_Q(const XYCheckableComboBox);
    QModelIndex startIndex = q->model()->index(0, 0, q->rootModelIndex());
    return q->model()->match(
        startIndex, Qt::CheckStateRole,
        static_cast<int>(Qt::Checked), -1, Qt::MatchRecursive);
}

QModelIndexList XYCheckableComboBoxPrivate::uncheckedIndexes() const
{
    Q_Q(const XYCheckableComboBox);
    QModelIndex startIndex = q->model()->index(0, 0, q->rootModelIndex());
    return q->model()->match(
        startIndex, Qt::CheckStateRole,
        static_cast<int>(Qt::Unchecked), -1, Qt::MatchRecursive);
}

XYCheckableComboBox::XYCheckableComboBox(QWidget *parentWidget)
    : QComboBox(parentWidget)
    , d_ptr(new XYCheckableComboBoxPrivate(*this))
{
    Q_D(XYCheckableComboBox);
    d->init();
}

XYCheckableComboBox::~XYCheckableComboBox()
{
}

bool XYCheckableComboBox::eventFilter(QObject *o, QEvent *e)
{
    Q_D(XYCheckableComboBox);
    switch (e->type())
    {
    case QEvent::MouseButtonPress:
    {
        if (this->view()->isVisible())
        {
            d->MouseButtonPressed = true;
        }
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent *m = static_cast<QMouseEvent *>(e);
        if (this->view()->isVisible() &&
            this->view()->rect().contains(m->pos()) &&
            this->view()->currentIndex().isValid()
            //&& !blockMouseReleaseTimer.isActive()
            && (this->view()->currentIndex().flags() & Qt::ItemIsEnabled) && (this->view()->currentIndex().flags() & Qt::ItemIsSelectable))
        {
            if (d->MouseButtonPressed)
            {
                this->setCurrentIndex(this->view()->currentIndex().row());
                d->CheckableModelHelper->toggleCheckState(this->view()->currentIndex());
            }
            d->MouseButtonPressed = false;
            return true;
        }
        d->MouseButtonPressed = false;
        break;
    }
    default:
        break;
    }
    return this->QComboBox::eventFilter(o, e);
}

void XYCheckableComboBox::setFullString(const QString &string)
{
    Q_D(XYCheckableComboBox);
    d->fullString = string;
}

void XYCheckableComboBox::setEmptyString(const QString &string)
{
    Q_D(XYCheckableComboBox);
    d->emptyString = string;
}

void XYCheckableComboBox::setCheckableModel(QAbstractItemModel *newModel)
{
    Q_D(XYCheckableComboBox);
    this->disconnect(this->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                     this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
    if (newModel != this->model())
    {
        this->setModel(newModel);
    }
    this->connect(this->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                  this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
    d->CheckableModelHelper->setModel(newModel);
    d->updateCheckedList();
}

QAbstractItemModel *XYCheckableComboBox::checkableModel() const
{
    return this->model();
}

QModelIndexList XYCheckableComboBox::checkedIndexes() const
{
    Q_D(const XYCheckableComboBox);
    return d->cachedCheckedIndexes();
}

bool XYCheckableComboBox::allChecked() const
{
    Q_D(const XYCheckableComboBox);
    return d->uncheckedIndexes().count() == 0;
}

bool XYCheckableComboBox::noneChecked() const
{
    Q_D(const XYCheckableComboBox);
    return d->cachedCheckedIndexes().count() == 0;
}

void XYCheckableComboBox::setCheckState(const QModelIndex &index, Qt::CheckState check)
{
    Q_D(XYCheckableComboBox);
    return d->CheckableModelHelper->setCheckState(index, check);
}

Qt::CheckState XYCheckableComboBox::checkState(const QModelIndex &index) const
{
    Q_D(const XYCheckableComboBox);
    return d->CheckableModelHelper->checkState(index);
}

XYCheckableModelHelper *XYCheckableComboBox::checkableModelHelper() const
{
    Q_D(const XYCheckableComboBox);
    return d->CheckableModelHelper;
}

void XYCheckableComboBox::onDataChanged(const QModelIndex &start, const QModelIndex &end)
{
    Q_D(XYCheckableComboBox);
    Q_UNUSED(start);
    Q_UNUSED(end);
    d->updateCheckedList();
}

void XYCheckableComboBox::paintEvent(QPaintEvent *)
{
    Q_D(XYCheckableComboBox);

    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));

    QStyleOptionComboBox opt;
    this->initStyleOption(&opt);

    if (this->allChecked())
    {
        opt.currentText = d->fullString;
        opt.currentIcon = QIcon();
    }
    else if (this->noneChecked())
    {
        opt.currentText = d->emptyString;
        opt.currentIcon = QIcon();
    }
    else
    {
        QModelIndexList indexes = d->cachedCheckedIndexes();
        if (indexes.count() == 1)
        {
            opt.currentText = this->model()->data(indexes[0], Qt::DisplayRole).toString();
            opt.currentIcon = qvariant_cast<QIcon>(this->model()->data(indexes[0], Qt::DecorationRole));
        }
        else
        {
            QStringList indexesText;
            foreach (QModelIndex checkedIndex, indexes)
            {
                indexesText << this->model()->data(checkedIndex, Qt::DisplayRole).toString();
            }
            opt.currentText = indexesText.join(", ");
            opt.currentIcon = QIcon();
        }
    }
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}
