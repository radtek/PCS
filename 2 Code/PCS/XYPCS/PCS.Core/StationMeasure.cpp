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
                      ,A.[CraftID]
                      ,A.[WorkStationID]
                      ,A.[MeasureType] + '#' + CAST(A.[MeasureOrder] AS NVARCHAR) AS [MeasureID]
                      ,D.[MeasureName] + '#' + CAST(A.[MeasureOrder] AS NVARCHAR) AS [MeasureName]
                      ,D.[Unit]
                      ,A.[StandardValue]
                      ,A.[UpperLimit]
                      ,A.[LowerLimit]
                      ,A.[State]
                      FROM [PCS_Craft_Station_Measure] A
                      LEFT JOIN [PCS_Base_Measure] D ON A.[MeasureType] = D.[MeasureType]
                      WHERE A.[CraftID] = ? AND A.[WorkStationID] = ?
                      AND A.[MeasureType] + '#' + CAST(A.[MeasureOrder] AS NVARCHAR) = ? AND A.[State] != ?)");
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
        measureData.measureUnit = query.value("Unit").toString();
        measureData.standardValue = query.value("StandardValue").toDouble();
        measureData.upperLimit = query.value("UpperLimit").toDouble();
        measureData.lowerLimit = query.value("LowerLimit").toDouble();
    } while (0);

    do
    {
        measureData.measureValue = 0.0;
        measureData.measureState = 0;
        measureData.retestCount = 0;
    } while (0);

    return true;
}

bool StationMeasure::verifyValue(double value)
{
    qDebug() << "StationMeasure verifyValue" << value << measureData.upperLimit << measureData.lowerLimit;

    return value <= measureData.upperLimit && value >= measureData.lowerLimit;
}
