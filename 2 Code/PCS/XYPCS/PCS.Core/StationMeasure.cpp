#include "StationMeasure.h"
#include "WorkManager.h"
#include "WorkStation.h"

StationMeasure::StationMeasure(WorkStation *station)
    : manager(station->getManager())
    , station(station)
{

}

StationMeasure::~StationMeasure()
{

}

bool StationMeasure::initialMeasure(const QString &measureID)
{
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[ProcessCode] AS [CraftID]
                      ,A.[WorkStationCode] AS [StationID]
                      ,A.[Dict_ParaTypeCode] + '#' + CAST(A.[OrderNum] AS NVARCHAR) AS [MeasureID]
                      ,D.[DName] + '#' + CAST(A.[OrderNum] AS NVARCHAR) AS [MeasureName]
                      ,D.[DClass] AS [MeasureUnit]
                      ,A.[StandardValue] AS [StandardValue]
                      ,A.[UpperLimit] AS [UpperLimit]
                      ,A.[LowerLimit] AS [LowerLimit]
                      ,A.[State]
                      FROM [MES_Process_Para] A
                      LEFT JOIN [MES_db_Dict] D ON A.[Dict_ParaTypeCode] = D.[Dcode]
                      WHERE A.[ProcessCode] = ? AND A.[WorkStationCode] = ?
                      AND A.[Dict_ParaTypeCode] + '#' + CAST(A.[OrderNum] AS NVARCHAR) = ? AND A.[State] != ?)");
        query.addBindValue(manager->getCraftID());
        query.addBindValue(station->getStationID());
        query.addBindValue(measureID);
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

        measureData.measureID = query.value("MeasureID").toString();
        measureData.measureName = query.value("MeasureName").toString();
        measureData.measureUnit = query.value("MeasureUnit").toString();
        measureData.standardValue = query.value("StandardValue").toDouble();
        measureData.upperLimit = query.value("UpperLimit").toDouble();
        measureData.lowerLimit = query.value("LowerLimit").toDouble();
    }
    while (0);

    do
    {
        measureData.measureValue = 0.0;
        measureData.measureState = 0;
        measureData.retestCount = 0;
    }
    while (0);

    return true;
}

bool StationMeasure::verifyValue(double value)
{
    qDebug() << "StationMeasure verifyValue" << value << measureData.upperLimit << measureData.lowerLimit;

    return value <= measureData.upperLimit && value >= measureData.lowerLimit;
}
