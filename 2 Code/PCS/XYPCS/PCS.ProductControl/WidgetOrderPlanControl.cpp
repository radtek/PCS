#include "WidgetOrderPlanControl.h"
#include "DialogLogisticsFeed.h"
#include "DialogMaterialInput.h"
#include "DialogOperatorLogin.h"
#include "StationMaterial.h"
#include "StationOperator.h"
#include "WorkCraft.h"
#include "WorkOrder.h"
#include "WorkStation.h"
#include "XYCustomButton.h"
#include "ui_WidgetOrderPlanControl.h"

WidgetOrderPlanControl::WidgetOrderPlanControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetOrderPlanControl)
{
    ui->setupUi(this);

    initialWidget();
}

WidgetOrderPlanControl::~WidgetOrderPlanControl()
{
    delete ui;
}

void WidgetOrderPlanControl::initialWidget()
{
    do
    {
        stationHeaderMap.insert(StationHeader::StationID, tr("工位编号"));
        stationHeaderMap.insert(StationHeader::StationName, tr("工位名称"));
        stationHeaderMap.insert(StationHeader::FinishQuantity, tr("已完成数量"));
        stationHeaderMap.insert(StationHeader::FailedQuantity, tr("不合格数量"));
        stationHeaderMap.insert(StationHeader::OperatorLogin, tr("操作员登陆"));
        stationHeaderMap.insert(StationHeader::MaterialInput, tr("物料投料"));
        stationHeaderMap.insert(StationHeader::MachineCheck, tr("设备点检"));
        stationHeaderMap.insert(StationHeader::ProductCheck, tr("产品点检"));
        stationHeaderMap.insert(StationHeader::FixtureCheck, tr("工装模具点检"));

        QTableWidget *table = ui->tableStation;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(stationHeaderMap.size());
        table->setHorizontalHeaderLabels(stationHeaderMap.values());
        table->setSelectionMode(QTableWidget::NoSelection);       //不可选
        table->setSelectionBehavior(QTableWidget::SelectRows);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);     //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        operatorHeaderMap.insert(OperatorHeader::StationID, tr("工位编号"));
        operatorHeaderMap.insert(OperatorHeader::OperatorID, tr("员工编号"));
        operatorHeaderMap.insert(OperatorHeader::OperatorCard, tr("员工卡号"));
        operatorHeaderMap.insert(OperatorHeader::OperatorName, tr("员工姓名"));
        operatorHeaderMap.insert(OperatorHeader::OperatorTeam, tr("员工班组"));
        operatorHeaderMap.insert(OperatorHeader::OperatorDuty, tr("员工职务"));
        operatorHeaderMap.insert(OperatorHeader::Operation, tr("操作"));

        operatorButtonList.append(tr("登陆"));
        operatorButtonList.append(tr("登出"));

        QTableWidget *table = ui->tableOperator;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(operatorHeaderMap.size());
        table->setHorizontalHeaderLabels(operatorHeaderMap.values());
        table->setSelectionMode(QTableWidget::NoSelection);       //不可选
        table->setSelectionBehavior(QTableWidget::SelectRows);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);     //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        materialHeaderMap.insert(MaterialHeader::StationID, tr("工位编号"));
        materialHeaderMap.insert(MaterialHeader::MaterialID, tr("物料编号"));
        materialHeaderMap.insert(MaterialHeader::MaterialName, tr("物料名称"));
        materialHeaderMap.insert(MaterialHeader::MaterialBatch, tr("投料批次"));
        materialHeaderMap.insert(MaterialHeader::BatchQuantity, tr("批次剩余数量"));
        materialHeaderMap.insert(MaterialHeader::UsageQuantity, tr("单件消耗数量"));
        materialHeaderMap.insert(MaterialHeader::AlarmQuantity, tr("缺料预警数量"));
        materialHeaderMap.insert(MaterialHeader::Operation, tr("操作"));

        materialButtonList.append(tr("上料"));
        materialButtonList.append(tr("退料"));

        QTableWidget *table = ui->tableMaterial;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(materialHeaderMap.size());
        table->setHorizontalHeaderLabels(materialHeaderMap.values());
        table->setSelectionMode(QTableWidget::NoSelection);       //不可选
        table->setSelectionBehavior(QTableWidget::SelectRows);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);     //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        measureHeaderMap.insert(MeasureHeader::StationID, tr("工位编号"));
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
        table->setSelectionMode(QTableWidget::NoSelection);       //不可选
        table->setSelectionBehavior(QTableWidget::SelectRows);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);     //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        fixtureHeaderMap.insert(FixtureHeader::StationID, tr("工位编号"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureID, tr("工装模具编号"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureName, tr("工装模具名称"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureBarcode, tr("工装模具条码"));
        fixtureHeaderMap.insert(FixtureHeader::FixtureClassify, tr("工装模具类型"));
        fixtureHeaderMap.insert(FixtureHeader::TotalTimes, tr("额定使用次数"));
        fixtureHeaderMap.insert(FixtureHeader::UsedTimes, tr("已使用次数"));
        fixtureHeaderMap.insert(FixtureHeader::RemindTimes, tr("提醒次数"));

        QTableWidget *table = ui->tableFixture;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(fixtureHeaderMap.size());
        table->setHorizontalHeaderLabels(fixtureHeaderMap.values());
        table->setSelectionMode(QTableWidget::NoSelection);       //不可选
        table->setSelectionBehavior(QTableWidget::SelectRows);    //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);     //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        ui->imageFirstProductQA->setRow(-1);
        ui->imageFirstPackageQA->setRow(-1);
        ui->imageFinalProductQA->setRow(-1);
        ui->imageFinalPackageQA->setRow(-1);

        ui->imageFirstProductQA->setIndex(static_cast<int>(QualityAssure::FirstProduct));
        ui->imageFirstPackageQA->setIndex(static_cast<int>(QualityAssure::FirstPackage));
        ui->imageFinalProductQA->setIndex(static_cast<int>(QualityAssure::FinalProduct));
        ui->imageFinalPackageQA->setIndex(static_cast<int>(QualityAssure::FinalPackage));

        connect(ui->imageFirstProductQA, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));
        connect(ui->imageFirstPackageQA, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));
        connect(ui->imageFinalProductQA, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));
        connect(ui->imageFinalPackageQA, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));
    } while (0);

    do
    {
        ui->tabWidget->setCurrentIndex(0);

        connect(ui->buttonStartWork, SIGNAL(clicked()), this, SLOT(slotStartWork()));
        connect(ui->buttonPauseWork, SIGNAL(clicked()), this, SLOT(slotPauseWork()));

        connect(ui->buttonOperatorLogin, SIGNAL(clicked()), this, SLOT(slotOperatorLogin()));
        connect(ui->buttonMaterialInput, SIGNAL(clicked()), this, SLOT(slotMaterialInput()));
        connect(ui->buttonMaterialFeed, SIGNAL(clicked()), this, SLOT(slotLogisticsFeed()));
    } while (0);

    do
    {
        if (qWorkManager->isWorkActivated())
            slotOrderActivated();
        else
            slotOrderSuspended();
    } while (0);
}

void WidgetOrderPlanControl::changeButtonState()
{
    if (qWorkManager->isWorkRunning())
    {
        do
        {
            QPalette palette = ui->buttonStartWork->palette();
            palette.setColor(QPalette::Normal, QPalette::Button, QColor("#00ff00"));
            ui->buttonStartWork->setPalette(palette);
        } while (0);

        do
        {
            QPalette palette = ui->buttonPauseWork->palette();
            palette.setColor(QPalette::Normal, QPalette::Button, QColor("#efebe7"));
            ui->buttonPauseWork->setPalette(palette);
        } while (0);
    }
    else
    {
        do
        {
            QPalette palette = ui->buttonStartWork->palette();
            palette.setColor(QPalette::Normal, QPalette::Button, QColor("#efebe7"));
            ui->buttonStartWork->setPalette(palette);
        } while (0);

        do
        {
            QPalette palette = ui->buttonPauseWork->palette();
            palette.setColor(QPalette::Normal, QPalette::Button, QColor("#ff0000"));
            ui->buttonPauseWork->setPalette(palette);
        } while (0);
    }
}

void WidgetOrderPlanControl::slotStartWork()
{
    qWorkManager->startWork();

    changeButtonState();
}

void WidgetOrderPlanControl::slotPauseWork()
{
    qWorkManager->pauseWork();

    changeButtonState();
}

void WidgetOrderPlanControl::slotOrderActivated()
{
    //工单信息
    do
    {
        WorkOrder *order = qWorkManager->getOrder();
        WorkCraft *craft = qWorkManager->getCraft();

        const OrderData &orderData = order->getOrderData();
        const CraftData &craftData = craft->getCraftData();

        ui->textOrderID->setText(orderData.orderID);
        ui->textProductID->setText(craftData.productID);
        ui->textProductName->setText(craftData.productName);
        ui->textProductBatch->setText(orderData.productBatch);
        ui->textPlanDate->setText(orderData.orderPlanDate.toString("yyyy-MM-dd"));

        ui->valueOrderPlanQuantity->setNum(orderData.orderPlanQuantity);
        ui->valueInspectQuantity->setNum(orderData.inspectQuantity);

        slotOrderPlanUpdate(orderData);

        changeButtonState();

        ui->buttonStartWork->setEnabled(true);
        ui->buttonPauseWork->setEnabled(true);
        ui->buttonOperatorLogin->setEnabled(true);
        ui->buttonMaterialInput->setEnabled(true);
    } while (0);

    //工位信息
    do
    {
        QTableWidget *table = ui->tableStation;
        QTableWidgetItem *item = nullptr;
        XYCustomLabel *label = nullptr;
        int row = 0, rowCount = 0;

        rowCount = qWorkManager->getStationList().size();
        table->setRowCount(rowCount);

        for (auto station : qWorkManager->getStationList())
        {
            const StationData &data = station->getStationData();

            item = new QTableWidgetItem(data.stationID);
            table->setItem(row, static_cast<int>(StationHeader::StationID), item);

            item = new QTableWidgetItem(data.stationName);
            table->setItem(row, static_cast<int>(StationHeader::StationName), item);

            item = new QTableWidgetItem(QString::number(data.finishQuantity));
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(StationHeader::FinishQuantity), item);

            item = new QTableWidgetItem(QString::number(data.failedQuantity));
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(StationHeader::FailedQuantity), item);

            label = new XYCustomLabel;
            label->setRow(row);
            label->setIndex(static_cast<int>(StationHeader::OperatorLogin));
            label->setValid(data.isOperatorLogin);
            table->setCellWidget(row, static_cast<int>(StationHeader::OperatorLogin), label);

            label = new XYCustomLabel;
            label->setRow(row);
            label->setIndex(static_cast<int>(StationHeader::MaterialInput));
            label->setValid(data.isMaterialInput);
            table->setCellWidget(row, static_cast<int>(StationHeader::MaterialInput), label);

            label = new XYCustomLabel;
            label->setRow(row);
            label->setIndex(static_cast<int>(StationHeader::MachineCheck));
            label->setValid(data.isMachineCheck);
            table->setCellWidget(row, static_cast<int>(StationHeader::MachineCheck), label);
            connect(label, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));

            label = new XYCustomLabel;
            label->setRow(row);
            label->setIndex(static_cast<int>(StationHeader::ProductCheck));
            label->setValid(data.isProductCheck);
            table->setCellWidget(row, static_cast<int>(StationHeader::ProductCheck), label);
            connect(label, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));

            label = new XYCustomLabel;
            label->setRow(row);
            label->setIndex(static_cast<int>(StationHeader::FixtureCheck));
            label->setValid(data.isFixtureCheck);
            table->setCellWidget(row, static_cast<int>(StationHeader::FixtureCheck), label);
            connect(label, SIGNAL(signalLabelDblClicked(int, int)), this, SLOT(slotCustomLabelDblClicked(int, int)));

            ++row;
        }
    } while (0);

    //员工信息
    do
    {
        QTableWidget *table = ui->tableOperator;
        QTableWidgetItem *item = nullptr;
        int row = 0, rowCount = 0, rowSpan = 0;

        for (auto station : qWorkManager->getStationList())
        {
            rowSpan = station->getOperatorList().size();

            if (rowSpan == 0)
            {
                rowSpan = 1;
                rowCount += rowSpan;
                table->setRowCount(rowCount);

                item = new QTableWidgetItem(station->getStationID());
                table->setItem(row, static_cast<int>(OperatorHeader::StationID), item);

                item = new QTableWidgetItem;
                table->setItem(row, static_cast<int>(OperatorHeader::OperatorID), item);

                item = new QTableWidgetItem;
                table->setItem(row, static_cast<int>(OperatorHeader::OperatorCard), item);

                item = new QTableWidgetItem;
                table->setItem(row, static_cast<int>(OperatorHeader::OperatorName), item);

                item = new QTableWidgetItem;
                table->setItem(row, static_cast<int>(OperatorHeader::OperatorTeam), item);

                item = new QTableWidgetItem;
                table->setItem(row, static_cast<int>(OperatorHeader::OperatorDuty), item);

                XYCustomButton *button = new XYCustomButton;
                button->setRow(row);
                button->addButtons(operatorButtonList);
                table->setCellWidget(row, static_cast<int>(OperatorHeader::Operation), button);
                connect(button, SIGNAL(signalButtonClicked(int, int)), this, SLOT(slotOperatorButtonClicked(int, int)));

                ++row;
            }
            else
            {
                rowCount += rowSpan;
                table->setRowCount(rowCount);
                table->setSpan(row, static_cast<int>(MaterialHeader::StationID), rowSpan, 1);

                for (auto operater : station->getOperatorList())
                {
                    const OperatorData &data = operater->getOperatorData();

                    item = new QTableWidgetItem(station->getStationID());
                    table->setItem(row, static_cast<int>(OperatorHeader::StationID), item);

                    item = new QTableWidgetItem(data.operatorID);
                    table->setItem(row, static_cast<int>(OperatorHeader::OperatorID), item);

                    item = new QTableWidgetItem(data.operatorCard);
                    table->setItem(row, static_cast<int>(OperatorHeader::OperatorCard), item);

                    item = new QTableWidgetItem(data.operatorName);
                    table->setItem(row, static_cast<int>(OperatorHeader::OperatorName), item);

                    item = new QTableWidgetItem(data.operatorTeam);
                    table->setItem(row, static_cast<int>(OperatorHeader::OperatorTeam), item);

                    item = new QTableWidgetItem(data.operatorDuty);
                    table->setItem(row, static_cast<int>(OperatorHeader::OperatorDuty), item);

                    XYCustomButton *button = new XYCustomButton;
                    button->setRow(row);
                    button->addButtons(operatorButtonList);
                    table->setCellWidget(row, static_cast<int>(OperatorHeader::Operation), button);
                    connect(button, SIGNAL(signalButtonClicked(int, int)), this, SLOT(slotOperatorButtonClicked(int, int)));

                    ++row;
                }
            }
        }
    } while (0);

    //物料信息
    do
    {
        QTableWidget *table = ui->tableMaterial;
        QTableWidgetItem *item = nullptr;
        int row = 0, rowCount = 0, rowSpan = 0;

        for (auto station : qWorkManager->getStationList())
        {
            rowSpan = station->getMaterialList().size();

            if (rowSpan == 0)
                continue;

            rowCount += rowSpan;
            table->setRowCount(rowCount);

            table->setSpan(row, static_cast<int>(MaterialHeader::StationID), rowSpan, 1);

            for (auto material : station->getMaterialList())
            {
                const MaterialData &data = material->getMaterialData();

                item = new QTableWidgetItem(station->getStationID());
                table->setItem(row, static_cast<int>(MaterialHeader::StationID), item);

                item = new QTableWidgetItem(data.materialID);
                table->setItem(row, static_cast<int>(MaterialHeader::MaterialID), item);

                item = new QTableWidgetItem(data.materialName);
                table->setItem(row, static_cast<int>(MaterialHeader::MaterialName), item);

                item = new QTableWidgetItem(data.materialBatch);
                table->setItem(row, static_cast<int>(MaterialHeader::MaterialBatch), item);

                item = new QTableWidgetItem(QString::number(data.batchQuantity));
                table->setItem(row, static_cast<int>(MaterialHeader::BatchQuantity), item);

                item = new QTableWidgetItem(QString::number(data.usageQuantity));
                table->setItem(row, static_cast<int>(MaterialHeader::UsageQuantity), item);

                item = new QTableWidgetItem(QString::number(data.alarmQuantity));
                table->setItem(row, static_cast<int>(MaterialHeader::AlarmQuantity), item);

                XYCustomButton *button = new XYCustomButton;
                button->setRow(row);
                button->addButtons(materialButtonList);
                table->setCellWidget(row, static_cast<int>(MaterialHeader::Operation), button);
                connect(button, SIGNAL(signalButtonClicked(int, int)), this, SLOT(slotMaterialButtonClicked(int, int)));

                ++row;
            }
        }
    } while (0);

    //测试信息
    do
    {
        QTableWidget *table = ui->tableMeasure;
        QTableWidgetItem *item = nullptr;
        int row = 0, rowCount = 0, rowSpan = 0;

        for (auto station : qWorkManager->getStationList())
        {
            rowSpan = station->getMeasureList().size();

            if (rowSpan == 0)
                continue;

            rowCount += rowSpan;
            table->setRowCount(rowCount);

            table->setSpan(row, static_cast<int>(MeasureHeader::StationID), rowSpan, 1);

            for (auto measure : station->getMeasureList())
            {
                const MeasureData &data = measure->getMeasureData();

                item = new QTableWidgetItem(station->getStationID());
                table->setItem(row, static_cast<int>(MeasureHeader::StationID), item);

                item = new QTableWidgetItem(data.measureID);
                table->setItem(row, static_cast<int>(MeasureHeader::MeasureID), item);

                item = new QTableWidgetItem(data.measureName);
                table->setItem(row, static_cast<int>(MeasureHeader::MeasureName), item);

                item = new QTableWidgetItem(data.measureUnit);
                table->setItem(row, static_cast<int>(MeasureHeader::MeasureUnit), item);

                item = new QTableWidgetItem(QString::number(data.standardValue));
                table->setItem(row, static_cast<int>(MeasureHeader::StandardValue), item);

                item = new QTableWidgetItem(QString::number(data.upperLimit));
                table->setItem(row, static_cast<int>(MeasureHeader::UpperLimit), item);

                item = new QTableWidgetItem(QString::number(data.lowerLimit));
                table->setItem(row, static_cast<int>(MeasureHeader::LowerLimit), item);

                ++row;
            }
        }
    } while (0);

    //工装模具信息
    do
    {
        QTableWidget *table = ui->tableFixture;
        QTableWidgetItem *item = nullptr;
        int row = 0, rowCount = 0, rowSpan = 0;

        for (auto station : qWorkManager->getStationList())
        {
            rowSpan = station->getFixtureList().size();

            if (rowSpan == 0)
            {
                QStringList fixtureClsList = qWorkManager->getFixtureClsList(
                    qWorkManager->getCraftID(),
                    station->getStationID());

                rowSpan = fixtureClsList.size();

                rowCount += rowSpan;
                table->setRowCount(rowCount);

                table->setSpan(row, static_cast<int>(FixtureHeader::StationID), rowSpan, 1);

                for (auto fixtureClassify : fixtureClsList)
                {
                    item = new QTableWidgetItem(station->getStationID());
                    table->setItem(row, static_cast<int>(FixtureHeader::StationID), item);

                    item = new QTableWidgetItem;
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureID), item);

                    item = new QTableWidgetItem;
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureName), item);

                    item = new QTableWidgetItem;
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureBarcode), item);

                    item = new QTableWidgetItem(fixtureClassify);
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureClassify), item);

                    item = new QTableWidgetItem;
                    table->setItem(row, static_cast<int>(FixtureHeader::TotalTimes), item);

                    item = new QTableWidgetItem;
                    table->setItem(row, static_cast<int>(FixtureHeader::UsedTimes), item);

                    item = new QTableWidgetItem;
                    table->setItem(row, static_cast<int>(FixtureHeader::RemindTimes), item);

                    ++row;
                }
            }
            else
            {
                rowCount += rowSpan;
                table->setRowCount(rowCount);

                table->setSpan(row, static_cast<int>(FixtureHeader::StationID), rowSpan, 1);

                for (auto fixture : station->getFixtureList())
                {
                    const FixtureData &data = fixture->getFixtureData();

                    item = new QTableWidgetItem(station->getStationID());
                    table->setItem(row, static_cast<int>(FixtureHeader::StationID), item);

                    item = new QTableWidgetItem(data.fixtureID);
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureID), item);

                    item = new QTableWidgetItem(data.fixtureName);
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureName), item);

                    item = new QTableWidgetItem(data.fixtureBarcode);
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureBarcode), item);

                    item = new QTableWidgetItem(data.fixtureClassify);
                    table->setItem(row, static_cast<int>(FixtureHeader::FixtureClassify), item);

                    item = new QTableWidgetItem(QString::number(data.totalTimes));
                    table->setItem(row, static_cast<int>(FixtureHeader::TotalTimes), item);

                    item = new QTableWidgetItem(QString::number(data.usedTimes));
                    table->setItem(row, static_cast<int>(FixtureHeader::UsedTimes), item);

                    item = new QTableWidgetItem(QString::number(data.remindTimes));
                    table->setItem(row, static_cast<int>(FixtureHeader::RemindTimes), item);

                    ++row;
                }
            }
        }
    } while (0);
}

void WidgetOrderPlanControl::slotOrderSuspended()
{
    //员工自动登出
    do
    {
        QTableWidget *table = ui->tableOperator;
        QTableWidgetItem *item = nullptr;

        for (int row = 0; row < table->rowCount(); ++row)
        {
            item = table->item(row, static_cast<int>(OperatorHeader::StationID));
            QString stationID = item->text();
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorID));
            QString operatorID = item->text();

            operatorLogout(stationID, operatorID);
        }
    } while (0);

    //物料自动退料
    do
    {
        QTableWidget *table = ui->tableMaterial;
        QTableWidgetItem *item = nullptr;

        for (int row = 0; row < table->rowCount(); ++row)
        {
            item = table->item(row, static_cast<int>(MaterialHeader::StationID));
            QString stationID = item->text();
            item = table->item(row, static_cast<int>(MaterialHeader::MaterialID));
            QString materialID = item->text();

            materialReturn(stationID, materialID);
        }
    } while (0);

    //初始化界面
    do
    {
        ui->textOrderID->setText(tr("工单未激活"));
        ui->textProductID->setText(tr("工单未激活"));
        ui->textProductName->setText(tr("工单未激活"));
        ui->textProductBatch->setText(tr("工单未激活"));
        ui->textPlanDate->setText(tr("工单未激活"));

        ui->valueOrderPlanQuantity->setNum(0);
        ui->valueInspectQuantity->setNum(0);
        ui->valueFinishQuantity->setNum(0);
        ui->valueFailedQuantity->setNum(0);
        ui->valueSampleQuantity->setNum(0);
        ui->valueRepairQuantity->setNum(0);
        ui->valuePackageQuantity->setNum(0);
        ui->valueFirstPassQuantity->setNum(0);
        ui->valueQualifiedRate->setText("0.00 %");
        ui->valueFirstPassRate->setText("0.00 %");

        ui->imageFirstProductQA->setValid(false);
        ui->imageFirstPackageQA->setValid(false);
        ui->imageFinalProductQA->setValid(false);
        ui->imageFinalPackageQA->setValid(false);

        ui->tableStation->clearContents();
        ui->tableStation->setRowCount(0);

        ui->tableMaterial->clearContents();
        ui->tableMaterial->setRowCount(0);

        ui->tableMeasure->clearContents();
        ui->tableMeasure->setRowCount(0);

        ui->tableFixture->clearContents();
        ui->tableFixture->setRowCount(0);

        ui->tableOperator->clearContents();
        ui->tableOperator->setRowCount(0);

        ui->buttonStartWork->setEnabled(false);
        ui->buttonPauseWork->setEnabled(false);
        ui->buttonOperatorLogin->setEnabled(false);
        ui->buttonMaterialInput->setEnabled(false);
    } while (0);
}

void WidgetOrderPlanControl::slotOrderPlanUpdate(const OrderData &data)
{
    do
    {
        ui->valueFinishQuantity->setNum(data.finishQuantity);
        ui->valueFailedQuantity->setNum(data.failedQuantity);
        ui->valueSampleQuantity->setNum(data.sampleQuantity);
        ui->valueRepairQuantity->setNum(data.repairQuantity);
        ui->valuePackageQuantity->setNum(data.packageQuantity);
        ui->valueFirstPassQuantity->setNum(data.firstPassQuantity);
    } while (0);

    do
    {
        double qualifiedRate = 0, firstPassRate = 0;

        if (data.finishQuantity != 0)
        {
            qualifiedRate = 100.0 * data.finishQuantity / (data.finishQuantity + data.failedQuantity);
            firstPassRate = 100.0 * data.firstPassQuantity / data.finishQuantity;
        }

        ui->valueQualifiedRate->setText(QString::number(qualifiedRate, 'f', 2).append(" %"));
        ui->valueFirstPassRate->setText(QString::number(firstPassRate, 'f', 2).append(" %"));
    } while (0);

    do
    {
        ui->imageFirstProductQA->setValid(data.isFirstProductQA);
        ui->imageFirstPackageQA->setValid(data.isFirstPackageQA);
        ui->imageFinalProductQA->setValid(data.isFinalProductQA);
        ui->imageFinalPackageQA->setValid(data.isFinalPackageQA);
    } while (0);
}

void WidgetOrderPlanControl::slotStationUpdate(const StationData &data)
{
    QTableWidget *table = ui->tableStation;
    QTableWidgetItem *item = nullptr;
    QWidget *widget = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(StationHeader::StationID));

        if (item->text() == data.stationID)
        {
            item = table->item(row, static_cast<int>(StationHeader::FinishQuantity));
            item->setText(QString::number(data.finishQuantity));

            item = table->item(row, static_cast<int>(StationHeader::FailedQuantity));
            item->setText(QString::number(data.failedQuantity));

            widget = table->cellWidget(row, static_cast<int>(StationHeader::OperatorLogin));
            dynamic_cast<XYCustomLabel *>(widget)->setValid(data.isOperatorLogin);

            widget = table->cellWidget(row, static_cast<int>(StationHeader::MaterialInput));
            dynamic_cast<XYCustomLabel *>(widget)->setValid(data.isMaterialInput);

            widget = table->cellWidget(row, static_cast<int>(StationHeader::MachineCheck));
            dynamic_cast<XYCustomLabel *>(widget)->setValid(data.isMachineCheck);

            widget = table->cellWidget(row, static_cast<int>(StationHeader::ProductCheck));
            dynamic_cast<XYCustomLabel *>(widget)->setValid(data.isProductCheck);

            widget = table->cellWidget(row, static_cast<int>(StationHeader::FixtureCheck));
            dynamic_cast<XYCustomLabel *>(widget)->setValid(data.isFixtureCheck);

            break;
        }
    }
}

void WidgetOrderPlanControl::slotMaterialInput(const QString &stationID, const MaterialData &data)
{
    QTableWidget *table = ui->tableMaterial;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(MaterialHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(MaterialHeader::MaterialID));

        if (data.materialID != item->text())
            continue;

        item = table->item(row, static_cast<int>(MaterialHeader::MaterialBatch));
        item->setText(data.materialBatch);

        item = table->item(row, static_cast<int>(MaterialHeader::BatchQuantity));
        item->setText(QString::number(data.batchQuantity));

        return;
    }
}

void WidgetOrderPlanControl::slotMaterialUpdate(const QString &stationID, const MaterialData &data)
{
    QTableWidget *table = ui->tableMaterial;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(MaterialHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(MaterialHeader::MaterialID));

        if (data.materialID != item->text())
            continue;

        item = table->item(row, static_cast<int>(MaterialHeader::BatchQuantity));
        item->setText(QString::number(data.batchQuantity));

        return;
    }
}

void WidgetOrderPlanControl::slotFixtureUpdate(const QString &stationID, const FixtureData &data)
{
    QTableWidget *table = ui->tableFixture;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(FixtureHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(FixtureHeader::FixtureID));

        if (data.fixtureID != item->text())
            continue;

        item = table->item(row, static_cast<int>(FixtureHeader::UsedTimes));
        item->setText(QString::number(data.usedTimes));

        return;
    }
}

void WidgetOrderPlanControl::slotOperatorLogin(const QString &stationID, const OperatorData &data)
{
    QTableWidget *table = ui->tableOperator;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(OperatorHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(OperatorHeader::OperatorID));

        if (item->text().isEmpty())
        {
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorID));
            item->setText(data.operatorID);

            item = table->item(row, static_cast<int>(OperatorHeader::OperatorCard));
            item->setText(data.operatorCard);

            item = table->item(row, static_cast<int>(OperatorHeader::OperatorName));
            item->setText(data.operatorName);

            item = table->item(row, static_cast<int>(OperatorHeader::OperatorTeam));
            item->setText(data.operatorTeam);

            item = table->item(row, static_cast<int>(OperatorHeader::OperatorDuty));
            item->setText(data.operatorDuty);
        }
        else
        {
            int rowSpan = table->rowSpan(row, static_cast<int>(OperatorHeader::StationID));
            int newRow = row + rowSpan;
            table->insertRow(newRow);

            item = new QTableWidgetItem(stationID);
            table->setItem(newRow, static_cast<int>(OperatorHeader::StationID), item);

            item = new QTableWidgetItem(data.operatorID);
            table->setItem(newRow, static_cast<int>(OperatorHeader::OperatorID), item);

            item = new QTableWidgetItem(data.operatorCard);
            table->setItem(newRow, static_cast<int>(OperatorHeader::OperatorCard), item);

            item = new QTableWidgetItem(data.operatorName);
            table->setItem(newRow, static_cast<int>(OperatorHeader::OperatorName), item);

            item = new QTableWidgetItem(data.operatorTeam);
            table->setItem(newRow, static_cast<int>(OperatorHeader::OperatorTeam), item);

            item = new QTableWidgetItem(data.operatorDuty);
            table->setItem(newRow, static_cast<int>(OperatorHeader::OperatorDuty), item);

            XYCustomButton *button = new XYCustomButton;
            button->setRow(row);
            button->addButtons(operatorButtonList);
            table->setCellWidget(newRow, static_cast<int>(OperatorHeader::Operation), button);
            connect(button, SIGNAL(signalButtonClicked(int, int)), this, SLOT(slotOperatorButtonClicked(int, int)));

            table->setSpan(row, static_cast<int>(OperatorHeader::StationID), rowSpan + 1, 1);

            for (int resRow = newRow + 1; resRow < table->rowCount(); ++resRow)
            {
                int culumn = static_cast<int>(OperatorHeader::Operation);
                QWidget *widget = table->cellWidget(resRow, culumn);
                dynamic_cast<XYCustomButton *>(widget)->setRow(resRow);
            }
        }

        return;
    }
}

void WidgetOrderPlanControl::slotMaterialReturn(const QString &stationID, const MaterialData &data)
{
    QTableWidget *table = ui->tableMaterial;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(MaterialHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(MaterialHeader::MaterialID));

        if (data.materialID != item->text())
            continue;

        item = table->item(row, static_cast<int>(MaterialHeader::MaterialBatch));
        item->setText(data.materialBatch);

        item = table->item(row, static_cast<int>(MaterialHeader::BatchQuantity));
        item->setText(QString::number(data.batchQuantity));

        return;
    }
}

void WidgetOrderPlanControl::slotOperatorLogout(const QString &stationID, const OperatorData &data)
{
    QTableWidget *table = ui->tableOperator;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(OperatorHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(OperatorHeader::OperatorID));

        if (data.operatorID != item->text())
            continue;

        int rowSpan = table->rowSpan(row, static_cast<int>(OperatorHeader::StationID));

        if (rowSpan == 1)
        {
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorID));
            item->setText(QString());
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorCard));
            item->setText(QString());
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorName));
            item->setText(QString());
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorTeam));
            item->setText(QString());
            item = table->item(row, static_cast<int>(OperatorHeader::OperatorDuty));
            item->setText(QString());
        }
        else
        {
            table->removeRow(row);

            for (int resRow = row; resRow < table->rowCount(); ++resRow)
            {
                int culumn = static_cast<int>(OperatorHeader::Operation);
                QWidget *widget = table->cellWidget(resRow, culumn);
                dynamic_cast<XYCustomButton *>(widget)->setRow(resRow);
            }
        }

        return;
    }
}

void WidgetOrderPlanControl::slotFixtureLoaded(const QString &stationID, const FixtureData &data)
{
    QTableWidget *table = ui->tableFixture;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(FixtureHeader::StationID));

        if (stationID != item->text())
            continue;

        item = table->item(row, static_cast<int>(FixtureHeader::FixtureClassify));

        if (data.fixtureClassify != item->text())
            continue;

        item = table->item(row, static_cast<int>(FixtureHeader::FixtureID));
        item->setText(data.fixtureID);

        item = table->item(row, static_cast<int>(FixtureHeader::FixtureName));
        item->setText(data.fixtureName);

        item = table->item(row, static_cast<int>(FixtureHeader::FixtureBarcode));
        item->setText(data.fixtureBarcode);

        item = table->item(row, static_cast<int>(FixtureHeader::TotalTimes));
        item->setText(QString::number(data.totalTimes));

        item = table->item(row, static_cast<int>(FixtureHeader::UsedTimes));
        item->setText(QString::number(data.usedTimes));

        item = table->item(row, static_cast<int>(FixtureHeader::RemindTimes));
        item->setText(QString::number(data.remindTimes));
    }
}

void WidgetOrderPlanControl::slotMaterialButtonClicked(int row, int index)
{
    QTableWidget *table = ui->tableMaterial;
    QTableWidgetItem *item = nullptr;

    if (index == 0)
    {
        DialogMaterialInput dialog(this);

        item = table->item(row, static_cast<int>(MaterialHeader::StationID));
        dialog.setStationID(item->text());
        item = table->item(row, static_cast<int>(MaterialHeader::MaterialID));
        dialog.setMaterialID(item->text());

        if (QDialog::Accepted != dialog.exec())
            return;

        materialInput(dialog.getStationID(),
                      dialog.getMaterialID(),
                      dialog.getMaterialBatch(),
                      dialog.getBatchQuantity());
    }

    if (index == 1)
    {
        item = table->item(row, static_cast<int>(MaterialHeader::StationID));
        QString stationID = item->text();
        item = table->item(row, static_cast<int>(MaterialHeader::MaterialID));
        QString materialID = item->text();

        materialReturn(stationID, materialID);
    }
}

void WidgetOrderPlanControl::slotOperatorButtonClicked(int row, int index)
{
    QTableWidget *table = ui->tableOperator;
    QTableWidgetItem *item = nullptr;

    if (index == 0)
    {
        DialogOperatorLogin dialog(this);

        item = table->item(row, static_cast<int>(OperatorHeader::StationID));
        dialog.setStationID(item->text());

        if (QDialog::Accepted != dialog.exec())
            return;

        operatorLogin(dialog.getStationID(),
                      dialog.getOperatorCard());
    }

    if (index == 1)
    {
        item = table->item(row, static_cast<int>(OperatorHeader::StationID));
        QString stationID = item->text();
        item = table->item(row, static_cast<int>(OperatorHeader::OperatorID));
        QString operatorID = item->text();

        operatorLogout(stationID, operatorID);
    }
}

void WidgetOrderPlanControl::slotOperatorLogin()
{
    DialogOperatorLogin dialog(this);

    if (QDialog::Accepted != dialog.exec())
        return;

    operatorLogin(dialog.getStationID(),
                  dialog.getOperatorCard());
}

void WidgetOrderPlanControl::slotMaterialInput()
{
    DialogMaterialInput dialog(this);

    if (QDialog::Accepted != dialog.exec())
        return;

    materialInput(dialog.getStationID(),
                  dialog.getMaterialID(),
                  dialog.getMaterialBatch(),
                  dialog.getBatchQuantity());
}

void WidgetOrderPlanControl::slotLogisticsFeed()
{
    DialogLogisticsFeed dialog(this);

    dialog.exec();
}

void WidgetOrderPlanControl::slotCustomLabelDblClicked(int row, int index)
{
    if (row == -1)
    {
        WorkOrder *order = qWorkManager->getOrder();

        if (order == nullptr)
            return;

        const OrderData &data = order->getOrderData();

        switch (static_cast<QualityAssure>(index))
        {
        case QualityAssure::FirstProduct:
        {
            if (data.isFirstProductQA)
                qWorkManager->browseDocument(SubmitDocType::FirstProductQADoc, data.firstProductQADocID);
            else
                qWorkManager->createDocument(SubmitDocType::FirstProductQADoc, QString());

            break;
        }

        case QualityAssure::FirstPackage:
        {
            if (data.isFirstPackageQA)
                qWorkManager->browseDocument(SubmitDocType::FirstPackageQADoc, data.firstPackageQADocID);
            else
                qWorkManager->createDocument(SubmitDocType::FirstPackageQADoc, QString());

            break;
        }

        case QualityAssure::FinalProduct:
        {
            if (data.isFinalProductQA)
                qWorkManager->browseDocument(SubmitDocType::FinalProductQADoc, data.finalProductQADocID);
            else
                qWorkManager->createDocument(SubmitDocType::FinalProductQADoc, QString());

            break;
        }

        case QualityAssure::FinalPackage:
        {
            if (data.isFinalPackageQA)
                qWorkManager->browseDocument(SubmitDocType::FinalPackageQADoc, data.finalPackageQADocID);
            else
                qWorkManager->createDocument(SubmitDocType::FinalPackageQADoc, QString());

            break;
        }
        }
    }
    else
    {
        QTableWidget *table = ui->tableStation;
        QTableWidgetItem *item = table->item(row, static_cast<int>(StationHeader::StationID));

        if (item == nullptr)
            return;

        WorkStation *station = qWorkManager->getStation(item->text());

        if (station == nullptr)
            return;

        const StationData &data = station->getStationData();

        switch (static_cast<StationHeader>(index))
        {
        case StationHeader::MachineCheck:
        {
            if (data.isMachineCheck)
                qWorkManager->browseDocument(SubmitDocType::MachineCheckDoc, data.machineCheckDocID);
            else
                qWorkManager->createDocument(SubmitDocType::MachineCheckDoc, data.stationID);

            break;
        }

        case StationHeader::ProductCheck:
        {
            if (data.isProductCheck)
                qWorkManager->browseDocument(SubmitDocType::ProductCheckDoc, data.productCheckDocID);
            else
                qWorkManager->createDocument(SubmitDocType::ProductCheckDoc, data.stationID);

            break;
        }

        case StationHeader::FixtureCheck:
        {
            if (data.isFixtureCheck)
                qWorkManager->browseDocument(SubmitDocType::FixtureCheckDoc, data.fixtureCheckDocID);
            else
                qWorkManager->createDocument(SubmitDocType::FixtureCheckDoc, data.stationID);

            break;
        }

        default:
            break;
        }
    }
}

void WidgetOrderPlanControl::materialInput(const QString &stationID, const QString &materialID, const QString &batch, int quantity)
{
    if (stationID.isEmpty())
    {
        MessageBox_Warning("请选择工位编号！");
        return;
    }

    if (materialID.isEmpty())
    {
        MessageBox_Warning("请选择物料编号！");
        return;
    }

    if (batch.isEmpty())
    {
        MessageBox_Warning("请输入物料批次！");
        return;
    }

    if (quantity == 0)
    {
        MessageBox_Warning("请输入批次数量！");
        return;
    }

    if (!qWorkManager->getStation(stationID)->materialInput(materialID, batch, quantity))
    {
        MessageBox_Critical("上料失败！");
        return;
    }
}

void WidgetOrderPlanControl::materialInput(const QString &stationID, const QString &barcode)
{
    if (stationID.isEmpty())
    {
        MessageBox_Warning("请选择工位编号！");
        return;
    }

    if (barcode.isEmpty())
    {
        MessageBox_Warning("请输入包装条码！");
        return;
    }

    if (barcode.split(';').size() < 4 || barcode.split(';').at(0).isEmpty() || barcode.split(';').at(3).isEmpty())
    {
        MessageBox_Warning("物料批次格式错误！");
        return;
    }

    do
    {
        QByteArray worklineID = qWorkManager->getWorklineID().toUtf8();
        QByteArray materialID = barcode.split('.').at(0).toUtf8();
        QByteArray packageBatch = barcode.split(';').at(0).toUtf8();
        QByteArray materialBatch = barcode.split(';').at(3).toUtf8();

        qDebug() << "[MESServiceSoapProxy] EXEC LineInventory success.";
    } while (0);

    if (!qWorkManager->getStation(stationID)->materialInput(barcode))
    {
        MessageBox_Critical("上料失败！");
        return;
    }
}

void WidgetOrderPlanControl::operatorLogin(const QString &stationID, const QString &barcode)
{
    if (stationID.isEmpty())
    {
        MessageBox_Warning("请选择工位编号！");
        return;
    }

    if (barcode.isEmpty())
    {
        MessageBox_Warning("请输入员工卡号！");
        return;
    }

    if (!qWorkManager->getStation(stationID)->operatorLogin(barcode))
    {
        MessageBox_Critical("登陆失败！");
        return;
    }
}

void WidgetOrderPlanControl::materialReturn(const QString &stationID, const QString &materialID)
{
    if (stationID.isEmpty() || materialID.isEmpty())
        return;

    qWorkManager->getStation(stationID)->materialReturn(materialID);
}

void WidgetOrderPlanControl::operatorLogout(const QString &stationID, const QString &operatorID)
{
    if (stationID.isEmpty() || operatorID.isEmpty())
        return;

    qWorkManager->getStation(stationID)->operatorLogout(operatorID);
}
