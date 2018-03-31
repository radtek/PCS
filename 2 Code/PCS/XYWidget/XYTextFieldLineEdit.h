/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYTextFieldLineEdit.h
 * @brief   动态文本框
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYTEXTFIELD_H
#define XYTECH_XYTEXTFIELD_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "XYWidgetExport.h"

class XYTextFieldLineEditPrivate;

class XYWIDGET_EXPORT XYTextFieldLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_DISABLE_COPY(XYTextFieldLineEdit)
    Q_DECLARE_PRIVATE(XYTextFieldLineEdit)
    Q_PROPERTY(QColor textColor WRITE setTextColor READ textColor)
    Q_PROPERTY(QColor inkColor WRITE setInkColor READ inkColor)
    Q_PROPERTY(QColor inputLineColor WRITE setInputLineColor READ inputLineColor)

public:
    explicit XYTextFieldLineEdit(QWidget *parent = Q_NULLPTR);
    virtual ~XYTextFieldLineEdit();

    void setUseThemeColors(bool value);
    bool useThemeColors() const;

    void setShowLabel(bool value);
    bool hasLabel() const;

    void setLabelFontSize(qreal size);
    qreal labelFontSize() const;

    void setLabel(const QString &label);
    QString label() const;

    void setTextColor(const QColor &color);
    QColor textColor() const;

    void setLabelColor(const QColor &color);
    QColor labelColor() const;

    void setInkColor(const QColor &color);
    QColor inkColor() const;

    void setInputLineColor(const QColor &color);
    QColor inputLineColor() const;

    void setShowInputLine(bool value);
    bool hasInputLine() const;

protected:
    XYTextFieldLineEdit(XYTextFieldLineEditPrivate &d, QWidget *parent = Q_NULLPTR);

    virtual bool event(QEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<XYTextFieldLineEditPrivate> d_ptr;
};

#endif
