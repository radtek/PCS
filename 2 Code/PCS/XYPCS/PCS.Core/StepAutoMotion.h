#ifndef STEPAUTOMOTION_H
#define STEPAUTOMOTION_H

#include "ProcessStep.h"

class StepAutoMotion : public ProcessStep
{
public:
    explicit StepAutoMotion(AssemblyProcess *process);
    virtual ~StepAutoMotion();

    inline void setStepItem(const AutoMotionItem &item) { motionItem = item; }
public:
    virtual void initStepData();         //初始化步骤
    virtual void saveStepData();         //保存步骤数据
    virtual void executeStep();          //执行步骤
    virtual void measureData();          //采集数据
    virtual bool receiveData(const DataDefine &data);      //处理数据采集
private:
    AutoMotionItem motionItem;
};

#endif // STEPAUTOMOTION_H
