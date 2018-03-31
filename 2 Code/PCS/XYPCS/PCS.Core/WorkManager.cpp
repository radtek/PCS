#include "WorkManager.h"
#include "AssemblyStation.h"
#include "CDocManager.h"
#include "PackageStation.h"
#include "WorkCraft.h"
#include "WorkOrder.h"
#include "WorkStation.h"

WorkManager::WorkManager()
    : order(nullptr)
    , craft(nullptr)
    , activated(false)
    , running(false)
{
}

WorkManager::~WorkManager()
{
    releaseManager();
}

bool WorkManager::loadSqlData(const QString &workshopID, const QString &worklineID)
{
    LOCAL_DB = QSqlDatabase::database("LOCAL_HOST");
    REMOTE_DB = QSqlDatabase::database("REMOTE_HOST");

    if (!LOCAL_DB.isOpen())
    {
        qCritical() << "Local db is not open!";
        return false;
    }

    if (!REMOTE_DB.isOpen())
    {
        qCritical() << "Remote db is not open!";
        //        return false;
    }

    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[WorkShopCode] AS [WorkshopID]
                      ,[WorkShopName] AS [WorkshopName]
                      FROM [MES_db_WorkShopd]
                      WHERE [WorkShopCode] = ? AND [State] != ?)");
        query.addBindValue(workshopID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query workshop ID failed!";
            return false;
        }

        workshopData.id = query.value("WorkshopID").toString();
        workshopData.name = query.value("WorkshopName").toString();
    } while (0);

    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[WorkLineCode] AS [WorklineID]
                      ,[WorkLineName] AS [WorklineName]
                      FROM [MES_db_WorkLine]
                      WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [State] != ?)");
        query.addBindValue(workshopID);
        query.addBindValue(worklineID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query workline ID failed!";
            return false;
        }

        worklineData.id = query.value("WorklineID").toString();
        worklineData.name = query.value("WorklineName").toString();
    } while (0);

    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[ProcessCode] AS [CraftID]
                      ,[MCode] AS [ProductID]
                      FROM [MES_Process_Info]
                      WHERE [WorkLineCode] = ? AND [State] != ?
                      ORDER BY [UID])");
        query.addBindValue(worklineID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        while (query.next())
        {
            craftIDList.append(query.value("CraftID").toString());

            craftProductMap.insert(query.value("CraftID").toString(),
                                   query.value("ProductID").toString());
        }
    } while (0);

    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[WorkStationCode] AS [StationID]
                      ,[WorkStationShortName] AS [StationCode]
                      ,CASE [IsPackage] WHEN ? THEN ? WHEN ? THEN ? END AS [StationType]
                      FROM [MES_db_WorkStation]
                      WHERE [WorkLineCode] = ? AND [State] != ?
                      ORDER BY [UID])");
        query.addBindValue(static_cast<int>(StationType::Station));
        query.addBindValue(stationTypeMap.value(StationType::Station));
        query.addBindValue(static_cast<int>(StationType::Package));
        query.addBindValue(stationTypeMap.value(StationType::Package));
        query.addBindValue(worklineID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        while (query.next())
        {
            qDebug() << query.value("StationType").toString();
            stationIDList.append(query.value("StationID").toString());

            station_CodeMap.insert(query.value("StationID").toString(),
                                   query.value("StationCode").toString());

            station_TypeMap.insert(query.value("StationID").toString(),
                                   query.value("StationType").toString());
        }
    } while (0);

    return true;
}

QStringList WorkManager::getMaterialIDList(const QString &craftID, const QString &stationID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [MCode] AS [MaterialID]
                  FROM [MES_Process_WorkStationMaterial]
                  WHERE [ProcessCode] = ? AND [WorkStationCode] = ? AND [State] != ?
                  ORDER BY [UID])");
    query.addBindValue(craftID);
    query.addBindValue(stationID);
    query.addBindValue(RECORD_DELETE);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return QStringList();
    }

    QStringList list;

    while (query.next())
    {
        list.append(query.value("MaterialID").toString());
    }

    return list;
}

QStringList WorkManager::getMeasureIDList(const QString &craftID, const QString &stationID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [Dict_ParaTypeCode] + '#' + CAST([OrderNum] AS NVARCHAR) AS [MeasureID]
                  FROM [MES_Process_Para]
                  WHERE [ProcessCode] = ? AND [WorkStationCode] = ? AND [State] != ?
                  ORDER BY [UID])");
    query.addBindValue(craftID);
    query.addBindValue(stationID);
    query.addBindValue(RECORD_DELETE);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return QStringList();
    }

    QStringList list;

    while (query.next())
    {
        list.append(query.value("MeasureID").toString());
    }

    return list;
}

QStringList WorkManager::getFixtureClsList(const QString &craftID, const QString &stationID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT DISTINCT T.[FixtureClassify] FROM(
                  SELECT D1.[DName] + D2.[DName] AS [FixtureClassify]
                  FROM [MES_Process_DeviceMapping] A
                  LEFT JOIN [MES_db_Dict] D1
                  ON D1.[PDCode] = 'Dict_DeviceType' AND A.[LedgerType] = D1.[Dcode]
                  LEFT JOIN [MES_db_Dict] D2
                  ON (D2.[PDCode] = 'TechnologyClassify' AND A.[LedgerType] = 1 AND A.[DeviceClassify] = D2.[Dcode])
                  OR (D2.[PDCode] = 'MouldClassify' AND A.[LedgerType] = 2 AND A.[DeviceClassify] = D2.[Dcode])
                  WHERE A.[ProcessCode] = ? AND A.[WorkStationCode] = ? AND A.[State] != ?) T)");
    query.addBindValue(craftID);
    query.addBindValue(stationID);
    query.addBindValue(RECORD_DELETE);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return QStringList();
    }

    QStringList list;

    while (query.next())
    {
        list.append(query.value("FixtureClassify").toString());
    }

    return list;
}

//初始化工单
bool WorkManager::initialManager(const QString &orderID)
{
    if (!createOrder(orderID))
    {
        qDebug() << "Initial order failed! orderID:" << orderID;
        return false;
    }

    if (!createCraft(order->getCraftID()))
    {
        qDebug() << "Initial craft failed! craftID:" << order->getCraftID();
        return false;
    }

    for (auto stationID : getStationIDList())
    {
        if (!appendStation(stationID, getStationType(stationID)))
        {
            qDebug() << "Initial station failed! stationID:" << stationID;
        }
    }

    activated = true;
    running = false;

    return true;
}

//释放工单资源
void WorkManager::releaseManager()
{
    removeStation();
    deleteCraft();
    deleteOrder();
}

bool WorkManager::createOrder(const QString &orderID)
{
    order = new WorkOrder(this);

    return order->initialOrder(orderID);
}

void WorkManager::deleteOrder()
{
    if (order != nullptr)
    {
        delete order;
        order = nullptr;
    }
}

bool WorkManager::createCraft(const QString &craftID)
{
    craft = new WorkCraft(this);

    return craft->initialCraft(craftID);
}

void WorkManager::deleteCraft()
{
    if (craft != nullptr)
    {
        delete craft;
        craft = nullptr;
    }
}

bool WorkManager::appendStation(const QString &stationID, const QString &stationType)
{
    WorkStation *ptr = nullptr;

    switch (stationTypeMap.key(stationType))
    {
    case StationType::Station:
        ptr = new AssemblyStation(this);
        break;

    case StationType::Package:
        ptr = new PackageStation(this);
        break;
    }

    if (ptr == nullptr)
    {
        return false;
    }

    if (ptr->initialStation(stationID))
    {
        stationList.append(ptr);
        return true;
    }
    else
    {
        delete ptr;
        return false;
    }
}

void WorkManager::removeStation()
{
    for (auto ptr : stationList)
    {
        delete ptr;
    }

    stationList.clear();
}

WorkStation *WorkManager::getStation(const QString &id) const
{
    for (auto ptr : stationList)
        if (ptr->getStationID() == id)
            return ptr;

    return nullptr;
}

PackageStation *WorkManager::getPackageStation() const
{
    QString type = stationTypeMap.value(StationType::Package);

    for (auto ptr : stationList)
        if (ptr->getStationType() == type)
            return dynamic_cast<PackageStation *>(ptr);

    return nullptr;
}

QList<AssemblyStation *> WorkManager::getProcessStation() const
{
    QList<AssemblyStation *> stationList;
    QString type = stationTypeMap.value(StationType::Station);

    for (auto ptr : stationList)
        if (ptr->getStationType() == type)
            stationList << dynamic_cast<AssemblyStation *>(ptr);

    return stationList;
}

//-----------------------通用方法-----------------------

QString WorkManager::getOrderID() const
{
    return order ? order->getOrderID() : QString();
}

QString WorkManager::getCraftID() const
{
    return craft ? craft->getCraftID() : QString();
}

QString WorkManager::getProductID() const
{
    return craft ? craft->getProductID() : QString();
}

QString WorkManager::getOperatorID(const QString &stationID) const
{
    auto ptr = getStation(stationID);

    return ptr ? ptr->getOperatorID() : QString();
}

void WorkManager::opcValueChanged(const QString &stationID, const DataDefine &data)
{
    auto station = getStation(stationID);

    if (station == nullptr)
        return;

    station->opcValueChanged(data);
}

void WorkManager::opcReadComplete(const QString &stationID, const DataDefine &data)
{
    auto station = getStation(stationID);

    if (station == nullptr)
        return;

    station->opcReadComplete(data);
}

void WorkManager::commDataReceived(const QString &stationID, const DataDefine &data)
{
    auto station = getStation(stationID);

    if (station == nullptr)
        return;

    station->dacDataReceived(data);
}

void WorkManager::commErrorOccurred(const QString &stationID, const ErrorDefine &error)
{
    QMessageBox::critical(nullptr,
                          "Error",
                          QString("%1, %2, %3")
                              .arg(stationID)
                              .arg(error.deviceID)
                              .arg(error.errorText));
}

void WorkManager::submitDocument(SubmitDocType type, const QString &stationID)
{
    switch (type)
    {
    case SubmitDocType::MachineCheckDoc:
    {
        auto station = getStation(stationID);

        if (station != nullptr)
            station->machineCheck();

        break;
    }

    case SubmitDocType::ProductCheckDoc:
    {
        auto station = getStation(stationID);

        if (station != nullptr)
            station->productCheck();

        break;
    }

    case SubmitDocType::FixtureCheckDoc:
    {
        auto station = getStation(stationID);

        if (station != nullptr)
            station->fixtureCheck();

        break;
    }

    case SubmitDocType::FirstProductQADoc:
    {
        auto order = getOrder();

        if (order != nullptr)
            order->firstProductQA();

        break;
    }

    case SubmitDocType::FinalProductQADoc:
    {
        auto order = getOrder();

        if (order != nullptr)
            order->finalProductQA();

        break;
    }

    case SubmitDocType::FirstPackageQADoc:
    {
        auto order = getOrder();

        if (order != nullptr)
            order->firstPackageQA();

        break;
    }

    case SubmitDocType::FinalPackageQADoc:
    {
        auto order = getOrder();

        if (order != nullptr)
            order->finalPackageQA();

        break;
    }

    default:
        break;
    }
}

//新建单据
void WorkManager::createDocument(SubmitDocType type, const QString &stationID)
{
    switch (type)
    {
    case SubmitDocType::MachineCheckDoc:
        CDocManager::CreateDoc(SubmitDocType::MachineCheckDoc, stationID);
        break;

    case SubmitDocType::ProductCheckDoc:
        CDocManager::CreateDoc(SubmitDocType::ProductCheckDoc, stationID);
        break;

    case SubmitDocType::FixtureCheckDoc:
        CDocManager::CreateDoc(SubmitDocType::FixtureCheckDoc, stationID);
        break;

    default:
        break;
    }
}

//浏览单据
void WorkManager::browseDocument(SubmitDocType type, const QString &documentID)
{
    switch (type)
    {
    case SubmitDocType::MachineCheckDoc:
        CDocManager::ModifyDoc(SubmitDocType::MachineCheckDoc, documentID);
        break;

    case SubmitDocType::ProductCheckDoc:
        CDocManager::ModifyDoc(SubmitDocType::ProductCheckDoc, documentID);
        break;

    case SubmitDocType::FixtureCheckDoc:
        CDocManager::ModifyDoc(SubmitDocType::FixtureCheckDoc, documentID);
        break;

    default:
        break;
    }
}

//激活工单
bool WorkManager::activateOrder(const QString &orderID)
{
    //释放程序资源
    releaseManager();

    if (initialManager(orderID))
    {
        activated = true;
        running = true;

        emit signalOrderActivated();

        return true;
    }

    return false;
}
//结束工单
void WorkManager::suspendOrder(const QString &orderID)
{
    if (orderID == getOrderID())
    {
        activated = false;
        running = false;

        emit signalOrderSuspended();
    }
}
//生产暂停
void WorkManager::pauseWork()
{
    running = false;

    emit signalWorkPaused();
}
//生产开始
void WorkManager::startWork()
{
    running = true;

    emit signalWorkStarted();
}
