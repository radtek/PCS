#include "MeasurePLCValue.h"

MeasurePLCValue::MeasurePLCValue(AssyProcess *process)
    : ProcessStep(process)
{

}

MeasurePLCValue::~MeasurePLCValue()
{

}

void MeasurePLCValue::addMeasure(const QString &measureID,
                                 const QString &readMeasure,
                                 const QString &writeDisplay,
                                 const QString &writeResult)
{
    Measure measure;

    measure.measureID = measureID;
    measure.readMeasure = readMeasure;
    measure.writeDisplay = writeDisplay;
    measure.writeResult = writeResult;
    measure.isFinished = false;
    measure.measureValue = 0.0;
    measure.judgeResult = 0;

    measureList.append(measure);
}

void MeasurePLCValue::initStepData()
{
    qDebug() << "MeasurePLCAddress initStepData()";

    state = StepState::Ready;

    for (auto &item : measureList)
    {
        item.measureValue = 0.0;
        item.judgeResult = 0;
        item.isFinished = false;
    }
}

void MeasurePLCValue::saveStepData()
{
    qDebug() << "MeasurePLCAddress saveStepData()";

    for (auto item : measureList)
    {
        station->updateMeasureData(item.measureID, item.measureValue, item.judgeResult);
    }
}

void MeasurePLCValue::executeStep()
{
    qDebug() << "MeasurePLCAddress executeStep()";

    for (auto item : measureList)
    {
        readOPCValue({item.readMeasure, QVariant()});
    }
}

bool MeasurePLCValue::receiveOPCValue(const OpcDataDefine &data)
{
    bool find = false;

    for (auto &item : measureList)
    {
        if (data.itemID != item.readMeasure)
            continue;

        find = true;

        item.measureValue = data.itemValue.toDouble();
        item.judgeResult = station->verifyMeasureValue(item.measureID, item.measureValue);
        item.isFinished = true;

        //把采集判断结果发送给PLC
        writeOPCValue({item.writeDisplay, item.measureValue});
        writeOPCValue({item.writeResult, item.judgeResult == 0});
    }

    if (!find)
    {
        return false;
    }

    if (isMeasureAllData())
    {
        finishStep(isMeasureSussess() ? StepResult::Success : StepResult::Failed);
        return true;
    }

    return false;
}

bool MeasurePLCValue::receiveCommData(const CommDataDefine &data)
{
    Q_UNUSED(data);
    return false;
}

bool MeasurePLCValue::isMeasureAllData()
{
    for (auto item : measureList)
        if (!item.isFinished)
            return false;

    return true;
}

bool MeasurePLCValue::isMeasureSussess()
{
    for (auto item : measureList)
        if (item.judgeResult)
            return false;

    return true;
}
