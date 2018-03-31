#include "FinishProcess.h"

FinishProcess::FinishProcess(AssyProcess *process)
    : ProcessStep(process)
{

}

FinishProcess::~FinishProcess()
{

}

void FinishProcess::initStepData()
{
    qDebug() << "FinishProcess initStepData()";

    state = StepState::Ready;
}

void FinishProcess::saveStepData()
{
    qDebug() << "FinishProcess saveStepData()";
}

void FinishProcess::executeStep()
{
    qDebug() << "FinishProcess executeStep()";

    process->saveStepData();

    finishStep(StepResult::Success);
}

bool FinishProcess::receiveOPCValue(const OpcDataDefine &data)
{
    Q_UNUSED(data);
    return false;
}

bool FinishProcess::receiveCommData(const CommDataDefine &data)
{
    Q_UNUSED(data);
    return false;
}

