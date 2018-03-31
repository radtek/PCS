#include "StationOperator.h"
#include "WorkManager.h"
#include "WorkStation.h"

StationOperator::StationOperator(WorkStation *station)
    : manager(station->getManager())
    , station(station)
{

}

StationOperator::~StationOperator()
{

}

bool StationOperator::operatorLogin(const QString &barcode)
{
    //获取员工信息
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[WorkerCode] AS [OperatorID]
                      ,A.[WorkerBarCode] AS [OperatorCard]
                      ,A.[WorkerName] AS [OperatorName]
                      ,A.[Team] AS [OperatorTeam]
                      ,B.[DName] AS [OperatorDuty]
                      ,A.[Duty] AS [DutyLevel]
                      FROM [MES_db_WorkerInfo] A LEFT JOIN [MES_db_Dict] B
                      ON A.[Duty] = B.[DCode] AND B.[PDCode] = 'WorkerDuty'
                      WHERE A.[WorkShopCode] = ? AND A.[WorkerBarCode] = ? AND A.[State] != ?)");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(barcode);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query first() failed!";
            return false;
        }

        operatorData.operatorID = query.value("OperatorID").toString();
        operatorData.operatorCard = query.value("OperatorCard").toString();
        operatorData.operatorName = query.value("OperatorName").toString();
        operatorData.operatorTeam = query.value("OperatorTeam").toString();
        operatorData.operatorDuty = query.value("OperatorDuty").toString();
        operatorData.dutyLevel = static_cast<OperatorDuty>(query.value("DutyLevel").toInt());
    }
    while (0);

    //校验人员能力
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID] FROM [MES_Capacity_Workers]
                      WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [WorkerCode] = ?
                      AND ? IN (SELECT col FROM F_SPLIT([WorkStationCodes],',')) AND [State] != ?)");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(operatorData.operatorID);
        query.addBindValue(station->getStationID());
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "operator capacity verify failed!";
            return false;
        }
    }
    while (0);

    //判断是否在其它产线登陆
    do
    {
        QSqlQuery query(REMOTE_DB);
        query.prepare(R"(SELECT [UID] FROM [PCS_Operator_Hours]
                      WHERE [WorkshopID] = ? AND [WorklineID] != ? AND [OperatorID] = ?
                      AND LogState = ?)");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(operatorData.operatorID);
        query.addBindValue(static_cast<int>(LogState::Login));

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (query.first())
        {
            qDebug() << "operator has logged on other workline!";
            return false;
        }
    }
    while (0);

    //判断之前登陆是否登出
    do
    {
        QSqlQuery query(REMOTE_DB);
        query.prepare(R"(SELECT A.[LogMarker]
                      FROM [PCS_Operator_Hours] A
                      LEFT JOIN [PCS_Operator_Hours] B
                      ON A.LogMarker = B.LogMarker AND B.LogState = ?
                      WHERE A.[WorkshopID] = ? AND A.[WorklineID] = ? AND A.[StationID] = ?
                      AND A.[OrderID] = ? AND A.[OperatorID] = ? AND A.[LogState] = ? AND b.[UID] IS NULL)");
        query.addBindValue(static_cast<int>(LogState::Logout));
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(manager->getOrderID());
        query.addBindValue(operatorData.operatorID);
        query.addBindValue(static_cast<int>(LogState::Login));

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (query.first())
        {
            operatorData.logMarker = query.value("LogMarker").toString();
            return true;
        }
    }
    while (0);

    //插入登陆信息
    do
    {
        operatorData.logMarker = QUuid::createUuid().toString().remove('{').remove('}');

        QSqlQuery query(REMOTE_DB);
        query.prepare(R"(INSERT INTO [PCS_Operator_Hours]
                      ([WorkshopID], [WorklineID], [StationID], [OrderID], [OperatorID], [LogMarker], [LogTime], [LogState])
                      VALUES(?, ?, ?, ?, ?, ?, ?, ?))");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(manager->getOrderID());
        query.addBindValue(operatorData.operatorID);
        query.addBindValue(operatorData.logMarker);
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(static_cast<int>(LogState::Login));

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }
    }
    while (0);

    return true;
}

bool StationOperator::operatorLogout()
{
    //插入登陆信息
    do
    {
        QSqlQuery query(REMOTE_DB);
        query.prepare(R"(INSERT INTO [PCS_Operator_Hours]
                      ([WorkshopID], [WorklineID], [StationID], [OrderID], [OperatorID], [LogMarker], [LogTime], [LogState])
                      VALUES(?, ?, ?, ?, ?, ?, ?, ?))");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(station->getStationID());
        query.addBindValue(manager->getOrderID());
        query.addBindValue(operatorData.operatorID);
        query.addBindValue(operatorData.logMarker);
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(static_cast<int>(LogState::Logout));

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }
    }
    while (0);

    return true;
}


