#include "WidgetBaseWorker.h"
#include "WorkManager.h"
#include "XYSqlQueryModel.h"
#include "ui_WidgetBaseWorker.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QtSql>

const QMap<QVariant, QString> Dict_Duty =
    {
        {Operator, "操作工"},
        {Monitor, "班长"},
        {Chargeman, "组长"},
        {Director, "车间主任"},
};

WidgetBaseWorker::WidgetBaseWorker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetBaseWorker)
{
    ui->setupUi(this);
    model = new XYSqlQueryModel();
    initialWidget();
    initialWidgetWorkerList();
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotWorkerSelect(const QModelIndex &)));
    connect(ui->buttonGroup, SIGNAL(signalAdd()), this, SLOT(slotAddWorker()));
    connect(ui->buttonGroup, SIGNAL(signalModify()), this, SLOT(slotModifyWorker()));
    connect(ui->buttonGroup, SIGNAL(signalDelete()), this, SLOT(slotDeleteWorker()));
    connect(ui->buttonGroup, SIGNAL(signalSave()), this, SLOT(slotSaveWorker()));
    connect(ui->buttonGroup, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
    saveStateMark = StateMark::NoMove;
}

WidgetBaseWorker::~WidgetBaseWorker()
{
    delete ui;
}

void WidgetBaseWorker::slotWorkerSelect(const QModelIndex &)
{
    tableIndex = ui->tableView->currentIndex();
    QSqlRecord record = model->record(tableIndex.row());

    ui->editWorkerID->setText(record.value("WorkerID").toString());
    ui->editWorkerName->setText(record.value("WorkerName").toString());
    ui->editWorkerBarcode->setText(record.value("WorkerBarcode").toString());
    ui->editDescription->setText(record.value("Description").toString());
    ui->comboBoxWorkerDuty->setCurrentIndex(record.value("WorkerDuty").toInt());
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
}

void WidgetBaseWorker::slotAddWorker()
{
    ui->editWorkerID->clear();
    ui->editWorkerName->clear();
    ui->editWorkerBarcode->clear();
    ui->editDescription->clear();
    ui->comboBoxWorkerDuty->setCurrentIndex(-1);
    updateWidget();
    ui->editWorkerID->setEnabled(true);
    saveStateMark = StateMark::AddNew;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseWorker::slotDeleteWorker()
{
    if (QMessageBox::Yes !=
        QMessageBox::question(this, tr("信息"), tr("确定删除员工编号：%1用户信息？").arg(ui->editWorkerID->text())))
    {
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Base_Worker] SET [State]=2 WHERE [WorkerID]=? )");
    query.addBindValue(ui->editWorkerID->text());
    query.exec();
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    updateWidgetWorkerList();
    initialWidget();
}

void WidgetBaseWorker::slotModifyWorker()
{
    updateWidget();
    ui->editWorkerID->setEnabled(false);
    saveStateMark = StateMark::ModifyOld;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseWorker::slotSaveWorker()
{
    if (ui->editWorkerID->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入员工编号！"));
        return;
    }
    if (ui->editWorkerName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入员工名称！"));
        return;
    }
    if (ui->editWorkerBarcode->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入员工卡号！"));
        return;
    }
    if (ui->comboBoxWorkerDuty->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请选择员工职务！"));
        return;
    }
    QSqlQuery query(LOCAL_DB);
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        query.prepare(R"(SELECT [WorkerID] FROM [PCS_Base_Worker] WHERE [WorkerID]=? OR [WorkerBarcode]=? )");
        query.addBindValue(ui->editWorkerID->text());
        query.addBindValue(ui->editWorkerBarcode->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("用户已存在！"));
            return;
        }

        query.prepare(R"(INSERT INTO [PCS_Base_Worker]
                      ([WorkShopID]
                      ,[WorkLineID]
                      ,[WorkerID]
                      ,[WorkerName]
                      ,[WorkerBarcode]
                      ,[WorkerDuty]
                      ,[CreateUser]
                      ,[CreateTime]
                      ,[State]
                      ,[Description])
                VALUES
                      (?,?,?,?,?,?,?,?,?,?)       )");
        query.addBindValue(qWorkManager->getWorkshopID());
        query.addBindValue(qWorkManager->getWorklineID());
        query.addBindValue(ui->editWorkerID->text());
        query.addBindValue(ui->editWorkerName->text());
        query.addBindValue(ui->editWorkerBarcode->text());
        query.addBindValue(ui->comboBoxWorkerDuty->currentData());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(0);
        query.addBindValue(ui->editDescription->toPlainText());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        updateWidgetWorkerList();
        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        ui->tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
        // ui->tableView->ro
        break;
    }
    case StateMark::ModifyOld:
    {
        query.prepare(R"(SELECT [WorkerID] FROM [PCS_Base_Worker] WHERE [WorkerID]!=? AND [WorkerBarcode]=? )");
        query.addBindValue(ui->editWorkerID->text());
        query.addBindValue(ui->editWorkerBarcode->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("用户卡号已存在！"));
            return;
        }

        query.prepare(R"(UPDATE [PCS_Base_Worker]
                      SET [WorkerName] = ?
                         ,[WorkerBarcode] = ?
                         ,[WorkerDuty] = ?
                         ,[ModifyUser] = ?
                         ,[ModifyTime] = ?
                         ,[State] = ?
                         ,[Description] = ?
                    WHERE [WorkerID] = ?    )");

        query.addBindValue(ui->editWorkerName->text());
        query.addBindValue(ui->editWorkerBarcode->text());
        query.addBindValue(ui->comboBoxWorkerDuty->currentData());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.addBindValue(ui->editDescription->toPlainText());
        query.addBindValue(ui->editWorkerID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        updateWidgetWorkerList();
        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        ui->tableView->setCurrentIndex(tableIndex);
        break;
    }
    default:
        break;
    }
}

void WidgetBaseWorker::slotCancel()
{
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    }
    break;
    case StateMark::ModifyOld:
    {

        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
    }
    break;
    default:
        break;
    }
}

void WidgetBaseWorker::initialWidget()
{
    ui->comboBoxWorkerDuty->clear();
    for (auto it = Dict_Duty.begin(); it != Dict_Duty.end(); ++it)
    {
        ui->comboBoxWorkerDuty->addItem(it.value(), it.key());
    }

    ui->editWorkerID->clear();
    ui->editWorkerName->clear();
    ui->editWorkerBarcode->clear();
    ui->editDescription->clear();
    ui->comboBoxWorkerDuty->setCurrentIndex(-1);
    ui->groupBoxMessage->setEnabled(false);
}

void WidgetBaseWorker::initialWidgetWorkerList()
{

    headerList << tr("员工编号") << tr("员工姓名") << tr("员工卡号") << tr("员工职务") << tr("备注");
    QTableView *view = ui->tableView;
    QHeaderView *header = view->horizontalHeader();
    header->setFixedHeight(30);
    //header->setStretchLastSection(true);
    header->setHighlightSections(false);
    view->setSelectionMode(QTableView::SingleSelection);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->horizontalHeader()->setMinimumSectionSize(120);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [WorkerID]
                  ,[WorkerName]
                  ,[WorkerBarcode]
                  ,[WorkerDuty]
                  ,[Description]
              FROM [PCS_Base_Worker] WHERE [State]!=2 )");
    query.exec();

    model->setQuery(query);
    for (int col = 0; col < model->columnCount(); col++)
        model->setHeaderData(col, Qt::Horizontal, headerList.at(col));

    ui->tableView->setModel(model);

    model->setColumnEnum("WorkerDuty", Dict_Duty);
}

void WidgetBaseWorker::updateWidgetWorkerList()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [WorkerID]
                  ,[WorkerName]
                  ,[WorkerBarcode]
                  ,[WorkerDuty]
                  ,[Description]
              FROM [PCS_Base_Worker] WHERE [State]!=2 )");
    query.exec();

    model->setQuery(query);
    ui->tableView->setModel(model);

    model->setColumnEnum("WorkerDuty", Dict_Duty);
}

void WidgetBaseWorker::updateWidget()
{

    ui->tableView->setEnabled(false);
    ui->groupBoxMessage->setEnabled(true);
}
