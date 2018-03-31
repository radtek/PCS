#include "StepMeasureData.h"

StepMeasureData::StepMeasureData(AssemblyProcess *process)
    : ProcessStep(process)
{
}

StepMeasureData::~StepMeasureData()
{
}

void StepMeasureData::initStepData()
{
    qDebug() << QString("[%1] [Process %2] StepMeasureData initStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    currentState = StepState::Initial;
    measureCount = 0;

    for (auto &item : itemList)
        item.initial();
}

void StepMeasureData::saveStepData()
{
    qDebug() << QString("[%1] [Process %2] StepMeasureData saveStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    for (auto item : itemList)
    {
        if (!item.isDisabled && !item.measureID.isEmpty() && item.isFinished)
        {
            station->updateMeasureData(item.measureID,
                                       item.measureValue,
                                       item.measureState,
                                       item.retestCount);
        }
    }
}

void StepMeasureData::executeStep()
{
    qDebug() << QString("[%1] [Process %2] StepMeasureData executeStep()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    //如果步骤屏蔽、采集列表为空、组装不合格则跳过此步
    if (stepItem.isDisabled || !process->verifyAssemblyState())
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.skipResponse);
        process->finishStep(this);
    }
    else
    {
        currentState = StepState::Start;
        writeOpcData(stepItem.startResponse);
    }
}

void StepMeasureData::measureData()
{
    qDebug() << QString("[%1] [Process %2] StepMeasureData measureData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    //如果采集列表为空则直接反馈成功
    if (isMeasureAllData())
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.successResponse);
        process->finishStep(this);
        return;
    }

    currentState = StepState::Running;

    for (auto item : itemList)
    {
        switch (item.measureType)
        {
        case MeasureType::ByDAC:
            writeDacData(item.readMeasure);
            break;

        case MeasureType::ByOPC:
            readOpcData(item.readMeasure);
            break;
        }
    }
}

bool StepMeasureData::receiveData(const DataDefine &data)
{
    if (currentState != StepState::Running)
        return false;

    bool find = false;

    for (auto &item : itemList)
    {
        if (item.isDisabled)
            continue;

        if (data.itemID != item.readMeasure.itemID)
            continue;

        find = true;

        double value = data.itemValue.toDouble();
        EventState state = EventState::E_General_Successfully;
        bool result = false;

        value = value - item.offsetValue.toDouble();
        state = station->verifyMeasureValue(item.measureID, value);
        result = (state == EventState::E_General_Successfully);

        item.measureValue = value;
        item.measureState = result ? 0 : 1;

        if (item.isFinished)
            item.retestCount++;
        else
            item.isFinished = true;

        //更新流程状态
        if (item.retestCount > 0 && process->verifyRetestState())
            process->updateRetestState();

        //把采集判断结果发送给PLC
        item.writeDisplay.itemValue = value;
        item.writeResult.itemValue = result ? item.resultOK : item.resultNG;

        writeOpcData(item.writeDisplay);
        writeOpcData(item.writeResult);
        break;
    }

    if (!find)
    {
        return false;
    }

    if (!isMeasureAllData())
    {
        return true;
    }
    else
    {
        measureCount++;
    }

    if (isMeasureSussess())
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.successResponse);
        process->finishStep(this);
    }
    else
    {
        if (measureCount >= stepItem.maxMeasure)
            process->updateAssemblyState();

        currentState = StepState::Initial;
        writeOpcData(stepItem.failedResponse);
    }

    return true;
}

bool StepMeasureData::isMeasureAllData()
{
    for (auto item : itemList)
        if (!item.isDisabled && !item.isFinished)
            return false;

    return true;
}

bool StepMeasureData::isMeasureSussess()
{
    for (auto item : itemList)
        if (item.measureState != 0)
            return false;

    return true;
}


