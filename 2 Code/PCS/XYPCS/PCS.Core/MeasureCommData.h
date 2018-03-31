#ifndef MEASUREDEVICEDATA_H
#define MEASUREDEVICEDATA_H

#include "ProcessStep.h"

class MeasureCommData : public ProcessStep
{
public:
    struct Measure
    {
        QString deviceType;
        QString measureType;
        QString measureID;
        QString deviceID;
        QString writeDisplay;
        QString writeResult;

        bool isFinished;

        double measureValue;
        int judgeResult;
    };

public:
    explicit MeasureCommData(AssyProcess *process);
    virtual ~MeasureCommData();

public:
    virtual void executeStep();                             //处理PLC反馈
    virtual void initStepData();                            //初始化步骤数据
    virtual void saveStepData();                            //保存步骤数据
    virtual bool receiveOPCValue(const OpcDataDefine &data);         //处理OPC数据采集
    virtual bool receiveCommData(const CommDataDefine &data);      //处理COM数据采集

public:
    void addMeasure(const QString &deviceType,
                    const QString &measureType,
                    const QString &measureID,
                    const QString &deviceID,
                    const QString &writeDisplay,
                    const QString &writeResult);
private:
    bool isMeasureAllData();
    bool isMeasureSussess();
private:
    QList<Measure> measureList;
};

#endif // MEASUREDEVICEDATA_H
