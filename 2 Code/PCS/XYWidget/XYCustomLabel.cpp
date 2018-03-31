#include "XYCustomLabel.h"

XYCustomLabel::XYCustomLabel(QWidget *parent)
    : QLabel(parent)
    , m_row(0)
    , m_index(0)
    , m_valid(false)
{
    setAlignment(Qt::AlignCenter);

    m_pixmap1 = QPixmap(":/images/pngs/table_accept.png")
                .scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixmap2 = QPixmap(":/images/pngs/table_reject.png")
                .scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPixmap(m_valid ? m_pixmap1 : m_pixmap2);
}

XYCustomLabel::~XYCustomLabel()
{

}

void XYCustomLabel::setValid(bool valid)
{
    if (valid == m_valid)
        return;

    m_valid = valid;

    setPixmap(m_valid ? m_pixmap1 : m_pixmap2);
}

void XYCustomLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        event->type() == QEvent::MouseButtonDblClick)
    {
        emit signalLabelDblClicked(m_row, m_index);
    }

    QLabel::mousePressEvent(event);
}

