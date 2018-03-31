#include "WidgetAssemblyControl.h"
#include "ui_WidgetAssemblyControl.h"
#include "DialogPowerLogin.h"
#include "PackageStation.h"
#include "WorkCraft.h"
#include "WorkOrder.h"

WidgetAssemblyControl::WidgetAssemblyControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAssemblyControl)
    , station(nullptr)
{
    ui->setupUi(this);

    initialWidget();
}

WidgetAssemblyControl::~WidgetAssemblyControl()
{
    delete ui;
}

void WidgetAssemblyControl::initialWidget()
{
    do
    {
        assemblyHeaderMap.insert(AssemblyHeader::AssemblyBarcode, tr("总成条码"));
        assemblyHeaderMap.insert(AssemblyHeader::PackageBarcode, tr("包装箱码"));
        assemblyHeaderMap.insert(AssemblyHeader::RepairFlag, tr("返修标识"));
        assemblyHeaderMap.insert(AssemblyHeader::SampleFlag, tr("抽检标识"));
        assemblyHeaderMap.insert(AssemblyHeader::CreateTime, tr("开始时间"));
        assemblyHeaderMap.insert(AssemblyHeader::FinishTime, tr("完成时间"));

        QTableWidget *table = ui->tableAssembly;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(assemblyHeaderMap.size());
        table->setHorizontalHeaderLabels(assemblyHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    while (0);

    do
    {
        packageHeaderMap.insert(PackageHeader::PackageBarcode, tr("包装箱码"));
        packageHeaderMap.insert(PackageHeader::PackageWeight, tr("包装重量"));
        packageHeaderMap.insert(PackageHeader::PackageCount, tr("包装数量"));
        packageHeaderMap.insert(PackageHeader::CreateTime, tr("开始时间"));
        packageHeaderMap.insert(PackageHeader::FinishTime, tr("完成时间"));

        QTableWidget *table = ui->tablePackage;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(packageHeaderMap.size());
        table->setHorizontalHeaderLabels(packageHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    while (0);

    do
    {
        connect(ui->editScanTransfer, SIGNAL(returnPressed()), this, SLOT(slotScanTransferBarcode()));
        connect(ui->editRescanAssembly, SIGNAL(returnPressed()), this, SLOT(slotRescanAssmelbyBarcode()));

        connect(ui->editScanMarker, SIGNAL(returnPressed()), this, SLOT(slotScanPackageMarker()));
        connect(ui->editRescanPackage, SIGNAL(returnPressed()), this, SLOT(slotRescanPackageBarcode()));

        connect(ui->buttonReprintAssembly, SIGNAL(clicked()), this, SLOT(slotReprintAssemblyBarcode()));
        connect(ui->buttonReprintPackage, SIGNAL(clicked()), this, SLOT(slotReprintPackageBarcode()));

        connect(ui->buttonScaleMeasure, SIGNAL(clicked()), this, SLOT(slotScaleZero()));
        connect(ui->buttonForcePackage, SIGNAL(clicked()), this, SLOT(slotForcePackage()));
    }
    while (0);

    do
    {
        checkedPixmap = QPixmap(":/images/pngs/checkbox_full.png")
                        .scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        uncheckPixmap = QPixmap(":/images/pngs/checkbox_empty.png")
                        .scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->tabWidget->setCurrentIndex(0);

        ui->valuePackageWeight->setNum(0);
        ui->valuePackageCount->setNum(0);
        ui->valueAssemblyCount->setNum(0);

        ui->editPrintAssembly->setEnabled(false);
        ui->editPrintPackage->setEnabled(false);
    }
    while (0);

    do
    {
        if (qWorkManager->isWorkActivated())
        {
            slotOrderActivated();
        }
        else
        {
            slotOrderSuspended();
        }
    }
    while (0);
}

void WidgetAssemblyControl::slotOrderActivated()
{
    setScanResult(tr("工单计划已激活。"), true);

    ui->buttonReprintAssembly->setEnabled(true);
    ui->buttonReprintPackage->setEnabled(true);
    ui->buttonScaleMeasure->setEnabled(true);
    ui->buttonForcePackage->setEnabled(true);

    station = qWorkManager->getPackageStation();

    slotScaleRead();

    slotStartProcess();
}

void WidgetAssemblyControl::slotOrderSuspended()
{
    setScanResult(tr("工单计划未激活！"), false);
    setScanRemind(tr("请激活工单计划！"));

    ui->editScanTransfer->clear();
    ui->editScanMarker->clear();
    ui->editRescanAssembly->clear();
    ui->editRescanPackage->clear();
    ui->editPrintAssembly->clear();
    ui->editPrintPackage->clear();

    ui->editScanTransfer->setEnabled(false);
    ui->editScanMarker->setEnabled(false);
    ui->editRescanAssembly->setEnabled(false);
    ui->editRescanPackage->setEnabled(false);

    ui->buttonReprintAssembly->setEnabled(false);
    ui->buttonReprintPackage->setEnabled(false);
    ui->buttonScaleMeasure->setEnabled(false);
    ui->buttonForcePackage->setEnabled(false);

    ui->valuePackageCount->setNum(0);
    ui->valueAssemblyCount->setNum(0);

    ui->tableAssembly->clearContents();
    ui->tableAssembly->setRowCount(0);
    ui->tablePackage->clearContents();
    ui->tablePackage->setRowCount(0);

    slotScaleReset();

    station = nullptr;
}

void WidgetAssemblyControl::setPackageState(int state)
{
    switch (state)
    {
    case -1:
        ui->imgPackageStatus->setPixmap(QPixmap(":/images/pngs/circle_grey.png"));
        break;

    case 0:
        ui->imgPackageStatus->setPixmap(QPixmap(":/images/pngs/circle_green.png"));
        break;

    case 1:
        ui->imgPackageStatus->setPixmap(QPixmap(":/images/pngs/circle_yellow.png"));
        break;

    case 2:
        ui->imgPackageStatus->setPixmap(QPixmap(":/images/pngs/circle_red.png"));
        break;
    }
}

void WidgetAssemblyControl::setScanResult(const QString &text, bool ok)
{
    QPalette palette;
    QColor color = ok ? Qt::darkGreen : Qt::red;
    palette.setColor(QPalette::WindowText, color);
    ui->labelScanResult->setPalette(palette);
    ui->labelScanResult->setText(text);
}

void WidgetAssemblyControl::setScanRemind(const QString &text)
{
    QPalette palette;
    QColor color = Qt::blue;
    palette.setColor(QPalette::WindowText, color);
    ui->labelScanRemind->setPalette(palette);
    ui->labelScanRemind->setText(text);
}

//*******************包装及打印流程************************//
void WidgetAssemblyControl::packageProcess(PackageStep step, const QString &barcode)
{
    switch (step)
    {
    case PackageStep::LockPackageStation:    //包装工位锁定
    {
        setScanRemind(tr("包装工位被锁定！"));

        ui->editScanTransfer->setEnabled(false);
        ui->editRescanAssembly->setEnabled(false);
        ui->editScanMarker->setEnabled(false);
        ui->editRescanPackage->setEnabled(false);
        break;
    }

    case PackageStep::PrepareScanMarker:    //准备扫描包装箱标识
    {
        setScanRemind(tr("请扫描包装箱标识码！"));

        ui->editScanMarker->setEnabled(true);
        ui->editScanMarker->clear();
        ui->editScanMarker->setFocus();

        ui->editScanTransfer->setEnabled(false);
        ui->editRescanAssembly->setEnabled(false);
        ui->editRescanPackage->setEnabled(false);
        break;
    }

    case PackageStep::ScanPackageMarker:    //扫描包装箱标识
    {
        switch (station->verifyPackageMarker(barcode))
        {
        case EventState::E_ScanMark_Successfully:
            setScanResult(tr("包装箱标识码：%1\n扫描成功，允许总成打印。").arg(barcode), true);
            packageProcess(PackageStep::PrepareScanTransfer);
            break;

        case EventState::E_ScanMark_ContentMismatch:
            setScanResult(tr("包装箱标识码：%1\n扫描失败，条码内容有误！").arg(barcode), false);
            packageProcess(PackageStep::PrepareScanMarker);
            break;

        default:
            break;
        }

        break;
    }

    case PackageStep::PrepareScanTransfer:    //准备扫描过渡条码
    {
        setScanRemind(tr("请扫描过渡条码！"));

        ui->editScanTransfer->setEnabled(true);
        ui->editScanTransfer->clear();
        ui->editScanTransfer->setFocus();

        ui->editRescanAssembly->setEnabled(false);
        ui->editScanMarker->setEnabled(false);
        ui->editRescanPackage->setEnabled(false);
        break;
    }

    case PackageStep::ScanTransferBarcode:    //扫描过渡条码
    {
        switch (station->verifyTransferBarcode(barcode))
        {
        case EventState::E_ScanTbar_Successfully:
            setScanResult(tr("过渡条码：%1\n扫描成功，打印总成条码。").arg(barcode), true);
            packageProcess(PackageStep::PrintAssemblyBarcode);
            break;

        case EventState::E_ScanTbar_PrevStationNonData:
            setScanResult(tr("过渡条码：%1\n扫描失败，上工位无相关数据！").arg(barcode), false);
            packageProcess(PackageStep::PrepareScanTransfer);
            break;

        case EventState::E_ScanTbar_ThisStationHasData:
            setScanResult(tr("过渡条码：%1\n扫描失败，本工位有相关数据！").arg(barcode), false);
            packageProcess(PackageStep::PrepareScanTransfer);
            break;

        default:
            break;
        }

        break;
    }

    case PackageStep::PrintAssemblyBarcode:    //打印总成条码
    {
        QString barcode = station->printAssemblyBarcode();
        ui->editPrintAssembly->setText(barcode);

        if (station->isAssyEnableRescan())
        {
            setScanResult(tr("总成条码：%1\n打印成功，需要复扫条码。").arg(barcode), true);
            packageProcess(PackageStep::PrepareRescanAssembly);
        }
        else
        {
            setScanResult(tr("总成条码：%1\n打印成功，保存总成数据。").arg(barcode), true);
            packageProcess(PackageStep::SaveAssemblyData);
        }

        break;
    }

    case PackageStep::PrepareRescanAssembly:    //准备复扫总成条码
    {
        setScanRemind(tr("请复扫总成条码！"));

        ui->editRescanAssembly->setEnabled(true);
        ui->editRescanAssembly->clear();
        ui->editRescanAssembly->setFocus();

        ui->editScanTransfer->setEnabled(false);
        ui->editScanMarker->setEnabled(false);
        ui->editRescanPackage->setEnabled(false);
        break;
    }

    case PackageStep::RescanAssemblyBarcode:    //复扫总成条码
    {
        switch (station->verifyAssemblyBarcode(barcode))
        {
        case EventState::E_ScanAbar_Successfully:
            setScanResult(tr("总成条码：%1\n复扫成功，保存总成数据。").arg(barcode), true);
            packageProcess(PackageStep::SaveAssemblyData);      //继续复扫总成
            break;

        case EventState::E_ScanAbar_ContentMismatch:
            setScanResult(tr("总成条码：%1\n复扫失败，条码内容不匹配！").arg(barcode), false);
            packageProcess(PackageStep::PrepareRescanAssembly); //继续复扫总成
            break;

        case EventState::E_ScanAbar_HistoryHasData:
            setScanResult(tr("总成条码：%1\n复扫失败，数据库存在重复条码！").arg(barcode), false);
            packageProcess(PackageStep::LockPackageStation);    //包装工位锁定
            break;

        default:
            break;
        }

        break;
    }

    case PackageStep::SaveAssemblyData:    //保存总成数据
    {
        station->saveAssemblyData();
        int count = station->getAssemblyCount();
        ui->valueAssemblyCount->setNum(count);

        ui->editScanTransfer->clear();
        ui->editPrintAssembly->clear();
        ui->editRescanAssembly->clear();

        if (station->isAssemblySample())    //判断是否为抽检件
        {
            DialogPowerLogin dialog;
            dialog.hideClose(true);
            dialog.setTitle(tr("请取出抽检件！"));
            dialog.exec();

            packageProcess(PackageStep::PrepareScanTransfer);    //继续扫描过渡
        }

        if (station->isPackEnable() && count == station->getPackageNumber())
        {
            packageProcess(PackageStep::PutAssemblyInPackage);    //总成放入箱中
        }
        else
        {
            packageProcess(PackageStep::PrepareScanTransfer);    //继续扫描过渡
        }

        break;
    }

    case PackageStep::PutAssemblyInPackage:
    {
        setScanRemind(tr("请将总成放入箱中！"));

        ui->editScanTransfer->setEnabled(false);
        ui->editRescanAssembly->setEnabled(false);
        ui->editScanMarker->setEnabled(false);
        ui->editRescanPackage->setEnabled(false);
        break;
    }

    case PackageStep::PrintPackageBarcode:    //打印包装箱码
    {
        QString barcode = station->printPackageBarcode();
        ui->editPrintPackage->setText(barcode);

        if (station->isPackEnableRescan())
        {
            setScanResult(tr("包装箱码：%1\n打印成功，需要复扫条码。").arg(barcode), true);
            packageProcess(PackageStep::PrepareRescanPackage);  //进入包装复扫
        }
        else
        {
            setScanResult(tr("包装箱码：%1\n打印成功，保存包装数据。").arg(barcode), true);
            packageProcess(PackageStep::SavePackageData);       //保存包装数据
        }

        break;
    }

    case PackageStep::PrepareRescanPackage:    //准备复扫包装箱码
    {
        setScanRemind(tr("请复扫包装箱码！"));

        ui->editRescanPackage->setEnabled(true);
        ui->editRescanPackage->clear();
        ui->editRescanPackage->setFocus();

        ui->editScanTransfer->setEnabled(false);
        ui->editRescanAssembly->setEnabled(false);
        ui->editScanMarker->setEnabled(false);
        break;
    }

    case PackageStep::RescanPackageBarcode:    //复扫包装箱码
    {
        switch (station->verifyPackageBarcode(barcode))
        {
        case EventState::E_ScanPbar_Successfully:
            setScanResult(tr("包装箱码：%1\n复扫成功，保存包装数据。").arg(barcode), true);
            packageProcess(PackageStep::SavePackageData);    //保存包装数据
            break;

        case EventState::E_ScanPbar_ContentMismatch:
            setScanResult(tr("包装箱码：%1\n复扫失败，条码内容不匹配！").arg(barcode), false);
            packageProcess(PackageStep::PrepareRescanPackage);    //继续复扫箱码
            break;

        case EventState::E_ScanPbar_HistoryHasData:
            setScanResult(tr("包装箱码：%1\n复扫失败，数据库存在重复条码！").arg(barcode), false);
            packageProcess(PackageStep::LockPackageStation);    //包装工位锁定
            break;

        default:
            break;
        }

        break;
    }

    case PackageStep::SavePackageData:    //保存包装数据
    {
        station->savePackageData();       //保存生产数据
        int count = station->getAssemblyCount();
        ui->valueAssemblyCount->setNum(count);

        ui->editScanMarker->clear();
        ui->editPrintPackage->clear();
        ui->editRescanPackage->clear();

        packageProcess(PackageStep::PrepareScanMarker);    //扫描包装箱标识

        break;
    }
    }
}

//*******************扫描过渡条码************************//
void WidgetAssemblyControl::slotScanTransferBarcode()
{
    packageProcess(PackageStep::ScanTransferBarcode, ui->editScanTransfer->text());
}

//*******************复扫总成条码************************//
void WidgetAssemblyControl::slotRescanAssmelbyBarcode()
{
    packageProcess(PackageStep::RescanAssemblyBarcode, ui->editRescanAssembly->text());
}

//*******************扫描包装箱标识************************//
void WidgetAssemblyControl::slotScanPackageMarker()
{
    packageProcess(PackageStep::ScanPackageMarker, ui->editScanMarker->text());
}

//*******************复扫包装箱码************************//
void WidgetAssemblyControl::slotRescanPackageBarcode()
{
    packageProcess(PackageStep::RescanPackageBarcode, ui->editRescanPackage->text());
}

//*******************补打总成条码************************//
void WidgetAssemblyControl::slotReprintAssemblyBarcode()
{
    if (station)
        station->reprintAssemblyBarcode();
}

//*******************补打包装条码************************//
void WidgetAssemblyControl::slotReprintPackageBarcode()
{
    if (station)
        station->reprintPackageBarcode();
}

void WidgetAssemblyControl::slotScaleReset()
{
    if (station)
        station->scaleReset();
}

void WidgetAssemblyControl::slotScaleRead()
{
    if (station)
        station->scaleRead();
}

//*******************电子秤采集************************//
void WidgetAssemblyControl::slotScaleZero()
{
    if (station)
        station->scaleZero();
}

void WidgetAssemblyControl::slotStartProcess()
{
    if (!station)
        return;

    if (station->isPackEnable())
    {
        packageProcess(PackageStep::PrepareScanMarker);
    }
    else if (station->isAssyEnable())
    {
        packageProcess(PackageStep::PrepareScanTransfer);
    }
    else
    {
        packageProcess(PackageStep::LockPackageStation);
    }
}

//*******************强制装箱************************//
void WidgetAssemblyControl::slotForcePackage()
{
    if (!station)
        return;

    if (!station->isPackEnable())
        return;

    if (station->getAssemblyCount() == 0)
        return;

    if (station->getAssemblyCount() == station->getPackageCount())
    {
        setScanResult(tr("包装数量不足，强制打印包装箱码！"), true);
        packageProcess(PackageStep::PrintPackageBarcode);
    }
    else
    {
        setScanResult(tr("包装数量与总成数量不等，请继续装箱！"), false);
        packageProcess(PackageStep::PutAssemblyInPackage);
    }
}

//*******************包装重量更新************************//
void WidgetAssemblyControl::slotPackageUpdate(const PackageData &data)
{
    ui->valuePackageWeight->setNum(data.packageWeight);
    ui->valuePackageCount->setNum(data.packageCount);

    setPackageState(data.packageState);

    if (!station)
        return;

    if (!station->isPackEnable())
        return;

    if (station->getAssemblyCount() != station->getPackageNumber())
        return;

    if (station->getPackageCount() == station->getPackageNumber())
    {
        setScanResult(tr("包装数量满足，打印包装箱码。"), true);
        packageProcess(PackageStep::PrintPackageBarcode);
    }
    else
    {
        setScanResult(tr("包装数量不足，请继续装箱！"), false);
        packageProcess(PackageStep::PutAssemblyInPackage);
    }
}

//*******************总成装配完成************************//
void WidgetAssemblyControl::slotAssemblyFinish(const AssemblyData &data)
{
    do
    {
        QTableWidget *table = ui->tableAssembly;
        QTableWidgetItem *item = nullptr;
        QLabel *label = nullptr;
        int row = table->rowCount();
        table->insertRow(row);

        item = new QTableWidgetItem(data.assemblyBarcode);
        item->setData(Qt::UserRole, data.assemblyID);
        table->setItem(row, static_cast<int>(AssemblyHeader::AssemblyBarcode), item);

        item = new QTableWidgetItem;
        table->setItem(row, static_cast<int>(AssemblyHeader::PackageBarcode), item);

        label = new QLabel;
        label->setAlignment(Qt::AlignCenter);
        label->setPixmap(data.repairState ? checkedPixmap : uncheckPixmap);
        table->setCellWidget(row, static_cast<int>(AssemblyHeader::RepairFlag), label);

        label = new QLabel;
        label->setAlignment(Qt::AlignCenter);
        label->setPixmap(data.sampleState ? checkedPixmap : uncheckPixmap);
        table->setCellWidget(row, static_cast<int>(AssemblyHeader::SampleFlag), label);

        item = new QTableWidgetItem(data.createTime.toString("yyyy-MM-dd hh:mm:ss"));
        table->setItem(row, static_cast<int>(AssemblyHeader::CreateTime), item);

        item = new QTableWidgetItem(data.finishTime.toString("yyyy-MM-dd hh:mm:ss"));
        table->setItem(row, static_cast<int>(AssemblyHeader::FinishTime), item);

        table->selectRow(row);
    }
    while (0);
}

//*******************总成装箱完成************************//
void WidgetAssemblyControl::slotPackageFinish(const PackageData &data)
{
    do
    {
        QTableWidget *table = ui->tableAssembly;
        QTableWidgetItem *item = nullptr;
        int rowCount = table->rowCount();
        int rowBegin = rowCount - data.packageCount;

        for (int row = rowBegin; row < rowCount; ++row)
        {
            item = table->item(row, static_cast<int>(AssemblyHeader::AssemblyBarcode));

            if (!data.assemblyIDList.contains(item->data(Qt::UserRole).toString()))
                continue;

            item = table->item(row, static_cast<int>(AssemblyHeader::PackageBarcode));
            item->setText(data.packageBarcode);
            item->setData(Qt::UserRole, data.packageID);
        };
    }
    while (0);

    do
    {
        QTableWidget *table = ui->tablePackage;
        QTableWidgetItem *item = nullptr;
        int row = table->rowCount();
        table->insertRow(row);

        item = new QTableWidgetItem(data.packageBarcode);
        item->setData(Qt::UserRole, data.packageID);
        table->setItem(row, static_cast<int>(PackageHeader::PackageBarcode), item);

        item = new QTableWidgetItem(QString::number(data.packageWeight));
        table->setItem(row, static_cast<int>(PackageHeader::PackageWeight), item);

        item = new QTableWidgetItem(QString::number(data.packageCount));
        table->setItem(row, static_cast<int>(PackageHeader::PackageCount), item);

        item = new QTableWidgetItem(data.createTime.toString("yyyy-MM-dd hh:mm:ss"));
        table->setItem(row, static_cast<int>(PackageHeader::CreateTime), item);

        item = new QTableWidgetItem(data.finishTime.toString("yyyy-MM-dd hh:mm:ss"));
        table->setItem(row, static_cast<int>(PackageHeader::FinishTime), item);

        table->selectRow(row);
    }
    while (0);
}
