#include "StepLoopStart.h"

StepLoopStart::StepLoopStart(AssemblyProcess *process)
    : ProcessStep(process)
{
}

StepLoopStart::~StepLoopStart()
{
}


void StepLoopStart::initStepData()
{
    qDebug() << QString("[%1] [Process %2] StepLoopStart initStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    currentState = StepState::Initial;
    measureCount = 0;
}

void StepLoopStart::saveStepData()
{
    qDebug() << QString("[%1] [Process %2] StepLoopStart saveStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

void StepLoopStart::executeStep()
{
    qDebug() << QString("[%1] [Process %2] StepLoopStart executeStep()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    //如果步骤屏蔽则跳过此步
    if (stepItem.isDisabled)
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.skipResponse);
        process->finishStep(this);
        return;
    }

    EventState state = station->verifyStationStatus();

    if (state == EventState::E_General_Successfully)
    {
        process->updateCreateTime();

        currentState = StepState::Finish;
        writeOpcData(stepItem.startResponse);
        process->finishStep(this);

        return;
    }

    currentState = StepState::Start;
    station->opcWriteProcessPrompt(state);
}

void StepLoopStart::measureData()
{
    qDebug() << QString("[%1] [Process %2] StepLoopStart measureData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

bool StepLoopStart::receiveData(const DataDefine &data)
{
    if (currentState != StepState::Start)
        return false;

    QString barcode = data.itemValue.toString();

    //员工登陆
    if (!station->verifyOperatorLogin())
    {
        station->operatorLogin(barcode);
        return true;
    }

    //物料上料
    if (!station->verifyMaterialInput() ||
        !station->verifyMaterialUseup())
    {
        station->materialInput(barcode);
        return true;
    }

    return false;
}
