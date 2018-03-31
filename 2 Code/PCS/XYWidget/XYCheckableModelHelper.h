/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYCheckableModelHelper.h
 * @brief   复选组合框辅助类
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYCHECKABLEMODELHELPER_H
#define XYTECH_XYCHECKABLEMODELHELPER_H

#include "XYWidgetExport.h"

#include <QModelIndex>
#include <QObject>

class QAbstractItemModel;
class XYCheckableModelHelperPrivate;

class XYWIDGET_EXPORT XYCheckableModelHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool forceCheckability READ forceCheckability WRITE setForceCheckability)
    Q_PROPERTY(int propagateDepth READ propagateDepth WRITE setPropagateDepth)
    Q_PROPERTY(Qt::CheckState defaultCheckState READ defaultCheckState WRITE setDefaultCheckState)

public:
    XYCheckableModelHelper(Qt::Orientation orientation, QObject *parent = Q_NULLPTR);
    virtual ~XYCheckableModelHelper();

    Qt::Orientation orientation() const;

    QAbstractItemModel *model() const;
    virtual void setModel(QAbstractItemModel *model);

    QModelIndex rootIndex() const;
    virtual void setRootIndex(const QModelIndex &index);

    bool isHeaderCheckable(int section) const;
    bool isCheckable(const QModelIndex &index) const;

    Qt::CheckState headerCheckState(int section) const;
    Qt::CheckState checkState(const QModelIndex &) const;

    bool headerCheckState(int section, Qt::CheckState &checkState) const;
    bool checkState(const QModelIndex &, Qt::CheckState &checkState) const;

    void setPropagateDepth(int depth);
    int propagateDepth() const;

    void setForceCheckability(bool force);
    bool forceCheckability() const;

    Qt::CheckState defaultCheckState() const;
    void setDefaultCheckState(Qt::CheckState);

public Q_SLOTS:
    void setCheckState(const QModelIndex &modelIndex, Qt::CheckState checkState);

    void setHeaderCheckState(int section, Qt::CheckState checkState);

    void toggleCheckState(const QModelIndex &modelIndex);
    void toggleHeaderCheckState(int section);

private Q_SLOTS:
    void onHeaderDataChanged(Qt::Orientation orient, int first, int last);

    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void updateHeadersFromItems();
    void onColumnsInserted(const QModelIndex &parent, int start, int end);
    void onRowsInserted(const QModelIndex &parent, int start, int end);

protected:
    QScopedPointer<XYCheckableModelHelperPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(XYCheckableModelHelper)
    Q_DISABLE_COPY(XYCheckableModelHelper)
};

#endif
