/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLabel.cpp
 *
 * @example
 *
 * @brief
 *
 * @date 2018-04-01 @author zhuangming          @note create this file
 * *****************************************************************************
 */

#include "XYLabel.h"

XYClickLabel::XYClickLabel(QWidget *parent)
    : QLabel(parent)
{
    this->setMouseTracking(true);
}

XYClickLabel::~XYClickLabel()
{
}

void XYClickLabel::setStaticPixmap(const QPixmap &pixmap)
{
    staticPixmap = pixmap;
    this->setPixmap(staticPixmap);
}

void XYClickLabel::setActivePixmap(const QPixmap &pixmap)
{
    activePixmap = pixmap;
}

void XYClickLabel::setClickPixmap(const QPixmap &pixmap)
{
    clickPixmap = pixmap;
}

void XYClickLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->setPixmap(clickPixmap);
    }

    QLabel::mouseMoveEvent(event);
}

void XYClickLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
        if (this->rect().contains(event->pos()))
        {
            this->setPixmap(activePixmap);
        }
        else
        {
            this->setPixmap(staticPixmap);
        }
    }

    QLabel::mouseReleaseEvent(event);
}

void XYClickLabel::enterEvent(QEvent *event)
{
    this->setPixmap(activePixmap);

    QLabel::enterEvent(event);
}

void XYClickLabel::leaveEvent(QEvent *event)
{
    this->setPixmap(staticPixmap);

    QLabel::leaveEvent(event);
}
