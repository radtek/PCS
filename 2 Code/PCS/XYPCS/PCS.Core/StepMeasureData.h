#ifndef STEPMEASDACDATA_H
#define STEPMEASDACDATA_H

#include "ProcessStep.h"

class StepMeasureData : public ProcessStep
{
public:
    explicit StepMeasureData(AssemblyProcess *process);
    virtual ~StepMeasureData();

    inline void setStepItemList(const QList<MeasureDataItem> &list) { itemList = list; }
public:
    virtual void initStepData();         //初始化步骤
    virtual void saveStepData();         //保存步骤数据
    virtual void executeStep();          //执行步骤
    virtual void measureData();          //采集数据
    virtual bool receiveData(const DataDefine &data);      //处理数据采集
private:
    bool isMeasureAllData();
    bool isMeasureSussess();
private:
    QList<MeasureDataItem> itemList;
};

#endif // STEPMEASDACDATA_H
