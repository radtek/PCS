#include "WidgetBaseFixture.h"
#include "WorkManager.h"
#include "XYSqlQueryModel.h"
#include "ui_WidgetBaseFixture.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QtSql>
const QMap<QVariant, QString> Dict_Scrap =
    {
        {Useing, "使用中"},
        {Scraped, "报废"},
};
WidgetBaseFixture::WidgetBaseFixture(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetBaseFixture)
{
    ui->setupUi(this);
    model = new XYSqlQueryModel();
    initialWidget();
    initialWidgetFixtureList();
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotFixtureSelect(const QModelIndex &)));
    connect(ui->buttonGroup, SIGNAL(signalAdd()), this, SLOT(slotAddFixture()));
    connect(ui->buttonGroup, SIGNAL(signalModify()), this, SLOT(slotModifyFixture()));
    connect(ui->buttonGroup, SIGNAL(signalDelete()), this, SLOT(slotDeleteFixture()));
    connect(ui->buttonGroup, SIGNAL(signalSave()), this, SLOT(slotSaveFixture()));
    connect(ui->buttonGroup, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
    saveStateMark = StateMark::NoMove;
    ui->editUsedCount->setEnabled(false);
}

WidgetBaseFixture::~WidgetBaseFixture()
{
    delete ui;
}

void WidgetBaseFixture::slotFixtureSelect(const QModelIndex &)
{
    tableIndex = ui->tableView->currentIndex();
    QSqlRecord record = model->record(tableIndex.row());

    ui->editFixtureID->setText(record.value("FixtureID").toString());

    ui->editFixtureName->setText(record.value("FixtureName").toString());
    ui->editFixtureType->setText(record.value("FixtureType").toString());
    ui->editFixtureBarcode->setText(record.value("FixtureBarcode").toString());
    ui->editTotalCount->setText(record.value("TotalCount").toString());
    ui->editUsedCount->setText(record.value("UsedCount").toString());
    ui->editWarningCount->setText(record.value("WarningCount").toString());
    ui->editDescription->setText(record.value("Description").toString());
    ui->comboBoxScrap->setCurrentIndex(record.value("ScrapMarker").toInt());
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
}

void WidgetBaseFixture::slotAddFixture()
{
    ui->editFixtureID->clear();
    ui->editFixtureName->clear();
    ui->editFixtureType->clear();
    ui->editFixtureBarcode->clear();
    ui->editTotalCount->clear();
    ui->editUsedCount->setText("0");
    ui->editWarningCount->clear();
    ui->editDescription->clear();
    ui->comboBoxScrap->setCurrentIndex(-1);
    updateWidget();
    ui->editFixtureID->setEnabled(true);
    saveStateMark = StateMark::AddNew;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseFixture::slotDeleteFixture()
{
    if (QMessageBox::Yes !=
        QMessageBox::question(this, tr("信息"), tr("确定删除工装编号：%1工装信息？").arg(ui->editFixtureID->text())))
    {
        return;
    }
    QSqlQuery query(LOCAL_DB);
    query.prepare("UPDATE PCS_Base_Fixture SET State=2 WHERE FixtureID=? ");
    query.addBindValue(ui->editFixtureID->text());
    query.exec();
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Unselect);
    updateWidgetFixtureList();
    initialWidget();
}

void WidgetBaseFixture::slotModifyFixture()
{
    updateWidget();
    ui->editFixtureID->setEnabled(false);
    saveStateMark = StateMark::ModifyOld;
    ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Edit);
}

void WidgetBaseFixture::slotSaveFixture()
{
    if (ui->editFixtureID->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工装编号！"));
        return;
    }
    if (ui->editFixtureName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工装名称！"));
        return;
    }
    if (ui->editFixtureType->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工装类型！"));
        return;
    }
    if (ui->editFixtureBarcode->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工装条码！"));
        return;
    }
    if (ui->editTotalCount->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工装使用总次数！"));
        return;
    }
    if (ui->editWarningCount->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入工装预警次数！"));
        return;
    }
    if (ui->comboBoxScrap->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请选择工装报废标记！"));
        return;
    }
    QSqlQuery query(LOCAL_DB);
    switch (saveStateMark)
    {
    case StateMark::AddNew:
    {

        query.prepare(R"(SELECT [FixtureID] FROM [PCS_Base_Fixture] WHERE [FixtureID]=? )");
        query.addBindValue(ui->editFixtureID->text());
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

        query.prepare(R"(INSERT INTO [PCS_Base_Fixture]
                      ([FixtureID]
                      ,[FixtureName]
                      ,[FixtureType]
                      ,[FixtureBarcode]
                      ,[TotalCount]
                      ,[UsedCount]
                      ,[WarningCount]
                      ,[ScrapMarker]
                      ,[CreateUser]
                      ,[CreateTime]
                      ,[State]
                      ,[Description])
                VALUES
                      (?,?,?,?,?,?,?,?,?,?,?,?)       )");
        query.addBindValue(ui->editFixtureID->text());
        query.addBindValue(ui->editFixtureName->text());
        query.addBindValue(ui->editFixtureType->text());
        query.addBindValue(ui->editFixtureBarcode->text());
        query.addBindValue(ui->editTotalCount->text().toInt());
        query.addBindValue(ui->editUsedCount->text().toInt());
        query.addBindValue(ui->editWarningCount->text().toInt());
        query.addBindValue(ui->comboBoxScrap->currentData());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(0);
        query.addBindValue(ui->editDescription->toPlainText());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }
        updateWidgetFixtureList();
        ui->tableView->setEnabled(true);
        ui->groupBoxMessage->setEnabled(false);
        ui->buttonGroup->setGroupState(ButtonGroup::GroupState::Select);
        ui->tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
        // ui->tableView->ro
        break;
    }
    case StateMark::ModifyOld:
    {

        query.prepare(R"(UPDATE [PCS_Base_Fixture]
                      SET [FixtureName] = ?
                         ,[FixtureType] = ?
                         ,[FixtureBarcode] = ?
                         ,[TotalCount] = ?
                         ,[WarningCount] = ?
                         ,[ScrapMarker] = ?
                         ,[ModifyUser] = ?
                         ,[ModifyTime] = ?
                         ,[State] = ?
                         ,[Description] = ?
                    WHERE [FixtureID] = ?    )");

        query.addBindValue(ui->editFixtureName->text());
        query.addBindValue(ui->editFixtureType->text());
        query.addBindValue(ui->editFixtureBarcode->text());
        query.addBindValue(ui->editTotalCount->text().toInt());
        query.addBindValue(ui->editWarningCount->text().toInt());
        query.addBindValue(ui->comboBoxScrap->currentData());
        query.addBindValue("");
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(1);
        query.addBindValue(ui->editDescription->toPlainText());
        query.addBindValue(ui->editFixtureID->text());
        if (!query.exec())
        {
            qDebug().noquote() << query.lastError().text();
            return;
        }

        updateWidgetFixtureList();
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

void WidgetBaseFixture::slotCancel()
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

void WidgetBaseFixture::initialWidget()
{
    ui->comboBoxScrap->clear();
    for (auto it = Dict_Scrap.begin(); it != Dict_Scrap.end(); ++it)
    {
        ui->comboBoxScrap->addItem(it.value(), it.key());
    }

    ui->editFixtureID->clear();
    ui->editFixtureName->clear();
    ui->editFixtureType->clear();
    ui->editFixtureBarcode->clear();
    ui->editTotalCount->clear();
    ui->editUsedCount->clear();
    ui->editWarningCount->clear();
    ui->editDescription->clear();
    ui->comboBoxScrap->setCurrentIndex(-1);
    ui->groupBoxMessage->setEnabled(false);
}

void WidgetBaseFixture::initialWidgetFixtureList()
{
    headerList << tr("工装编号") << tr("工装名称") << tr("工装类型") << tr("工装条码") << tr("使用总次数") << tr("已使用次数") << tr("预警次数") << tr("报废标记") << tr("备注");
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
    query.prepare(R"(SELECT [FixtureID]
                  ,[FixtureName]
                  ,[FixtureType]
                  ,[FixtureBarcode]
                  ,[TotalCount]
                  ,[UsedCount]
                  ,[WarningCount]
                  ,[ScrapMarker]
                  ,[Description]
              FROM [PCS_Base_Fixture] WHERE [State]!=2 )");
    query.exec();

    model->setQuery(query);
    for (int col = 0; col < model->columnCount(); col++)
        model->setHeaderData(col, Qt::Horizontal, headerList.at(col));

    ui->tableView->setModel(model);

    model->setColumnEnum("ScrapMarker", Dict_Scrap);
}

void WidgetBaseFixture::updateWidgetFixtureList()
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [FixtureID]
                  ,[FixtureName]
                  ,[FixtureType]
                  ,[FixtureBarcode]
                  ,[TotalCount]
                  ,[UsedCount]
                  ,[WarningCount]
                  ,[ScrapMarker]
                  ,[Description]
              FROM [PCS_Base_Fixture] WHERE [State]!=2 )");
    query.exec();
    model->setQuery(query);
    ui->tableView->setModel(model);
    model->setColumnEnum("ScrapMarker", Dict_Scrap);
}

void WidgetBaseFixture::updateWidget()
{
    ui->tableView->setEnabled(false);
    ui->groupBoxMessage->setEnabled(true);
}
