#include "WidgetCraftRouteDesign.h"
#include "WorkManager.h"
#include "ui_WidgetCraftRouteDesign.h"

WidgetCraftRouteDesign::WidgetCraftRouteDesign(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetCraftRouteDesign)
{
    ui->setupUi(this);

    initialWidget();
    initialRecord();
    ui->label_2->hide();
    ui->labelCraftName->hide();
}

WidgetCraftRouteDesign::~WidgetCraftRouteDesign()
{
    delete ui;
}

void WidgetCraftRouteDesign::initialWidget()
{
    do
    {
        QTreeWidget *tree = ui->treeWidget;

        tree->setFont(qApp->font());
        tree->header()->setFont(qApp->font());
        tree->header()->setFixedHeight(25);
        tree->setHeaderLabel(tr("工艺列表"));
        tree->setStyleSheet("QTreeWidget::item{height:21px}");

        connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(slotTreeSelectionChanged()));
    } while (0);

    do
    {
        materialHeaderMap.insert(MaterialHeader::MaterialID, tr("物料编号"));
        materialHeaderMap.insert(MaterialHeader::MaterialName, tr("物料名称"));
        materialHeaderMap.insert(MaterialHeader::BarcodeRule, tr("条码防错规则"));
        materialHeaderMap.insert(MaterialHeader::UsageQuantity, tr("单件消耗数量"));
        materialHeaderMap.insert(MaterialHeader::AlarmQuantity, tr("缺料预警数量"));

        QTableWidget *table = ui->tableMaterial;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(materialHeaderMap.size());
        table->setHorizontalHeaderLabels(materialHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
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
        measureHeaderMap.insert(MeasureHeader::MeasureID, tr("测试编号"));
        measureHeaderMap.insert(MeasureHeader::MeasureName, tr("测试名称"));
        measureHeaderMap.insert(MeasureHeader::MeasureUnit, tr("测试单位"));
        measureHeaderMap.insert(MeasureHeader::StandardValue, tr("标准值"));
        measureHeaderMap.insert(MeasureHeader::UpperLimit, tr("上限"));
        measureHeaderMap.insert(MeasureHeader::LowerLimit, tr("下限"));

        QTableWidget *table = ui->tableMeasure;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(measureHeaderMap.size());
        table->setHorizontalHeaderLabels(measureHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
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
        fixtureHeaderMap.insert(FixtureHeader::FixtureID, tr("工装编号"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureName, tr("工装名称"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureBarcode, tr("工装代码"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureClassify, tr("工装分类"));
        fixtureHeaderMap.insert(FixtureHeader::TotalTimes, tr("额定使用次数"));
        fixtureHeaderMap.insert(FixtureHeader::UsedTimes, tr("已使用次数"));
        fixtureHeaderMap.insert(FixtureHeader::RemindTimes, tr("提醒次数"));

        QTableWidget *table = ui->tableFixture;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(fixtureHeaderMap.size());
        table->setHorizontalHeaderLabels(fixtureHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);
}

void WidgetCraftRouteDesign::initialRecord()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = nullptr;

    do
    {
        for (auto craftID : qWorkManager->getCraftIDList())
        {
            item = new QTreeWidgetItem;
            item->setText(0, craftID);
            item->setIcon(0, QIcon(":/images/pngs/tree_craft.png"));
            tree->addTopLevelItem(item);
        }
    } while (0);

    for (int i = 0; i < tree->topLevelItemCount(); ++i)
    {
        for (auto stationID : qWorkManager->getStationIDList())
        {
            item = new QTreeWidgetItem;
            item->setText(0, stationID);
            item->setIcon(0, QIcon(":/images/pngs/tree_station.png"));
            tree->topLevelItem(i)->addChild(item);
        }
    }
}

void WidgetCraftRouteDesign::slotTreeSelectionChanged()
{
    ui->tableMaterial->clearContents();
    ui->tableMaterial->setRowCount(0);
    ui->tableMeasure->clearContents();
    ui->tableMeasure->setRowCount(0);
    ui->tableFixture->clearContents();
    ui->tableFixture->setRowCount(0);

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();
    bool root = (item->parent() == nullptr);
    QString craftID, stationID;

    if (root)
    {
        craftID = item->text(0);
    }
    else
    {
        craftID = item->parent()->text(0);
        stationID = item->text(0);
    }

    do
    {
        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT [UID]
                      ,[CraftID]
                      ,[ProductID]
                      ,[ProductName]
                      ,[AssySerialRule]
                      ,[AssySerialInit]
                      ,[PackSerialRule]
                      ,[PackSerialInit]
                      FROM [PCS_Craft]
                      WHERE [WorkLineID] = ? AND [CraftID] = ?)");
        query.addBindValue(qWorkManager->getWorklineID());
        query.addBindValue(craftID);
        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            break;
        }

        if (query.first())
        {
            ui->labelCraftID->setText(query.value("CraftID").toString());
            //  ui->labelCraftName->setText(query.value("CraftName").toString());
            ui->labelProductID->setText(query.value("ProductID").toString());
            ui->labelProductName->setText(query.value("ProductName").toString());

            SerialRule assyRule = static_cast<SerialRule>(query.value("AssySerialRule").toInt());    //@@@
            ui->labelAssySerialRule->setText(SerialRuleMap.value(assyRule));
            SerialRule packRule = static_cast<SerialRule>(query.value("PackSerialRule").toInt());
            ui->labelPackSerialRule->setText(SerialRuleMap.value(packRule));

            ui->labelAssySerialInit->setNum(query.value("AssySerialInit").toInt());
            ui->labelPackSerialInit->setNum(query.value("PackSerialInit").toInt());
        }
    } while (0);

    if (root)
        return;

    do
    {
        QTableWidget *table = ui->tableMaterial;
        QTableWidgetItem *item = nullptr;

        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[MaterialID]
                      ,M.[MaterialName]
                      ,M.[BarcodeRule]
                      ,A.[SingleConsumeQuantity] AS [UsageQuantity]
                      ,A.[ShortWarningQuantity] AS [AlarmQuantity]
                      FROM [PCS_Craft_Station_Material] A
                      LEFT JOIN [PCS_Base_Material] M ON A.MaterialID = M.MaterialID
                      WHERE A.[CraftID] = ? AND A.[WorkStationID] = ? AND A.[State] != ?)");
        query.addBindValue(craftID);
        query.addBindValue(stationID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            break;
        }

        for (int row = 0; query.next(); ++row)
        {
            table->insertRow(row);

            item = new QTableWidgetItem(query.value("MaterialID").toString());
            table->setItem(row, static_cast<int>(MaterialHeader::MaterialID), item);

            item = new QTableWidgetItem(query.value("MaterialName").toString());
            table->setItem(row, static_cast<int>(MaterialHeader::MaterialName), item);

            item = new QTableWidgetItem(query.value("BarcodeRule").toString());
            table->setItem(row, static_cast<int>(MaterialHeader::BarcodeRule), item);

            item = new QTableWidgetItem(query.value("UsageQuantity").toString());
            table->setItem(row, static_cast<int>(MaterialHeader::UsageQuantity), item);

            item = new QTableWidgetItem(query.value("AlarmQuantity").toString());
            table->setItem(row, static_cast<int>(MaterialHeader::AlarmQuantity), item);
        }
    } while (0);

    do
    {
        QTableWidget *table = ui->tableMeasure;
        QTableWidgetItem *item = nullptr;

        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[MeasureType] + '#' + CAST(A.[MeasureOrder] AS NVARCHAR) AS [MeasureID]
                      ,D.[MeasureName] + '#' + CAST(A.[MeasureOrder] AS NVARCHAR) AS [MeasureName]
                      ,D.[Unit] AS [MeasureUnit]
                      ,A.[StandardValue] AS [StandardValue]
                      ,A.[UpperLimit] AS [UpperLimit]
                      ,A.[LowerLimit] AS [LowerLimit]
                      FROM [PCS_Craft_Station_Measure] A
                      LEFT JOIN [PCS_Base_Measure] D ON A.[MeasureType] = D.[MeasureType]
                      WHERE A.[CraftID] = ? AND A.[WorkStationID] = ? AND A.[State] != ?)");
        query.addBindValue(craftID);
        query.addBindValue(stationID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            break;
        }

        for (int row = 0; query.next(); ++row)
        {
            table->insertRow(row);

            item = new QTableWidgetItem(query.value("MeasureID").toString());
            table->setItem(row, static_cast<int>(MeasureHeader::MeasureID), item);

            item = new QTableWidgetItem(query.value("MeasureName").toString());
            table->setItem(row, static_cast<int>(MeasureHeader::MeasureName), item);

            item = new QTableWidgetItem(query.value("MeasureUnit").toString());
            table->setItem(row, static_cast<int>(MeasureHeader::MeasureUnit), item);

            item = new QTableWidgetItem(query.value("StandardValue").toString());
            table->setItem(row, static_cast<int>(MeasureHeader::StandardValue), item);

            item = new QTableWidgetItem(query.value("UpperLimit").toString());
            table->setItem(row, static_cast<int>(MeasureHeader::UpperLimit), item);

            item = new QTableWidgetItem(query.value("LowerLimit").toString());
            table->setItem(row, static_cast<int>(MeasureHeader::LowerLimit), item);
        }
    } while (0);

    /*  do
    {
        QTableWidget *table = ui->tableFixture;
        QTableWidgetItem *item = nullptr;

        QSqlQuery query(LOCAL_DB);
        query.prepare(R"(SELECT A.[UID]
                      ,A.[DeviceCode] AS [FixtureID]
                      ,I.[DeviceName] AS [FixtureName]
                      ,A.[DeviceBarCode] AS [FixtureBarcode]
                      ,D1.[DName] + D2.[DName] AS [FixtureClassify]
                      ,I.[TotalTimes] AS [TotalTimes]
                      ,I.[UsedTimes] AS [UsedTimes]
                      ,I.[RemindTimes] AS [RemindTimes]
                      ,A.[State]
                      FROM [MES_Process_DeviceMapping] A
                      LEFT JOIN [MES_Device_Info] I ON A.[DeviceCode] = I.[DeviceCode]
                      LEFT JOIN [MES_db_Dict] D1 ON D1.[PDCode] = 'Dict_DeviceType' AND A.[LedgerType] = D1.[Dcode]
                      LEFT JOIN [MES_db_Dict] D2
                      ON (D2.[PDCode] = 'TechnologyClassify' AND A.[LedgerType] = 1 AND A.[DeviceClassify] = D2.[Dcode])
                      OR (D2.[PDCode] = 'MouldClassify' AND A.[LedgerType] = 2 AND A.[DeviceClassify] = D2.[Dcode])
                      WHERE A.[ProcessCode] = ? AND A.[WorkStationCode] = ? AND A.[State] != ?)");
        query.addBindValue(craftID);
        query.addBindValue(stationID);
        query.addBindValue(RECORD_DELETE);

        if (!query.exec())
        {
            qDebug().noquote() << query.lastQuery();
            break;
        }

        for (int row = 0; query.next(); ++row)
        {
            table->insertRow(row);

            item = new QTableWidgetItem(query.value("FixtureID").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::FixtureID), item);

            item = new QTableWidgetItem(query.value("FixtureName").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::FixtureName), item);

            item = new QTableWidgetItem(query.value("FixtureBarcode").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::FixtureBarcode), item);

            item = new QTableWidgetItem(query.value("FixtureClassify").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::FixtureClassify), item);

            item = new QTableWidgetItem(query.value("TotalTimes").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::TotalTimes), item);

            item = new QTableWidgetItem(query.value("UsedTimes").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::UsedTimes), item);

            item = new QTableWidgetItem(query.value("RemindTimes").toString());
            table->setItem(row, static_cast<int>(FixtureHeader::RemindTimes), item);
        }
    } while (0);*/
    //   @@@
}
