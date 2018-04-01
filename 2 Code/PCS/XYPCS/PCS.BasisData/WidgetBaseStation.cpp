#include "WidgetBaseStation.h"
#include "WorkManager.h"
#include "XYSqlQueryModel.h"
#include "ui_WidgetBaseStation.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QtSql>
const QMap<QVariant, QString> Dict_Type =
    {
        {Assembly, "装配"},
        {Package, "包装"},
};
WidgetBaseStation::WidgetBaseStation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetBaseStation)
{
    ui->setupUi(this);
    model = new XYSqlQueryModel();
    initialWidget();
    initialWidgetStationList();
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotStationSelect(const QModelIndex &)));
    connect(ui->buttonGroup, SIGNAL(signalAdd()), this, SLOT(slotAddStation()));
    connect(ui->buttonGroup, SIGNAL(signalModify()), this, SLOT(slotModifyStation()));
    connect(ui->buttonGroup, SIGNAL(signalDelete()), this, SLOT(slotDeleteStation()));
    connect(ui->buttonGroup, SIGNAL(signalSave()), this, SLOT(slotSaveStation()));
    connect(ui->buttonGroup, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
    saveStateMark = StateMark::NoMove;
}

WidgetBaseStation::~WidgetBaseStation()
{
    delete ui;
}

void WidgetBaseStation::slotStationSelect(const QModelIndex &)
{
    tableIndex = ui->tableView->currentIndex();
    QSqlRecord record = model->record(tableIndex.row());

    ui->editStationID->setText(record.value("WorkStationID").toString());
    ui->editStationName->setText(record.value("WorkStationName").toString());
    ui->editStationCode->setText(record.value("WorkStationCode").toString());
    ui->editDescription->setText(record.value("Description").toString());
    ui->comboBoxStationType->setCurrentIndex(record.value("IsPackage").toInt());
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
}

void WidgetBaseStation::slotAddStation()
{
    ui->editStationID->clear();
    ui->editStationName->clear();
    ui->editStationCode->clear();
    ui->editDescription->clear();
    ui->comboBoxStationType->setCurrentIndex(-1);
    updateWidget();
    ui->editStationID->setEnabled(true);
    saveStateMark = StateMark::AddNew;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseStation::slotDeleteStation()
{
    if (QMessageBox::Yes !=
        QMessageBox::question(this, tr("信息"), tr("确定删除工位编号：%1信息？").arg(ui->editStationID->text())))
    {
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare("UPDATE PCS_Base_Station SET State=2 WHERE WorkStationID=? ");
    query.addBindValue(ui->editStationID->text());
    query.exec();
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    updateWidgetStationList();
    initialWidget();
}

void WidgetBaseStation::slotModifyStation()
{
    updateWidget();
    ui->editStationID->setEnabled(false);
    saveStateMark = StateMark::ModifyOld;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseStation::slotSaveStation()
{
    if (ui->editStationID->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工位编号！"));
        return;
    }
    if (ui->editStationName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工位名称！"));
        return;
    }
    if (ui->editStationCode->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工位代号！"));
        return;
    }
    if (ui->comboBoxStationType->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请选择工位类型！"));
        return;
    }
    QSqlQuery query(LOCAL_DB);
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        query.prepare(R"(SELECT [WorkStationID] FROM [PCS_Base_Station] WHERE [WorkStationID]=? OR [WorkStationCode]=? )");
        query.addBindValue(ui->editStationID->text());
        query.addBindValue(ui->editStationCode->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("工位已存在！"));
            return;
        }

        query.prepare(R"(INSERT INTO [PCS_Base_Station]
                      ([WorkShopID]
                      ,[WorkLineID]
                      ,[WorkStationID]
                      ,[WorkStationName]
                      ,[WorkStationCode]
                      ,[IsPackage]
                      ,[CreateUser]
                      ,[CreateTime]
                      ,[State]
                      ,[Description])
                VALUES
                      (?,?,?,?,?,?,?,?,?,?)       )");
        query.addBindValue("");
        query.addBindValue("");
        query.addBindValue(ui->editStationID->text());
        query.addBindValue(ui->editStationName->text());
        query.addBindValue(ui->editStationCode->text());
        query.addBindValue(ui->comboBoxStationType->currentData());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(0);
        query.addBindValue(ui->editDescription->toPlainText());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        updateWidgetStationList();
        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        ui->tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
        // ui->tableView->ro
        break;
    }
    case StateMark::ModifyOld:
    {
        query.prepare(R"(SELECT [WorkStationID] FROM [PCS_Base_Station] WHERE [WorkStationID]!=? AND [WorkStationCode]=? )");
        query.addBindValue(ui->editStationID->text());
        query.addBindValue(ui->editStationCode->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("工位代号已存在！"));
            return;
        }

        query.prepare(R"(UPDATE [PCS_Base_Station]
                      SET [WorkStationName] = ?
                         ,[WorkStationCode] = ?
                         ,[IsPackage] = ?
                         ,[ModifyUser] = ?
                         ,[ModifyTime] = ?
                         ,[State] = ?
                         ,[Description] = ?
                    WHERE [WorkStationID] = ?    )");

        query.addBindValue(ui->editStationName->text());
        query.addBindValue(ui->editStationCode->text());
        query.addBindValue(ui->comboBoxStationType->currentData());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.addBindValue(ui->editDescription->toPlainText());
        query.addBindValue(ui->editStationID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        updateWidgetStationList();
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

void WidgetBaseStation::slotCancel()
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

void WidgetBaseStation::initialWidget()
{
    ui->comboBoxStationType->clear();
    for (auto it = Dict_Type.begin(); it != Dict_Type.end(); ++it)
    {
        ui->comboBoxStationType->addItem(it.value(), it.key());
    }

    ui->editStationID->clear();
    ui->editStationName->clear();
    ui->editStationCode->clear();
    ui->editDescription->clear();
    ui->comboBoxStationType->setCurrentIndex(-1);
    ui->groupBoxMessage->setEnabled(false);
}

void WidgetBaseStation::initialWidgetStationList()
{
    headerList << tr("工位编号") << tr("工位名称") << tr("工位代号") << tr("工位类型") << tr("备注");
    QTableView *view = ui->tableView;
    QHeaderView *header = view->horizontalHeader();
    header->setFixedHeight(30);
    header->setHighlightSections(false);
    view->setSelectionMode(QTableView::SingleSelection);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->horizontalHeader()->setMinimumSectionSize(120);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [WorkStationID]
                  ,[WorkStationName]
                  ,[WorkStationCode]
                  ,[IsPackage]
                  ,[Description]
              FROM [PCS_Base_Station] WHERE [State]!=2 )");
    query.exec();

    model->setQuery(query);
    for (int col = 0; col < model->columnCount(); col++)
        model->setHeaderData(col, Qt::Horizontal, headerList.at(col));

    ui->tableView->setModel(model);

    model->setColumnEnum("IsPackage", Dict_Type);
}

void WidgetBaseStation::updateWidgetStationList()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [WorkStationID]
                  ,[WorkStationName]
                  ,[WorkStationCode]
                  ,[IsPackage]
                  ,[Description]
              FROM [PCS_Base_Station] WHERE [State]!=2 )");
    query.exec();
    model->setQuery(query);
    ui->tableView->setModel(model);
    model->setColumnEnum("IsPackage", Dict_Type);
}

void WidgetBaseStation::updateWidget()
{
    ui->tableView->setEnabled(false);
    ui->groupBoxMessage->setEnabled(true);
}
