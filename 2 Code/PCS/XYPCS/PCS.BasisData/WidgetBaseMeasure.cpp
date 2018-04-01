#include "WidgetBaseMeasure.h"
#include "WorkManager.h"
#include "XYSqlQueryModel.h"
#include "ui_WidgetBaseMeasure.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

WidgetBaseMeasure::WidgetBaseMeasure(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetBaseMeasure)
{
    ui->setupUi(this);
    model = new XYSqlQueryModel();
    initialWidget();
    initialWidgetMeasureList();
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotMeasureSelect(const QModelIndex &)));
    connect(ui->buttonGroup, SIGNAL(signalAdd()), this, SLOT(slotAddMeasure()));
    connect(ui->buttonGroup, SIGNAL(signalModify()), this, SLOT(slotModifyMeasure()));
    connect(ui->buttonGroup, SIGNAL(signalDelete()), this, SLOT(slotDeleteMeasure()));
    connect(ui->buttonGroup, SIGNAL(signalSave()), this, SLOT(slotSaveMeasure()));
    connect(ui->buttonGroup, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
    saveStateMark = StateMark::NoMove;
}

WidgetBaseMeasure::~WidgetBaseMeasure()
{
    delete ui;
}

void WidgetBaseMeasure::slotMeasureSelect(const QModelIndex &)
{
    tableIndex = ui->tableView->currentIndex();
    QSqlRecord record = model->record(tableIndex.row());

    ui->editMeasureType->setText(record.value("MeasureType").toString());
    ui->editMeasurerName->setText(record.value("MeasureName").toString());
    ui->editUnit->setText(record.value("Unit").toString());
    ui->editDescription->setText(record.value("Description").toString());
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
}

void WidgetBaseMeasure::slotAddMeasure()
{
    ui->editMeasureType->clear();
    ui->editMeasurerName->clear();
    ui->editUnit->clear();
    ui->editDescription->clear();
    updateWidget();
    ui->editMeasureType->setEnabled(true);
    saveStateMark = StateMark::AddNew;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseMeasure::slotDeleteMeasure()
{
    if (QMessageBox::Yes !=
        QMessageBox::question(this, tr("信息"), tr("确定删除测试类型：%1信息？").arg(ui->editMeasureType->text())))
    {
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Base_Measure] SET [State]=2 WHERE [MeasureType]=? )");
    query.addBindValue(ui->editMeasureType->text());
    query.exec();
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    updateWidgetMeasureList();
    initialWidget();
}

void WidgetBaseMeasure::slotModifyMeasure()
{
    updateWidget();
    ui->editMeasureType->setEnabled(false);
    saveStateMark = StateMark::ModifyOld;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseMeasure::slotSaveMeasure()
{
    if (ui->editMeasureType->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入测试类型！"));
        return;
    }
    if (ui->editMeasurerName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入测试名称！"));
        return;
    }
    if (ui->editUnit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入测试单位！"));
        return;
    }
    QSqlQuery query(LOCAL_DB);
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        query.prepare(R"(SELECT [MeasureType] FROM [PCS_Base_Measure] WHERE [MeasureType]=? )");
        query.addBindValue(ui->editMeasureType->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("测试类型已存在！"));
            return;
        }

        query.prepare(R"(INSERT INTO [PCS_Base_Measure]
                      ([MeasureType]
                      ,[MeasureName]
                      ,[Unit]
                      ,[CreateUser]
                      ,[CreateTime]
                      ,[State]
                      ,[Description])
                VALUES
                      (?,?,?,?,?,?,?)       )");
        query.addBindValue(ui->editMeasureType->text());
        query.addBindValue(ui->editMeasurerName->text());
        query.addBindValue(ui->editUnit->text());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(0);
        query.addBindValue(ui->editDescription->toPlainText());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        updateWidgetMeasureList();
        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        ui->tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
        break;
    }
    case StateMark::ModifyOld:
    {

        query.prepare(R"(UPDATE [PCS_Base_Measure]
                      SET [MeasureName] = ?
                         ,[Unit] = ?
                         ,[ModifyUser] = ?
                         ,[ModifyTime] = ?
                         ,[State] = ?
                         ,[Description] = ?
                    WHERE [MeasureType] = ?    )");

        query.addBindValue(ui->editMeasurerName->text());
        query.addBindValue(ui->editUnit->text());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.addBindValue(ui->editDescription->toPlainText());
        query.addBindValue(ui->editMeasureType->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        updateWidgetMeasureList();
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

void WidgetBaseMeasure::slotCancel()
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

void WidgetBaseMeasure::initialWidget()
{

    ui->editMeasureType->clear();
    ui->editMeasurerName->clear();
    ui->editUnit->clear();
    ui->editDescription->clear();
    ui->groupBoxMessage->setEnabled(false);
}

void WidgetBaseMeasure::initialWidgetMeasureList()
{
    headerList << tr("测试类型") << tr("测试名称") << tr("测试单位") << tr("备注");
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
    query.prepare(R"(SELECT [MeasureType]
                  ,[MeasureName]
                  ,[Unit]
                  ,[Description]
              FROM [PCS_Base_Measure] WHERE [State]!=2 )");
    query.exec();

    model->setQuery(query);
    for (int col = 0; col < model->columnCount(); col++)
        model->setHeaderData(col, Qt::Horizontal, headerList.at(col));

    ui->tableView->setModel(model);
}

void WidgetBaseMeasure::updateWidgetMeasureList()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [MeasureType]
                  ,[MeasureName]
                  ,[Unit]
                  ,[Description]
              FROM [PCS_Base_Measure] WHERE [State]!=2 )");
    query.exec();

    model->setQuery(query);
    ui->tableView->setModel(model);
}

void WidgetBaseMeasure::updateWidget()
{
    ui->tableView->setEnabled(false);
    ui->groupBoxMessage->setEnabled(true);
}
