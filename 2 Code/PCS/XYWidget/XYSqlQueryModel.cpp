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

#include "XYSqlQueryModel.h"

class XYSqlQueryModelPrivate
{
    Q_DISABLE_COPY(XYSqlQueryModelPrivate)
    Q_DECLARE_PUBLIC(XYSqlQueryModel)

public:
    XYSqlQueryModelPrivate(XYSqlQueryModel *q);
    ~XYSqlQueryModelPrivate();

    void setColumnEnum(const QString &colName, const QMap<QVariant, QString> &colMap);

    XYSqlQueryModel *const q_ptr;

    QMap<QString, QMap<QVariant, QString>> mapColumns;
};

XYSqlQueryModelPrivate::XYSqlQueryModelPrivate(XYSqlQueryModel *q)
    : q_ptr(q)
{
}

XYSqlQueryModelPrivate::~XYSqlQueryModelPrivate()
{
}

void XYSqlQueryModelPrivate::setColumnEnum(const QString &colName, const QMap<QVariant, QString> &colMap)
{
    mapColumns[colName] = colMap;
}

////////////////////////////////////////////////////////////////////////////////

XYSqlQueryModel::XYSqlQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
    , d_ptr(new XYSqlQueryModelPrivate(this))
{
}

XYSqlQueryModel::~XYSqlQueryModel()
{
}

void XYSqlQueryModel::setColumnEnum(const QString &colName, const QMap<QVariant, QString> &colMap)
{
    Q_D(XYSqlQueryModel);

    d->setColumnEnum(colName, colMap);
}

QVariant XYSqlQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        Q_D(const XYSqlQueryModel);

        QString colName = record().fieldName(index.column());
        if (d->mapColumns.contains(colName))
        {
            QVariant value = record(index.row()).value(index.column());

            if (value.isNull())
            {
                return QVariant();
            }
            const QMap<QVariant, QString> colMap = d->mapColumns.value(colName);

            return colMap.value(value);
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }

    return QSqlQueryModel::data(index, role);
}
