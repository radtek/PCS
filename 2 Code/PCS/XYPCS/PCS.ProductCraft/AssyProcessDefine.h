#ifndef ASSYPROCESSDEFINE_H
#define ASSYPROCESSDEFINE_H

#include <QString>
#include <QList>

struct OpcData
{
    QString itemID;
    QString value;
};

struct StartProcessItem
{
    bool verifyOperatorLog;
    bool verifyMaterialFeed;
    bool verifyDeviceCheck;
    bool verifyProductCheck;
};

struct EndProcessItem
{
    bool saveStationData;
    bool saveMaterialData;
    bool saveMeasureData;
};

struct ScanBarcodeItem
{
    QString barcodeType;
    QString materialID;
    QString stationID;
    QString deviceID;
    OpcData writeDisplay;
    OpcData writeResult;
};

struct MeasComDataItem
{
    QString deviceType;
    QString measureType;
    QString measureID;
    QString deviceID;
    OpcData writeDisplay;
    OpcData writeResult;
};

struct MeasOpcDataItem
{
    QString measureID;
    OpcData readMeasure;
    OpcData writeDisplay;
    OpcData writeResult;
};

struct StepData
{
    QString description;
    QString interaction;

    OpcData startRequest;
    OpcData skipRequest;
    OpcData successResponse;
    OpcData failedResponse;
    OpcData skipResponse;

    StartProcessItem startItem;
    EndProcessItem endItem;
    QList<ScanBarcodeItem> barcodeItems;
    QList<MeasComDataItem> comDataItems;
    QList<MeasOpcDataItem> opcDataItems;
};

struct StationData
{
    int processCount;

    OpcData writeCraftID;
    OpcData writeOrderID;
    OpcData writeFinishCount;
    OpcData writeFailedCount;

    OpcData resetRequest;
    OpcData alarmRequest;
    OpcData writeFormulaID;
};

#endif // ASSYPROCESSDEFINE_H
