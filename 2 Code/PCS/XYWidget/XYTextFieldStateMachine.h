/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYTextFieldStateMachine.h
 * @brief   动态文本框辅助状态机
 * @version 1.0
 * @note
 *
 * *****************************************************************************
 */

#ifndef XYTECH_XYTEXTFIELDSTATEMACHINE_H
#define XYTECH_XYTEXTFIELDSTATEMACHINE_H

#include "XYTextFieldLineEdit.h"

class QPropertyAnimation;
class XYTextFieldLabel;

class XYTextFieldStateMachine : public QStateMachine
{
    Q_OBJECT
    Q_DISABLE_COPY(XYTextFieldStateMachine)
    Q_PROPERTY(qreal progress WRITE setProgress READ progress)

public:
    XYTextFieldStateMachine(XYTextFieldLineEdit *parent);
    virtual ~XYTextFieldStateMachine();

    void setLabel(XYTextFieldLabel *label);

    inline void setProgress(qreal progress);
    inline qreal progress() const;

public slots:
    void setupProperties();

private:
    XYTextFieldLineEdit *const m_textField;
    QState *const m_normalState;
    QState *const m_focusedState;
    XYTextFieldLabel *m_label;
    QPropertyAnimation *m_offsetAnimation;
    QPropertyAnimation *m_colorAnimation;
    qreal m_progress;
};

inline void XYTextFieldStateMachine::setProgress(qreal progress)
{
    m_progress = progress;
    m_textField->update();
}

inline qreal XYTextFieldStateMachine::progress() const
{
    return m_progress;
}

class XYTextFieldLabel : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(XYTextFieldLabel)
    Q_PROPERTY(qreal scale WRITE setScale READ scale)
    Q_PROPERTY(QPointF offset WRITE setOffset READ offset)
    Q_PROPERTY(QColor color WRITE setColor READ color)

public:
    XYTextFieldLabel(XYTextFieldLineEdit *parent);
    virtual ~XYTextFieldLabel();

    inline void setScale(qreal scale);
    inline qreal scale() const;

    inline void setOffset(const QPointF &pos);
    inline QPointF offset() const;

    inline void setColor(const QColor &color);
    inline QColor color() const;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    XYTextFieldLineEdit *const textField;
    qreal labelScale;
    qreal labelPosX;
    qreal labelPosY;
    QColor labelColor;
};

inline void XYTextFieldLabel::setScale(qreal scale)
{
    labelScale = scale;
    update();
}

inline qreal XYTextFieldLabel::scale() const
{
    return labelScale;
}

inline void XYTextFieldLabel::setOffset(const QPointF &pos)
{
    labelPosX = pos.x();
    labelPosY = pos.y();
    update();
}

inline QPointF XYTextFieldLabel::offset() const
{
    return QPointF(labelPosX, labelPosY);
}

inline void XYTextFieldLabel::setColor(const QColor &color)
{
    labelColor = color;
    update();
}

inline QColor XYTextFieldLabel::color() const
{
    return labelColor;
}

#endif
