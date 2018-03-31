#include "MeasureCommData.h"

MeasureCommData::MeasureCommData(AssyProcess *process)
    : ProcessStep(process)
{
}

MeasureCommData::~MeasureCommData()
{
}

void MeasureCommData::addMeasure(const QString &deviceType,
                                 const QString &measureType,
                                 const QString &measureID,
                                 const QString &deviceID,
                                 const QString &writeDisplay,
                                 const QString &writeResult)
{
    Measure measure;

    measure.deviceType = deviceType;
    measure.measureType = measureType;
    measure.measureID = measureID;
    measure.deviceID = deviceID;
    measure.writeDisplay = writeDisplay;
    measure.writeResult = writeResult;
    measure.isFinished = false;
    measure.measureValue = 0.0;
    measure.judgeResult = 0;

    measureList.append(measure);
}

void MeasureCommData::initStepData()
{
    qDebug() << "MeasureDeviceData initStepData()";

    state = StepState::Ready;

    for (auto &item : measureList)
    {
        item.measureValue = 0.0;
        item.judgeResult = 0;
        item.isFinished = false;
    }
}

void MeasureCommData::saveStepData()
{
    qDebug() << "MeasureDeviceData saveStepData()";

    for (auto item : measureList)
    {
        station->updateMeasureData(item.measureID, item.measureValue, item.judgeResult);
    }
}

void MeasureCommData::executeStep()
{
    qDebug() << "MeasureDeviceData executeStep()";

    for (auto item : measureList)
    {
        CommDataDefine data;

        data.deviceID = item.deviceID;
        data.deviceType = item.deviceType;
        data.itemType = item.measureType;
        data.itemValue = QVariant();

        writeCommData(data);
    }
}

bool MeasureCommData::receiveOPCValue(const OpcDataDefine &data)
{
    Q_UNUSED(data);

    return false;
}

bool MeasureCommData::receiveCommData(const CommDataDefine &data)
{
    bool find = false;

    for (auto &item : measureList)
    {
        if (data.deviceID != item.deviceID ||
            data.deviceType != item.deviceType ||
            data.itemType != item.measureType)
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

bool MeasureCommData::isMeasureAllData()
{
    for (auto item : measureList)
        if (!item.isFinished)
            return false;

    return true;
}

bool MeasureCommData::isMeasureSussess()
{
    for (auto item : measureList)
        if (item.judgeResult)
            return false;

    return true;
}


