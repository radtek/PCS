#include "WorkStation.h"

WorkStation::WorkStation(WorkManager *manager)
    : manager(manager)
    , order(manager->getOrder())
    , craft(manager->getCraft())
{
}

WorkStation::~WorkStation()
{
    removeOperator();
    removeMaterial();
    removeMeasure();
    removeFixture();
}

// 初始化工位参数，读取数据库
bool WorkStation::initialStation(const QString &stationID)
{
    //读取工位信息
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[WorkStationCode] AS [StationID]
                      ,[WorkStationShortName] AS [StationCode]
                      ,[WorkStationName] AS [StationName]
                      ,CASE [IsPackage] WHEN 0 THEN 'Station' WHEN 1 THEN 'Package' END AS [StationType]
                      FROM [MES_db_WorkStation]
                      WHERE [WorkLineCode] = ? AND [WorkStationCode] = ?)");
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(stationID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query first()";
            return false;
        }

        stationData.stationID = query.value("StationID").toString();
        stationData.stationCode = query.value("StationCode").toString();
        stationData.stationName = query.value("StationName").toString();
        stationData.stationType = query.value("StationType").toString();
    }
    while (0);

    //读取工位完成数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Station "
                      "WHERE OrderID = ? AND StationID = ? AND AssemblyState = 0");
        query.addBindValue(order->getOrderID());
        query.addBindValue(stationID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query station finish quantity failed!";
            return false;
        }

        stationData.finishQuantity = query.value("Count").toInt();
    }
    while (0);

    //获取工位不合格数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Station "
                      "WHERE OrderID = ? AND StationID = ? AND AssemblyState = 1");
        query.addBindValue(order->getOrderID());
        query.addBindValue(stationID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query station failed quantity failed!";
            return false;
        }

        stationData.failedQuantity = query.value("Count").toInt();
    }
    while (0);

    //获取工位一次成功数量
    //    do
    //    {
    //        QSqlQuery query(LOCAL_DB);
    //        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Station "
    //                      "WHERE OrderID = ? AND StationID = ? AND AssemblyState = 0 AND RetestState = 0");
    //        query.addBindValue(order->getOrderID());
    //        query.addBindValue(stationID);
    //        if (!query.exec())
    //        {
    //            qDebug().noquote() << query.lastQuery();
    //            return false;
    //        }

    //        if (!query.first())
    //        {
    //            qDebug() << "query station firstPass quantity failed!";
    //            return false;
    //        }

    //        stationData.firstPassQuantity = query.value("Count").toInt();
    //    }
    //    while (0);

    for (auto materialID : manager->getMaterialIDList(craft->getCraftID(), stationID))
    {
        if (!appendMaterial(materialID))
        {
            qDebug() << "Append material failed! materialID:" << materialID;
            return false;
        }
    }

    for (auto measureID : manager->getMeasureIDList(craft->getCraftID(), stationID))
    {
        if (!appendMeasure(measureID))
        {
            qDebug() << "Append measure failed! materialID:" << measureID;
            return false;
        }
    }

    do
    {
        stationData.isOperatorLogin = operatorList.size() != 0; //员工登陆
        stationData.isMaterialInput = materialList.size() == 0; //物料上料

        machineCheck();     //设备点检
        productCheck();     //产品点检
        fixtureCheck();     //工装模具点检
    }
    while (0);

    return true;
}

StationMaterial *WorkStation::getMaterial(const QString &id) const
{
    for (auto ptr : materialList)
        if (ptr->getMaterialID() == id)
            return ptr;

    return nullptr;
}
StationMeasure *WorkStation::getMeasure(const QString &id) const
{
    for (auto ptr : measureList)
        if (ptr->getMeasureID() == id)
            return ptr;

    return nullptr;
}
StationFixture *WorkStation::getFixture(const QString &id) const
{
    for (auto ptr : fixtureList)
        if (ptr->getFixtureID() == id)
            return ptr;

    return nullptr;
}
StationOperator *WorkStation::getOperator(const QString &id) const
{
    for (auto ptr : operatorList)
        if (ptr->getOperatorID() == id)
            return ptr;

    return nullptr;
}

StationOperator *WorkStation::getOperatorByBarcode(const QString &barcode) const
{
    for (auto ptr : operatorList)
        if (ptr->getOperatorCard() == barcode)
            return ptr;

    return nullptr;
}

StationMaterial *WorkStation::getMaterialByBarcode(const QString &barcode) const
{
    QString materialID = barcode.split('.').at(0);

    return getMaterial(materialID);
}

// 加载物料数据
bool WorkStation::appendMaterial(const QString &materialID)
{
    StationMaterial *ptr = new StationMaterial(this);

    if (ptr->initialMaterial(materialID))
    {
        materialList.append(ptr);
        return true;
    }

    {
        delete ptr;
        return false;
    }
}

void WorkStation::removeMaterial()
{
    for (auto ptr : materialList)
    {
        delete ptr;
    }

    materialList.clear();
}

// 加载测试数据
bool WorkStation::appendMeasure(const QString &measureID)
{
    StationMeasure *ptr = new StationMeasure(this);

    if (ptr->initialMeasure(measureID))
    {
        measureList.append(ptr);
        return true;
    }

    {
        delete ptr;
        return false;
    }
}

void WorkStation::removeMeasure()
{
    for (auto ptr : measureList)
    {
        delete ptr;
    }

    measureList.clear();
}

//载入工装信息
bool WorkStation::appendFixture(const QString &fixtureID)
{
    StationFixture *ptr = new StationFixture(this);

    if (ptr->loadFixture(fixtureID))
    {
        fixtureList.append(ptr);
        return true;
    }

    {
        delete ptr;
        return false;
    }
}

void WorkStation::removeFixture()
{
    for (auto ptr : fixtureList)
    {
        delete ptr;
    }

    fixtureList.clear();
}

//加载操作员信息
bool WorkStation::appendOperator(const QString &barcode)
{
    StationOperator *ptr = new StationOperator(this);

    if (ptr->operatorLogin(barcode))
    {
        operatorList.append(ptr);
        return true;
    }
    else
    {
        delete ptr;
        return false;
    }
}

bool WorkStation::removeOperator(StationOperator *ptr)
{
    if (ptr == nullptr)
        return false;

    ptr->operatorLogout();

    delete ptr;

    operatorList.removeOne(ptr);

    return true;
}

void WorkStation::removeOperator()
{
    for (auto ptr : operatorList)
    {
        ptr->operatorLogout();

        delete ptr;
    }

    operatorList.clear();
}

//校验工单状态
bool WorkStation::verifyOrderStatus()
{
    return manager->isWorkActivated();
}

//校验生产状态
bool WorkStation::verifyWorkStatus()
{
    return manager->isWorkRunning();
}

//校验工单是否完成
bool WorkStation::verifyOrderFinished()
{
    //各工位的目标数量 = 排产数量 + 抽检数量 + 不合格数量
    return stationData.finishQuantity >=
           order->getOrderData().orderPlanQuantity +
           order->getOrderData().inspectQuantity +
           order->getOrderData().failedQuantity;
}

//校验员工信息
bool WorkStation::verifyOperatorLogin()
{
    return operatorList.size() > 0;
}

//校验物料批次
bool WorkStation::verifyMaterialInput()
{
    for (auto ptr : materialList)
        if (!ptr->isBatchInput())
            return false;

    return true;
}

//校验物料是否用完
bool WorkStation::verifyMaterialUseup()
{
    for (auto ptr : materialList)
        if (ptr->isBatchUseup())
            return false;

    return true;
}
//校验物料是否发出预警
bool WorkStation::verifyMaterialAlarm()
{
    for (auto ptr : materialList)
        if (ptr->isBatchAlarm())
            return false;

    return true;
}

bool WorkStation::verifyFixtureUseup()
{
    for (auto ptr : fixtureList)
        if (ptr->isTimesUseup())
            return false;

    return true;
}

//校验设备点检
bool WorkStation::verifyMachineCheck()
{
    return stationData.isMachineCheck;
}

//校验产品点检
bool WorkStation::verifyProductCheck()
{
    return stationData.isProductCheck;
}

//校验工装模具
bool WorkStation::verifyFixtureCheck()
{
    return stationData.isFixtureCheck;
}

void WorkStation::machineCheck()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_Device_CheckMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [WorkStationCode] = ?
                  AND CONVERT(NVARCHAR, [CheckTime], 23) = ? AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(manager->getWorkshopID());
    query.addBindValue(manager->getWorklineID());
    query.addBindValue(stationData.stationID);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        stationData.machineCheckDocID = query.value("DocumentID").toString();
        stationData.isMachineCheck = true;
    }
    else
    {
        stationData.machineCheckDocID.clear();
        stationData.isMachineCheck = false;
    }

    emit manager->signalStationUpdate(stationData);

    updateStationStatus();
}

void WorkStation::productCheck()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_PCheck_DocMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [WorkStationCode] = ?
                  AND [WorkOrderCode] = ? AND CONVERT(NVARCHAR, [CheckDate], 23) = ?
                  AND [CheckStyle] = 0 AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(manager->getWorkshopID());
    query.addBindValue(manager->getWorklineID());
    query.addBindValue(stationData.stationID);
    query.addBindValue(order->getOrderID());
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        stationData.productCheckDocID = query.value("DocumentID").toString();
        stationData.isProductCheck = true;
    }
    else
    {
        stationData.productCheckDocID.clear();
        stationData.isProductCheck = false;
    }

    emit manager->signalStationUpdate(stationData);

    updateStationStatus();
}

void WorkStation::fixtureCheck()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_PCheck_DocMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [WorkStationCode] = ?
                  AND [WorkOrderCode] = ? AND CONVERT(NVARCHAR, [CheckDate], 23) = ?
                  AND [CheckStyle] = 1 AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(manager->getWorkshopID());
    query.addBindValue(manager->getWorklineID());
    query.addBindValue(stationData.stationID);
    query.addBindValue(order->getOrderID());
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        stationData.fixtureCheckDocID = query.value("DocumentID").toString();
        stationData.isFixtureCheck = true;

        fixtureLoad(stationData.fixtureCheckDocID);
    }
    else
    {
        stationData.fixtureCheckDocID.clear();
        stationData.isFixtureCheck = false;
    }

    emit manager->signalStationUpdate(stationData);

    updateStationStatus();
}

bool WorkStation::fixtureLoad(const QString &documentID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DeviceCode] AS [FixtureID]
                  ,[DeviceClassify] AS [FixtureClassify]
                  FROM [MES_PCheck_DocItems]
                  WHERE [MasterCode] = ? AND [ItemResult] = 0
                  AND ([CheckStyle] = 1 OR [CheckStyle] = 2))");
    query.addBindValue(documentID);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    while (query.next())
    {
        QString fixtureID = query.value("FixtureID").toString();

        if (!appendFixture(fixtureID))
        {
            qDebug() << "appendFixture() failed!" << fixtureID;
            continue;
        }

        auto ptr = getFixture(fixtureID);

        ptr->setFixutreClassify(query.value("FixtureClassify").toString());

        emit manager->signalFixtureLoaded(getStationID(), ptr->getFixtureData());
    }

    return true;
}

//操作员登陆
bool WorkStation::operatorLogin(const QString &barcode)
{
    //校验操作员是否登陆
    auto ptr = getOperatorByBarcode(barcode);

    if (ptr != nullptr)
    {
        qDebug() << "operator has logged!";
        return false;
    }

    //校验操作员工号是否正确
    if (!appendOperator(barcode))
    {
        qDebug() << "operatorLogin() failed!";
        return false;
    }

    ptr = getOperatorByBarcode(barcode);

    emit manager->signalOperatorLogin(getStationID(), ptr->getOperatorData());

    stationData.isOperatorLogin = verifyOperatorLogin();

    emit manager->signalStationUpdate(stationData);

    updateStationStatus();

    return true;
}

//操作员登出
bool WorkStation::operatorLogout(const QString &operatorID)
{
    auto ptr = getOperator(operatorID);

    if (ptr == nullptr)
    {
        qDebug() << "operator has not logged!";
        return false;
    }

    if (!removeOperator(ptr))
    {
        qDebug() << "operatorLogout() failed!" << operatorID;
        return false;
    }

    emit manager->signalOperatorLogout(getStationID(), ptr->getOperatorData());

    stationData.isOperatorLogin = verifyOperatorLogin();

    emit manager->signalStationUpdate(stationData);

    return true;
}

bool WorkStation::materialInput(const QString &materialID, const QString &batch, int quantity)
{
    auto ptr = getMaterial(materialID);

    if (ptr == nullptr)
    {
        qDebug() << "material cannot find!";
        return false;
    }

    if (!ptr->inputMaterialBatch(batch, quantity))
    {
        qDebug() << "materialInput() failed!";
        return false;
    }

    emit manager->signalMaterialInput(this->getStationID(), ptr->getMaterialData());

    stationData.isMaterialInput = verifyMaterialInput();

    emit manager->signalStationUpdate(stationData);

    updateStationStatus();

    return true;
}

//投入物料
bool WorkStation::materialInput(const QString &barcode)
{
    auto ptr = getMaterialByBarcode(barcode);

    if (ptr == nullptr)
    {
        qDebug() << "material cannot find!";
        return false;
    }

    if (!ptr->inputMaterialBatch(barcode))
    {
        qDebug() << "materialInput() failed!";
        return false;
    }

    emit manager->signalMaterialInput(getStationID(), ptr->getMaterialData());

    stationData.isMaterialInput = verifyMaterialInput();

    emit manager->signalStationUpdate(stationData);

    updateStationStatus();

    return true;
}

bool WorkStation::materialReturn(const QString &materialID)
{
    auto ptr = getMaterial(materialID);

    if (ptr == nullptr)
    {
        qDebug() << "material cannot find!";
        return false;
    }

    if (!ptr->returnMaterialBatch())
    {
        qDebug() << "materialReturn() failed!";
        return false;
    }

    emit manager->signalMaterialReturn(getStationID(), ptr->getMaterialData());

    stationData.isMaterialInput = verifyMaterialInput();

    emit manager->signalStationUpdate(stationData);

    return true;
}
