#include "StepAutoMotion.h"

StepAutoMotion::StepAutoMotion(AssemblyProcess *process)
    : ProcessStep(process)
{
}

StepAutoMotion::~StepAutoMotion()
{
}

void StepAutoMotion::initStepData()
{
    qDebug() << QString("[%1] [Process %2] StepAutoMotion initStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    currentState = StepState::Initial;
    measureCount = 0;
}

void StepAutoMotion::saveStepData()
{
    qDebug() << QString("[%1] [Process %2] StepAutoMotion saveStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

void StepAutoMotion::executeStep()
{
    qDebug() << QString("[%1] [Process %2] StepAutoMotion executeStep()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    //如果步骤屏蔽、组装不合格则跳过此步
    if (stepItem.isDisabled || !process->verifyAssemblyState())
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.skipResponse);
        process->finishStep(this);
    }
    else
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.startResponse);
        process->finishStep(this);
    }
}

void StepAutoMotion::measureData()
{
    qDebug() << QString("[%1] [Process %2] StepAutoMotion measureData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

bool StepAutoMotion::receiveData(const DataDefine &data)
{
    Q_UNUSED(data);
    return false;
}
