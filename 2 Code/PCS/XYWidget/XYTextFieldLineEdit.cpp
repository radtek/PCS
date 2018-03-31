/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYTextFieldLineEdit.cpp
 *
 * @example
 *
 * @brief
 *
 * @date 2018-04-01 @author weiheng             @note create this file
 * *****************************************************************************
 */

#include "XYTextFieldLineEdit.h"
#include "XYTextFieldStateMachine.h"

class XYTextFieldLineEditPrivate
{
    Q_DISABLE_COPY(XYTextFieldLineEditPrivate)
    Q_DECLARE_PUBLIC(XYTextFieldLineEdit)

public:
    XYTextFieldLineEditPrivate(XYTextFieldLineEdit *q);
    virtual ~XYTextFieldLineEditPrivate();

    void init();

    XYTextFieldLineEdit *const q_ptr;
    XYTextFieldStateMachine *stateMachine;
    XYTextFieldLabel *label;

    QColor textColor;
    QColor labelColor;
    QColor inkColor;
    QColor inputLineColor;
    qreal labelFontSize;
    QString labelString;

    bool showLabel;
    bool showInputLine;
    bool useThemeColors;
};

XYTextFieldLineEditPrivate::XYTextFieldLineEditPrivate(XYTextFieldLineEdit *q)
    : q_ptr(q)
{
}

XYTextFieldLineEditPrivate::~XYTextFieldLineEditPrivate()
{
}

void XYTextFieldLineEditPrivate::init()
{
    Q_Q(XYTextFieldLineEdit);

    textColor = QColor("#9e9e9e");
    labelColor = QColor("#bdbdbd");
    inkColor = QColor("#00838f");
    inputLineColor = QColor("#eeeeee");

    stateMachine = new XYTextFieldStateMachine(q);
    label = Q_NULLPTR;
    labelFontSize = 9;
    showLabel = false;
    showInputLine = true;
    useThemeColors = true;

    q->setFrame(false);
    q->setAttribute(Qt::WA_Hover);
    q->setMouseTracking(true);
    q->setTextMargins(0, 2, 0, 4);

    stateMachine->start();
    QCoreApplication::processEvents();
}

XYTextFieldLineEdit::XYTextFieldLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , d_ptr(new XYTextFieldLineEditPrivate(this))
{
    d_func()->init();
}

XYTextFieldLineEdit::~XYTextFieldLineEdit()
{
}

void XYTextFieldLineEdit::setUseThemeColors(bool value)
{
    Q_D(XYTextFieldLineEdit);

    if (d->useThemeColors == value)
    {
        return;
    }

    d->useThemeColors = value;
    d->stateMachine->setupProperties();
}

bool XYTextFieldLineEdit::useThemeColors() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->useThemeColors;
}

void XYTextFieldLineEdit::setShowLabel(bool value)
{
    Q_D(XYTextFieldLineEdit);

    if (d->showLabel == value)
    {
        return;
    }

    d->showLabel = value;

    if (!d->label && value)
    {
        d->label = new XYTextFieldLabel(this);
        d->stateMachine->setLabel(d->label);
    }

    if (value)
    {
        setContentsMargins(0, 23, 0, 0);
    }
    else
    {
        setContentsMargins(0, 0, 0, 0);
    }
}

bool XYTextFieldLineEdit::hasLabel() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->showLabel;
}

void XYTextFieldLineEdit::setLabelFontSize(qreal size)
{
    Q_D(XYTextFieldLineEdit);

    d->labelFontSize = size;

    if (d->label)
    {
        QFont font(d->label->font());
        font.setPointSizeF(size);
        d->label->setFont(font);
        d->label->update();
    }
}

qreal XYTextFieldLineEdit::labelFontSize() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->labelFontSize;
}

void XYTextFieldLineEdit::setLabel(const QString &label)
{
    Q_D(XYTextFieldLineEdit);

    d->labelString = label;
    setShowLabel(true);
    d->label->update();
}

QString XYTextFieldLineEdit::label() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->labelString;
}

void XYTextFieldLineEdit::setTextColor(const QColor &color)
{
    Q_D(XYTextFieldLineEdit);

    d->textColor = color;
    setStyleSheet(QString("QLineEdit { color: %1; }").arg(color.name()));

    d->stateMachine->setupProperties();
}

QColor XYTextFieldLineEdit::textColor() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->textColor;
}

void XYTextFieldLineEdit::setLabelColor(const QColor &color)
{
    Q_D(XYTextFieldLineEdit);

    d->labelColor = color;
    d->stateMachine->setupProperties();
}

QColor XYTextFieldLineEdit::labelColor() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->labelColor;
}

void XYTextFieldLineEdit::setInkColor(const QColor &color)
{
    Q_D(XYTextFieldLineEdit);

    d->inkColor = color;
    d->stateMachine->setupProperties();
}

QColor XYTextFieldLineEdit::inkColor() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->inkColor;
}

void XYTextFieldLineEdit::setInputLineColor(const QColor &color)
{
    Q_D(XYTextFieldLineEdit);

    d->inputLineColor = color;
    d->stateMachine->setupProperties();
}

QColor XYTextFieldLineEdit::inputLineColor() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->inputLineColor;
}

void XYTextFieldLineEdit::setShowInputLine(bool value)
{
    Q_D(XYTextFieldLineEdit);

    if (d->showInputLine == value)
    {
        return;
    }

    d->showInputLine = value;
    update();
}

bool XYTextFieldLineEdit::hasInputLine() const
{
    Q_D(const XYTextFieldLineEdit);

    return d->showInputLine;
}

XYTextFieldLineEdit::XYTextFieldLineEdit(XYTextFieldLineEditPrivate &d, QWidget *parent)
    : QLineEdit(parent)
    , d_ptr(&d)
{
    d_func()->init();
}

bool XYTextFieldLineEdit::event(QEvent *event)
{
    Q_D(XYTextFieldLineEdit);

    switch (event->type())
    {
    case QEvent::Resize:
    case QEvent::Move:
    {
        if (d->label)
        {
            d->label->setGeometry(rect());
        }
        break;
    }
    default:
        break;
    }
    return QLineEdit::event(event);
}

void XYTextFieldLineEdit::paintEvent(QPaintEvent *event)
{
    Q_D(XYTextFieldLineEdit);

    QLineEdit::paintEvent(event);

    QPainter painter(this);

    const qreal progress = d->stateMachine->progress();

    if (text().isEmpty() && progress < 1)
    {
        painter.setOpacity(1 - progress);
        if (parentWidget())
        {
            painter.fillRect(rect(), parentWidget()->palette().color(backgroundRole()));
        }
    }

    const int y = height() - 1;
    const int wd = width() - 5;

    if (d->showInputLine)
    {
        QPen pen;
        pen.setWidth(1);
        pen.setColor(inputLineColor());

        if (!isEnabled())
            pen.setStyle(Qt::DashLine);

        painter.setPen(pen);
        painter.setOpacity(1);
        painter.drawLine(QLineF(2.5, y, wd, y));

        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(inkColor());

        if (progress > 0)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(brush);
            const int w = static_cast<int>((1 - progress) * static_cast<qreal>(wd / 2));
            painter.drawRect(w + 2.5, height() - 2, wd - w * 2, 2);
        }
    }
}
