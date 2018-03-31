/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYSqlQueryModel.h
 * @brief   带枚举映射的SqlQueryModel
 * @version 1.0
 * @note
 *  普通用法同QSqlQueryModel
 * *****************************************************************************
 */

#ifndef XYTECH_XYSQLQUERYMODEL_H
#define XYTECH_XYSQLQUERYMODEL_H

#include "XYWidgetExport.h"

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>

class XYSqlQueryModelPrivate;
class XYWIDGET_EXPORT XYSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_DISABLE_COPY(XYSqlQueryModel)
    Q_DECLARE_PRIVATE(XYSqlQueryModel)

public:
    explicit XYSqlQueryModel(QObject *parent = Q_NULLPTR);
    virtual ~XYSqlQueryModel();

public:
    void setColumnEnum(const QString &colName, const QMap<QVariant, QString> &colMap);

public:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    const QScopedPointer<XYSqlQueryModelPrivate> d_ptr;
};

#endif
