#include "DialogCreateWorkOrder.h"
#include "WorkManager.h"
#include "ui_DialogCreateWorkOrder.h"
#include <QDebug>
#include <QMessageBox>

DialogCreateWorkOrder::DialogCreateWorkOrder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCreateWorkOrder)
{
    ui->setupUi(this);
    initialDialog();
    connect(ui->comboBoxCraftID, SIGNAL(currentTextChanged(const QString &)), this, SLOT(slotSelectCraft(const QString &)));
    QObject::connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
    QObject::connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

DialogCreateWorkOrder::~DialogCreateWorkOrder()
{
    delete ui;
}

void DialogCreateWorkOrder::slotSelectCraft(const QString &CraftID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [ProductID]
                  ,[ProductName]
                  FROM [PCS_Craft] WHERE [CraftID]=? )");
    query.addBindValue(CraftID);
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->editProductID->setText(query.value("ProductID").toString());
        ui->editProductName->setText(query.value("ProductName").toString());
    }
}

void DialogCreateWorkOrder::slotAccept()
{
    if (ui->comboBoxCraftID->currentText().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择工艺编号");
        return;
    }
    if (ui->editOrderID->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入工单编号");
        return;
    }
    if (ui->editProductionBatch->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入生产批次");
        return;
    }
    if (ui->editPlanProductionQuantity->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入计划生产数量");
        return;
    }
    if (ui->editPlanInspectionQuantity->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入计划抽检数量");
        return;
    }
    if (ui->dateEditPlanDate->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入计划生产日期");
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [UID]
                  FROM [PCS_WorkOrder] WHERE [OrderID]=? )");
    query.addBindValue(ui->editOrderID->text());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    if (query.first())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "工单编号已存在!");
        return;
    }
    query.prepare(R"(INSERT INTO [PCS_WorkOrder]
                  ([WorkShopID]
                  ,[WorkLineID]
                  ,[OrderID]
                  ,[ProductionBatch]
                  ,[CraftID]
                  ,[PlanDate]
                  ,[PlanProductionQuantity]
                  ,[PlanInspectionQuantity]
                  ,[CreateUser]
                  ,[CreateTime]
                  ,[State]
                  ,[Description])
            VALUES
                  (?,?,?,?,?,?,?,?,?,?,?,?))");
    query.addBindValue(qWorkManager->getWorkshopID());
    query.addBindValue(qWorkManager->getWorklineID());
    query.addBindValue(ui->editOrderID->text());
    query.addBindValue(ui->editProductionBatch->text());
    query.addBindValue(ui->comboBoxCraftID->currentText());
    query.addBindValue(ui->dateEditPlanDate->text());
    query.addBindValue(ui->editPlanProductionQuantity->text().toInt());
    query.addBindValue(ui->editPlanInspectionQuantity->text().toInt());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(0);
    query.addBindValue(ui->editDescription->toPlainText());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    accept();
}

void DialogCreateWorkOrder::initialDialog()
{
    ui->comboBoxCraftID->clear();
    ui->editProductID->clear();
    ui->editProductName->clear();
    ui->editOrderID->clear();
    ui->editProductionBatch->clear();
    ui->editPlanInspectionQuantity->clear();
    ui->editPlanProductionQuantity->clear();
    ui->editDescription->clear();
    //ui->dateEditPlanDate->
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT DISTINCT [CraftID]
                  FROM [PCS_Craft] WHERE [State]!=2 )");
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->comboBoxCraftID->addItem(query.value("CraftID").toString());
    }
    ui->comboBoxCraftID->setCurrentIndex(-1);
}
