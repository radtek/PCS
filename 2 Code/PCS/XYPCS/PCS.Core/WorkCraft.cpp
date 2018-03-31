#include "WorkCraft.h"
#include "WorkManager.h"

WorkCraft::WorkCraft(WorkManager *manager)
    : manager(manager)
{

}

WorkCraft::~WorkCraft()
{

}

bool WorkCraft::initialCraft(const QString &craftID)
{
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[ProcessCode] AS [CraftID]
                      ,A.[ProcessName] AS [CraftName]
                      ,M.[MCode] AS [ProductID]
                      ,M.[MName] AS [ProductName]
                      ,A.[AssySerialNumRule] AS [AssySerialRule]
                      ,A.[BoxSerialNumRule] AS [PackSerialRule]
                      ,A.[AssySerialNumStart] AS [AssySerialInit]
                      ,A.[BoxSerialNumStart] AS [PackSerialInit]
                      FROM [MES_Process_Info] A
                      LEFT JOIN [MES_db_MaterialInfo] M ON A.[MCode] = M.[MCode]
                      WHERE A.[WorkLineCode] = ? AND A.[ProcessCode] = ?)");
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(craftID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query craft route failed!";
            return false;
        }

        craftData.craftID = query.value("CraftID").toString();
        craftData.craftName = query.value("CraftName").toString();
        craftData.productID = query.value("ProductID").toString();
        craftData.productName = query.value("ProductName").toString();
        craftData.assySerialRule = static_cast<SerialRule>(query.value("AssySerialRule").toInt());
        craftData.packSerialRule = static_cast<SerialRule>(query.value("PackSerialRule").toInt());
        craftData.assySerialInit = query.value("AssySerialInit").toInt();
        craftData.packSerialInit = query.value("PackSerialInit").toInt();
    }
    while (0);

    return true;
}
