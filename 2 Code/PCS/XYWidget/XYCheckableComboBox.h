/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYCheckableComboBox.h
 * @brief   复选组合框
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYCHECKABLECOMBOBOX_H
#define XYTECH_XYCHECKABLECOMBOBOX_H

#include "XYWidgetExport.h"

#include <QComboBox>

class XYCheckableModelHelper;
class XYCheckableComboBoxPrivate;

class XYWIDGET_EXPORT XYCheckableComboBox : public QComboBox
{
    Q_OBJECT

public:
    XYCheckableComboBox(QWidget *parent = Q_NULLPTR);
    virtual ~XYCheckableComboBox();

    Q_INVOKABLE QAbstractItemModel *checkableModel() const;
    Q_INVOKABLE void setCheckableModel(QAbstractItemModel *model);

    Q_INVOKABLE QModelIndexList checkedIndexes() const;

    Q_INVOKABLE bool allChecked() const;

    Q_INVOKABLE bool noneChecked() const;

    Q_INVOKABLE void setCheckState(const QModelIndex &index, Qt::CheckState check);

    Q_INVOKABLE Qt::CheckState checkState(const QModelIndex &index) const;

    Q_INVOKABLE XYCheckableModelHelper *checkableModelHelper() const;

    bool eventFilter(QObject *o, QEvent *e);

    void setFullString(const QString &string);
    void setEmptyString(const QString &string);

Q_SIGNALS:
    void checkedIndexesChanged();

protected Q_SLOTS:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

protected:
    virtual void paintEvent(QPaintEvent *);

protected:
    QScopedPointer<XYCheckableComboBoxPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(XYCheckableComboBox)
    Q_DISABLE_COPY(XYCheckableComboBox)
};

#endif
