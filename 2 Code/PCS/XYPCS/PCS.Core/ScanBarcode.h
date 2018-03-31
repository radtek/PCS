#ifndef SCANBARCODE_H
#define SCANBARCODE_H

#include "ProcessStep.h"

class ScanBarcode : public ProcessStep
{
public:
    struct Barcode
    {
        QString barcodeType;    //条码类型
        QString materialID;      //物料ID，用于物料条码
        QString stationID;      //工位ID，用于过渡条码
        QString deviceID;       //设备ID，一般为条码枪
        QString writeDisplay; //条码显示地址
        QString writeResult;  //扫描结果地址

        bool isFinished;        //是否扫描完成

        QString currentbarcode; //当前条码
        int repairState;        //条码返修状态

        QString assemblyID;     //总成ID
    };

public:
    explicit ScanBarcode(AssyProcess *process);
    virtual ~ScanBarcode();

public:
    virtual void executeStep();                             //处理PLC反馈
    virtual void initStepData();                            //初始化步骤数据
    virtual void saveStepData();                            //保存步骤数据
    virtual bool receiveOPCValue(const OpcDataDefine &data);         //处理OPC数据采集
    virtual bool receiveCommData(const CommDataDefine &data);      //处理COM数据采集
public:
    void addBarcode(const QString &barcodeType,
                    const QString &materialID,
                    const QString &stationID,
                    const QString &deviceID,
                    const QString &writeDisplay,
                    const QString &writeResult);
private:
    bool isAllBarcodeScanned();
private:
    QList<Barcode> barcodeList;
};

#endif // SCANBARCODE_H
