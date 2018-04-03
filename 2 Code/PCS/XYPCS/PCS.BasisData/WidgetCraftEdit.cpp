#include "WidgetCraftEdit.h"
#include "DialogCraftFixture.h"
#include "DialogCraftMaterial.h"
#include "DialogCraftMeasure.h"
#include "DialogCraftStation.h"
#include "DialogSelectStation.h"
#include "WorkManager.h"
#include "XYSqlQueryModel.h"
#include "ui_WidgetCraftEdit.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

WidgetCraftEdit::WidgetCraftEdit(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetCraftEdit)
    , model(new XYSqlQueryModel)
    , modelMeasure(new XYSqlQueryModel)
    , modelMaterial(new XYSqlQueryModel)
    , modelFixture(new XYSqlQueryModel)
{
    ui->setupUi(this);
    initialTreeWidget();
    initialFrameCraft();
    lockFrameCraft();
    initialFrameStation();
    lockFrameStation();

    ui->buttonGroup->setEnabled(true);
    connect(ui->buttonGroup, SIGNAL(signalAdd()), this, SLOT(slotAddCraft()));
    connect(ui->buttonGroup, SIGNAL(signalModify()), this, SLOT(slotModifyCraft()));
    connect(ui->buttonGroup, SIGNAL(signalDelete()), this, SLOT(slotDeleteCraft()));
    connect(ui->buttonGroup, SIGNAL(signalSave()), this, SLOT(slotSaveCraft()));
    connect(ui->buttonGroup, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
    saveStateMark = StateMark::NoMove;
    connect(ui->buttonStationAdd, SIGNAL(clicked()), this, SLOT(showCraftStationDialog()));
    connect(ui->buttonMaterialAdd, SIGNAL(clicked()), this, SLOT(showCraftMaterialDialog()));
    connect(ui->buttonMeasureAdd, SIGNAL(clicked()), this, SLOT(showCraftMeasureDialog()));
    connect(ui->buttonFixtureAdd, SIGNAL(clicked()), this, SLOT(showCraftFixtureDialog()));
    connect(ui->treeWidgetCraft, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotTreeSelect(QTreeWidgetItem *, int)));
    connect(ui->tableViewMaterial, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotMaterialSelect(const QModelIndex &)));
    connect(ui->tableViewMeasure, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotMeasureSelect(const QModelIndex &)));
    connect(ui->tableViewFixture, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotFixtureSelect(const QModelIndex &)));
    connect(ui->buttonStationDelete, SIGNAL(clicked()), this, SLOT(slotDeleteStation()));
    connect(ui->buttonMaterialModify, SIGNAL(clicked()), this, SLOT(slotModifyCraftMaterial()));
    connect(ui->buttonMaterialDelete, SIGNAL(clicked()), this, SLOT(slotDeleteCraftMaterial()));
    connect(ui->buttonMeasureModify, SIGNAL(clicked()), this, SLOT(slotModifyCraftMeasure()));
    connect(ui->buttonMeasureDelete, SIGNAL(clicked()), this, SLOT(slotDeleteCraftMeasure()));
    connect(ui->buttonFixtureModify, SIGNAL(clicked()), this, SLOT(slotModifyCraftFixture()));
    connect(ui->buttonFixtureDelete, SIGNAL(clicked()), this, SLOT(slotDeleteCraftFixture()));
}

WidgetCraftEdit::~WidgetCraftEdit()
{
    delete ui;
}

void WidgetCraftEdit::slotTreeSelect(QTreeWidgetItem *item, int)
{
    qDebug() << "slotTreeSelect" << item->data(0, Qt::UserRole).toInt() << item->text(0) << CraftTree::Craft;

    switch (item->data(0, Qt::UserRole).toInt())
    {
    case CraftTree::Craft:
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [CraftID]
                      ,[ProductID]
                      ,[ProductName]
                      ,[AssySerialRule]
                      ,[AssySerialInit]
                      ,[PackSerialRule]
                      ,[PackSerialInit]
                      ,[Description]
                  FROM [PCS_Craft] WHERE [CraftID]=? )");    //////
        query.addBindValue(item->text(0));
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        while (query.next())
        {
            ui->editCraftID->setText(query.value("CraftID").toString());
            ui->editProductID->setText(query.value("ProductID").toString());
            ui->editProductName->setText(query.value("ProductName").toString());
            ui->textEditDescription->setPlainText(query.value("Description").toString());
            ui->editAssySerialInit->setText(query.value("AssySerialInit").toString());
            ui->editPackSerialInit->setText(query.value("PackSerialInit").toString());
            ui->comboBoxAssySerialRule->setCurrentIndex(query.value("AssySerialRule").toInt());
            ui->comboBoxPackSerialRule->setCurrentIndex(query.value("PackSerialRule").toInt());
        }
        ui->buttonGroup->setEnabled(true);
        initialFrameStation();
        lockFrameStation();
        ui->buttonStationAdd->setEnabled(true);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
    }
    break;
    case CraftTree::Station:
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [CraftID]
                      ,[ProductID]
                      ,[ProductName]
                      ,[AssySerialRule]
                      ,[AssySerialInit]
                      ,[PackSerialRule]
                      ,[PackSerialInit]
                      ,[Description]
                  FROM [PCS_Craft] WHERE [CraftID]=? )");    //////
        query.addBindValue(item->parent()->text(0));
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        while (query.next())
        {
            ui->editCraftID->setText(query.value("CraftID").toString());
            ui->editProductID->setText(query.value("ProductID").toString());
            ui->editProductName->setText(query.value("ProductName").toString());
            ui->textEditDescription->setPlainText(query.value("Description").toString());
            ui->editAssySerialInit->setText(query.value("AssySerialInit").toString());
            ui->editPackSerialInit->setText(query.value("PackSerialInit").toString());
            ui->comboBoxAssySerialRule->setCurrentIndex(query.value("AssySerialRule").toInt());
            ui->comboBoxPackSerialRule->setCurrentIndex(query.value("PackSerialRule").toInt());
        }

        query.prepare(R"(SELECT [WorkStationID]
                      ,[WorkStationName]
                      ,[WorkStationCode]
                  FROM [PCS_Base_Station] WHERE [WorkStationID]=? )");    //////
        query.addBindValue(item->text(0));
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        while (query.next())
        {
            ui->editStationID->setText(query.value("WorkStationID").toString());
            ui->editStationName->setText(query.value("WorkStationName").toString());
            ui->editStationCode->setText(query.value("WorkStationCode").toString());
        }
        ui->buttonGroup->setEnabled(false);
        ui->groupBoxStation->setEnabled(true);
        ui->buttonStationAdd->setEnabled(false);
        ui->buttonStationDelete->setEnabled(true);
        initialTableViewMaterial();
        initialTableViewMeasure();
        initialTableViewFixture();
    }
    break;
    default:
        break;
    }
}

void WidgetCraftEdit::slotMaterialSelect(const QModelIndex &)
{
    tableIndexMaterial = ui->tableViewMaterial->currentIndex();
    ui->buttonMaterialDelete->setEnabled(true);
    ui->buttonMaterialModify->setEnabled(true);
}

void WidgetCraftEdit::slotMeasureSelect(const QModelIndex &)
{
    tableIndexMeasure = ui->tableViewMeasure->currentIndex();
    QSqlRecord record = modelMeasure->record(tableIndexMeasure.row());
    ui->buttonMeasureDelete->setEnabled(true);
    ui->buttonMeasureModify->setEnabled(true);
}

void WidgetCraftEdit::slotFixtureSelect(const QModelIndex &)
{
    tableIndexFixture = ui->tableViewFixture->currentIndex();
    QSqlRecord record = modelFixture->record(tableIndexFixture.row());
    ui->buttonFixtureDelete->setEnabled(true);
    ui->buttonFixtureModify->setEnabled(true);
}

void WidgetCraftEdit::slotAddCraft()
{
    qInfo() << "slotAddCraft";

    initialFrameCraft();
    openFrameCraft();
    ui->editCraftID->setEnabled(true);
    ui->treeWidgetCraft->setEnabled(false);
    ui->buttonStationAdd->setEnabled(false);
    saveStateMark = StateMark::AddNew;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetCraftEdit::slotDeleteCraft()
{
    if (QMessageBox::Yes !=
        QMessageBox::question(this, tr("信息"), tr("确定删除工艺编号：%1信息？").arg(ui->editCraftID->text())))
    {
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Craft] SET [State]=2 WHERE [CraftID]=? )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.exec();
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    initialTreeWidget();
    initialFrameCraft();
}

void WidgetCraftEdit::slotModifyCraft()
{
    openFrameCraft();
    ui->treeWidgetCraft->setEnabled(false);
    ui->editCraftID->setEnabled(false);
    ui->buttonStationAdd->setEnabled(false);
    saveStateMark = StateMark::ModifyOld;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetCraftEdit::slotSaveCraft()
{
    if (ui->editCraftID->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工艺编号！"));
        return;
    }
    if (ui->editProductID->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入产品编号！"));
        return;
    }
    if (ui->editProductName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入产品名称！"));
        return;
    }
    if (ui->comboBoxAssySerialRule->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入包装流水规则！"));
        return;
    }
    if (ui->comboBoxPackSerialRule->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入总成流水规则！"));
        return;
    }
    if (ui->editAssySerialInit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入总成流水起始值！"));
        return;
    }
    if (ui->editPackSerialInit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入包装流水起始值！"));
        return;
    }
    QSqlQuery query(LOCAL_DB);
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        query.prepare(R"(SELECT [CraftID] FROM [PCS_Craft] WHERE [CraftID]=? )");    //////
        query.addBindValue(ui->editCraftID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        if (query.first())
        {
            QMessageBox::warning(this, tr("警告"), tr("工艺编号已存在！"));
            return;
        }

        query.prepare(R"(INSERT INTO [PCS_Craft]
                      ([WorkShopID]
                      ,[WorkLineID]
                      ,[CraftID]
                      ,[ProductID]
                      ,[ProductName]
                      ,[AssySerialRule]
                      ,[AssySerialInit]
                      ,[PackSerialRule]
                      ,[PackSerialInit]
                      ,[CreateUser]
                      ,[CreateTime]
                      ,[State]
                      ,[Description])
                VALUES
                      (?,?,?,?,?,?,?,?,?,?,?,?,?)       )");    //////
        query.addBindValue(qWorkManager->getWorkshopID());
        query.addBindValue(qWorkManager->getWorklineID());
        query.addBindValue(ui->editCraftID->text());
        query.addBindValue(ui->editProductID->text());
        query.addBindValue(ui->editProductName->text());
        query.addBindValue(static_cast<int>(SerialRuleMap.key(ui->comboBoxAssySerialRule->currentText())));
        query.addBindValue(ui->editAssySerialInit->text().toInt());
        query.addBindValue(static_cast<int>(SerialRuleMap.key(ui->comboBoxPackSerialRule->currentText())));
        query.addBindValue(ui->editPackSerialInit->text().toInt());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(0);
        query.addBindValue(ui->textEditDescription->toPlainText());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        initialTreeWidget();
        ui->treeWidgetCraft->setEnabled(true);
        lockFrameCraft();
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        break;
    }
    case StateMark::ModifyOld:
    {
        query.prepare(R"(UPDATE [PCS_Craft]
                      SET [ProductID] = ?
                         ,[ProductName] = ?
                         ,[AssySerialRule]=?
                         ,[AssySerialInit]=?
                         ,[PackSerialRule]=?
                         ,[PackSerialInit]=?
                         ,[ModifyUser] = ?
                         ,[ModifyTime] = ?
                         ,[State] = ?
                         ,[Description] = ?
                    WHERE [CraftID] = ?    )");    //////
        query.addBindValue(ui->editProductID->text());
        query.addBindValue(ui->editProductName->text());
        query.addBindValue(static_cast<int>(SerialRuleMap.key(ui->comboBoxAssySerialRule->currentText())));
        query.addBindValue(ui->editAssySerialInit->text().toInt());
        query.addBindValue(static_cast<int>(SerialRuleMap.key(ui->comboBoxPackSerialRule->currentText())));
        query.addBindValue(ui->editPackSerialInit->text().toInt());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.addBindValue(ui->textEditDescription->toPlainText());
        query.addBindValue(ui->editCraftID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        initialTreeWidget();
        lockFrameCraft();
        ui->treeWidgetCraft->setEnabled(true);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        break;
    }
    default:
        break;
    }

    ui->buttonStationAdd->setEnabled(true);
}

void WidgetCraftEdit::slotCancel()
{
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {
        ui->treeWidgetCraft->setEnabled(true);
        lockFrameCraft();
        initialFrameCraft();
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    }
    break;
    case StateMark::ModifyOld:
    {
        ui->treeWidgetCraft->setEnabled(true);
        lockFrameCraft();
        ui->buttonStationAdd->setEnabled(true);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
    }
    break;
    default:
        break;
    }
}

void WidgetCraftEdit::slotDeleteStation()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(DELETE FROM [PCS_Craft_Station]
                  WHERE [CraftID]=? AND [WorkStationID]=? )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    query.prepare(R"(DELETE FROM [PCS_Craft_Station_Fixture]
                  WHERE [CraftID]=? AND [WorkStationID]=? )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    query.prepare(R"(DELETE FROM [PCS_Craft_Station_Material]
                  WHERE [CraftID]=? AND [WorkStationID]=? )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }

    query.prepare(R"(DELETE FROM [PCS_Craft_Station_Measure]
                  WHERE [CraftID]=? AND [WorkStationID]=? )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    initialFrameStation();
    lockFrameStation();
    initialTreeWidget();
    ui->buttonStationAdd->setEnabled(true);
    ui->buttonGroup->setEnabled(true);
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
}
void WidgetCraftEdit::showCraftStationDialog()
{
    DialogSelectStation dialog;
    dialog.setCraftID(ui->editCraftID->text());
    if (QDialog::Accepted != dialog.exec())
        return;
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(INSERT INTO [PCS_Craft_Station] ([CraftID],[WorkStationID]) VALUES (? ,?) )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(dialog.getStationID());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    initialTreeWidget();
    qDebug() << "get station id : " << dialog.getStationID();
    query.prepare(R"(SELECT [WorkStationID]
                  ,[WorkStationName]
                  ,[WorkStationCode]
              FROM [PCS_Base_Station] WHERE [WorkStationID]=? )");    //////
    query.addBindValue(dialog.getStationID());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->editStationID->setText(query.value("WorkStationID").toString());
        ui->editStationName->setText(query.value("WorkStationName").toString());
        ui->editStationCode->setText(query.value("WorkStationCode").toString());
    }
    ui->buttonGroup->setEnabled(false);
    ui->groupBoxStation->setEnabled(true);
    ui->buttonStationAdd->setEnabled(false);
    ui->buttonStationDelete->setEnabled(true);
    // 保存
}

void WidgetCraftEdit::showCraftMeasureDialog()
{
    QSqlRecord record = modelMeasure->record();
    record.setValue("WorkStationID", ui->editStationID->text());
    record.setValue("CraftID", ui->editCraftID->text());
    DialogCraftMeasure dialog(record);
    if (QDialog::Accepted != dialog.exec())
        return;
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [UID] FROM [PCS_Craft_Station_Measure] WHERE [CraftID] = :CraftID AND [WorkStationID] = :WorkStationID AND [MeasureType]=:MeasureType AND [MeasureOrder]=:MeasureOrder )");    //////
    query.bindValue(":WorkStationID", ui->editStationID->text());
    query.bindValue(":CraftID", ui->editCraftID->text());
    query.bindValue(":MeasureType", dialog.getMeasureType());
    query.bindValue(":MeasureOrder", dialog.getMeasureSerial());
    if (!query.exec())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "数据库错误,请重试");
        return;
    }
    if (query.first())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "已存在测试绑定");
        return;
    }
    query.prepare(R"(INSERT INTO [PCS_Craft_Station_Measure]
                    ([CraftID]
                    ,[WorkStationID]
                    ,[MeasureType]
                    ,[MeasureOrder]
                    ,[StandardValue]
                    ,[UpperLimit]
                    ,[LowerLimit]
                    ,[CreateUser]
                    ,[CreateTime]
                    ,[State])
              VALUES
                    (?,?,?,?,?,?,?,?,?,? ))");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.addBindValue(dialog.getMeasureType());
    query.addBindValue(dialog.getMeasureSerial());
    query.addBindValue(dialog.getStandardValue());
    query.addBindValue(dialog.getUpperLimit());
    query.addBindValue(dialog.getLowerLimit());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(0);
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewMeasure();
}

void WidgetCraftEdit::slotModifyCraftMeasure()
{
    QModelIndex index = ui->tableViewMeasure->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    QSqlRecord record = modelMeasure->record(index.row());

    DialogCraftMeasure dialog(record);

    if (QDialog::Accepted != dialog.exec())
        return;

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Craft_Station_Measure]
                  SET [MeasureOrder] = ?
                     ,[StandardValue] = ?
                     ,[UpperLimit] = ?
                     ,[LowerLimit] = ?
                     ,[ModifyUser] = ?
                     ,[ModifyTime] = ?
                     ,[State] = ?
                WHERE  [CraftID] = ? AND [WorkStationID] = ? AND [MeasureType] = ? AND [UID] =?)");    //////
    query.addBindValue(dialog.getMeasureSerial().toInt());
    query.addBindValue(dialog.getStandardValue().toDouble());
    query.addBindValue(dialog.getUpperLimit().toDouble());
    query.addBindValue(dialog.getLowerLimit().toDouble());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(1);
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.addBindValue(dialog.getMeasureType());
    query.addBindValue(record.value("UID").toInt());

    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewMeasure();
}

void WidgetCraftEdit::slotDeleteCraftMeasure()
{
    QModelIndex index = ui->tableViewMeasure->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    QSqlRecord record = modelMeasure->record(index.row());
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(DELETE FROM [PCS_Craft_Station_Measure]
                  WHERE[CraftID] = ? AND [WorkStationID] = ? AND [MeasureType] = ?  AND [MeasureOrder] = ? AND [UID] =?)");    //////
    query.addBindValue(record.value("CraftID").toString());
    query.addBindValue(record.value("WorkStationID").toString());
    query.addBindValue(record.value("MeasureType").toString());
    query.addBindValue(record.value("MeasureOrder").toString());
    query.addBindValue(record.value("UID").toInt());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewMeasure();
}

void WidgetCraftEdit::showCraftMaterialDialog()
{
    QSqlRecord record = modelMaterial->record();
    record.setValue("WorkStationID", ui->editStationID->text());
    record.setValue("CraftID", ui->editCraftID->text());
    DialogCraftMaterial dialog(record);

    if (QDialog::Accepted != dialog.exec())
        return;
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [UID] FROM [PCS_Craft_Station_Material] WHERE [CraftID] = :CraftID AND [WorkStationID] = :WorkStationID AND [MaterialID]=:MaterialID )");    //////
    query.bindValue(":WorkStationID", ui->editStationID->text());
    query.bindValue(":CraftID", ui->editCraftID->text());
    query.bindValue(":MaterialID", dialog.getMaterialID());
    if (!query.exec())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "数据库错误,请重试");
        return;
    }
    if (query.first())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "已存在物料绑定");
        return;
    }
    query.prepare(R"(INSERT INTO [PCS_Craft_Station_Material]
                    ([CraftID]
                    ,[WorkStationID]
                    ,[MaterialID]
                    ,[SingleConsumeQuantity]
                    ,[ShortWarningQuantity]
                    ,[CreateUser]
                    ,[CreateTime]
                    ,[State])
              VALUES
                    (?,?,?,?,?,?,?,? ))");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.addBindValue(dialog.getMaterialID());
    query.addBindValue(dialog.getConsumeQuantity());
    query.addBindValue(dialog.getWarningQuantity());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(0);
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewMaterial();
}

void WidgetCraftEdit::slotModifyCraftMaterial()
{
    QModelIndex index = ui->tableViewMaterial->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    QSqlRecord record = modelMaterial->record(index.row());

    DialogCraftMaterial dialog(record);

    if (QDialog::Accepted != dialog.exec())
        return;

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Craft_Station_Material]
                  SET [SingleConsumeQuantity] = ?
                     ,[ShortWarningQuantity] = ?
                     ,[ModifyUser] = ?
                     ,[ModifyTime] = ?
                     ,[State] = ?
                WHERE  [CraftID] = ? AND [WorkStationID] = ? AND [MaterialID] = ? AND [UID] =?)");    //////
    query.addBindValue(dialog.getConsumeQuantity().toDouble());
    query.addBindValue(dialog.getWarningQuantity().toDouble());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(1);
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.addBindValue(dialog.getMaterialID());
    query.addBindValue(record.value("UID").toInt());

    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewMaterial();
}

void WidgetCraftEdit::slotDeleteCraftMaterial()
{
    QModelIndex index = ui->tableViewMaterial->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    QSqlRecord record = modelMaterial->record(index.row());
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(DELETE FROM [PCS_Craft_Station_Material]
                WHERE  [CraftID] = ? AND [WorkStationID] = ? AND [MaterialID] = ? AND [UID] =?)");    //////
    query.addBindValue(record.value("CraftID").toString());
    query.addBindValue(record.value("WorkStationID").toString());
    query.addBindValue(record.value("MaterialID").toString());
    query.addBindValue(record.value("UID").toInt());

    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewMaterial();
}

void WidgetCraftEdit::showCraftFixtureDialog()
{
    QSqlRecord record = modelFixture->record();
    record.setValue("WorkStationID", ui->editStationID->text());
    record.setValue("CraftID", ui->editCraftID->text());
    DialogCraftFixture dialog(record);

    if (QDialog::Accepted != dialog.exec())
        return;
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [UID] FROM [PCS_Craft_Station_Fixture] WHERE [CraftID] = :CraftID AND [WorkStationID] = :WorkStationID AND [FixtureType]=:FixtureType AND [FixtureOrder]=:FixtureOrder )");    //////
    query.bindValue(":WorkStationID", ui->editStationID->text());
    query.bindValue(":CraftID", ui->editCraftID->text());
    query.bindValue(":FixtureType", dialog.getFixtureType());
    query.bindValue(":FixtureOrder", dialog.getFixtureSerial());
    if (!query.exec())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "数据库错误,请重试");
        return;
    }
    if (query.first())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "已存在工装绑定");
        return;
    }
    query.prepare(R"(INSERT INTO [PCS_Craft_Station_Fixture]
                    ([CraftID]
                    ,[WorkStationID]
                    ,[FixtureType]
                    ,[FixtureOrder]
                    ,[CreateUser]
                    ,[CreateTime]
                    ,[State])
              VALUES
                    (?,?,?,?,?,?,? ))");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.addBindValue(dialog.getFixtureType());
    query.addBindValue(dialog.getFixtureSerial());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(0);
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewFixture();
}

void WidgetCraftEdit::slotModifyCraftFixture()
{
    QModelIndex index = ui->tableViewFixture->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    QSqlRecord record = modelFixture->record(index.row());

    DialogCraftFixture dialog(record);

    if (QDialog::Accepted != dialog.exec())
        return;

    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(UPDATE [PCS_Craft_Station_Fixture]
                  SET [FixtureOrder] = ?
                     ,[ModifyUser] = ?
                     ,[ModifyTime] = ?
                     ,[State] = ?
                WHERE  [CraftID] = ? AND [WorkStationID] = ? AND [FixtureType] = ? AND [UID] =?)");    //////
    query.addBindValue(dialog.getFixtureSerial().toInt());
    query.addBindValue("");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(1);
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.addBindValue(dialog.getFixtureType());
    query.addBindValue(record.value("UID").toInt());

    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewFixture();
}

void WidgetCraftEdit::slotDeleteCraftFixture()
{
    QModelIndex index = ui->tableViewFixture->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    QSqlRecord record = modelFixture->record(index.row());
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(DELETE FROM [PCS_Craft_Station_Fixture]
                  WHERE[CraftID] = ? AND [WorkStationID] = ? AND [FixtureType] = ?  AND [FixtureOrder] = ? AND [UID] =?)");    //////
    query.addBindValue(record.value("CraftID").toString());
    query.addBindValue(record.value("WorkStationID").toString());
    query.addBindValue(record.value("FixtureType").toString());
    query.addBindValue(record.value("FixtureOrder").toString());
    query.addBindValue(record.value("UID").toInt());
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    updateTableViewFixture();
}

void WidgetCraftEdit::initialTreeWidget()
{
    ui->treeWidgetCraft->setStyleSheet("QTreeWidget::item{height:24px}");
    ui->treeWidgetCraft->setEditTriggers(QTreeWidget::NoEditTriggers);
    QTreeWidget *tree = ui->treeWidgetCraft;
    tree->clear();
    QTreeWidgetItem *root = 0, *child = 0;
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT
                    [CraftID]
                FROM [PCS_Craft] WHERE [State] !=2 )");    //////
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
    }
    while (query.next())
    {
        root = new QTreeWidgetItem(tree);
        root->setText(0, query.value(0).toString());
        root->setData(0, Qt::UserRole, CraftTree::Craft);
        tree->addTopLevelItem(root);

        QSqlQuery queryChild(LOCAL_DB);
        queryChild.prepare(R"(SELECT
                        [WorkStationID]
                    FROM [PCS_Craft_Station] WHERE [CraftID] =? )");    //////
        queryChild.addBindValue(root->text(0));
        if (!queryChild.exec())
        {
            qDebug().noquote() << queryChild.lastError().text();
        }
        while (queryChild.next())
        {
            child = new QTreeWidgetItem(root);
            child->setText(0, queryChild.value(0).toString());
            child->setData(0, Qt::UserRole, CraftTree::Station);
            root->addChild(child);
        }
    }
}

void WidgetCraftEdit::initialFrameCraft()
{
    ui->editCraftID->clear();
    ui->editProductID->clear();
    ui->editProductName->clear();
    ui->textEditDescription->clear();
    ui->editAssySerialInit->clear();
    ui->editPackSerialInit->clear();
    ui->comboBoxAssySerialRule->clear();
    ui->comboBoxAssySerialRule->addItems(SerialRuleMap.values());
    ui->comboBoxAssySerialRule->setCurrentIndex(-1);
    ui->comboBoxPackSerialRule->clear();
    ui->comboBoxPackSerialRule->addItems(SerialRuleMap.values());
    ui->comboBoxPackSerialRule->setCurrentIndex(-1);
}

void WidgetCraftEdit::lockFrameCraft()
{

    ui->frame->setEnabled(false);
}

void WidgetCraftEdit::openFrameCraft()
{

    ui->frame->setEnabled(true);
}

void WidgetCraftEdit::initialFrameStation()
{
    ui->editStationID->clear();
    ui->editStationName->clear();
    ui->editStationCode->clear();
    modelFixture->clear();
    modelMaterial->clear();
    modelMeasure->clear();

    // TODO TableView清理
}

void WidgetCraftEdit::lockFrameStation()
{

    ui->groupBoxStation->setEnabled(false);
}

void WidgetCraftEdit::openFrameStation()
{
    ui->editStationID->setEnabled(true);
    ui->editStationName->setEnabled(true);
    ui->editStationCode->setEnabled(true);
}

void WidgetCraftEdit::initialTableViewMaterial()
{
    headerListMaterial << tr("序号") << tr("工艺编号") << tr("工位编号") << tr("物料编号") << tr("物料名称") << tr("条码规则") << tr("单件消耗数量") << tr("缺料预警数量");
    QTableView *view = ui->tableViewMaterial;
    QHeaderView *header = view->horizontalHeader();
    header->setFixedHeight(30);
    header->setHighlightSections(false);
    view->setSelectionMode(QTableView::SingleSelection);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->horizontalHeader()->setMinimumSectionSize(120);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /*QSqlQuery dbQuery;
    dbQuery.prepare(R"(SELECT [UID], [CraftID], [WorkStationID],[MaterialID]
                 ,[SingleConsumeQuantity]
                 ,[ShortWarningQuantity]
             FROM [PCS_Craft_Station_Material] WHERE [CraftID]= ? AND [WorkStationID]=? AND [State]!=2 )");
    dbQuery.addBindValue(ui->editCraftID->text());
    dbQuery.addBindValue(ui->editStationID->text());
    dbQuery.exec();

    modelMaterial->setQuery(dbQuery);*/
    updateTableViewMaterial();

    for (int col = 0; col < modelMaterial->columnCount(); col++)
        modelMaterial->setHeaderData(col, Qt::Horizontal, headerListMaterial.at(col));

    ui->tableViewMaterial->setModel(modelMaterial);
    ui->buttonMaterialDelete->setEnabled(false);
    ui->buttonMaterialModify->setEnabled(false);
    ui->tableViewMaterial->setColumnHidden(0, true);
    ui->tableViewMaterial->setColumnHidden(1, true);
    ui->tableViewMaterial->setColumnHidden(2, true);
}

void WidgetCraftEdit::updateTableViewMaterial()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT A.[UID], A.[CraftID], A.[WorkStationID],A.[MaterialID]
                 ,B.[MaterialName]
                 ,B.[BarcodeRule]
                 ,A.[SingleConsumeQuantity]
                 ,A.[ShortWarningQuantity]
             FROM [PCS_Craft_Station_Material] A LEFT JOIN  [PCS_Base_Material] B ON A.[MaterialID]= B.[MaterialID] WHERE A.[CraftID]= ? AND A.[WorkStationID]=? AND A.[State]!=2)");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.exec();

    modelMaterial->setQuery(query);
}

void WidgetCraftEdit::initialTableViewMeasure()
{
    headerListMeasure << tr("序号") << tr("工艺编号") << tr("工位编号") << tr("测试类型") << tr("测试名称") << tr("测试单位") << tr("测试序号") << tr("标准值") << tr("上限值") << tr("下限值");
    QTableView *view = ui->tableViewMeasure;
    QHeaderView *header = view->horizontalHeader();
    header->setFixedHeight(30);
    header->setHighlightSections(false);
    view->setSelectionMode(QTableView::SingleSelection);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->horizontalHeader()->setMinimumSectionSize(120);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /*QSqlQuery dbQuery;
    dbQuery.prepare(R"(SELECT [UID], [CraftID], [WorkStationID], [MeasureType]
                    ,[MeasureOrder]
                    ,[StandardValue]
                    ,[UpperLimit]
                    ,[LowerLimit]
                FROM [PCS_Craft_Station_Measure] WHERE [CraftID]= ? AND [WorkStationID]=? AND [State]!=2 )");
    dbQuery.addBindValue(ui->editCraftID->text());
    dbQuery.addBindValue(ui->editStationID->text());
    dbQuery.exec();
    modelMeasure->setQuery(dbQuery);*/
    updateTableViewMeasure();

    for (int col = 0; col < modelMeasure->columnCount(); col++)
        modelMeasure->setHeaderData(col, Qt::Horizontal, headerListMeasure.at(col));

    ui->tableViewMeasure->setModel(modelMeasure);
    ui->buttonMeasureDelete->setEnabled(false);
    ui->buttonMeasureModify->setEnabled(false);
    ui->tableViewMeasure->setColumnHidden(0, true);
    ui->tableViewMeasure->setColumnHidden(1, true);
    ui->tableViewMeasure->setColumnHidden(2, true);
}

void WidgetCraftEdit::updateTableViewMeasure()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT A.[UID], A.[CraftID], A.[WorkStationID], A.[MeasureType]
                   ,B.[MeasureName]
                   ,B.[Unit]
                   ,A.[MeasureOrder]
                   ,A.[StandardValue]
                   ,A.[UpperLimit]
                   ,A.[LowerLimit]
               FROM  [PCS_Craft_Station_Measure] A LEFT JOIN  [PCS_Base_Measure] B ON A.[MeasureType]= B.[MeasureType] WHERE A.[CraftID]=? AND A.[WorkStationID]=? AND A.[State]!=2 )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.exec();
    modelMeasure->setQuery(query);
}

void WidgetCraftEdit::initialTableViewFixture()
{
    headerListFixture << tr("序号") << tr("工艺编号") << tr("工位编号") << tr("工装类型") << tr("工装序号");
    QTableView *view = ui->tableViewFixture;
    QHeaderView *header = view->horizontalHeader();
    header->setFixedHeight(30);
    header->setHighlightSections(false);
    view->setSelectionMode(QTableView::SingleSelection);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->horizontalHeader()->setMinimumSectionSize(120);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /*  QSqlQuery dbQuery;
    dbQuery.prepare(R"(SELECT [UID], [CraftID], [WorkStationID], [FixtureType]
                    ,[FixtureOrder]
              FROM [PCS_Craft_Station_Fixture] WHERE [CraftID]= ? AND [WorkStationID]=? AND [State]!=2 )");
    dbQuery.addBindValue(ui->editCraftID->text());
    dbQuery.addBindValue(ui->editStationID->text());
    dbQuery.exec();
    modelFixture->setQuery(dbQuery);*/
    updateTableViewFixture();

    for (int col = 0; col < modelFixture->columnCount(); col++)
        modelFixture->setHeaderData(col, Qt::Horizontal, headerListFixture.at(col));

    ui->tableViewFixture->setModel(modelFixture);
    ui->buttonFixtureDelete->setEnabled(false);
    ui->buttonFixtureModify->setEnabled(false);
    ui->tableViewFixture->setColumnHidden(0, true);
    ui->tableViewFixture->setColumnHidden(1, true);
    ui->tableViewFixture->setColumnHidden(2, true);
}

void WidgetCraftEdit::updateTableViewFixture()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [UID], [CraftID], [WorkStationID], [FixtureType]
                    ,[FixtureOrder]
              FROM [PCS_Craft_Station_Fixture] WHERE [CraftID]= ? AND [WorkStationID]=? AND [State]!=2 )");    //////
    query.addBindValue(ui->editCraftID->text());
    query.addBindValue(ui->editStationID->text());
    query.exec();
    modelFixture->setQuery(query);
}
