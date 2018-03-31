#ifndef STEPSCANBARCODE_H
#define STEPSCANBARCODE_H

#include "ProcessStep.h"

class StepScanBarcode : public ProcessStep
{
public:
    explicit StepScanBarcode(AssemblyProcess *process);
    virtual ~StepScanBarcode();

    void setStepItemList(const QList<ScanBarcodeItem> &list)  { itemList = list; }
public:
    virtual void initStepData();         //初始化步骤
    virtual void saveStepData();         //保存步骤数据
    virtual void executeStep();          //执行步骤
    virtual void measureData();          //采集数据
    virtual bool receiveData(const DataDefine &data);      //处理数据采集
private:
    bool isAllBarcodeScanned();
private:
    QList<ScanBarcodeItem> itemList;
};

#endif // STEPSCANBARCODE_H
