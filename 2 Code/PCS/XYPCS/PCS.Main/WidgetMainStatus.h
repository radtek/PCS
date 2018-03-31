#ifndef PCS_MAIN_WIDGETMAINSTATUS_H
#define PCS_MAIN_WIDGETMAINSTATUS_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui
{
class WidgetMainStatus;
}

class WidgetMainStatus : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMainStatus(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetMainStatus();

private:
    Ui::WidgetMainStatus *ui;
};

#endif
