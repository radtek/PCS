/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYCustomButton.h
 * @brief   自定义按钮
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYTABLEBUTTON_H
#define XYTECH_XYTABLEBUTTON_H

#include "XYWidgetExport.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QWidget>

class XYWIDGET_EXPORT ChildButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ChildButton(QWidget *parent = Q_NULLPTR);
    virtual ~ChildButton();

    inline void setIndex(int index) { m_index = index; }
    void setButtonText(const QString &text);

Q_SIGNALS:
    void signalClicked(int index);

public Q_SLOTS:
    void slotClicked();

private:
    int m_index;
};

class XYWIDGET_EXPORT XYCustomButton : public QWidget
{
    Q_OBJECT

public:
    explicit XYCustomButton(QWidget *parent = Q_NULLPTR);
    virtual ~XYCustomButton();

    inline void setRow(int row) { m_row = row; }
    void addButtons(const QStringList &items);

Q_SIGNALS:
    void signalButtonClicked(int row, int index);

public Q_SLOTS:
    void slotButtonClicked(int index);

private:
    int m_row;
    int m_count;
};

#endif
