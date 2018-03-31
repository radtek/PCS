#include "WorkOrder.h"
#include "WorkManager.h"

WorkOrder::WorkOrder(WorkManager *manager)
    : manager(manager)
{
}

WorkOrder::~WorkOrder()
{
}

bool WorkOrder::initialOrder(const QString &orderID)
{
    //加载工单信息
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[WorkShopCode] AS [WorkshopID]
                      ,[WorkLineCode] AS [WorklineID]
                      ,[WOCode] AS [OrderID]
                      ,[PocessCode] AS [CraftID]
                      ,[ProduceBatch] AS [ProductBatch]
                      ,[PlanDate] AS [OrderPlanDate]
                      ,[PlanDailyYields] AS [OrderPlanQuantity]
                      ,[QASubmitted] AS [InspectQuantity]
                      ,[TotalCompleted] AS [FinishQuantity]
                      ,[TotalBrokenParts] AS [FailedQuantity]
                      ,[TotalPackageBox] AS [PackageQuanity]
                      ,[TotalSampleParts] AS [SampleQuantity]
                      ,[TotalRepairParts] AS [RepairQuantity]
                      ,[State]
                      FROM [MES_WorkOrder]
                      WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [WOCode] = ?)");
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order plan failed!";
            return false;
        }

        orderData.orderID = query.value("OrderID").toString();
        orderData.craftID = query.value("CraftID").toString();
        orderData.productBatch = query.value("ProductBatch").toString();
        orderData.orderPlanDate = query.value("OrderPlanDate").toDate();
        orderData.orderPlanQuantity = query.value("OrderPlanQuantity").toInt();
        orderData.inspectQuantity = query.value("InspectQuantity").toInt();

        if (orderData.inspectQuantity == 0)
        {
            orderData.inspectInterval = 0.0;
        }
        else
        {
            orderData.inspectInterval = orderData.orderPlanQuantity / (orderData.inspectQuantity + 1.0);
        }
    } while (0);

    //获取已完成数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Assembly "
                      "WHERE OrderID = ? AND AssemblyState = 0");
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order finish quantity failed!";
            return false;
        }

        orderData.finishQuantity = query.value("Count").toInt();
    } while (0);

    //获取不合格数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Station "
                      "WHERE OrderID = ? AND AssemblyState = 1");
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order failed quantity failed!";
            return false;
        }

        orderData.failedQuantity = query.value("Count").toInt();
    } while (0);

    //获取包装箱数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Package "
                      "WHERE OrderID = ?");
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order package quantity failed!";
            return false;
        }

        orderData.packageQuantity = query.value("Count").toInt();
    } while (0);

    //获取抽检件数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Assembly "
                      "WHERE OrderID = ? AND SampleState = 1");
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order sample quantity failed!";
            return false;
        }

        orderData.sampleQuantity = query.value("Count").toInt();
    } while (0);

    //获取返修件数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Assembly "
                      "WHERE OrderID = ? AND RepairState = 1");
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order repair quantity failed!";
            return false;
        }

        orderData.repairQuantity = query.value("Count").toInt();
    } while (0);

    //获取一致性成功数量
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare("SELECT COUNT(UID) AS Count FROM PCS_Data_Assembly "
                      "WHERE OrderID = ? AND AssemblyState = 0 AND RetestState = 0");
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }

        if (!query.first())
        {
            qDebug() << "query order firstpass quantity failed!";
            return false;
        }

        orderData.firstPassQuantity = query.value("Count").toInt();
    } while (0);

    do
    {
        firstProductQA();
        firstPackageQA();
        finalProductQA();
        finalPackageQA();
    } while (0);

    return true;
}

// TODO 更新工单数量，存入数据库
bool WorkOrder::saveOrderData()
{
    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(UPDATE MES_WorkOrder SET
                      [TotalCompleted] = ?,
                      [TotalBrokenParts] = ?,
                      [TotalPackageBox] = ?,
                      [TotalSampleParts] = ?,
                      [TotalRepairParts] = ?,
                      [TotalPrimaryMolding] = ?
                      WHERE [WorkShopCode] = ? AND [WorkLineCode] = ? AND [WOCode] = ?)");
        query.addBindValue(orderData.finishQuantity);
        query.addBindValue(orderData.failedQuantity);
        query.addBindValue(orderData.packageQuantity);
        query.addBindValue(orderData.sampleQuantity);
        query.addBindValue(orderData.repairQuantity);
        query.addBindValue(orderData.firstPassQuantity);
        query.addBindValue(manager->getWorkshopID());
        query.addBindValue(manager->getWorklineID());
        query.addBindValue(orderData.orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return false;
        }
    } while (0);

    emit manager->signalOrderPlanUpdate(orderData);

    return true;
}

//校验首件检验是否执行
bool WorkOrder::verifyFirstProductQA()
{
    return orderData.isFirstProductQA;
}

//校验首箱检验是否执行
bool WorkOrder::verifyFirstPackageQA()
{
    return orderData.isFirstPackageQA;
}

//校验末件质检是否执行
bool WorkOrder::verifyFinalProductQA()
{
    return orderData.isFinalProductQA;
}

//校验末箱质检是否执行
bool WorkOrder::verifyFinalPackageQA()
{
    return orderData.isFinalPackageQA;
}

//首件检验
void WorkOrder::firstProductQA()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_QA_DocMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ?
                  AND [WOCode] = ? AND CONVERT(NVARCHAR, [QATime], 23) = ?
                  AND [Dict_MouldTypeCode] = 'QAMT01' AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(qWorkManager->getWorkshopID());
    query.addBindValue(qWorkManager->getWorklineID());
    query.addBindValue(orderData.orderID);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        orderData.firstProductQADocID = query.value("DocumentID").toString();
        orderData.isFirstProductQA = true;
    }
    else
    {
        orderData.firstProductQADocID.clear();
        orderData.isFirstProductQA = false;
    }

    emit manager->signalOrderPlanUpdate(orderData);
}

//首箱检验
void WorkOrder::firstPackageQA()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_QA_DocMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ?
                  AND [WOCode] = ? AND CONVERT(NVARCHAR, [QATime], 23) = ?
                  AND [Dict_MouldTypeCode] = 'QAMT06' AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(qWorkManager->getWorkshopID());
    query.addBindValue(qWorkManager->getWorklineID());
    query.addBindValue(orderData.orderID);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        orderData.firstPackageQADocID = query.value("DocumentID").toString();
        orderData.isFirstPackageQA = true;
    }
    else
    {
        orderData.firstPackageQADocID.clear();
        orderData.isFirstPackageQA = false;
    }

    emit manager->signalOrderPlanUpdate(orderData);
}

//末件质检
void WorkOrder::finalProductQA()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_QA_DocMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ?
                  AND [WOCode] = ? AND CONVERT(NVARCHAR, [QATime], 23) = ?
                  AND [Dict_MouldTypeCode] = 'QAMT03' AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(qWorkManager->getWorkshopID());
    query.addBindValue(qWorkManager->getWorklineID());
    query.addBindValue(orderData.orderID);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        orderData.finalProductQADocID = query.value("DocumentID").toString();
        orderData.isFinalProductQA = true;
    }
    else
    {
        orderData.finalProductQADocID.clear();
        orderData.isFinalProductQA = false;
    }

    emit manager->signalOrderPlanUpdate(orderData);
}

//末箱质检
void WorkOrder::finalPackageQA()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [DocCode] AS [DocumentID] FROM [MES_QA_DocMaster]
                  WHERE [WorkShopCode] = ? AND [WorkLineCode] = ?
                  AND [WOCode] = ? AND CONVERT(NVARCHAR, [QATime], 23) = ?
                  AND [Dict_MouldTypeCode] = 'QAMT07' AND [DocResult] = 0 AND [State] = 3)");
    query.addBindValue(qWorkManager->getWorkshopID());
    query.addBindValue(qWorkManager->getWorklineID());
    query.addBindValue(orderData.orderID);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (query.last())
    {
        orderData.finalPackageQADocID = query.value("DocumentID").toString();
        orderData.isFinalPackageQA = true;
    }
    else
    {
        orderData.finalPackageQADocID.clear();
        orderData.isFinalPackageQA = false;
    }

    emit manager->signalOrderPlanUpdate(orderData);
}
