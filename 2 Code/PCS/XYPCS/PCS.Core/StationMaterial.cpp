#include "StationMaterial.h"
#include "WorkManager.h"
#include "WorkStation.h"

StationMaterial::StationMaterial(WorkStation *station)
    : manager(station->getManager())
    , station(station)
{

}

StationMaterial::~StationMaterial()
{

}


bool StationMaterial::initialMaterial(const QString &materialID)
{
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[ProcessCode] AS [CraftID]
                      ,A.[WorkStationCode] AS [StationID]
                      ,A.[MCode] AS [MaterialID]
                      ,M.[MName] AS [MaterialName]
                      ,A.[MBarCode] AS [BarcodeRule]
                      ,A.[CQ] AS [UsageQuantity]
                      ,A.[WarningCount] AS [AlarmQuantity]
                      FROM [MES_Process_WorkStationMaterial] A
                      LEFT JOIN [MES_db_MaterialInfo] M ON A.MCode = M.MCode
                      WHERE A.[ProcessCode] = ? AND A.[WorkStationCode] = ? AND A.[MCode] = ? AND A.[State] != ?)");
        query.addBindValue(manager->getCraftID());
        query.addBindValue(station->getStationID());
        query.addBindValue(materialID);
        query.addBindValue(RECORD_DELETE);

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

        materialData.materialID = query.value("MaterialID").toString();
        materialData.materialName = query.value("MaterialName").toString();
        materialData.barcodeRule = query.value("BarcodeRule").toString();
        materialData.usageQuantity = query.value("UsageQuantity").toInt();
        materialData.alarmQuantity = query.value("AlarmQuantity").toInt();

    }
    while (0);

    do
    {
        materialData.materialBarcode.clear();
        materialData.repairCount = 0;
        materialData.materialBatch.clear();
        materialData.packageBatch.clear();
        materialData.batchQuantity = 0;
    }
    while (0);

    return true;
}


bool StationMaterial::verifyBarcodeFormat(const QString &barcode)
{
    if (materialData.barcodeRule.size() == 0)
        return true;

    if (materialData.barcodeRule.size() != barcode.size())
        return false;

    for (int i = 0; i < barcode.size(); ++i)
    {
        if (materialData.barcodeRule.at(i) == QChar('*'))
            continue;

        if (materialData.barcodeRule.at(i) != barcode.at(i))
            return false;
    }

    return true;
}

bool StationMaterial::verifyBarcodeUnique(const QString &barcode)
{
    QSqlQuery query(qApplication->isOnline() ? REMOTE_DB : LOCAL_DB);
    query.prepare("SELECT UID FROM PCS_Data_Station_Material "
                  "WHERE MaterialID = ? AND MaterialBarcode = ?");
    query.addBindValue(materialData.materialID);
    query.addBindValue(barcode);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    return !query.first();
}

bool StationMaterial::verifyBarcodeRepair(const QString &barcode)
{
    return true;
}

bool StationMaterial::inputMaterialBatch(const QString &batch, int quantity)
{
    //判断工位物料表中是否有相关数据
//    do
//    {
//        QSqlQuery query(LOCAL_DB);
//        query.prepare(R"(SELECT * FROM [PCS_Material_batch]
//                      WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ?
//                      AND [MaterialID] = ? AND [MaterialBatch] = ? AND [BatchState] = ?)");
//        query.addBindValue(manager->getWorkshopID());
//        query.addBindValue(manager->getWorklineID());
//        query.addBindValue(station->getStationID());
//        query.addBindValue(materialData.materialID);
//        query.addBindValue(batch);
//        query.addBindValue(static_cast<int>(BatchState::Return));

//        if (!query.exec())
//        {
//            qDebug().noquote() << query.lastQuery();
//            return false;
//        }

//        if (query.first())
//        {
//            qDebug() << "inputMaterialBatch() error, batch has returned!";
//            return false;
//        }
//    }
//    while (0);

    //判断工位物料表中是否有相关数据
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT * FROM [PCS_Material_batch]
                      WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ?
                      AND [MaterialID] = ? AND [MaterialBatch] = ?)");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(materialData.materialID);
        query.addBindValue(batch);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (query.first())
        {
            materialData.materialBatch = query.value("MaterialBatch").toString();
            materialData.packageBatch = query.value("PackageBatch").toString();
            materialData.batchQuantity = query.value("BatchQuantity").toInt();
            return true;
        }
    }
    while (0);

    //向工位物料表中插入数据
    do
    {
        materialData.materialBatch = batch;
        materialData.batchQuantity = quantity;

        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(INSERT INTO [PCS_Material_Batch]
                      ([WorkshopID], [WorklineID], [StationID], [OrderID], [MaterialID], [MaterialBatch], [PackageBatch], [BatchQuantity], [BatchState])
                      VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?))");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(manager->getOrderID());
        query.addBindValue(materialData.materialID);
        query.addBindValue(materialData.materialBatch);
        query.addBindValue(materialData.packageBatch);
        query.addBindValue(materialData.batchQuantity);
        query.addBindValue(static_cast<int>(BatchState::Input));

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

    }
    while (0);

    return true;
}

//投入物料批次
bool StationMaterial::inputMaterialBatch(const QString &barcode)
{
    QString packageBatch = barcode.split(';').at(0);
    QString materialBatch = barcode.split(';').at(3);
    int batchQuantity = barcode.split(';').at(1).toInt();

    //判断工位物料表中是否有相关数据
//    do
//    {
//        QSqlQuery query(LOCAL_DB);
//        query.prepare(R"(SELECT * FROM [PCS_Material_batch]
//                      WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ?
//                      AND [MaterialID] = ? AND [MaterialBatch] = ? AND [PackageBatch] = ? AND [BatchState] = ?)");
//        query.addBindValue(manager->getWorkshopID());
//        query.addBindValue(manager->getWorklineID());
//        query.addBindValue(station->getStationID());
//        query.addBindValue(materialData.materialID);
//        query.addBindValue(materialBatch);
//        query.addBindValue(packageBatch);
//        query.addBindValue(static_cast<int>(BatchState::Return));

//        if (!query.exec())
//        {
//            qDebug().noquote() << query.lastQuery();
//            return false;
//        }

//        if (query.first())
//        {
//            qDebug() << "inputMaterialBatch() error, batch has returned!";
//            return false;
//        }
//    }
//    while (0);

    //判断工位物料表中是否有相关数据
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT * FROM [PCS_Material_batch]
                      WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ?
                      AND [MaterialID] = ? AND [MaterialBatch] = ? AND [PackageBatch] = ?)");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(materialData.materialID);
        query.addBindValue(materialBatch);
        query.addBindValue(packageBatch);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (query.first())
        {
            materialData.materialBatch = query.value("MaterialBatch").toString();
            materialData.packageBatch = query.value("PackageBatch").toString();
            materialData.batchQuantity = query.value("BatchQuantity").toInt();
            return true;
        }
    }
    while (0);

    //向工位物料表中插入数据
    do
    {
        materialData.materialBatch = materialBatch;
        materialData.packageBatch = packageBatch;
        materialData.batchQuantity = batchQuantity;

        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(INSERT INTO [PCS_Material_Batch]
                      ([WorkshopID], [WorklineID], [StationID], [OrderID], [MaterialID], [MaterialBatch], [PackageBatch], [BatchQuantity], [BatchState])
                      VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?))");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(manager->getOrderID());
        query.addBindValue(materialData.materialID);
        query.addBindValue(materialData.materialBatch);
        query.addBindValue(materialData.packageBatch);
        query.addBindValue(materialData.batchQuantity);
        query.addBindValue(static_cast<int>(BatchState::Input));

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

    }
    while (0);

    return true;
}

//退回物料批次
bool StationMaterial::returnMaterialBatch()
{
    if (!isBatchInput())
        return false;

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Material_batch] SET [BatchState] = ?, [BatchQuantity] = ?
                  WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ? AND [OrderID] = ?
                  AND [MaterialID] = ? AND [MaterialBatch] = ? AND [PackageBatch] = ? AND [BatchState] = ?)");
    query.addBindValue(static_cast<int>(BatchState::Return));
    query.addBindValue(materialData.batchQuantity);
    query.addBindValue(manager->getWorkshopID());
    query.addBindValue(manager->getWorklineID());
    query.addBindValue(station->getStationID());
    query.addBindValue(manager->getOrderID());
    query.addBindValue(materialData.materialID);
    query.addBindValue(materialData.materialBatch);
    query.addBindValue(materialData.packageBatch);
    query.addBindValue(static_cast<int>(BatchState::Input));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return false;
    }

    materialData.materialBatch.clear();
    materialData.packageBatch.clear();
    materialData.batchQuantity = 0;

    return true;
}

void StationMaterial::updateBatchQuantity()
{
    if (!isBatchInput())
        return;

    materialData.batchQuantity -= materialData.usageQuantity;

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Material_batch] SET [BatchQuantity] = ?
                  WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ? AND [OrderID] = ?
                  AND [MaterialID] = ? AND [MaterialBatch] = ? AND [PackageBatch] = ? AND [BatchState] = ?)");
    query.addBindValue(materialData.batchQuantity);
    query.addBindValue(manager->getWorkshopID());
    query.addBindValue(manager->getWorklineID());
    query.addBindValue(station->getStationID());
    query.addBindValue(manager->getOrderID());
    query.addBindValue(materialData.materialID);
    query.addBindValue(materialData.materialBatch);
    query.addBindValue(materialData.packageBatch);
    query.addBindValue(static_cast<int>(BatchState::Input));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }
}

void StationMaterial::adjustBatchQuantity(int quantity)
{
    if (!isBatchInput())
        return;

    materialData.batchQuantity = quantity;

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Material_batch] SET [BatchQuantity] = ?
                  WHERE [WorkshopID] = ? AND [WorklineID] = ? AND [StationID] = ? AND [OrderID] = ?
                  AND [MaterialID] = ? AND [MaterialBatch] = ? AND [PackageBatch] = ? AND [BatchState] = ?)");
    query.addBindValue(materialData.batchQuantity);
    query.addBindValue(manager->getWorkshopID());
    query.addBindValue(manager->getWorklineID());
    query.addBindValue(station->getStationID());
    query.addBindValue(manager->getOrderID());
    query.addBindValue(materialData.materialID);
    query.addBindValue(materialData.materialBatch);
    query.addBindValue(materialData.packageBatch);
    query.addBindValue(static_cast<int>(BatchState::Input));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }
}

