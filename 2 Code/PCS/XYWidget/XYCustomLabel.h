/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYCustomLabel.h
 * @brief   自定义双击标签
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYCUSTOMLABEL_H
#define XYTECH_XYCUSTOMLABEL_H

#include "XYWidgetExport.h"

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

class XYWIDGET_EXPORT XYCustomLabel : public QLabel
{
    Q_OBJECT

public:
    explicit XYCustomLabel(QWidget *parent = Q_NULLPTR);
    virtual ~XYCustomLabel();

    inline void setRow(int row) { m_row = row; }
    inline void setIndex(int index) { m_index = index; }
    void setValid(bool valid);

Q_SIGNALS:
    void signalLabelDblClicked(int row, int index);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    int m_row;
    int m_index;
    bool m_valid;
    QPixmap m_pixmap1;
    QPixmap m_pixmap2;
};

#endif    // XYCUSTOMLABEL_H
