#ifndef XYCUSTOMLABEL_H
#define XYCUSTOMLABEL_H

#include "XYWidgetExport.h"

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class XYWIDGET_EXPORT XYCustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit XYCustomLabel(QWidget *parent = Q_NULLPTR);
    virtual ~XYCustomLabel();

    inline void setRow(int row) { m_row = row; }
    inline void setIndex(int index) { m_index = index; }
    void setValid(bool valid);
signals:
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

#endif // XYCUSTOMLABEL_H
