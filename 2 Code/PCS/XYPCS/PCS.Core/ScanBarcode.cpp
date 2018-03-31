#include "ScanBarcode.h"

ScanBarcode::ScanBarcode(AssyProcess *process)
    : ProcessStep(process)
{

}

ScanBarcode::~ScanBarcode()
{

}

void ScanBarcode::addBarcode(const QString &barcodeType,
                             const QString &materialID,
                             const QString &stationID,
                             const QString &deviceID,
                             const QString &writeDisplay,
                             const QString &writeResult)
{
    Barcode barcode;

    barcode.barcodeType = barcodeType;
    barcode.materialID = materialID;
    barcode.stationID = stationID;
    barcode.deviceID = deviceID;
    barcode.writeDisplay = writeDisplay;
    barcode.writeResult = writeResult;
    barcode.isFinished = false;
    barcode.currentbarcode.clear();
    barcode.repairState = 0;
    barcode.assemblyID.clear();

    barcodeList.append(barcode);
}

void ScanBarcode::initStepData()
{
    qDebug() << "ScanBarcode initStepData()";

    state = StepState::Ready;

    for (auto &item : barcodeList)
    {
        item.isFinished = false;
        item.currentbarcode.clear();
        item.repairState = 0;
        item.assemblyID.clear();
    }
}

void ScanBarcode::saveStepData()
{
    qDebug() << "ScanBarcode saveStepData()";

    for (auto item : barcodeList)
    {
        if (item.barcodeType == "物料条码")
        {
            station->updateMaterialData(item.materialID, item.currentbarcode, item.repairState);
        }
    }
}

void ScanBarcode::executeStep()
{
    qDebug() << "ScanBarcode executeStep()";
}

bool ScanBarcode::receiveOPCValue(const OpcDataDefine &data)
{
    Q_UNUSED(data);
    return false;
}

bool ScanBarcode::receiveCommData(const CommDataDefine &data)
{
    if (data.itemType != "条码")
        return false;

    QString barcode = data.itemValue.toString();
    int result = 0;

    bool find = false;

    for (auto &item : barcodeList)
    {
        if (data.deviceID != item.deviceID)
            continue;

        if (item.isFinished)
            continue;

        find = true;

        if (item.barcodeType == "物料条码")
        {
            result = station->verifyMaterialBarcode(item.materialID, barcode);
            if (result == 0)
            {
                item.isFinished = true;
                item.currentbarcode = barcode;
                item.repairState = station->getRepairStateByMaterialBarcode(item.materialID, barcode);
                process->updateAssemblyState(item.repairState);
                process->appendMaterial(item.currentbarcode);

            }
            //把扫描信息发送给PLC
            writeOPCValue({item.writeDisplay, barcode});
            writeOPCValue({item.writeResult, result});
        }
        if (item.barcodeType == "过渡条码")
        {
            result = station->verifyTransferBarcode(item.stationID, barcode);
            if (result == 0)
            {
                item.isFinished = true;
                item.currentbarcode = barcode;
                item.assemblyID = station->getAssemblyIDByTransferBarcode(item.stationID, barcode);
                item.repairState = station->getRepairStateByTransferBarcode(item.stationID, barcode);
                process->updateAssemblyID(item.assemblyID);
                process->updateAssemblyState(item.repairState);
                process->appendAssemlby(item.assemblyID);
            }
            //把扫描信息发送给PLC
            writeOPCValue({item.writeDisplay, barcode});
            writeOPCValue({item.writeResult, result});
        }

        break;
    }

    if (!find)
    {
        return false;
    }

    if (result != 0)
    {
        finishStep(StepResult::Failed);
        return true;
    }

    //全部条码是否都已扫描
    if (isAllBarcodeScanned())
    {
        finishStep(StepResult::Success);
        return true;
    }

    return false;
}

bool ScanBarcode::isAllBarcodeScanned()
{
    for (auto item : barcodeList)
        if (!item.isFinished)
            return false;

    return true;
}
