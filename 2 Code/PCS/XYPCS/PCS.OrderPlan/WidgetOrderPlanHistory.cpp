#include "WidgetOrderPlanHistory.h"
#include "WorkManager.h"
#include "ui_WidgetOrderPlanHistory.h"

WidgetOrderPlanHistory::WidgetOrderPlanHistory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetOrderPlanHistory)
    , orderModel(new QSqlQueryModel)
{
    ui->setupUi(this);

    initialWidget();
}

WidgetOrderPlanHistory::~WidgetOrderPlanHistory()
{
    delete orderModel;
    delete ui;
}

void WidgetOrderPlanHistory::initialWidget()
{
    do
    {
        ui->comboOrderState->lineEdit()->setReadOnly(true);

        ui->comboOrderState->addItem(orderStateMap.value(OrderState::Closed));
        ui->comboOrderState->addItem(orderStateMap.value(OrderState::ForceClose));
        ui->comboOrderState->addItem(orderStateMap.value(OrderState::Delete));
        ui->comboOrderState->addItem(orderStateMap.value(OrderState::Return));
        ui->comboOrderState->addItem(orderStateMap.value(OrderState::PlanReturn));

        ui->datePlanDate->setDate(QDate::currentDate());
        ui->dateDeliveryDate->setDate(QDate::currentDate());

        connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(slotRefreshView()));
    } while (0);

    do
    {
        QTableView *table = ui->tableView;
        table->setModel(orderModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setSelectionMode(QTableView::SingleSelection);    //单选
        table->setSelectionBehavior(QTableView::SelectRows);     //选取一行
        table->setEditTriggers(QTableView::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        orderSQL = QString(R"(SELECT A.[UID] AS [序号]
                           ,B.[ProductID] AS [产品编号]
                           ,B.[ProductName] AS [产品名称]
                           ,A.[ProductionBatch] AS [生产批次]
                           ,A.[OrderID] AS [工单编号]
                           ,A.[CraftID] AS [工艺编号]
                           ,CONVERT(NVARCHAR, A.[PlanDate], 23) AS [计划日期]
                           ,CONVERT(NVARCHAR, A.[FinishTime], 20) AS [完结时间]
                           ,A.[PlanProductionQuantity] AS [计划生产数量]
                           ,A.[PlanInspectionQuantity] AS [计划抽检数量]
                           ,A.[QualifiedQuantity] AS [合格数量]
                           ,A.[UnqualifiedQuantity] AS [不合格数量]
                           ,A.[FirstPassQuantity] AS [一次成功数]
                           ,A.[PackageQuantity] AS [包装箱数量]
                           ,A.[InspectionQuantity] AS [抽检件数量]
                           ,A.[RepairQuantity] AS [返修件数量]
                           ,CASE A.[State] WHEN %1 THEN '%2' WHEN %3 THEN '%4' WHEN %5 THEN '%6'
                           WHEN %7 THEN '%8' WHEN %9 THEN '%10' END AS [状态]
                           ,A.[Description] AS [备注]
                           FROM [PCS_WorkOrder] A LEFT JOIN [PCS_Craft] B ON B.[CraftID]=A.[CraftID]
                           WHERE A.[WorkShopID] = '%11' AND A.[WorkLineID] = '%12'
                           AND (A.[State] = %13 OR A.[State] = %14 OR A.[State] = %15 OR A.[State] = %16 OR A.[State] = %17))")
                       .arg(static_cast<int>(OrderState::Closed))
                       .arg(orderStateMap.value(OrderState::Closed))
                       .arg(static_cast<int>(OrderState::ForceClose))
                       .arg(orderStateMap.value(OrderState::ForceClose))
                       .arg(static_cast<int>(OrderState::Delete))
                       .arg(orderStateMap.value(OrderState::Delete))
                       .arg(static_cast<int>(OrderState::Return))
                       .arg(orderStateMap.value(OrderState::Return))
                       .arg(static_cast<int>(OrderState::PlanReturn))
                       .arg(orderStateMap.value(OrderState::PlanReturn))
                       .arg(qWorkManager->getWorkshopID(), qWorkManager->getWorklineID())
                       .arg(static_cast<int>(OrderState::Closed))
                       .arg(static_cast<int>(OrderState::ForceClose))
                       .arg(static_cast<int>(OrderState::Delete))
                       .arg(static_cast<int>(OrderState::Return))
                       .arg(static_cast<int>(OrderState::PlanReturn));
    } while (0);
}

void WidgetOrderPlanHistory::slotRefreshView()
{
    if (ui->radioAllOrder->isChecked())
        queryByAllOrder();
    else if (ui->radioProductID->isChecked())
        queryByProductID();
    else if (ui->radioCraftID->isChecked())
        queryByCraftID();
    else if (ui->radioProductBatch->isChecked())
        queryByProductBatch();
    else if (ui->radioOrderState->isChecked())
        queryByOrderState();
    else if (ui->radioPlanDate->isChecked())
        queryByPlanDate();
    else if (ui->radioDeliveryDate->isChecked())
        queryByDeliveryDate();
    else
        queryByAllOrder();
}

void WidgetOrderPlanHistory::queryByAllOrder()
{
    orderModel->setQuery(orderSQL, LOCAL_DB);
}

void WidgetOrderPlanHistory::queryByProductID()
{
    QString productID = ui->editProductID->text();
    QString filter = QString(" AND B.[ProductID] = '%1'").arg(productID);

    orderModel->setQuery(orderSQL + filter, LOCAL_DB);
}

void WidgetOrderPlanHistory::queryByCraftID()
{
    QString craftID = ui->editCraftID->text();
    QString filter = QString(" AND A.[CraftID] = '%1'").arg(craftID);

    orderModel->setQuery(orderSQL + filter, LOCAL_DB);
}

void WidgetOrderPlanHistory::queryByProductBatch()
{
    QString prductBatch = ui->editProductBatch->text();
    QString filter = QString(" AND A.[ProductionBatch] = '%1'").arg(prductBatch);

    orderModel->setQuery(orderSQL + filter, LOCAL_DB);
}

void WidgetOrderPlanHistory::queryByOrderState()
{
    OrderState state = orderStateMap.key(ui->comboOrderState->currentText());
    QString filter = QString(" AND A.[State] = %1").arg(static_cast<int>(state));

    orderModel->setQuery(orderSQL + filter, LOCAL_DB);
}

void WidgetOrderPlanHistory::queryByPlanDate()
{
    QString planDate = ui->datePlanDate->date().toString("yyyy-MM-dd");
    QString filter = QString(" AND CONVERT(NVARCHAR, A.[PlanDate], 23) = '%1'").arg(planDate);

    orderModel->setQuery(orderSQL + filter, LOCAL_DB);
}

void WidgetOrderPlanHistory::queryByDeliveryDate()
{
    QString deliveryDate = ui->dateDeliveryDate->date().toString("yyyy-MM-dd");
    QString filter = QString(" AND CONVERT(NVARCHAR, A.[FinishTime], 23) = '%1'").arg(deliveryDate);

    orderModel->setQuery(orderSQL + filter, LOCAL_DB);
}
