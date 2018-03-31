#include "CommWidget.h"
#include "ui_CommWidget.h"
#include <QTableWidget>
#include <QTableWidgetItem>

CommWidget::CommWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CommWidget)
{
    ui->setupUi(this);

    initialWidget();
    initialMenu();
    initialData();
}

CommWidget::~CommWidget()
{
    delete ui;
}

void CommWidget::initialWidget()
{
    do
    {
        tableHeaderMap.insert(TableHeader::DeviceID,    "Device ID");
        tableHeaderMap.insert(TableHeader::StationID,   "Station ID");
        tableHeaderMap.insert(TableHeader::DeviceType,  "Device Type");
        tableHeaderMap.insert(TableHeader::IsEnable,    "Enable Status");
        tableHeaderMap.insert(TableHeader::CommType,    "Comm Type");
        tableHeaderMap.insert(TableHeader::CommPara,    "Comm Parameter");

        QTableWidget *table = ui->tableWidget;

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(tableHeaderMap.size());
        table->setHorizontalHeaderLabels(tableHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);     //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);      //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);       //禁用编辑
        table->setContextMenuPolicy(Qt::CustomContextMenu);         //启用菜单

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(60);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    while (0);
}

void CommWidget::initialMenu()
{

}

void CommWidget::initialData()
{

}


