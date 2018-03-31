/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    XYComboBox.h
 * @brief   弹出式组合框
 * @version 1.0
 * @authors WeiHeng(weiheng@njid.cn)
 * @date    2018/03/01
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef XYTEC_XYCOMBOBOX_H
#define XYTEC_XYCOMBOBOX_H

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
signals:
    void clickPopup();
    void clickPopup(QComboBox *ptr);

private:
    bool enablePopup;
};

#endif
