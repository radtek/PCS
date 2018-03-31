#ifndef PCS_EXWIDGET_XYLABEL_H_
#define PCS_EXWIDGET_XYLABEL_H_

#include "XYWidgetExport.h"

#include <QLabel>

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
