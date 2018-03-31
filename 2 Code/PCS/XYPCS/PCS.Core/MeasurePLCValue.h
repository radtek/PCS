#ifndef MEASUREPLCADDRESS_H
#define MEASUREPLCADDRESS_H

#include "ProcessStep.h"

class MeasurePLCValue : public ProcessStep
{
public:
    struct Measure
    {
        QString measureID;
        QString readMeasure;
        QString writeDisplay;
        QString writeResult;

        bool isFinished;

        double measureValue;
        int judgeResult;
    };
public:
    explicit MeasurePLCValue(AssyProcess *process);
    virtual ~MeasurePLCValue();

public:
    virtual void executeStep();                             //处理PLC反馈
    virtual void initStepData();                            //初始化步骤数据
    virtual void saveStepData();                            //保存步骤数据
    virtual bool receiveOPCValue(const OpcDataDefine &data);         //处理OPC数据采集
    virtual bool receiveCommData(const CommDataDefine &data);      //处理COM数据采集
public:
    void addMeasure(const QString &measureID,
                    const QString &readMeasure,
                    const QString &writeDisplay,
                    const QString &writeResult);
private:
    bool isMeasureAllData();
    bool isMeasureSussess();
private:
    QList<Measure> measureList;
};

#endif // MEASUREPLCADDRESS_H
