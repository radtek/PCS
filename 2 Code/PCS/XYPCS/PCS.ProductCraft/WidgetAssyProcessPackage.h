#ifndef WIDGETASSYPROCESSPACKAGE_H
#define WIDGETASSYPROCESSPACKAGE_H

#include "WorkManager.h"

namespace Ui
{
class WidgetAssyProcessPackage;
}

class WidgetAssyProcessPackage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAssyProcessPackage(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetAssyProcessPackage();

public slots:
    void loadParameter(const QString &craftID, const QString &stationID);
    void saveParameter();

    void slotAssemblyToggled(bool checked);
    void slotPackageToggled(bool checked);

private:
    void initialWidget();
    void setWidgetData();
    void getWidgetData();

private:
    Ui::WidgetAssyProcessPackage *ui;

    QString craftID;
    QString stationID;

    QStringList deviceIDList;
    QStringList stationIDList;

    PreparePara preparePara;
    AssemblyPara assemblyPara;
    PackagePara packagePara;
};

#endif    // WIDGETASSYPROCESSPACKAGE_H
