#include "DialogLogisticsFeed.h"
#include "WorkManager.h"
#include "ui_DialogLogisticsFeed.h"
DialogLogisticsFeed::DialogLogisticsFeed(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogLogisticsFeed)
{
    ui->setupUi(this);

    initialWidget();
}

DialogLogisticsFeed::~DialogLogisticsFeed()
{
    delete ui;
}

void DialogLogisticsFeed::initialWidget()
{
    do
    {
        tableHeaderMap.insert(TableHeader::MaterialID, tr("物料编号"));
        tableHeaderMap.insert(TableHeader::MaterialName, tr("物料名称"));
        tableHeaderMap.insert(TableHeader::MaterialBatch, tr("生产批次"));
        tableHeaderMap.insert(TableHeader::PackageBatch, tr("最小包装批次"));
        tableHeaderMap.insert(TableHeader::BatchQuantity, tr("包装数量"));
        tableHeaderMap.insert(TableHeader::BatchState, tr("状态"));

        QTableWidget *table = ui->tableWidget;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(tableHeaderMap.size());
        table->setHorizontalHeaderLabels(tableHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //不可选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {

    } while (0);
}

void DialogLogisticsFeed::slotFeed()
{
}

void DialogLogisticsFeed::slotReturn()
{
}

void DialogLogisticsFeed::slotReturnAll()
{
}
