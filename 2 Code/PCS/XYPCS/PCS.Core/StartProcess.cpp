#include "StartProcess.h"

StartProcess::StartProcess(AssyProcess *process)
    : ProcessStep(process)
{

}

StartProcess::~StartProcess()
{

}


void StartProcess::initStepData()
{
    qDebug() << "StartProcess initStepData()";

    state = StepState::Ready;
}

void StartProcess::saveStepData()
{
    qDebug() << "StartProcess saveStepData()";
}

void StartProcess::executeStep()
{
    qDebug() << "StartProcess executeStep()";

    bool result = station->verifyStationData();     //校验工位数据

    finishStep(result ? StepResult::Success : StepResult::Failed);
}

bool StartProcess::receiveOPCValue(const OpcDataDefine &data)
{
    Q_UNUSED(data);
    return false;
}

bool StartProcess::receiveCommData(const CommDataDefine &data)
{
    Q_UNUSED(data);
    return false;
}
