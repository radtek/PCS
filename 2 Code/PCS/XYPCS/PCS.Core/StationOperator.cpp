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
        query.prepare(R"(SELECT [UID]
                      ,[WorkerID]
                      ,[WorkerBarCode]
                      ,[WorkerName]
                      ,[WorkerDuty]
                      ,[WorkerDuty] AS [DutyLevel]
                      FROM [PCS_Base_Worker]
                      WHERE [WorkShopID] = ? AND [WorkerBarCode] = ? AND [State] != ?)");
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

        operatorData.operatorID = query.value("WorkerID").toString();
        operatorData.operatorCard = query.value("WorkerBarCode").toString();
        operatorData.operatorName = query.value("WorkerName").toString();
        //   operatorData.operatorTeam = query.value("OperatorTeam").toString();//@@@
        operatorData.operatorDuty = query.value("WorkerDuty").toString();
        operatorData.dutyLevel = static_cast<OperatorDuty>(query.value("DutyLevel").toInt());    //@@@
    } while (0);

    //校验人员能力
    /*   do
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
    while (0);*/

    //判断是否在其它产线登陆
    /*  do
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
    while (0);*/

    //判断之前登陆是否登出
    /*   do
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
    while (0);*/

    //插入登陆信息
    do
    {
        operatorData.logMarker = QUuid::createUuid().toString().remove('{').remove('}');

        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(INSERT INTO [PCS_Operator_Hours]
                      ([WorkShopID], [WorkLineID], [WorkStationID], [OrderID], [WorkerID], [LogMarker], [LogTime], [LogState])
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
    } while (0);

    return true;
}

bool StationOperator::operatorLogout()
{
    //插入登陆信息
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(INSERT INTO [PCS_Operator_Hours]
                      ([WorkShopID], [WorkLineID], [WorkStationID], [OrderID], [WorkerID], [LogMarker], [LogTime], [LogState])
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
    } while (0);

    return true;
}
