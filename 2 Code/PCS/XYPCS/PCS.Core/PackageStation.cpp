#include "PackageStation.h"
#include "LabelPrint.h"

PackageStation::PackageStation(WorkManager *manager)
    : WorkStation(manager)
    , assemblyPrint(new LabelPrint)
    , packagePrint(new LabelPrint)
{

}

PackageStation::~PackageStation()
{
    delete assemblyPrint;
    delete packagePrint;
}

bool PackageStation::initialStation(const QString &stationID)
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
                           .arg(PROCESS_PACKAGE_PATH)
                           .arg(craft->getCraftID())
                           .arg(stationID);

        if (!AssyPackage_ReadXmlFile(filename, preparePara, assemblyPara, packagePara))
            return false;

        if (assemblyPara.enable)
        {
            assemblyPrint->loadBarcode(craft->getCraftID(), labelTypeMap.value(LabelType::Assembly));
        }

        if (packagePara.enable)
        {
            packagePrint->loadBarcode(craft->getCraftID(), labelTypeMap.value(LabelType::Package));
        }
    }
    while (0);

    if (assemblyPara.enable)
    {
        const CraftData &craftData = craft->getCraftData();

        QSqlQuery query(LOCAL_DB);

        switch (craftData.assySerialRule)
        {
        case SerialRule::UpdateByBatch:
            query.prepare(R"(SELECT MAX([AssemblySerial]) AS [MaxSerial]
                          FROM [PCS_Data_Assembly]
                          WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [OrderID] = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getOrderID());
            break;

        case SerialRule::UpdateByDay:
            query.prepare(R"(SELECT MAX(A.[AssemblySerial]) AS [MaxSerial]
                          FROM [PCS_Data_Assembly] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?
                          AND CONVERT(NVARCHAR(8), B.[PlanDate], 112) = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            query.addBindValue(order->getPlanDate().toString("yyyyMMdd"));
            break;

        case SerialRule::UpdateByMonth:
            query.prepare(R"(SELECT MAX(A.[AssemblySerial]) AS [MaxSerial]
                          FROM [PCS_Data_Assembly] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?
                          AND CONVERT(NVARCHAR(6), B.[PlanDate], 112) = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            query.addBindValue(order->getPlanDate().toString("yyyyMM"));
            break;

        case SerialRule::UpdateByYear:
            query.prepare(R"(SELECT MAX(A.[AssemblySerial]) AS [MaxSerial]
                          FROM [PCS_Data_Assembly] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?
                          AND CONVERT(NVARCHAR(4), B.[PlanDate], 112) = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            query.addBindValue(order->getPlanDate().toString("yyyy"));
            break;

        case SerialRule::KeepContinuous:
            query.prepare(R"(SELECT MAX(A.[AssemblySerial]) AS [MaxSerial]
                          FROM [PCS_Data_Assembly] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            break;
        }

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            return false;
        }

        //如果没有生产记录，则流水号从设定的初始值开始打印
        if (query.value("MaxSerial").isNull())
        {
            assemblyData.assemblySerial = craftData.assySerialInit;
        }
        //如果是保持连续规则，且已有生产记录，则流水号从初始值+已生产数开始打印
        else if (craftData.assySerialRule == SerialRule::KeepContinuous)
        {
            assemblyData.assemblySerial = query.value("MaxSerial").toInt() + craftData.assySerialInit;
        }
        //其他情况，从已生产数+1的流水号开始打印
        else
        {
            assemblyData.assemblySerial = query.value("MaxSerial").toInt() + 1;
        }

        qDebug() << "Assembly Serial Start With " << assemblyData.assemblySerial;
    }

    if (packagePara.enable)
    {
        const CraftData &craftData = craft->getCraftData();

        QSqlQuery query(LOCAL_DB);

        switch (craftData.packSerialRule)
        {
        case SerialRule::UpdateByBatch:
            query.prepare(R"(SELECT MAX([PackageSerial]) AS [MaxSerial]
                          FROM [PCS_Data_Package]
                          WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [OrderID] = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getOrderID());
            break;

        case SerialRule::UpdateByDay:
            query.prepare(R"(SELECT MAX(A.[PackageSerial]) AS [MaxSerial]
                          FROM [PCS_Data_Package] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?
                          AND CONVERT(NVARCHAR(8), B.[PlanDate], 112) = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            query.addBindValue(order->getPlanDate().toString("yyyyMMdd"));
            break;

        case SerialRule::UpdateByMonth:
            query.prepare(R"(SELECT MAX(A.[PackageSerial]) AS [MaxSerial]
                          FROM [PCS_Data_Package] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?
                          AND CONVERT(NVARCHAR(6), B.[PlanDate], 112) = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            query.addBindValue(order->getPlanDate().toString("yyyyMM"));
            break;

        case SerialRule::UpdateByYear:
            query.prepare(R"(SELECT MAX(A.[PackageSerial]) AS [MaxSerial]
                          FROM [PCS_Data_Package] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?
                          AND CONVERT(NVARCHAR(4), B.[PlanDate], 112) = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            query.addBindValue(order->getPlanDate().toString("yyyy"));
            break;

        case SerialRule::KeepContinuous:
            query.prepare(R"(SELECT MAX(A.[PackageSerial]) AS [MaxSerial]
                          FROM [PCS_Data_Package] A, [MES_WorkOrder] B
                          WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ?
                          AND B.WOCode = A.OrderID AND B.PocessCode = ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(order->getCraftID());
            break;
        }

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            return false;
        }

        if (query.value("MaxSerial").isNull())
        {
            packageData.packageSerial = craftData.packSerialInit;
        }
        else if (craftData.packSerialRule == SerialRule::KeepContinuous)
        {
            packageData.packageSerial = query.value("MaxSerial").toInt() + craftData.packSerialInit;
        }
        else
        {
            packageData.packageSerial = query.value("MaxSerial").toInt() + 1;
        }
    }

    do
    {
        assemblyData.initial();
        packageData.initial();
    }
    while (0);

    return true;
}

bool PackageStation::isTransferBarcodeValidInPrevStation(const QString &barcode)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare("SELECT AssemblyState FROM PCS_Data_Transfer "
                  "WHERE OrderID = ? AND StationID = ? AND TransferBarcode = ? AND AssemblyState = 0");
    query.addBindValue(order->getOrderID());
    query.addBindValue(assemblyPara.prevStationID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    return query.first();
}

bool PackageStation::isTransferBarcodeExistInThisStation(const QString &barcode)
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

bool PackageStation::isAssemblyBarcodeExistInHistoryData(const QString &barcode)
{
    QSqlQuery query(qApplication->isOnline() ? REMOTE_DB : LOCAL_DB);
    query.prepare("SELECT UID FROM PCS_Data_Assembly WHERE AssemblyBarcode = ?");
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    return query.first();
}

bool PackageStation::isPackageBarcodeExistInHistoryData(const QString &barcode)
{
    QSqlQuery query(qApplication->isOnline() ? REMOTE_DB : LOCAL_DB);
    query.prepare("SELECT UID FROM PCS_Data_Package WHERE PackageBarcode = ?");
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    return query.first();
}

void PackageStation::getAssemblyDataByTransferBarcode(const QString &barcode)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare("SELECT AssemblyID FROM PCS_Data_Transfer "
                  "WHERE OrderID = ? AND StationID = ? AND TransferBarcode = ? AND AssemblyState = 0");
    query.addBindValue(order->getOrderID());
    query.addBindValue(assemblyPara.prevStationID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (!query.first())
    {
        return;
    }

    assemblyData.assemblyID = query.value("AssemblyID").toString();
    assemblyData.assemblyState = query.value("AssemblyState").toInt();
    assemblyData.repairState = query.value("RepairState").toInt();
    assemblyData.retestState = query.value("RetestState").toInt();
    assemblyData.sampleState = order->isExecuteSample() ? 1 : 0;
    assemblyData.createTime = QDateTime::currentDateTime();
}


//判断上工位过渡条码是否完成
EventState PackageStation::verifyTransferBarcode(const QString &barcode)
{
    //本工位是否有相关数据
    if (isTransferBarcodeExistInThisStation(barcode))
    {
        return EventState::E_ScanTbar_ThisStationHasData;
    }

    //上工位是否有相关数据
    if (!isTransferBarcodeValidInPrevStation(barcode))
    {
        return EventState::E_ScanTbar_PrevStationNonData;
    }

    getAssemblyDataByTransferBarcode(barcode);

    return EventState::E_ScanTbar_Successfully;
}

//判断总成条码是否存在历史记录
EventState PackageStation::verifyAssemblyBarcode(const QString &barcode)
{
    //判断扫描内容与总成条码是否匹配
    if (barcode != assemblyBarcode)
    {
        return EventState::E_ScanAbar_ContentMismatch;
    }

    //是否启用总成条码校验
    if (!assemblyPara.verifyBarcode)
    {
        return EventState::E_ScanAbar_Successfully;
    }

    //判断数据库是否存在历史数据
    if (isAssemblyBarcodeExistInHistoryData(barcode))
    {
        return EventState::E_ScanAbar_HistoryHasData;
    }

    return EventState::E_ScanAbar_Successfully;
}

//打印总成条码
QString PackageStation::printAssemblyBarcode()
{
    PrintVariants vars;

    vars.planDate = order->getPlanDate();
    vars.serialNo = assemblyData.assemblySerial;
    vars.packWeight = packageData.packageWeight;
    vars.packNumber = packageData.packageCount;

    assemblyPrint->generateTemplate(vars);
    assemblyPrint->printOut();

    assemblyBarcode = assemblyPrint->getBarcodeText();
    return assemblyBarcode;
}

//打印总成条码
void PackageStation::reprintAssemblyBarcode()
{
    assemblyPrint->printOut();
}

void PackageStation::reprintAssemblyBarcode(const QString &barcode)
{
    assemblyPrint->printOut(barcode);
}

//保存总成数据
void PackageStation::saveAssemblyData()
{
    assemblyData.assemblyBarcode = assemblyBarcode;
    assemblyData.finishTime = QDateTime::currentDateTime();

    //允许包装且非抽检件
    if (packagePara.enable && 0 == assemblyData.sampleState)
    {
        packageData.assemblyIDList.append(assemblyData.assemblyID);
    }

    ///***************************************************************************
    ///更新过渡条码数据
    ///***************************************************************************

    //更新过渡条码
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("UPDATE PCS_Data_Transfer SET "
                      "StationID = ?, AssemblyState = ?, RepairState = ?, RetestState = ?, CreateTime = ?, FinishTime = ? "
                      "WHERE AssemblyID = ? AND OrderID = ?");
        query.addBindValue(stationData.stationID);
        query.addBindValue(assemblyData.assemblyState);
        query.addBindValue(assemblyData.repairState);
        query.addBindValue(assemblyData.retestState);
        query.addBindValue(assemblyData.createTime);
        query.addBindValue(assemblyData.finishTime);
        query.addBindValue(assemblyData.assemblyID);
        query.addBindValue(order->getOrderID());

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            break;
        }
    }
    while (0);

    //获取总成装配开始、结束时间
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT MIN(CreateTime) AS CreateTime, MAX(FinishTime) AS FinishTime "
                      "FROM PCS_Data_Transfer WHERE OrderID = ? AND AssemblyID = ?");
        query.addBindValue(order->getOrderID());
        query.addBindValue(assemblyData.assemblyID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            break;
        }

        if (query.first())
        {
            assemblyData.createTime = query.value("CreateTime").toDateTime();
            assemblyData.finishTime = query.value("FinishTime").toDateTime();
        }
    }
    while (0);

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
                          "(AssemblyID, OrderID, StationID, OperatorID, AssemblyState, RepairState, RetestState, CreateTime, FinishTime, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(assemblyData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(getOperatorID());
            query.addBindValue(assemblyData.assemblyState);
            query.addBindValue(assemblyData.repairState);
            query.addBindValue(assemblyData.retestState);
            query.addBindValue(assemblyData.createTime);
            query.addBindValue(assemblyData.finishTime);
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
                          "(AssemblyID, OrderID, StationID, MaterialID, MaterialBarcode, MaterialBatch, RepairCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(assemblyData.assemblyID);
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
                          "(AssemblyID, OrderID, StationID, MeasureType, MeasureOrder, MeasureValue, MeasureState, RetestCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(assemblyData.assemblyID);
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

        //保存总成数据
        do
        {
            QSqlQuery query(REMOTE_DB);
            query.prepare("INSERT INTO PCS_Data_Assembly "
                          "(WorkshopID, WorklineID, AssemblyID, OrderID, ProductID, OperatorID, AssemblyBarcode, AssemblySerial, "
                          "AssemblyState, SampleState, RepairState, RetestState, CreateTime, FinishTime, UploadTime) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(assemblyData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(craft->getProductID());
            query.addBindValue(getOperatorID());
            query.addBindValue(assemblyData.assemblyBarcode);
            query.addBindValue(assemblyData.assemblySerial);
            query.addBindValue(assemblyData.assemblyState);
            query.addBindValue(assemblyData.sampleState);
            query.addBindValue(assemblyData.repairState);
            query.addBindValue(assemblyData.retestState);
            query.addBindValue(assemblyData.createTime);
            query.addBindValue(assemblyData.finishTime);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);
    }

    ///***************************************************************************
    ///保存数据至本地服务器
    ///***************************************************************************

    if (1)
    {
        //保存工位数据
        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Station "
                          "(AssemblyID, OrderID, StationID, OperatorID, AssemblyState, RepairState, RetestState, CreateTime, FinishTime, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(assemblyData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(stationData.stationID);
            query.addBindValue(getOperatorID());
            query.addBindValue(assemblyData.assemblyState);
            query.addBindValue(assemblyData.repairState);
            query.addBindValue(assemblyData.retestState);
            query.addBindValue(assemblyData.createTime);
            query.addBindValue(assemblyData.finishTime);
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
                          "(AssemblyID, OrderID, StationID, MaterialID, MaterialBarcode, MaterialBatch, RepairCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(assemblyData.assemblyID);
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
                          "(AssemblyID, OrderID, StationID, MeasureType, MeasureOrder, MeasureValue, MeasureState, RetestCount, UploadTime) "
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(assemblyData.assemblyID);
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

        //保存总成数据
        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Assembly "
                          "(WorkshopID, WorklineID, AssemblyID, OrderID, ProductID, OperatorID, AssemblyBarcode, AssemblySerial, "
                          "AssemblyState, SampleState, RepairState, RetestState, CreateTime, FinishTime, UploadTime) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(assemblyData.assemblyID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(craft->getProductID());
            query.addBindValue(getOperatorID());
            query.addBindValue(assemblyData.assemblyBarcode);
            query.addBindValue(assemblyData.assemblySerial);
            query.addBindValue(assemblyData.assemblyState);
            query.addBindValue(assemblyData.sampleState);
            query.addBindValue(assemblyData.repairState);
            query.addBindValue(assemblyData.retestState);
            query.addBindValue(assemblyData.createTime);
            query.addBindValue(assemblyData.finishTime);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);
    }

    do
    {
        if (0 == assemblyData.assemblyState)
        {
            //更新完成数量
            this->updateFinishQuantity();
        }
        else
        {
            //更新完成数量
            this->updateFailedQuantity();
        }

        //更新物料数量
        for (auto ptr : materialList)
        {
            ptr->updateBatchQuantity();

            emit manager->signalMaterialUpdate(this->getStationID(), ptr->getMaterialData());
        }

        //更新模具剩余次数
        for (auto ptr : fixtureList)
        {
            ptr->updateUsedTimes();

            emit manager->signalFixtureUpdate(this->getStationID(), ptr->getFixtureData());
        }

        //更新界面信息
        emit manager->signalStationUpdate(stationData);
    }
    while (0);

    do
    {
        if (0 == assemblyData.sampleState)
        {
            //更新工单完成数量
            order->updateFinishQuantity();
        }
        else
        {
            //更新工单抽检数量
            order->updateSampleQuantity();
        }

        if (0 == assemblyData.retestState)
        {
            //更新一次性通过数量
            order->updateFirstPassQuantity();
        }

        if (1 == assemblyData.repairState)
        {
            //更新工单返修数量
            order->updateRepairQuantity();
        }

        //保存工单数量信息
        order->saveOrderData();
        //发送总成组装完成信号，通知界面更新
        emit manager->signalAssemblyFinish(assemblyData);

        assemblyData.initial();
        assemblyData.serialInc();
    }
    while (0);
}

//判断包装箱防错标识
EventState PackageStation::verifyPackageMarker(const QString &barcode)
{
    if (barcode != packagePara.packageMarker)
    {
        return EventState::E_ScanMark_ContentMismatch;
    }

    packageData.createTime = QDateTime::currentDateTime();
    return EventState::E_ScanMark_Successfully;
}

//判断包装条码是否存在历史记录
EventState PackageStation::verifyPackageBarcode(const QString &barcode)
{
    if (barcode != packageBarcode)
    {
        return EventState::E_ScanPbar_ContentMismatch;
    }

    if (!packagePara.verifyBarcode)
    {
        return EventState::E_ScanPbar_Successfully;
    }

    if (isPackageBarcodeExistInHistoryData(barcode))
    {
        return EventState::E_ScanPbar_HistoryHasData;
    }

    return EventState::E_ScanPbar_Successfully;
}

//打印包装箱码
QString PackageStation::printPackageBarcode()
{
    PrintVariants vars;

    vars.planDate = order->getPlanDate();
    vars.serialNo = packageData.packageSerial;
    vars.packWeight = packageData.packageWeight;
    vars.packNumber = packageData.packageCount;

    packagePrint->generateTemplate(vars);
    packagePrint->printOut();

    packageBarcode = packagePrint->getBarcodeText();
    return packageBarcode;
}

void PackageStation::reprintPackageBarcode()
{
    packagePrint->printOut();
}

void PackageStation::reprintPackageBarcode(const QString &barcode)
{
    packagePrint->printOut(barcode);
}

//保存总成与包装箱信息
void PackageStation::savePackageData()
{
    packageData.packageID = QUuid::createUuid().toString().remove('{').remove('}');
    packageData.packageBarcode = packagePrint->getBarcodeText();
    packageData.finishTime = QDateTime::currentDateTime();

    ///***************************************************************************
    ///保存数据至车间服务器
    ///***************************************************************************
    if (qApplication->isOnline())
    {
        do
        {
            QSqlQuery query(REMOTE_DB);
            query.prepare("INSERT INTO PCS_Data_Package "
                          "(WorkshopID, WorklineID, PackageID, OrderID, ProductID, OperatorID, PackageBarcode, PackageSerial, "
                          "PackageWeight, PackageCount, PackageState, CreateTime, FinishTime, UploadTime) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(packageData.packageID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(craft->getProductID());
            query.addBindValue(getOperatorID());
            query.addBindValue(packageData.packageBarcode);
            query.addBindValue(packageData.packageSerial);
            query.addBindValue(packageData.packageWeight);
            query.addBindValue(packageData.packageCount);
            query.addBindValue(packageData.packageState);
            query.addBindValue(packageData.createTime);
            query.addBindValue(packageData.finishTime);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);
    }

    ///***************************************************************************
    ///保存数据至本地数据库
    ///***************************************************************************
    {
        for (auto assemblyID : packageData.assemblyIDList)
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Package_Assembly "
                          "(PackageID, AssemblyID) VALUES(?, ?)");
            query.addBindValue(packageData.packageID);
            query.addBindValue(assemblyID);

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }

        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare("INSERT INTO PCS_Data_Package "
                          "(WorkshopID, WorklineID, PackageID, OrderID, ProductID, OperatorID, PackageBarcode, PackageSerial, "
                          "PackageWeight, PackageCount, PackageState, CreateTime, FinishTime, UploadTime) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(manager->getWorkshopID());
            query.addBindValue(manager->getWorklineID());
            query.addBindValue(packageData.packageID);
            query.addBindValue(order->getOrderID());
            query.addBindValue(craft->getProductID());
            query.addBindValue(getOperatorID());
            query.addBindValue(packageData.packageBarcode);
            query.addBindValue(packageData.packageSerial);
            query.addBindValue(packageData.packageWeight);
            query.addBindValue(packageData.packageCount);
            query.addBindValue(packageData.packageState);
            query.addBindValue(packageData.createTime);
            query.addBindValue(packageData.finishTime);
            query.addBindValue(getUploadTime());

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        }
        while (0);
    }

    do
    {
        //更新工单包装箱数量
        order->updatePackageQuantity();
        //保存工单数量信息
        order->saveOrderData();
        //发送总成组装完成信号，通知界面更新
        emit manager->signalPackageFinish(packageData);

        packageData.initial();
        packageData.serialInc();
    }
    while (0);
}

void PackageStation::scaleRead()
{
    DataDefine data;

    data.itemID = QString("%1,读取").arg(packagePara.scaleDeviceID);

    writeDacData(data);
}

void PackageStation::scaleZero()
{
    DataDefine data;

    data.itemID = QString("%1,清零").arg(packagePara.scaleDeviceID);

    writeDacData(data);
}

void PackageStation::scaleReset()
{
    DataDefine data;

    data.itemID = QString("%1,复位").arg(packagePara.scaleDeviceID);

    writeDacData(data);
}

//收到串口数据
void PackageStation::dacDataReceived(const DataDefine &data)
{
    if (data.itemID != QString("%1,读取").arg(packagePara.scaleDeviceID))
        return;

    packageData.packageWeight = data.itemValue.toDouble();

    if (packagePara.singleWeight == 0.0 || packageData.packageWeight <= 0.0)
    {
        packageData.packageCount = 0;
    }
    else
    {
        packageData.packageCount = int((packageData.packageWeight + packagePara.offsetWeight) / packagePara.singleWeight);
    }

    if (packageData.packageCount == 0)
    {
        packageData.packageState = -1;      //空箱
    }
    else if (packageData.packageCount < packagePara.packageNumber)
    {
        packageData.packageState = 1;       //不足
    }
    else if (packageData.packageCount > packagePara.packageNumber)
    {
        packageData.packageState = 2;       //过量
    }
    else
    {
        packageData.packageState = 0;       //满箱
    }

    emit manager->signalPackageUpdate(packageData);
}

//发送串口数据
void PackageStation::writeDacData(const DataDefine &data)
{
    emit manager->commWriteData(this->getStationID(), data);
}

EventState PackageStation::verifyStationStatus()
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

void PackageStation::updateStationStatus()
{

}

