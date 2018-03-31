#include "StepScanBarcode.h"

StepScanBarcode::StepScanBarcode(AssemblyProcess *process)
    : ProcessStep(process)
{
}

StepScanBarcode::~StepScanBarcode()
{
}

void StepScanBarcode::initStepData()
{
    qDebug() << QString("[%1] [Process %2] StepScanBarcode initStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    currentState = StepState::Initial;
    measureCount = 0;

    for (auto &item : itemList)
        item.initial();
}

void StepScanBarcode::saveStepData()
{
    qDebug() << QString("[%1] [Process %2] StepScanBarcode saveStepData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    for (auto &item : itemList)
    {
        if (!item.isDisabled && !item.barcodeID.isEmpty() && item.isFinished
            && item.barcodeType == BarcodeType::Material)
        {
            station->updateMaterialData(item.barcodeID,
                                        item.barcodeText,
                                        item.repairCount);
        }
    }
}

void StepScanBarcode::executeStep()
{
    qDebug() << QString("[%1] [Process %2] StepScanBarcode executeStep()")
             .arg(station->getStationID()).arg(process->getProcessIndex());

    //如果步骤屏蔽则跳过此步
    if (stepItem.isDisabled)
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.skipResponse);
        process->finishStep(this);
        return;
    }

    //如果条码列表为空则直接反馈成功
    if (isAllBarcodeScanned())
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.startResponse);
        process->finishStep(this);
        return;
    }

    currentState = StepState::Start;

    station->opcWriteProcessPrompt(EventState::E_General_ScanBarcode);
}

void StepScanBarcode::measureData()
{
    qDebug() << QString("[%1] [Process %2] StepScanBarcode measureData()")
             .arg(station->getStationID()).arg(process->getProcessIndex());
}

bool StepScanBarcode::receiveData(const DataDefine &data)
{
    if (currentState != StepState::Start)
        return false;

    bool find = false;

    for (auto &item : itemList)
    {
        if (item.isDisabled)
            continue;

        if (item.isFinished)
            continue;

        if (data.itemID != item.readBarcode.itemID)
            continue;

        find = true;

        QString barcode = data.itemValue.toString();
        EventState state = EventState::E_ScanMbar_Successfully;
        bool result = false;

        switch (item.barcodeType)
        {
        case BarcodeType::Material:
            state = station->verifyMaterialBarcode(item.barcodeID, barcode);
            result = (state == EventState::E_ScanMbar_Successfully);

            if (result)
            {
                item.isFinished = true;
                item.barcodeText = barcode;
                item.repairCount = station->getRepairCountByMaterialBarcode(item.barcodeID, barcode);

                if (item.repairCount > 0 && process->verifyRepairState())
                    process->updateRepairState();

                process->appendMaterial(item.barcodeText);
            }

            break;

        case BarcodeType::Transfer:
            state = station->verifyTransferBarcode(item.barcodeID, barcode);
            result = (state == EventState::E_ScanTbar_Successfully);

            if (result)
            {
                item.isFinished = true;
                item.barcodeText = barcode;
                item.assemblyID = station->getAssemblyIDByTransferBarcode(item.barcodeID, barcode);

                process->updateAssemblyID(item.assemblyID);

                process->appendAssemlby(item.assemblyID);
            }

            break;
        }

        //把扫描信息发送给PLC
        item.writeDisplay.itemValue = barcode;
        item.writeResult.itemValue = result ? item.resultOK : item.resultNG;

        writeOpcData(item.writeDisplay);
        writeOpcData(item.writeResult);

        station->opcWriteProcessPrompt(state);
        break;
    }

    if (!find)
    {
        return false;
    }

    if (isAllBarcodeScanned())
    {
        currentState = StepState::Finish;
        writeOpcData(stepItem.startResponse);
        process->finishStep(this);
    }

    return true;
}

bool StepScanBarcode::isAllBarcodeScanned()
{
    for (auto item : itemList)
        if (!item.isDisabled && !item.isFinished)
            return false;

    return true;
}
