/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLabel.h
 * @brief   自定义多态标签
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYLABEL_H
#define XYTECH_XYLABEL_H

#include "XYWidgetExport.h"

#include <QLabel>
#include <QMouseEvent>

class XYWIDGET_EXPORT XYClickLabel : public QLabel
{
    Q_OBJECT

public:
    explicit XYClickLabel(QWidget *parent = Q_NULLPTR);
    virtual ~XYClickLabel();

public:
    void setStaticPixmap(const QPixmap &pixmap);
    void setActivePixmap(const QPixmap &pixmap);
    void setClickPixmap(const QPixmap &pixmap);

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void clicked();

private:
    QPixmap staticPixmap;
    QPixmap activePixmap;
    QPixmap clickPixmap;
};

#endif
