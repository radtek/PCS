#ifndef PROCESSSTEP_H
#define PROCESSSTEP_H

#include "AssemblyStation.h"
#include "AssemblyProcess.h"

class ProcessStep
{
public:
    enum class StepState
    {
        Initial,
        Start,
        Running,
        Finish,
    };
    enum class StepResult
    {
        Start,          //允许启动
        Skip,           //跳过步骤
        Success,        //采集成功
        Failed,         //采集失败
    };
public:
    explicit ProcessStep(AssemblyProcess *process);
    virtual ~ProcessStep();

    inline void setStepItem(const StepItem &para) { stepItem = para; }
public:
    bool opcValueChanged(const DataDefine &data);
    bool opcReadComplete(const DataDefine &data);
    bool dacDataReceived(const DataDefine &data);

    void readOpcData(const DataDefine &data);
    void writeOpcData(const DataDefine &data);
    void writeDacData(const DataDefine &data);
public:
    virtual void initStepData() = 0;         //初始化步骤
    virtual void saveStepData() = 0;         //保存步骤数据
    virtual void executeStep() = 0;          //执行步骤
    virtual void measureData() = 0;          //采集数据
    virtual bool receiveData(const DataDefine &data) = 0;      //处理数据采集
public:
    inline void setStepType(StepType type) { stepType = type; }
    inline StepType getStepType() const { return stepType; }
    inline void setStepIndex(int index) { stepIndex = index; }
    inline int getStepIndex() const { return stepIndex; }
protected:
    AssemblyStation *station;
    AssemblyProcess *process;

    StepState currentState;             //步骤状态
    int measureCount;                   //检测次数
    StepItem stepItem;                  //步骤参数
    StepType stepType;                  //步骤类型
    int stepIndex;
};

#endif // PROCESSSTEP_H
