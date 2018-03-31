#ifndef STEPLOOPEND_H
#define STEPLOOPEND_H

#include "ProcessStep.h"

class StepLoopEnd : public ProcessStep
{
public:
    explicit StepLoopEnd(AssemblyProcess *process);
    virtual ~StepLoopEnd();

    inline void setStepItem(const LoopEndItem &item) { endItem = item; }
public:
    virtual void initStepData();         //初始化步骤
    virtual void saveStepData();         //保存步骤数据
    virtual void executeStep();          //执行步骤
    virtual void measureData();          //采集数据
    virtual bool receiveData(const DataDefine &data);      //处理数据采集
private:
    LoopEndItem endItem;
};

#endif // STEPLOOPEND_H
