#include "WidgetOrderPlanManage.h"
#include "DialogCreateWorkOrder.h"
#include "DialogOrderPlanRemark.h"
#include "PCSApplication.h"
#include "ui_WidgetOrderPlanManage.h"

WidgetOrderPlanManage::WidgetOrderPlanManage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetOrderPlanManage)
{
    ui->setupUi(this);

    initialWidget();
    initialOrder();
    initialButton();
}

WidgetOrderPlanManage::~WidgetOrderPlanManage()
{
    delete ui;
}

void WidgetOrderPlanManage::initialWidget()
{
    do
    {
        tableHeaderMap.insert(TableHeader::OrderID, "工单编号");
        tableHeaderMap.insert(TableHeader::CraftID, "工艺编号");
        tableHeaderMap.insert(TableHeader::ProductID, "产品编号");
        tableHeaderMap.insert(TableHeader::ProductName, "产品名称");
        tableHeaderMap.insert(TableHeader::ProductBatch, "生产批次");
        tableHeaderMap.insert(TableHeader::OrderPlanDate, "计划生产日期");
        tableHeaderMap.insert(TableHeader::OrderPlanQuantity, "计划生产数量");
        tableHeaderMap.insert(TableHeader::InspectQuantity, "计划抽检数量");
        tableHeaderMap.insert(TableHeader::OrderState, "工单状态");

        QTableWidget *table = ui->tableWidget;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(tableHeaderMap.size());
        table->setHorizontalHeaderLabels(tableHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑
        table->setAlternatingRowColors(true);

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectOrder()));
    } while (0);

    do
    {
        connect(ui->buttonSyncOrder, SIGNAL(clicked()), this, SLOT(slotSyncOnline()));
        connect(ui->buttonLoadOffline, SIGNAL(clicked()), this, SLOT(slotLoadOffline()));
        connect(ui->buttonRefreshOrder, SIGNAL(clicked()), this, SLOT(slotRefreshOrder()));
        connect(ui->buttonReturnOrder, SIGNAL(clicked()), this, SLOT(slotReturnOrder()));
        connect(ui->buttonForceClose, SIGNAL(clicked()), this, SLOT(slotForceClose()));
        connect(ui->buttonActivateOrder, SIGNAL(clicked()), this, SLOT(slotActivateOrder()));
        connect(ui->buttonSuspendOrder, SIGNAL(clicked()), this, SLOT(slotSuspendOrder()));
        connect(ui->buttonCloseOrder, SIGNAL(clicked()), this, SLOT(slotCloseOrder()));
        connect(ui->buttonAddOrder, SIGNAL(clicked()), this, SLOT(slotAddOrder()));
    } while (0);
}

void WidgetOrderPlanManage::initialOrder()
{
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;
    table->clearContents();
    table->setRowCount(0);

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT A.[UID]
                  ,A.[OrderID]
                  ,A.[ProductionBatch]
                  ,A.[CraftID]
                  ,CONVERT(NVARCHAR, A.[PlanDate], 23) AS [PlanDate]
                  ,A.[PlanProductionQuantity]
                  ,A.[PlanInspectionQuantity]
                  ,A.[State]
                  ,B.[ProductID]
                  ,B.[ProductName]
              FROM [PCS_WorkOrder] A LEFT JOIN [PCS_Craft] B ON B.[CraftID]=A.[CraftID]
                  WHERE A.[State] = ? OR A.[State] = ? OR A.[State] = ?)");
    //   query.addBindValue(qWorkManager->getWorkshopID());
    //   query.addBindValue(qWorkManager->getWorklineID());
    query.addBindValue(static_cast<int>(OrderState::Create));
    query.addBindValue(static_cast<int>(OrderState::Activated));
    query.addBindValue(static_cast<int>(OrderState::Suspended));

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    for (int row = 0; query.next(); ++row)
    {
        table->insertRow(row);
        table->setRowHeight(row, 25);

        item = new QTableWidgetItem(query.value("OrderID").toString());
        table->setItem(row, static_cast<int>(TableHeader::OrderID), item);

        item = new QTableWidgetItem(query.value("CraftID").toString());
        table->setItem(row, static_cast<int>(TableHeader::CraftID), item);

        item = new QTableWidgetItem(query.value("ProductID").toString());
        table->setItem(row, static_cast<int>(TableHeader::ProductID), item);

        item = new QTableWidgetItem(query.value("ProductName").toString());
        table->setItem(row, static_cast<int>(TableHeader::ProductName), item);

        item = new QTableWidgetItem(query.value("ProductionBatch").toString());
        table->setItem(row, static_cast<int>(TableHeader::ProductBatch), item);

        item = new QTableWidgetItem(query.value("PlanDate").toString());
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, static_cast<int>(TableHeader::OrderPlanDate), item);

        item = new QTableWidgetItem(query.value("PlanProductionQuantity").toString());
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, static_cast<int>(TableHeader::OrderPlanQuantity), item);

        item = new QTableWidgetItem(query.value("PlanInspectionQuantity").toString());
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, static_cast<int>(TableHeader::InspectQuantity), item);

        OrderState state = static_cast<OrderState>(query.value("State").toInt());
        item = new QTableWidgetItem(orderStateMap.value(state));
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, static_cast<int>(TableHeader::OrderState), item);
    }
}

void WidgetOrderPlanManage::initialButton()
{
    ui->buttonActivateOrder->setEnabled(false);
    ui->buttonSuspendOrder->setEnabled(false);
    ui->buttonCloseOrder->setEnabled(false);
    ui->buttonReturnOrder->setEnabled(false);
    ui->buttonForceClose->setEnabled(false);
}

void WidgetOrderPlanManage::changeButtonState(OrderState state)
{
    switch (state)
    {
    case OrderState::Issued:

    case OrderState::Delete:
    case OrderState::Closed:
    case OrderState::ForceClose:
    case OrderState::Return:
    case OrderState::PlanReturn:
        ui->buttonActivateOrder->setEnabled(false);
        ui->buttonSuspendOrder->setEnabled(false);
        ui->buttonCloseOrder->setEnabled(false);
        ui->buttonReturnOrder->setEnabled(false);
        ui->buttonForceClose->setEnabled(false);
        break;

    case OrderState::Create:
        ui->buttonActivateOrder->setEnabled(true);
        ui->buttonSuspendOrder->setEnabled(false);
        ui->buttonReturnOrder->setEnabled(true);
        ui->buttonForceClose->setEnabled(false);
        ui->buttonCloseOrder->setEnabled(false);
        break;

    case OrderState::Activated:
        ui->buttonActivateOrder->setEnabled(true);
        ui->buttonSuspendOrder->setEnabled(true);
        ui->buttonReturnOrder->setEnabled(false);
        ui->buttonForceClose->setEnabled(true);
        ui->buttonCloseOrder->setEnabled(true);
        break;

    case OrderState::Suspended:
        ui->buttonActivateOrder->setEnabled(true);
        ui->buttonSuspendOrder->setEnabled(false);
        ui->buttonReturnOrder->setEnabled(false);
        ui->buttonForceClose->setEnabled(true);
        ui->buttonCloseOrder->setEnabled(true);
        break;
    }
}

void WidgetOrderPlanManage::displayOrderInfo(const QString &orderID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT A.[UID]
                  ,A.[ProductionBatch]
                  ,A.[CraftID]
                  ,A.[OrderID]
                  ,CONVERT(NVARCHAR, A.[PlanDate], 23) AS [PlanDate]
                  ,A.[PlanProductionQuantity]
                  ,A.[PlanInspectionQuantity]
                  ,A.[FirstPassQuantity]
                  ,A.[UnqualifiedQuantity]
                  ,A.[QualifiedQuantity]
                  ,A.[InspectionQuantity]
                  ,A.[RepairQuantity]
                  ,A.[PackageQuantity]
                  ,A.[FinishTime]
                  ,A.[Description]
                  ,B.[ProductID]
                  ,B.[ProductName]
                  ,CASE A.[State] WHEN ? THEN ? WHEN ? THEN ? WHEN ? THEN ?
                  WHEN ? THEN ? WHEN ? THEN ? WHEN ? THEN ? WHEN ? THEN ?
                  WHEN ? THEN ? WHEN ? THEN ? END AS [State]
                  FROM [PCS_WorkOrder] A LEFT JOIN [PCS_Craft] B ON B.[CraftID]=A.[CraftID]
                    WHERE A.[OrderID]=? )");
    query.addBindValue(static_cast<int>(OrderState::Create));
    query.addBindValue(orderStateMap.value(OrderState::Create));
    query.addBindValue(static_cast<int>(OrderState::Issued));
    query.addBindValue(orderStateMap.value(OrderState::Issued));
    query.addBindValue(static_cast<int>(OrderState::Closed));
    query.addBindValue(orderStateMap.value(OrderState::Closed));
    query.addBindValue(static_cast<int>(OrderState::ForceClose));
    query.addBindValue(orderStateMap.value(OrderState::ForceClose));
    query.addBindValue(static_cast<int>(OrderState::Delete));
    query.addBindValue(orderStateMap.value(OrderState::Delete));
    query.addBindValue(static_cast<int>(OrderState::Return));
    query.addBindValue(orderStateMap.value(OrderState::Return));
    query.addBindValue(static_cast<int>(OrderState::PlanReturn));
    query.addBindValue(orderStateMap.value(OrderState::PlanReturn));
    query.addBindValue(static_cast<int>(OrderState::Activated));
    query.addBindValue(orderStateMap.value(OrderState::Activated));
    query.addBindValue(static_cast<int>(OrderState::Suspended));
    query.addBindValue(orderStateMap.value(OrderState::Suspended));
    query.addBindValue(orderID);

    if (!query.exec())
    {
        qDebug().noquote() << query.lastQuery();
        return;
    }

    if (!query.first())
    {
        return;
    }

    ui->textProductID->setText(query.value("ProductID").toString());
    ui->textProductName->setText(query.value("ProductName").toString());
    ui->textOrderID->setText(query.value("OrderID").toString());
    ui->textCraftID->setText(query.value("CraftID").toString());
    ui->textProductBatch->setText(query.value("ProductionBatch").toString());
    ui->textState->setText(query.value("State").toString());

    ui->textOrderPlanDate->setText(query.value("PlanDate").toString());
    ui->valueOrderPlanQty->setText(query.value("PlanProductionQuantity").toString());
    ui->valueInspectQty->setText(query.value("PlanInspectionQuantity").toString());
    ui->valueFirstPassQty->setText(query.value("FirstPassQuantity").toString());
    ui->valueFailedQty->setText(query.value("UnqualifiedQuantity").toString());
    ui->textRemark->setText(query.value("Description").toString());

    ui->valueFinishQty->setText(query.value("QualifiedQuantity").toString());
    ui->valueSampleQty->setText(query.value("InspectionQuantity").toString());
    ui->valueRepairQty->setText(query.value("RepairQuantity").toString());
    ui->valuePackageQty->setText(query.value("PackageQuantity").toString());
    ui->textDeliveryTime->setText(query.value("FinishTime").toString());

    // ui->textPlanID->setText(query.value("PlanID").toString());
    // ui->textPlanBatch->setText(query.value("PlanBatch").toString());
    // ui->textPlanDate->setText(query.value("PlanDate").toString());
    // ui->textPlanType->setText(query.value("PlanType").toString());
    //  ui->textProductTeam->setText(query.value("ProductTeam").toString());
    // ui->textPriority->setText(query.value("Priority").toString());
    // ui->textNumber->setText(query.value("Number").toString());
}

void WidgetOrderPlanManage::slotSelectOrder()
{
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;
    int row = table->currentRow();

    if (row == -1)
    {
        initialButton();
        return;
    }

    item = table->item(row, static_cast<int>(TableHeader::OrderState));
    changeButtonState(orderStateMap.key(item->text()));

    item = table->item(row, static_cast<int>(TableHeader::OrderID));
    qDebug() << item->text();
    displayOrderInfo(item->text());
}

void WidgetOrderPlanManage::slotRefreshOrder()
{
    initialOrder();
    initialButton();
}

//在线同步工单
void WidgetOrderPlanManage::slotSyncOnline()
{
    if (!qApplication->isOnline())
    {
        MessageBox_Warning("目前处于离线生产状态！");
        return;
    }

    slotRefreshOrder();
}

//离线导入工单
void WidgetOrderPlanManage::slotLoadOffline()
{
}

//退回工单
void WidgetOrderPlanManage::slotReturnOrder()
{
    if (!qApplication->isOnline())
    {
        MessageBox_Warning("目前处于离线生产状态！");
        return;
    }

    QTableWidget *table = ui->tableWidget;
    int row = table->currentRow();

    if (row == -1)
        return;

    DialogOrderPlanRemark dialog(this);
    dialog.setReason("请说明退回工单的原因：");
    dialog.setNotice("注意：退回此工单后，无法恢复！");

    if (QDialog::Accepted != dialog.exec())
        return;

    changeOrderState(row, OrderState::Return, dialog.getRemark());
}

//强制完结工单
void WidgetOrderPlanManage::slotForceClose()
{
    if (!qApplication->isOnline())
    {
        MessageBox_Warning("目前处于离线生产状态！");
        return;
    }

    QTableWidget *table = ui->tableWidget;
    int row = table->currentRow();

    if (row == -1)
        return;

    DialogOrderPlanRemark dialog(this);
    dialog.setReason("请说明强制关闭工单的原因：");
    dialog.setNotice("注意：强制关闭此工单后，无法恢复！");

    if (QDialog::Accepted != dialog.exec())
        return;

    changeOrderState(row, OrderState::ForceClose, dialog.getRemark());

    auto item = table->item(row, static_cast<int>(TableHeader::OrderID));

    qWorkManager->suspendOrder(item->text());
}

void WidgetOrderPlanManage::slotCloseOrder()
{
    if (!qApplication->isOnline())
    {
        MessageBox_Warning("目前处于离线生产状态！");
        return;
    }

    QTableWidget *table = ui->tableWidget;
    int row = table->currentRow();

    if (row == -1)
        return;

    int orderPlanQty = ui->valueOrderPlanQty->text().toInt();
    int finishQty = ui->valueFinishQty->text().toInt();

    if (orderPlanQty > finishQty)
    {
        MessageBox_Warning("工单排产数量尚未完成，无法关闭！");
        return;
    }

    if (!MessageBox_Question("是否关闭此工单？"))
        return;

    changeOrderState(row, OrderState::Closed);

    auto item = table->item(row, static_cast<int>(TableHeader::OrderID));

    qWorkManager->suspendOrder(item->text());
}

void WidgetOrderPlanManage::slotAddOrder()
{
    DialogCreateWorkOrder dialog;

    if (QDialog::Accepted != dialog.exec())
        return;
}

//激活工单
void WidgetOrderPlanManage::slotActivateOrder()
{
    QTableWidget *table = ui->tableWidget;
    int row = table->currentRow();

    if (row == -1)
        return;

    if (qWorkManager->isWorkActivated())
    {
        MessageBox_Warning("已有工单被激活，请先停止生产！");
        return;
    }

    if (!MessageBox_Question("是否激活此工单？"))
        return;

    auto item = table->item(row, static_cast<int>(TableHeader::OrderID));

    if (!qWorkManager->activateOrder(item->text()))
    {
        MessageBox_Critical("激活过程出现错误！");
        return;
    }

    changeOrderState(row, OrderState::Activated);
}

//中断工单
void WidgetOrderPlanManage::slotSuspendOrder()
{
    QTableWidget *table = ui->tableWidget;
    int row = table->currentRow();

    if (row == -1)
        return;

    if (!MessageBox_Question("是否暂停此工单？"))
        return;

    changeOrderState(row, OrderState::Suspended);

    auto item = table->item(row, static_cast<int>(TableHeader::OrderID));

    qWorkManager->suspendOrder(item->text());
}

void WidgetOrderPlanManage::changeOrderState(int row, OrderState state, const QString &remark)
{
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;

    item = table->item(row, static_cast<int>(TableHeader::OrderID));
    QString orderID = item->text();

    if (state == OrderState::ForceClose || state == OrderState::Closed)
    {
        //更新工单状态，添加完工时间
        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare(R"(UPDATE [MES_WorkOrder]
                          SET [DeliveryTime] = ?, [State] = ?, [remark] = ?
                          WHERE [WOCode] = ?)");
            query.addBindValue(QDateTime::currentDateTime());
            query.addBindValue(static_cast<int>(state));
            query.addBindValue(remark);
            query.addBindValue(orderID);

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        } while (0);

        //删除总成数据传递表
        do
        {
            QSqlQuery query(LOCAL_DB);
            query.prepare(R"(DELETE FROM [PCS_Data_Transfer]
                      WHERE [OrderID] = ?)");
            query.addBindValue(orderID);

            if (!query.exec())
            {
                qDebug().noquote() << query.lastQuery();
                break;
            }
        } while (0);

        //上传总成关系对应表
        do
        {
            QSqlQuery locQuery(LOCAL_DB);
            QSqlQuery remQuery(REMOTE_DB);

            locQuery.prepare(R"(SELECT A.UID, A.AssemblyID, A.SubAssemblyID
                             FROM PCS_Data_Assembly_Relation A
                             LEFT JOIN PCS_Data_Assembly B
                             ON A.AssemblyID = B.AssemblyID
                             WHERE B.OrderID = ?)");
            locQuery.addBindValue(orderID);

            if (!locQuery.exec())
            {
                qDebug().noquote() << locQuery.lastQuery();
                break;
            }

            while (locQuery.next())
            {
                QDateTime uploadTime = QDateTime::currentDateTime();

                remQuery.prepare(R"(INSERT INTO PCS_Data_Assembly_Relation
                                 (AssemblyID, SubAssemblyID, UploadTime)
                                 VALUES(?, ?, ?))");
                remQuery.addBindValue(locQuery.value("AssemblyID"));
                remQuery.addBindValue(locQuery.value("SubAssemblyID"));
                remQuery.addBindValue(uploadTime);

                if (!remQuery.exec())
                {
                    qDebug().noquote() << remQuery.lastQuery();
                    break;
                }

                QSqlQuery query(LOCAL_DB);
                query.prepare(R"(UPDATE PCS_Data_Assembly_Relation
                              SET UploadTime = ? WHERE UID = ?)");
                query.addBindValue(uploadTime);
                query.addBindValue(locQuery.value("UID"));

                if (!query.exec())
                {
                    qDebug().noquote() << query.lastQuery();
                    break;
                }
            }
        } while (0);

        //上传包装总成对应表
        do
        {
            QSqlQuery locQuery(LOCAL_DB);
            QSqlQuery remQuery(REMOTE_DB);

            locQuery.prepare(R"(SELECT A.UID, A.PackageID, A.AssemblyID
                             FROM PCS_Data_Package_Assembly A
                             LEFT JOIN PCS_Data_Package B
                             ON A.PackageID = B.PackageID
                             WHERE B.OrderID = ?)");
            locQuery.addBindValue(orderID);

            if (!locQuery.exec())
            {
                qDebug().noquote() << locQuery.lastQuery();
                break;
            }

            while (locQuery.next())
            {
                QDateTime uploadTime = QDateTime::currentDateTime();

                remQuery.prepare(R"(INSERT INTO PCS_Data_Package_Assembly
                                 (PackageID, AssemblyID, UploadTime)
                                 VALUES(?, ?, ?))");
                remQuery.addBindValue(locQuery.value("PackageID"));
                remQuery.addBindValue(locQuery.value("AssemblyID"));
                remQuery.addBindValue(uploadTime);

                if (!remQuery.exec())
                {
                    qDebug().noquote() << remQuery.lastQuery();
                    break;
                }

                QSqlQuery query(LOCAL_DB);
                query.prepare(R"(UPDATE PCS_Data_Package_Assembly
                              SET UploadTime = ? WHERE UID = ?)");
                query.addBindValue(uploadTime);
                query.addBindValue(locQuery.value("UID"));

                if (!query.exec())
                {
                    qDebug().noquote() << query.lastQuery();
                    break;
                }
            }
        } while (0);
    }
    else
    {
        //更新工单状态
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(UPDATE [MES_WorkOrder]
                      SET [State] = ?,  [remark] = ?
                      WHERE [WOCode] = ?)");
        query.addBindValue(static_cast<int>(state));
        query.addBindValue(remark);
        query.addBindValue(orderID);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            return;
        }
    }

    item = table->item(row, static_cast<int>(TableHeader::OrderState));
    item->setText(orderStateMap.value(state));

    changeButtonState(state);

    displayOrderInfo(orderID);

    uploadZKserver(orderID);
}

//上传至中控服务器
void WidgetOrderPlanManage::uploadZKserver(const QString &orderID)
{
    if (!qApplication->isOnline())
    {
        qDebug().noquote() << "WorkOrder is in offline mode! uploadZKserver() failed";
        return;
    }
}
