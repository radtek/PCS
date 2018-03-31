/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYComboBox.h
 * @brief   弹出式组合框
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYCOMBOBOX_H
#define XYTECH_XYCOMBOBOX_H

#include "XYWidgetExport.h"

#include <QComboBox>
#include <QListView>

class XYWIDGET_EXPORT XYComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit XYComboBox(QWidget *parent = Q_NULLPTR);
    virtual ~XYComboBox();

public:
    inline void setPopupEnable(bool enable) { enablePopup = enable; }

protected:
    virtual void showPopup();

Q_SIGNALS:
    void clickPopup();
    void clickPopup(QComboBox *ptr);

private:
    bool enablePopup;
};

#endif
