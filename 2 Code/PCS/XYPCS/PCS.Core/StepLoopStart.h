#ifndef STEPLOOPSTART_H
#define STEPLOOPSTART_H

#include "ProcessStep.h"

class StepLoopStart : public ProcessStep
{
public:
    explicit StepLoopStart(AssemblyProcess *process);
    virtual ~StepLoopStart();

    inline void setStepItem(const LoopStartItem &item) { startItem = item; }
public:
    virtual void initStepData();         //初始化步骤
    virtual void saveStepData();         //保存步骤数据
    virtual void executeStep();          //执行步骤
    virtual void measureData();          //采集数据
    virtual bool receiveData(const DataDefine &data);      //处理数据采集
private:
    LoopStartItem startItem;
};

#endif // STEPLOOPSTART_H
