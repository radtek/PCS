#ifndef WIDGETASSYPROCESSPRODUCE_H
#define WIDGETASSYPROCESSPRODUCE_H

#include "WorkManager.h"

namespace Ui
{
class WidgetAssyProcessProduce;
}

class WidgetAssyProcessProduce : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAssyProcessProduce(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetAssyProcessProduce();

public slots:
    void loadParameter(const QString &craftID);
    void saveParameter();

private:
    void initialWidget();
    void setWidgetData();
    void getWidgetData();

private:
    Ui::WidgetAssyProcessProduce *ui;

    QString craftID;

    ProducePara producePara;
};

#endif    // WIDGETASSYPROCESSPRODUCE_H
