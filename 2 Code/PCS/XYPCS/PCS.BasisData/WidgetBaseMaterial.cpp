#include "WidgetBaseMaterial.h"
#include "WorkManager.h"
#include "XYSqlQueryModel.h"
#include "ui_WidgetBaseMaterial.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

WidgetBaseMaterial::WidgetBaseMaterial(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetBaseMaterial)
{
    ui->setupUi(this);
    model = new XYSqlQueryModel();
    initialWidget();
    initialWidgetMaterialList();
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotMaterialSelect(const QModelIndex &)));
    connect(ui->buttonGroup, SIGNAL(signalAdd()), this, SLOT(slotAddMaterial()));
    connect(ui->buttonGroup, SIGNAL(signalModify()), this, SLOT(slotModifyMaterial()));
    connect(ui->buttonGroup, SIGNAL(signalDelete()), this, SLOT(slotDeleteMaterial()));
    connect(ui->buttonGroup, SIGNAL(signalSave()), this, SLOT(slotSaveMaterial()));
    connect(ui->buttonGroup, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
    saveStateMark = StateMark::NoMove;
}

WidgetBaseMaterial::~WidgetBaseMaterial()
{
    delete ui;
}

void WidgetBaseMaterial::slotMaterialSelect(const QModelIndex &)
{
    tableIndex = ui->tableView->currentIndex();
    QSqlRecord record = model->record(tableIndex.row());

    ui->editMaterialID->setText(record.value("MaterialID").toString());
    ui->editMaterialName->setText(record.value("MaterialName").toString());
    ui->editMaterialBarcode->setText(record.value("BarcodeRule").toString());
    ui->editDescription->setText(record.value("Description").toString());
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
}

void WidgetBaseMaterial::slotAddMaterial()
{
    ui->editMaterialID->clear();
    ui->editMaterialName->clear();
    ui->editMaterialBarcode->clear();
    ui->editDescription->clear();
    updateWidget();
    ui->editMaterialID->setEnabled(true);
    saveStateMark = StateMark::AddNew;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseMaterial::slotDeleteMaterial()
{
    if (QMessageBox::Yes !=
        QMessageBox::question(this, tr("信息"), tr("确定删除物料编号：%1信息？").arg(ui->editMaterialID->text())))
    {
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Base_Material] SET [State]=2 WHERE [MaterialID]=? )");    //////
    query.addBindValue(ui->editMaterialID->text());
    query.exec();
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    updateWidgetMaterialList();
    initialWidget();
}

void WidgetBaseMaterial::slotModifyMaterial()
{
    updateWidget();
    ui->editMaterialID->setEnabled(false);
    saveStateMark = StateMark::ModifyOld;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseMaterial::slotSaveMaterial()
{
    if (ui->editMaterialID->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入物料编号！"));
        return;
    }
    if (ui->editMaterialName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入物料名称！"));
        return;
    }

    QSqlQuery query(LOCAL_DB);
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        query.prepare(R"(SELECT [MaterialID] FROM [PCS_Base_Material] WHERE [MaterialID]=? )");    //////
        query.addBindValue(ui->editMaterialID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("物料编号已存在！"));
            return;
        }

        query.prepare(R"(INSERT INTO [PCS_Base_Material]
                      ([MaterialID]
                      ,[MaterialName]
                      ,[BarcodeRule]
                      ,[CreateUser]
                      ,[CreateTime]
                      ,[State]
                      ,[Description])
                VALUES
                      (?,?,?,?,?,?,?)       )");    //////
        query.addBindValue(ui->editMaterialID->text());
        query.addBindValue(ui->editMaterialName->text());
        query.addBindValue(ui->editMaterialBarcode->text());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(0);
        query.addBindValue(ui->editDescription->toPlainText());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        updateWidgetMaterialList();
        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        ui->tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
        break;
    }
    case StateMark::ModifyOld:
    {

        query.prepare(R"(UPDATE [PCS_Base_Material]
                      SET [MaterialName] = ?
                         ,[BarcodeRule] = ?
                         ,[ModifyUser] = ?
                         ,[ModifyTime] = ?
                         ,[State] = ?
                         ,[Description] = ?
                    WHERE [MaterialID] = ?    )");    //////

        query.addBindValue(ui->editMaterialName->text());
        query.addBindValue(ui->editMaterialBarcode->text());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.addBindValue(ui->editDescription->toPlainText());
        query.addBindValue(ui->editMaterialID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        updateWidgetMaterialList();
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

void WidgetBaseMaterial::slotCancel()
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

void WidgetBaseMaterial::initialWidget()
{
    ui->editMaterialID->clear();
    ui->editMaterialName->clear();
    ui->editMaterialBarcode->clear();
    ui->editDescription->clear();
    ui->groupBoxMessage->setEnabled(false);
}

void WidgetBaseMaterial::initialWidgetMaterialList()
{
    headerList << tr("物料编号") << tr("物料名称") << tr("条码规则") << tr("备注");
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
    query.prepare(R"(SELECT [MaterialID]
                  ,[MaterialName]
                  ,[BarcodeRule]
                  ,[Description]
              FROM [PCS_Base_Material] WHERE [State]!=2 )");    //////
    query.exec();

    model->setQuery(query);
    for (int col = 0; col < model->columnCount(); col++)
        model->setHeaderData(col, Qt::Horizontal, headerList.at(col));

    ui->tableView->setModel(model);
}

void WidgetBaseMaterial::updateWidgetMaterialList()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [MaterialID]
                  ,[MaterialName]
                  ,[BarcodeRule]
                  ,[Description]
              FROM [PCS_Base_Material] WHERE [State]!=2 )");    //////
    query.exec();

    model->setQuery(query);
    ui->tableView->setModel(model);
}

void WidgetBaseMaterial::updateWidget()
{
    ui->tableView->setEnabled(false);
    ui->groupBoxMessage->setEnabled(true);
}
