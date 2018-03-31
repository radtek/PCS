#ifndef STARTASSEMBLYPROCESS_H
#define STARTASSEMBLYPROCESS_H

#include "ProcessStep.h"

class StartProcess : public ProcessStep
{
public:
    explicit StartProcess(AssyProcess *process);
    virtual ~StartProcess();

public:
    virtual void executeStep();                            //处理PLC反馈
    virtual void initStepData();                            //初始化步骤数据
    virtual void saveStepData();                            //保存步骤数据
    virtual bool receiveOPCValue(const OpcDataDefine &data);         //处理OPC数据采集
    virtual bool receiveCommData(const CommDataDefine &data);      //处理COM数据采集
private:

};

#endif // STARTASSEMBLYPROCESS_H
