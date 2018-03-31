#ifndef PCS_MAIN_WIDGETMAINHEADER_H
#define PCS_MAIN_WIDGETMAINHEADER_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui
{
class WidgetMainHeader;
}

class WidgetMainHeader : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMainHeader(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetMainHeader();

public:
    void setWorkshopName(const QString &name);
    void setWorklineName(const QString &name);

private:
    Ui::WidgetMainHeader *ui;
};

#endif
