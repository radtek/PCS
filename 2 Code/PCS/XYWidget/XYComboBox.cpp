/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYComboBox.cpp
 *
 * @example
 *
 * @brief
 *
 * @date 2018-04-01 @author zhuangming          @note create this file
 * *****************************************************************************
 */

#include "XYComboBox.h"

XYComboBox::XYComboBox(QWidget *parent)
    : QComboBox(parent)
    , enablePopup(true)
{
}

XYComboBox::~XYComboBox()
{
}

void XYComboBox::showPopup()
{
    emit clickPopup();
    emit clickPopup(this);

    if (enablePopup)
        QComboBox::showPopup();
}
