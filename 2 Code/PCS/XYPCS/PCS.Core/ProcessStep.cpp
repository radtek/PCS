#include "ProcessStep.h"

ProcessStep::ProcessStep(AssemblyProcess *process)
    : station(process->getStation())
    , process(process)
    , currentState(StepState::Initial)
    , measureCount(0)
{
}

ProcessStep::~ProcessStep()
{

}

bool ProcessStep::opcValueChanged(const DataDefine &data)
{
//    qDebug() << station->getStationID() << "valueChanged()";

    //如果步骤已完成，则不处理
    if (currentState == StepState::Finish)
    {
        return false;
    }

    //装配状态已经失败 或者 步骤已被屏蔽
    if (stepItem.isDisabled)
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.skipResponse);
        process->finishStep(this);
        return true;
    }

    //上位机启动请求信号
    if (data == stepItem.startRequest)
    {
        executeStep();
        return true;
    }

    //上位机采集请求信号
    if (data == stepItem.measureRequest)
    {
        measureData();
        return true;
    }

    return false;
}

//OPC数据到达
bool ProcessStep::opcReadComplete(const DataDefine &data)
{
    if (currentState == StepState::Initial ||
        currentState == StepState::Finish)
        return false;

    return receiveData(data);
}

//DAC数据到达
bool ProcessStep::dacDataReceived(const DataDefine &data)
{
    if (currentState == StepState::Initial ||
        currentState == StepState::Finish)
        return false;

    return receiveData(data);
}

void ProcessStep::readOpcData(const DataDefine &data)
{
    station->readOpcData(data);
}

void ProcessStep::writeOpcData(const DataDefine &data)
{
    station->writeOpcData(data);
}

void ProcessStep::writeDacData(const DataDefine &data)
{
    station->writeDacData(data);
}
