#include "AssemblyStation.h"
#include "AssemblyProcess.h"
#include "ProcessStep.h"

AssemblyStation::AssemblyStation(WorkManager *manager)
    : WorkStation(manager)
{

}

AssemblyStation::~AssemblyStation()
{
    for (auto process : processList)
    {
        delete process;
    }

    processList.clear();
}

bool AssemblyStation::initialStation(const QString &stationID)
{
    do
    {
        if (!WorkStation::initialStation(stationID))
            return false;
    }
    while (0);

    do
    {
        QString filename = QString("%1/%2.%3.xml")
                           .arg(PROCESS_ASSEMBLY_PATH)
                           .arg(craft->getCraftID())
                           .arg(stationID);

        if (!AssyStation_ReadXmlFile(filename, preparePara, processPara, stepParaList))
            return false;

        for (int i = 0; i < processPara.processNumber; ++i)
        {
            AssemblyProcess *process = new AssemblyProcess(this);

            processList.append(process);

            if (!process->loadProcess(stepParaList))
                return false;

            process->setProcessIndex(i);
        }
    }
    while (0);

    do
    {
        opcWriteProductInfo();
        opcWriteOrderQuantity();
        opcWriteProcessPrompt(EventState::E_General_ResetMachine);
    }
    while (0);

    return true;
}

//扫描物料条码
EventState AssemblyStation::verifyMaterialBarcode(const QString &materialID, const QString &barcode)
{
    auto material = getMaterial(materialID);

    //判断条码是否符合规则
    if (!material->verifyBarcodeFormat(barcode))
    {
        qDebug() << "verifyMaterialBarcode() verify barcode format failed!";
        return EventState::E_ScanMbar_VerifyFormatFailed;
    }

    //判断条码是否唯一
    if (!material->verifyBarcodeUnique(barcode))
    {
        qDebug() << "verifyMaterialBarcode() verify barcode is not unique!";
        return EventState::E_ScanMbar_VerifyUniqueFailed;
    }

    //当前工位是否已经扫描过此物料条码
    if (isMaterialBarcodeAlreadyScanned(barcode))
    {
        qDebug() << "verifyMaterialBarcode() barcode is already scanned in station!";
        return EventState::E_ScanMbar_AlreadyInUse;
    }

    return EventState::E_ScanMbar_Successfully;
}
//扫描过渡条码
EventState AssemblyStation::verifyTransferBarcode(const QString &stationID, const QString &barcode)
{
    //本工位是否有相关数据
    if (isTransferBarcodeExistInThisStation(barcode))
    {
        qWarning() << "verifyTransferBarcode() barcode is already in current station!";
        return EventState::E_ScanTbar_ThisStationHasData;
    }

    //上工位是否有相关数据
    if (!isTransferBarcodeValidInPrevStation(stationID, barcode))
    {
        qWarning() << "verifyTransferBarcode() barcode is not exist in previous station!";
        return EventState::E_ScanTbar_PrevStationNonData;
    }

    QString assemblyID = getAssemblyIDByTransferBarcode(stationID, barcode);

    //当前工位是否已经扫描过此过渡条码
    if (isTransferBarcodeAlreadyScanned(assemblyID))
    {
        qWarning() << "verifyTransferBarcode() barcode is already scanned in station!";
        return EventState::E_ScanTbar_AlreadyInUse;
    }

    return EventState::E_ScanTbar_Successfully;
}

//判断采集结果
EventState AssemblyStation::verifyMeasureValue(const QString &measureID, double value)
{
    auto measure = getMeasure(measureID);
    return measure->verifyValue(value)
           ? EventState::E_General_Successfully
           : EventState::E_General_Failed;
}


//物料条码是否已经被扫描
bool AssemblyStation::isMaterialBarcodeAlreadyScanned(const QString &matBarcode)
{
    for (auto process : processList)
        if (process->findMaterial(matBarcode))
            return true;

    return false;
}

//过渡条码是否已经被扫描
bool AssemblyStation::isTransferBarcodeAlreadyScanned(const QString &assemblyID)
{
    for (auto process : processList)
        if (process->findAssembly(assemblyID))
            return true;

    return false;
}

//过渡条码在工位是否有相关数据
bool AssemblyStation::isTransferBarcodeValidInPrevStation(const QString &stationID, const QString &barcode)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare("SELECT AssemblyState FROM PCS_Data_Transfer "
                  "WHERE OrderID = ? AND StationID = ? AND TransferBarcode = ? AND AssemblyState = 0");
    query.addBindValue(order->getOrderID());
    query.addBindValue(stationID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    return query.first();
}

//过渡条码在本工位是否存在
bool AssemblyStation::isTransferBarcodeExistInThisStation(const QString &barcode)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare("SELECT UID FROM PCS_Data_Transfer "
                  "WHERE OrderID = ? AND StationID = ? AND TransferBarcode = ?");
    query.addBindValue(order->getOrderID());
    query.addBindValue(stationData.stationID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    return query.first();
}

//通过过渡条码获取总成ID
QString AssemblyStation::getAssemblyIDByTransferBarcode(const QString &stationID, const QString &barcode)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare("SELECT AssemblyID FROM PCS_Data_Transfer "
                  "WHERE OrderID = ? AND StationID = ? AND TransferBarcode = ?");
    query.addBindValue(order->getOrderID());
    query.addBindValue(stationID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return QString();
    }

    if (query.first())
    {
        return query.value("AssemblyID").toString();
    }

    return QString();
}

//TODO 通过物料条码获取返修次数
int AssemblyStation::getRepairCountByMaterialBarcode(const QString &materialID, const QString &barcode)
{
    return 0;
}

//通过过渡条码获取返修状态
int AssemblyStation::getRepairStateByTransferBarcode(const QString &assemblyID, const QString &barcode)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare("SELECT RepairState FROM PCS_Data_Transfer "
                  "WHERE OrderID = ? AND AssemblyID = ? AND TransferBarcode = ?");
    query.addBindValue(order->getOrderID());
    query.addBindValue(assemblyID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return 0;
    }

    if (query.first())
    {
        return query.value("RepairState").toInt();
    }

    return 0;
}

//保存工位数据
void AssemblyStation::saveStationData(const ProcessData &processData)
{
    ///***************************************************************************
    ///更新过渡条码数据
    ///***************************************************************************
    {
        //更新过渡条码
        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("UPDATE PCS_Data_Transfer SET "
                          "StationID = ?, AssemblyState = ?, RepairState = ?, RetestState = ? "
                          "WHERE AssemblyID = ? AND OrderID = ?");
            query.addBindValue(stationData.stationID);
            query.addBindValue(processData.assemblyState);
            query.addBindValue(processData.repairState);
            query.addBindValue(processData.retestState);
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);

        //保存过渡条码
        for (auto material : materialList)
        {
            const MaterialData &materialData = material->getMaterialData();

            if (materialData.materialBarcode.isEmpty())
                continue;

            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Transfer "
                          "(AssemblyID, OrderID, StationID, TransferBarcode, AssemblyState, "
                          "RepairState, RetestState, CreateTime, FinishTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(materialData.materialBarcode);
            query.addBindValue(processData.assemblyState);
            query.addBindValue(processData.repairState);
            query.addBindValue(processData.retestState);
            query.addBindValue(processData.createTime);
            query.addBindValue(processData.finishTime);

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
    }

    ///***************************************************************************
    ///更新总成条码数据
    ///***************************************************************************
    {
        for (auto assemblyID : processData.subAssemblyIDs)
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("UPDATE PCS_Data_Assembly_Relation SET "
                          "AssemblyID = ? WHERE AssemblyID = ?");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(assemblyID);

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }

        for (auto assemblyID : processData.subAssemblyIDs)
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Assembly_Relation "
                          "(AssemblyID, SubAssemblyID) VALUES(?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(assemblyID);

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
    }

    ///***************************************************************************
    ///保存数据至车间服务器
    ///***************************************************************************
    if (qApplication->isOnline())
    {
        //保存工位数据
        do
        {
            QSqlQuery query(REMOTE_DB);
            query.prepare("INSERT INTO PCS_Data_Station "
                          "(AssemblyID, OrderID, StationID, OperatorID, AssemblyState, "
                          "RepairState, RetestState, CreateTime, FinishTime, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(getOperatorID());
            query.addBindValue(processData.assemblyState);
            query.addBindValue(processData.repairState);
            query.addBindValue(processData.retestState);
            query.addBindValue(processData.createTime);
            query.addBindValue(processData.finishTime);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);

        //保存物料数据
        for (auto material : materialList)
        {
            const MaterialData &materialData = material->getMaterialData();

            QSqlQuery query(REMOTE_DB);
            query.prepare("INSERT INTO PCS_Data_Station_Material "
                          "(AssemblyID, OrderID, StationID, MaterialID, MaterialBarcode, "
                          "MaterialBatch, RepairCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(materialData.materialID);
            query.addBindValue(materialData.materialBarcode);
            query.addBindValue(materialData.materialBatch);
            query.addBindValue(materialData.repairCount);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }

        //保存采集数据
        for (auto measure : measureList)
        {
            const MeasureData &measureData = measure->getMeasureData();

            QSqlQuery query(REMOTE_DB);
            query.prepare("INSERT INTO PCS_Data_Station_Measure "
                          "(AssemblyID, OrderID, StationID, MeasureType, MeasureOrder, "
                          "MeasureValue, MeasureState, RetestCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(measureData.measureID.split('#').at(0));
            query.addBindValue(measureData.measureID.split('#').at(1));
            query.addBindValue(measureData.measureValue);
            query.addBindValue(measureData.measureState);
            query.addBindValue(measureData.retestCount);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
    }

    ///***************************************************************************
    ///保存数据至本地数据库
    ///***************************************************************************
    if (1)
    {
        //保存工位数据
        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Station "
                          "(AssemblyID, OrderID, StationID, OperatorID, AssemblyState, "
                          "RepairState, RetestState, CreateTime, FinishTime, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(getOperatorID());
            query.addBindValue(processData.assemblyState);
            query.addBindValue(processData.repairState);
            query.addBindValue(processData.retestState);
            query.addBindValue(processData.createTime);
            query.addBindValue(processData.finishTime);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);

        //保存物料数据
        for (auto material : materialList)
        {
            const MaterialData &materialData = material->getMaterialData();

            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Station_Material "
                          "(AssemblyID, OrderID, StationID, MaterialID, MaterialBarcode, "
                          "MaterialBatch, RepairCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(materialData.materialID);
            query.addBindValue(materialData.materialBarcode);
            query.addBindValue(materialData.materialBatch);
            query.addBindValue(materialData.repairCount);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }

        //保存采集数据
        for (auto measure : measureList)
        {
            const MeasureData &measureData = measure->getMeasureData();

            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Station_Measure "
                          "(AssemblyID, OrderID, StationID, MeasureType, MeasureOrder, "
                          "MeasureValue, MeasureState, RetestCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(processData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(measureData.measureID.split('#').at(0));
            query.addBindValue(measureData.measureID.split('#').at(1));
            query.addBindValue(measureData.measureValue);
            query.addBindValue(measureData.measureState);
            query.addBindValue(measureData.retestCount);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
    }

    do
    {
        if (0 == processData.assemblyState)
        {
            //更新工位完成数量
            this->updateFinishQuantity();
        }
        else
        {
            //更新工位不合格数量
            this->updateFailedQuantity();
        }

        //更新物料数量
        for (auto ptr : materialList)
        {
            ptr->updateBatchQuantity();

            emit manager->signalMaterialUpdate(this->getStationID(), ptr->getMaterialData());
        }

        //更新使用次数
        for (auto ptr : fixtureList)
        {
            ptr->updateUsedTimes();

            emit manager->signalFixtureUpdate(this->getStationID(), ptr->getFixtureData());
        }

        emit manager->signalStationUpdate(stationData);
    }
    while (0);

    do
    {
        if (1 == processData.assemblyState)
        {
            //工单更新不合格数量
            order->updateFailedQuantity();
            //保存工单数量信息
            order->saveOrderData();
        }
    }
    while (0);
}


//保存物料数据
void AssemblyStation::updateMaterialData(const QString &materialID, const QString &barcode, int count)
{
    auto ptr = getMaterial(materialID);

    if (ptr != nullptr)
    {
        ptr->setMaterialBarcode(barcode);
        ptr->setRepairCount(count);
    }
}

//保存采集数据
void AssemblyStation::updateMeasureData(const QString &measureID, double value, int state, int count)
{
    auto ptr = getMeasure(measureID);

    if (ptr != nullptr)
    {
        ptr->setMeasureValue(value);
        ptr->setMeasureState(state);
        ptr->setRetestCount(count);
    }
}

//工位复位
void AssemblyStation::stationReset()
{
    qDebug() << "stationReset()";

    for (auto process : processList)
    {
        process->initStepData();
    }
}

//工位复位
void AssemblyStation::stationAlarm()
{
    qDebug() << "stationAlarm()";
}

//流程结束
void AssemblyStation::finishProcess(AssemblyProcess *process)
{
    qDebug() << "finishProcess()";

    opcWriteOrderQuantity();

    process->initStepData();

    int index = processList.indexOf(process);
    process = processList.takeAt(index);
    processList.push_back(process);
}

//写入生产数据
void AssemblyStation::opcWriteProductInfo()
{
    processPara.writeProductID.itemValue = craft->getProductID();
    processPara.writeProductBatch.itemValue = order->getProductBatch();
    processPara.writeFormulaNumber.itemValue = processPara.formulaNumber;

    writeOpcData(processPara.writeProductID);
    writeOpcData(processPara.writeProductBatch);
    writeOpcData(processPara.writeFormulaNumber);
}

//更新数量信息
void AssemblyStation::opcWriteOrderQuantity()
{
    processPara.writeFinishQuantity.itemValue = stationData.finishQuantity;
    processPara.writeFailedQuantity.itemValue = stationData.failedQuantity;

    writeOpcData(processPara.writeFinishQuantity);
    writeOpcData(processPara.writeFailedQuantity);
}
//更新事件信息
void AssemblyStation::opcWriteProcessPrompt(EventState state)
{
    processPara.writeProcessPrompt.itemValue = static_cast<int>(state);
    writeOpcData(processPara.writeProcessPrompt);
}

//PLC监控数据事件
void AssemblyStation::opcValueChanged(const DataDefine &data)
{
//    qDebug() << stationID << data.itemID << data.itemValue;

    if (data == processPara.resetSignal)
    {
        stationReset();
        return;
    }

    if (data == processPara.alarmSignal)
    {
        stationAlarm();
        return;
    }

    for (auto process : processList)
        if (process->execValueChanged(data))
            return;
}

//PLC读取数据事件
void AssemblyStation::opcReadComplete(const DataDefine &data)
{
    for (auto process : processList)
        if (process->execReadComplete(data))
            return;
}

//串口数据到达事件
void AssemblyStation::dacDataReceived(const DataDefine &data)
{
    for (auto process : processList)
        if (process->execDataReceived(data))
            return;
}

void AssemblyStation::writeOpcData(const DataDefine &data)
{
    emit manager->opcWriteValue(stationData.stationID, data);
}

void AssemblyStation::readOpcData(const DataDefine &data)
{
    emit manager->opcReadValue(stationData.stationID, data);
}

void AssemblyStation::writeDacData(const DataDefine &data)
{
    emit manager->commWriteData(stationData.stationID, data);
}

EventState AssemblyStation::verifyStationStatus()
{
    if (!verifyOrderStatus())
    {
        return EventState::E_General_OrderClosed;
    }

    if (!verifyWorkStatus())
    {
        return EventState::E_General_WorkPaused;
    }

    if (verifyOrderFinished())
    {
        return EventState::E_General_OrderClosed;
    }

    if (preparePara.enableOperatorLogin && !verifyOperatorLogin())
    {
        return EventState::E_OperatorNotLogin;
    }

    if (preparePara.enableMaterialInput && !verifyMaterialInput())
    {
        return EventState::E_MaterialNotInput;
    }

    if (preparePara.enableMaterialInput && !verifyMaterialUseup())
    {
        return EventState::E_MaterialisUesup;
    }

    if (preparePara.enableFixtureCheck && !verifyFixtureUseup())
    {
        return EventState::E_FixtureisUesup;
    }

    if (preparePara.enableFixtureCheck && !verifyFixtureCheck())
    {
        return EventState::E_FixtureNotVerify;
    }

    if (preparePara.enableMachineCheck && !verifyMachineCheck())
    {
        return EventState::E_MachineNotCheck;
    }

    if (preparePara.enableProductCheck && !verifyProductCheck())
    {
        return EventState::E_ProductNotCheck;
    }

    return EventState::E_General_Successfully;
}

void AssemblyStation::updateStationStatus()
{
    for (auto process : processList)
    {
        if (StepType::LoopStart == process->getCurrentStep()->getStepType())
        {
            process->getCurrentStep()->executeStep();
        }
    }
}
