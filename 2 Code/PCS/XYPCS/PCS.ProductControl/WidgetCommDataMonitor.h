#ifndef WIDGETCOMMDATAMONITOR_H
#define WIDGETCOMMDATAMONITOR_H

#include "WorkManager.h"

namespace Ui
{
class WidgetCommDataMonitor;
}

class WidgetCommDataMonitor : public QWidget
{
    Q_OBJECT

public:
    enum class OpcDataHeader
    {
        StationID,
        ItemID,
        ItemValue,
        UpdateTime,
        Operation,
    };

    enum class ComDataHeader
    {
        StationID,
        ItemID,
        ItemValue,
        UpdateTime,
        Operation,
    };

    enum class LinkStatusHeader
    {

    };

public:
    explicit WidgetCommDataMonitor(QWidget *parent = 0);
    virtual ~WidgetCommDataMonitor();

public slots:
    void slotOpcDataReceived(const QString &stationID, const DataDefine &data);
    void slotDacDataReceived(const QString &stationID, const DataDefine &data);

    void slotOpcReadData(int row, int column);
    void slotDacReadData(int row, int column);
signals:
    void signalOpcReadData(const QString &stationID, const DataDefine &data);
    void signalDacReadData(const QString &stationID, const DataDefine &data);

private:
    void initialWidget();
    void initialRecord();

private:
    Ui::WidgetCommDataMonitor *ui;

    QMap<OpcDataHeader, QString> opcDataHeaderMap;
    QMap<ComDataHeader, QString> comDataHeaderMap;
};

#endif    // WIDGETCOMMDATAMONITOR_H
