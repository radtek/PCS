#include "StationFixture.h"
#include "WorkManager.h"
#include "WorkStation.h"

StationFixture::StationFixture(WorkStation *station)
    : manager(station->getManager())
    , station(station)
{

}

StationFixture::~StationFixture()
{

}

bool StationFixture::loadFixture(const QString &fixtureID)
{
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[DeviceCode] AS [FixtureID]
                      ,I.[DeviceName] AS [FixtureName]
                      ,A.[DeviceBarCode] AS [FixtureBarcode]
                      ,D1.[DName] + D2.[DName] AS [FixtureClassify]
                      ,I.[TotalTimes] AS [TotalTimes]
                      ,I.[UsedTimes] AS [UsedTimes]
                      ,I.[RemindTimes] AS [RemindTimes]
                      ,A.[State]
                      FROM [MES_Process_DeviceMapping] A
                      LEFT JOIN [MES_Device_Info] I ON A.[DeviceCode] = I.[DeviceCode]
                      LEFT JOIN [MES_db_Dict] D1 ON D1.[PDCode] = 'Dict_DeviceType' AND A.[LedgerType] = D1.[Dcode]
                      LEFT JOIN [MES_db_Dict] D2
                      ON (D2.[PDCode] = 'TechnologyClassify' AND A.[LedgerType] = 1 AND A.[DeviceClassify] = D2.[Dcode])
                      OR (D2.[PDCode] = 'MouldClassify' AND A.[LedgerType] = 2 AND A.[DeviceClassify] = D2.[Dcode])
                      WHERE A.[ProcessCode] = ? AND A.[WorkStationCode] = ? AND A.[DeviceCode] = ? AND A.[State] != ?)");
        query.addBindValue(manager->getCraftID());
        query.addBindValue(station->getStationID());
        query.addBindValue(fixtureID);
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

        fixtureData.fixtureID = query.value("FixtureID").toString();
        fixtureData.fixtureName = query.value("FixtureName").toString();
        fixtureData.fixtureBarcode = query.value("FixtureBarcode").toString();
        fixtureData.fixtureClassify = query.value("FixtureClassify").toString();
        fixtureData.totalTimes = query.value("TotalTimes").toInt();
        fixtureData.usedTimes = query.value("UsedTimes").toInt();
        fixtureData.remindTimes = query.value("RemindTimes").toInt();
    }
    while (0);

    return true;
}

void StationFixture::updateUsedTimes()
{
    ++fixtureData.usedTimes;
}













