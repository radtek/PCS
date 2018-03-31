#include "StepLoopEnd.h"

StepLoopEnd::StepLoopEnd(AssemblyProcess *process)
    : ProcessStep(process)
{
}

StepLoopEnd::~StepLoopEnd()
{
}

void StepLoopEnd::initStepData()
{
    qDebug() << QString("[%1] [Process %2] StepLoopEnd initStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    currentState = StepState::Initial;
    measureCount = 0;
}

void StepLoopEnd::saveStepData()
{
    qDebug() << QString("[%1] [Process %2] StepLoopEnd saveStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

void StepLoopEnd::executeStep()
{
    qDebug() << QString("[%1] [Process %2] StepLoopEnd executeStep()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    //如果步骤屏蔽则跳过此步
    if (stepItem.isDisabled)
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.skipResponse);
        process->finishStep(this);
        return;
    }

    EventState state = process->verifyProcessResult();
    station->opcWriteProcessPrompt(state);

    process->updateFinishTime();
    process->saveStepData();

    currentState = StepState::Finish;
    writeOpcData(stepItem.startResponse);
    process->finishStep(this);
}

void StepLoopEnd::measureData()
{
    qDebug() << QString("[%1] [Process %2] StepLoopEnd measureData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

bool StepLoopEnd::receiveData(const DataDefine &data)
{
    Q_UNUSED(data);
    return false;
}
