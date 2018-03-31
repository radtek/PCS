#include "WidgetCommDataMonitor.h"
#include "ui_WidgetCommDataMonitor.h"

WidgetCommDataMonitor::WidgetCommDataMonitor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetCommDataMonitor)
{
    ui->setupUi(this);

    initialWidget();

    initialRecord();
}

WidgetCommDataMonitor::~WidgetCommDataMonitor()
{
    delete ui;
}

void WidgetCommDataMonitor::initialWidget()
{
    do
    {
        ui->toolOpcData->setChecked(true);
        ui->stackedWidget->setCurrentIndex(0);

        QButtonGroup *buttonGroup = new QButtonGroup(this);

        buttonGroup->addButton(ui->toolOpcData, 0);
        buttonGroup->addButton(ui->toolComData, 1);
        buttonGroup->addButton(ui->toolLinkStatus, 2);

        connect(buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    } while (0);

    do
    {
        opcDataHeaderMap.insert(OpcDataHeader::StationID, "工位编号");
        opcDataHeaderMap.insert(OpcDataHeader::ItemID, "地址采集编号");
        opcDataHeaderMap.insert(OpcDataHeader::ItemValue, "值");
        opcDataHeaderMap.insert(OpcDataHeader::UpdateTime, "更新时间");
        opcDataHeaderMap.insert(OpcDataHeader::Operation, "测试");

        QTableWidget *table = ui->tableOpcData;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(opcDataHeaderMap.size());
        table->setHorizontalHeaderLabels(opcDataHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectItems);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(cellActivated(int, int)), this, SLOT(slotOpcReadData(int, int)));
    } while (0);

    do
    {
        comDataHeaderMap.insert(ComDataHeader::StationID, "工位编号");
        comDataHeaderMap.insert(ComDataHeader::ItemID, "设备采集编号");
        comDataHeaderMap.insert(ComDataHeader::ItemValue, "值");
        comDataHeaderMap.insert(ComDataHeader::UpdateTime, "更新时间");
        comDataHeaderMap.insert(ComDataHeader::Operation, "测试");

        QTableWidget *table = ui->tableComData;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(comDataHeaderMap.size());
        table->setHorizontalHeaderLabels(comDataHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectItems);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(cellActivated(int, int)), this, SLOT(slotDacReadData(int, int)));
    } while (0);
}

void WidgetCommDataMonitor::initialRecord()
{
    do
    {
        QTableWidget *table = ui->tableOpcData;
        QTableWidgetItem *item = nullptr;
        int row = 0;

        for (auto stationID : qOPCManager->getStationIDList())
        {
            for (auto itemID : qOPCManager->getItemIDList(stationID))
            {
                table->insertRow(row);

                item = new QTableWidgetItem(stationID);
                table->setItem(row, static_cast<int>(OpcDataHeader::StationID), item);

                item = new QTableWidgetItem(itemID);
                table->setItem(row, static_cast<int>(OpcDataHeader::ItemID), item);

                item = new QTableWidgetItem();
                table->setItem(row, static_cast<int>(OpcDataHeader::ItemValue), item);

                item = new QTableWidgetItem();
                table->setItem(row, static_cast<int>(OpcDataHeader::UpdateTime), item);

                item = new QTableWidgetItem("双击测试");
                item->setTextAlignment(Qt::AlignCenter);
                item->setTextColor(Qt::blue);
                table->setItem(row, static_cast<int>(OpcDataHeader::Operation), item);

                ++row;
            }
        }
    } while (0);

    do
    {
        QTableWidget *table = ui->tableComData;
        QTableWidgetItem *item = nullptr;
        int row = 0;

        for (auto stationID : qCommManager->getStationIDList())
        {
            for (auto itemID : qCommManager->getItemIDList(stationID))
            {
                table->insertRow(row);

                item = new QTableWidgetItem(stationID);
                table->setItem(row, static_cast<int>(ComDataHeader::StationID), item);

                item = new QTableWidgetItem(itemID);
                table->setItem(row, static_cast<int>(ComDataHeader::ItemID), item);

                item = new QTableWidgetItem();
                table->setItem(row, static_cast<int>(ComDataHeader::ItemValue), item);

                item = new QTableWidgetItem();
                table->setItem(row, static_cast<int>(ComDataHeader::UpdateTime), item);

                item = new QTableWidgetItem("双击测试");
                item->setTextAlignment(Qt::AlignCenter);
                item->setTextColor(Qt::blue);
                table->setItem(row, static_cast<int>(ComDataHeader::Operation), item);

                ++row;
            }
        }
    } while (0);
}

void WidgetCommDataMonitor::slotOpcDataReceived(const QString &stationID, const DataDefine &data)
{
    QTableWidget *table = ui->tableOpcData;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(OpcDataHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(OpcDataHeader::ItemID));

        if (data.itemID != item->text())
            continue;

        item = table->item(row, static_cast<int>(OpcDataHeader::ItemValue));
        item->setText(data.itemValue.toString());

        item = table->item(row, static_cast<int>(OpcDataHeader::UpdateTime));
        item->setText(QTime::currentTime().toString("hh:mm:ss.zzz"));
    }
}

void WidgetCommDataMonitor::slotDacDataReceived(const QString &stationID, const DataDefine &data)
{
    QTableWidget *table = ui->tableComData;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(ComDataHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(ComDataHeader::ItemID));

        if (data.itemID != item->text())
            continue;

        item = table->item(row, static_cast<int>(ComDataHeader::ItemValue));
        item->setText(data.itemValue.toString());

        item = table->item(row, static_cast<int>(ComDataHeader::UpdateTime));
        item->setText(QTime::currentTime().toString("hh:mm:ss.zzz"));
    }
}

void WidgetCommDataMonitor::slotOpcReadData(int row, int column)
{
    if (column != static_cast<int>(OpcDataHeader::Operation))
        return;

    QTableWidget *table = ui->tableOpcData;
    QTableWidgetItem *item = nullptr;
    DataDefine data;

    item = table->item(row, static_cast<int>(OpcDataHeader::StationID));
    QString stationID = item->text();

    item = table->item(row, static_cast<int>(OpcDataHeader::ItemID));
    data.itemID = item->text();

    emit signalOpcReadData(stationID, data);
}

void WidgetCommDataMonitor::slotDacReadData(int row, int column)
{
    if (column != static_cast<int>(ComDataHeader::Operation))
        return;

    QTableWidget *table = ui->tableComData;
    QTableWidgetItem *item = nullptr;
    DataDefine data;

    item = table->item(row, static_cast<int>(ComDataHeader::StationID));
    QString stationID = item->text();

    item = table->item(row, static_cast<int>(ComDataHeader::ItemID));
    data.itemID = item->text();

    emit signalDacReadData(stationID, data);
}
