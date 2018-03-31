#ifndef ASSEMBLYSTATION_H
#define ASSEMBLYSTATION_H

#include "WorkStation.h"

class AssemblyProcess;

class AssemblyStation : public WorkStation
{
public:
    explicit AssemblyStation(WorkManager *manager);
    virtual ~AssemblyStation();

    virtual bool initialStation(const QString &stationID);
public:
    //扫描物料条码
    EventState verifyMaterialBarcode(const QString &materialID, const QString &barcode);
    //扫描过渡条码
    EventState verifyTransferBarcode(const QString &stationID, const QString &barcode);
    //判断采集结果
    EventState verifyMeasureValue(const QString &measureID, double value);

    ///条码比对
    //物料条码是否已经被扫描
    bool isMaterialBarcodeAlreadyScanned(const QString &matBarcode);
    //过渡条码是否已经被扫描
    bool isTransferBarcodeAlreadyScanned(const QString &assemblyID);
    //过渡条码在工位是否有相关数据
    bool isTransferBarcodeValidInPrevStation(const QString &stationID, const QString &barcode);
    //过渡条码在工位是否可用
    bool isTransferBarcodeExistInThisStation(const QString &barcode);

    ///获取条码信息
    //通过过渡条码获取总成ID
    QString getAssemblyIDByTransferBarcode(const QString &stationID, const QString &barcode);
    //通过物料条码获取返修次数
    int getRepairCountByMaterialBarcode(const QString &materialID, const QString &barcode);
    //通过过渡条码获取返修状态
    int getRepairStateByTransferBarcode(const QString &assemblyID, const QString &barcode);


    //更新物料数据
    void updateMaterialData(const QString &materialID, const QString &barcode, int count);
    //更新采集数据
    void updateMeasureData(const QString &measureID, double value, int state, int count);
    //保存工位数据
    void saveStationData(const ProcessData &processData);
public:
    virtual void opcValueChanged(const DataDefine &data);
    virtual void opcReadComplete(const DataDefine &data);
    virtual void dacDataReceived(const DataDefine &data);

    virtual void readOpcData(const DataDefine &data);
    virtual void writeOpcData(const DataDefine &data);
    virtual void writeDacData(const DataDefine &data);

    virtual EventState verifyStationStatus();
    virtual void updateStationStatus();
public:
    //流程复位
    void stationReset();
    //流程复位
    void stationAlarm();
    //流程结束
    void finishProcess(AssemblyProcess *process);
    //写入生产数据
    void opcWriteProductInfo();
    //更新数量信息
    void opcWriteOrderQuantity();
    //更新事件信息
    void opcWriteProcessPrompt(EventState state);
private:
    QList<AssemblyProcess *> processList;        //流程列表

    PreparePara preparePara;
    ProcessPara processPara;
    QList<StepPara> stepParaList;
};

#endif // ASSEMBLYSTATION_H
