#include "WidgetAssyProcessStation.h"
#include "CommManager.h"
#include "DialogProcessStepData.h"
#include "DialogStepMeasureData.h"
#include "DialogStepScanBarcode.h"
#include "OPCManager.h"
#include "ui_WidgetAssyProcessStation.h"

WidgetAssyProcessStation::WidgetAssyProcessStation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAssyProcessStation)
{
    ui->setupUi(this);

    initialWidget();
    ui->checkFixtureCheck->hide();
    ui->checkMachineCheck->hide();
    ui->checkProductCheck->hide();
}

WidgetAssyProcessStation::~WidgetAssyProcessStation()
{
    delete ui;
}

void WidgetAssyProcessStation::initialWidget()
{
    do
    {
        stepHeaderMap.insert(StepHeader::IsDisabled, tr("是否跳过"));
        stepHeaderMap.insert(StepHeader::Interaction, tr("交互类型"));
        stepHeaderMap.insert(StepHeader::Description, tr("步骤描述"));
        stepHeaderMap.insert(StepHeader::MaxMeasure, tr("采集次数"));
        stepHeaderMap.insert(StepHeader::StartRequestItemID, tr("PLC启动请求地址"));
        stepHeaderMap.insert(StepHeader::StartRequestValue, tr("请求值"));
        stepHeaderMap.insert(StepHeader::StartResponseItemID, tr("PC启动反馈地址"));
        stepHeaderMap.insert(StepHeader::StartResponseValue, tr("反馈值"));
        stepHeaderMap.insert(StepHeader::SkipResponseItemID, tr("PC跳过反馈地址"));
        stepHeaderMap.insert(StepHeader::SkipResponseValue, tr("反馈值"));
        stepHeaderMap.insert(StepHeader::MeasureRequestItemID, tr("PLC采集请求地址"));
        stepHeaderMap.insert(StepHeader::MeasureRequestValue, tr("请求值"));
        stepHeaderMap.insert(StepHeader::SuccessResponseItemID, tr("PC成功反馈地址"));
        stepHeaderMap.insert(StepHeader::SuccessResponseValue, tr("反馈值"));
        stepHeaderMap.insert(StepHeader::FailedResponseItemID, tr("PC成功反馈地址"));
        stepHeaderMap.insert(StepHeader::FailedResponseValue, tr("反馈值"));

        QTableWidget *table = ui->tableStep;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(stepHeaderMap.size());
        table->setHorizontalHeaderLabels(stepHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(60);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(slotStepItemSelectionChanged()));
        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotStepItemDoubleClicked()));
    } while (0);

    do
    {
        barcodeHeaderMap.insert(BarcodeHeader::IsDisabled, tr("是否屏蔽"));
        barcodeHeaderMap.insert(BarcodeHeader::BarcodeType, tr("条码类型"));
        barcodeHeaderMap.insert(BarcodeHeader::BarcodeID, tr("物料或工位编号"));
        barcodeHeaderMap.insert(BarcodeHeader::ReadBarcode, tr("采集设备或读取地址"));
        barcodeHeaderMap.insert(BarcodeHeader::WriteDisplay, tr("条码内容写入地址"));
        barcodeHeaderMap.insert(BarcodeHeader::WriteResult, tr("扫描结果写入地址"));
        barcodeHeaderMap.insert(BarcodeHeader::ResultOK, tr("OK写入值"));
        barcodeHeaderMap.insert(BarcodeHeader::ResultNG, tr("NG写入值"));

        QTableWidget *table = ui->tableBarcode;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(barcodeHeaderMap.size());
        table->setHorizontalHeaderLabels(barcodeHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(60);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotBarcodeItemDoubleClicked()));
    } while (0);

    do
    {
        measureHeaderMap.insert(MeasureHeader::IsDisabled, tr("是否屏蔽"));
        measureHeaderMap.insert(MeasureHeader::MeasureID, tr("采集编号"));
        measureHeaderMap.insert(MeasureHeader::OffsetValue, tr("偏差值"));
        measureHeaderMap.insert(MeasureHeader::ReadMeasure, tr("采集设备或读取地址"));
        measureHeaderMap.insert(MeasureHeader::WriteDisplay, tr("采集数值写入地址"));
        measureHeaderMap.insert(MeasureHeader::WriteResult, tr("判断结果写入地址"));
        measureHeaderMap.insert(MeasureHeader::ResultOK, tr("OK写入值"));
        measureHeaderMap.insert(MeasureHeader::ResultNG, tr("NG写入值"));

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
        table->horizontalHeader()->setMinimumSectionSize(60);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotMeasureItemDoubleClicked()));
    } while (0);

    do
    {
        ui->toolStationParam->setChecked(true);
        ui->stackedWidget_1->setCurrentIndex(0);
        ui->stackedWidget_2->setCurrentIndex(0);

        QButtonGroup *buttonGroup = new QButtonGroup(this);
        buttonGroup->addButton(ui->toolStationParam, 0);
        buttonGroup->addButton(ui->toolProcessParam, 1);

        connect(buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget_1, SLOT(setCurrentIndex(int)));

        connect(ui->buttonSaveParam, SIGNAL(clicked()), this, SLOT(saveParameter()));

        connect(ui->buttonSave_1, SIGNAL(clicked()), this, SLOT(slotSaveLoopStart()));
        connect(ui->buttonSave_2, SIGNAL(clicked()), this, SLOT(slotSaveLoopEnd()));
        connect(ui->buttonSave_3, SIGNAL(clicked()), this, SLOT(slotSaveAutoMotion()));

        connect(ui->buttonAddnew, SIGNAL(clicked()), this, SLOT(slotAddnewStepItem()));
        connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(slotDeleteStepItem()));
        connect(ui->buttonMoveup, SIGNAL(clicked()), this, SLOT(slotMoveupStepItem()));
        connect(ui->buttonMovedown, SIGNAL(clicked()), this, SLOT(slotMovedownStepItem()));

        connect(ui->buttonAddnew_1, SIGNAL(clicked()), this, SLOT(slotAddnewBarcodeItem()));
        connect(ui->buttonDelete_1, SIGNAL(clicked()), this, SLOT(slotDeleteBarcodeItem()));
        connect(ui->buttonMoveup_1, SIGNAL(clicked()), this, SLOT(slotMoveupBarcodeItem()));
        connect(ui->buttonMovedown_1, SIGNAL(clicked()), this, SLOT(slotMovedownBarcodeItem()));

        connect(ui->buttonAddnew_2, SIGNAL(clicked()), this, SLOT(slotAddnewMeasureItem()));
        connect(ui->buttonDelete_2, SIGNAL(clicked()), this, SLOT(slotDeleteMeasureItem()));
        connect(ui->buttonMoveup_2, SIGNAL(clicked()), this, SLOT(slotMoveupMeasureItem()));
        connect(ui->buttonMovedown_2, SIGNAL(clicked()), this, SLOT(slotMovedownMeasureItem()));
    } while (0);
}

void WidgetAssyProcessStation::setWidgetData()
{
    do
    {
        ui->checkOperatorLogin->setChecked(preparePara.enableOperatorLogin);
        ui->checkMaterialInput->setChecked(preparePara.enableMaterialInput);
        ui->checkFixtureCheck->setChecked(preparePara.enableFixtureCheck);
        ui->checkMachineCheck->setChecked(preparePara.enableMachineCheck);
        ui->checkProductCheck->setChecked(preparePara.enableProductCheck);
    } while (0);

    do
    {
        ui->spinFormulaNumber->setValue(processPara.formulaNumber);
        ui->spinProcessNumber->setValue(processPara.processNumber);

        ui->comboWriteProductID->setCurrentText(processPara.writeProductID.itemID);
        ui->comboWriteProductBatch->setCurrentText(processPara.writeProductBatch.itemID);
        ui->comboWriteFinishQuantity->setCurrentText(processPara.writeFinishQuantity.itemID);
        ui->comboWriteFailedQuantity->setCurrentText(processPara.writeFailedQuantity.itemID);
        ui->comboWriteFormulaNumber->setCurrentText(processPara.writeFormulaNumber.itemID);
        ui->comboWriteProcessPrompt->setCurrentText(processPara.writeProcessPrompt.itemID);

        ui->comboResetSignal->setCurrentText(processPara.resetSignal.itemID);
        ui->editResetSignal->setText(processPara.resetSignal.itemValue.toString());
        ui->comboAlarmSignal->setCurrentText(processPara.alarmSignal.itemID);
        ui->editAlarmSignal->setText(processPara.alarmSignal.itemValue.toString());
    } while (0);

    do
    {
        QTableWidget *table = ui->tableStep;
        QTableWidgetItem *item = nullptr;
        int rowCount = stepParaList.size();
        table->setRowCount(rowCount);

        for (int row = 0; row < rowCount; ++row)
        {
            const StepItem &it = stepParaList.at(row).stepItem;

            item = new QTableWidgetItem(it.isDisabled ? tr("跳过") : tr("启用"));
            item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(StepHeader::IsDisabled), item);

            item = new QTableWidgetItem(it.interaction);
            table->setItem(row, static_cast<int>(StepHeader::Interaction), item);
            item = new QTableWidgetItem(it.description);
            table->setItem(row, static_cast<int>(StepHeader::Description), item);
            item = new QTableWidgetItem(QString::number(it.maxMeasure));
            table->setItem(row, static_cast<int>(StepHeader::MaxMeasure), item);

            item = new QTableWidgetItem(it.startRequest.itemID);
            table->setItem(row, static_cast<int>(StepHeader::StartRequestItemID), item);
            item = new QTableWidgetItem(it.startRequest.itemValue.toString());
            table->setItem(row, static_cast<int>(StepHeader::StartRequestValue), item);

            item = new QTableWidgetItem(it.startResponse.itemID);
            table->setItem(row, static_cast<int>(StepHeader::StartResponseItemID), item);
            item = new QTableWidgetItem(it.startResponse.itemValue.toString());
            table->setItem(row, static_cast<int>(StepHeader::StartResponseValue), item);

            item = new QTableWidgetItem(it.skipResponse.itemID);
            table->setItem(row, static_cast<int>(StepHeader::SkipResponseItemID), item);
            item = new QTableWidgetItem(it.skipResponse.itemValue.toString());
            table->setItem(row, static_cast<int>(StepHeader::SkipResponseValue), item);

            item = new QTableWidgetItem(it.measureRequest.itemID);
            table->setItem(row, static_cast<int>(StepHeader::MeasureRequestItemID), item);
            item = new QTableWidgetItem(it.measureRequest.itemValue.toString());
            table->setItem(row, static_cast<int>(StepHeader::MeasureRequestValue), item);

            item = new QTableWidgetItem(it.successResponse.itemID);
            table->setItem(row, static_cast<int>(StepHeader::SuccessResponseItemID), item);
            item = new QTableWidgetItem(it.successResponse.itemValue.toString());
            table->setItem(row, static_cast<int>(StepHeader::SuccessResponseValue), item);

            item = new QTableWidgetItem(it.failedResponse.itemID);
            table->setItem(row, static_cast<int>(StepHeader::FailedResponseItemID), item);
            item = new QTableWidgetItem(it.failedResponse.itemValue.toString());
            table->setItem(row, static_cast<int>(StepHeader::FailedResponseValue), item);
        }
    } while (0);

    do
    {
        ui->stackedWidget_2->setCurrentIndex(0);
    } while (0);
}

void WidgetAssyProcessStation::getWidgetData()
{
    do
    {
        preparePara.enableOperatorLogin = ui->checkOperatorLogin->isChecked();
        preparePara.enableMaterialInput = ui->checkMaterialInput->isChecked();
        preparePara.enableFixtureCheck = ui->checkFixtureCheck->isChecked();
        preparePara.enableMachineCheck = ui->checkMachineCheck->isChecked();
        preparePara.enableProductCheck = ui->checkProductCheck->isChecked();
    } while (0);

    do
    {
        processPara.formulaNumber = ui->spinFormulaNumber->value();
        processPara.processNumber = ui->spinProcessNumber->value();

        processPara.writeProductID.itemID = ui->comboWriteProductID->currentText();
        processPara.writeProductBatch.itemID = ui->comboWriteProductBatch->currentText();
        processPara.writeFinishQuantity.itemID = ui->comboWriteFinishQuantity->currentText();
        processPara.writeFailedQuantity.itemID = ui->comboWriteFailedQuantity->currentText();
        processPara.writeFormulaNumber.itemID = ui->comboWriteFormulaNumber->currentText();
        processPara.writeProcessPrompt.itemID = ui->comboWriteProcessPrompt->currentText();

        processPara.resetSignal.itemID = ui->comboResetSignal->currentText();
        processPara.resetSignal.itemValue = ui->editResetSignal->text();
        processPara.alarmSignal.itemID = ui->comboAlarmSignal->currentText();
        processPara.alarmSignal.itemValue = ui->editAlarmSignal->text();
    } while (0);
}

void WidgetAssyProcessStation::loadParameter(const QString &craftID, const QString &stationID)
{
    this->craftID = craftID;
    this->stationID = stationID;

    do
    {
        ui->stackedWidget_2->setCurrentIndex(0);

        ui->comboWriteProductID->clear();
        ui->comboWriteProductBatch->clear();
        ui->comboWriteFinishQuantity->clear();
        ui->comboWriteFailedQuantity->clear();
        ui->comboWriteFormulaNumber->clear();
        ui->comboWriteProcessPrompt->clear();

        ui->comboResetSignal->clear();
        ui->comboAlarmSignal->clear();

        ui->tableStep->clearContents();
        ui->tableStep->setRowCount(0);
        ui->tableBarcode->clearContents();
        ui->tableBarcode->setRowCount(0);
        ui->tableMeasure->clearContents();
        ui->tableMeasure->setRowCount(0);
    } while (0);

    do
    {
        stationIDList = qWorkManager->getStationIDList();
        materialIDList = qWorkManager->getMaterialIDList(craftID, stationID);
        measureIDList = qWorkManager->getMeasureIDList(craftID, stationID);
        dacItemIDList = qCommManager->getItemIDList(stationID);
        opcItemIDList = qOPCManager->getItemIDList(stationID);

        ui->comboWriteProductID->addItems(opcItemIDList);
        ui->comboWriteProductBatch->addItems(opcItemIDList);
        ui->comboWriteFinishQuantity->addItems(opcItemIDList);
        ui->comboWriteFailedQuantity->addItems(opcItemIDList);
        ui->comboWriteFormulaNumber->addItems(opcItemIDList);
        ui->comboWriteProcessPrompt->addItems(opcItemIDList);

        ui->comboResetSignal->addItems(opcItemIDList);
        ui->comboAlarmSignal->addItems(opcItemIDList);
    } while (0);

    QString filename = QString("%1/%2.%3.xml")
                           .arg(PROCESS_ASSEMBLY_PATH)
                           .arg(craftID)
                           .arg(stationID);

    if (AssyStation_ReadXmlFile(filename, preparePara, processPara, stepParaList))
    {
        setWidgetData();
    }
    else
    {
        MessageBox_Critical("参数读取失败！");
    }
}

void WidgetAssyProcessStation::saveParameter()
{
    getWidgetData();

    QString filename = QString("%1/%2.%3.xml")
                           .arg(PROCESS_ASSEMBLY_PATH)
                           .arg(craftID)
                           .arg(stationID);

    if (AssyStation_SaveXmlFile(filename, preparePara, processPara, stepParaList))
    {
        MessageBox_Information("参数保存成功。");
    }
    else
    {
        MessageBox_Critical("参数保存失败！");
    }
}

void WidgetAssyProcessStation::slotStepItemSelectionChanged()
{
    QTableWidget *table = ui->tableStep;
    int stepRow = table->currentRow();

    if (stepRow == -1)
    {
        ui->tableBarcode->clearContents();
        ui->tableBarcode->setRowCount(0);
        ui->tableMeasure->clearContents();
        ui->tableMeasure->setRowCount(0);

        ui->stackedWidget_2->setCurrentIndex(0);
        return;
    }

    const StepPara &para = stepParaList.at(stepRow);

    ui->stackedWidget_2->setCurrentIndex(static_cast<int>(para.stepType));

    do
    {
        QTableWidget *table = ui->tableBarcode;
        QTableWidgetItem *item = nullptr;
        int rowCount = para.barcodeItemList.size();
        table->clearContents();
        table->setRowCount(rowCount);

        for (int row = 0; row < rowCount; ++row)
        {
            const ScanBarcodeItem &it = para.barcodeItemList.at(row);

            item = new QTableWidgetItem(it.isDisabled ? tr("屏蔽") : tr("启用"));
            item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(BarcodeHeader::IsDisabled), item);

            item = new QTableWidgetItem(barcodeTypeStringMap.value(it.barcodeType));
            table->setItem(row, static_cast<int>(BarcodeHeader::BarcodeType), item);

            item = new QTableWidgetItem(it.barcodeID);
            table->setItem(row, static_cast<int>(BarcodeHeader::BarcodeID), item);

            item = new QTableWidgetItem(it.readBarcode.itemID);
            table->setItem(row, static_cast<int>(BarcodeHeader::ReadBarcode), item);

            item = new QTableWidgetItem(it.writeDisplay.itemID);
            table->setItem(row, static_cast<int>(BarcodeHeader::WriteDisplay), item);

            item = new QTableWidgetItem(it.writeResult.itemID);
            table->setItem(row, static_cast<int>(BarcodeHeader::WriteResult), item);

            item = new QTableWidgetItem(it.resultOK);
            table->setItem(row, static_cast<int>(BarcodeHeader::ResultOK), item);

            item = new QTableWidgetItem(it.resultNG);
            table->setItem(row, static_cast<int>(BarcodeHeader::ResultNG), item);
        }
    } while (0);

    do
    {
        QTableWidget *table = ui->tableMeasure;
        QTableWidgetItem *item = nullptr;
        int rowCount = para.measureItemList.size();
        table->clearContents();
        table->setRowCount(rowCount);

        for (int row = 0; row < rowCount; ++row)
        {
            const MeasureDataItem &it = para.measureItemList.at(row);

            item = new QTableWidgetItem(it.isDisabled ? tr("屏蔽") : tr("启用"));
            item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(MeasureHeader::IsDisabled), item);

            item = new QTableWidgetItem(it.measureID);
            table->setItem(row, static_cast<int>(MeasureHeader::MeasureID), item);

            item = new QTableWidgetItem(it.offsetValue);
            table->setItem(row, static_cast<int>(MeasureHeader::OffsetValue), item);

            item = new QTableWidgetItem(it.readMeasure.itemID);
            table->setItem(row, static_cast<int>(MeasureHeader::ReadMeasure), item);

            item = new QTableWidgetItem(it.writeDisplay.itemID);
            table->setItem(row, static_cast<int>(MeasureHeader::WriteDisplay), item);

            item = new QTableWidgetItem(it.writeResult.itemID);
            table->setItem(row, static_cast<int>(MeasureHeader::WriteResult), item);

            item = new QTableWidgetItem(it.resultOK);
            table->setItem(row, static_cast<int>(MeasureHeader::ResultOK), item);

            item = new QTableWidgetItem(it.resultNG);
            table->setItem(row, static_cast<int>(MeasureHeader::ResultNG), item);
        }
    } while (0);
}

void WidgetAssyProcessStation::slotSaveLoopStart()
{
}

void WidgetAssyProcessStation::slotSaveLoopEnd()
{
}

void WidgetAssyProcessStation::slotSaveAutoMotion()
{
}

void WidgetAssyProcessStation::slotStepItemDoubleClicked()
{
    QTableWidget *table = ui->tableStep;
    QTableWidgetItem *item = nullptr;
    int row = table->currentRow();

    StepPara &para = stepParaList[row];
    StepItem &it = para.stepItem;

    DialogProcessStepData dialog(this);

    dialog.setOpcItemIDList(opcItemIDList);

    dialog.setWidgetData(it);

    if (QDialog::Accepted != dialog.exec())
        return;

    it.interaction = dialog.getWidgetData().interaction;
    it.description = dialog.getWidgetData().description;
    it.isDisabled = dialog.getWidgetData().isDisabled;
    it.maxMeasure = dialog.getWidgetData().maxMeasure;
    it.startRequest = dialog.getWidgetData().startRequest;
    it.startResponse = dialog.getWidgetData().startResponse;
    it.skipResponse = dialog.getWidgetData().skipResponse;
    it.measureRequest = dialog.getWidgetData().measureRequest;
    it.successResponse = dialog.getWidgetData().successResponse;
    it.failedResponse = dialog.getWidgetData().failedResponse;

    item = table->item(row, static_cast<int>(StepHeader::IsDisabled));
    item->setText(it.isDisabled ? tr("跳过") : tr("启用"));
    item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);

    item = table->item(row, static_cast<int>(StepHeader::Interaction));
    item->setText(it.interaction);
    item = table->item(row, static_cast<int>(StepHeader::Description));
    item->setText(it.description);
    item = table->item(row, static_cast<int>(StepHeader::MaxMeasure));
    item->setText(QString::number(it.maxMeasure));

    item = table->item(row, static_cast<int>(StepHeader::StartRequestItemID));
    item->setText(it.startRequest.itemID);
    item = table->item(row, static_cast<int>(StepHeader::StartRequestValue));
    item->setText(it.startRequest.itemValue.toString());

    item = table->item(row, static_cast<int>(StepHeader::StartResponseItemID));
    item->setText(it.startResponse.itemID);
    item = table->item(row, static_cast<int>(StepHeader::StartResponseValue));
    item->setText(it.startResponse.itemValue.toString());

    item = table->item(row, static_cast<int>(StepHeader::SkipResponseItemID));
    item->setText(it.skipResponse.itemID);
    item = table->item(row, static_cast<int>(StepHeader::SkipResponseValue));
    item->setText(it.skipResponse.itemValue.toString());

    item = table->item(row, static_cast<int>(StepHeader::MeasureRequestItemID));
    item->setText(it.measureRequest.itemID);
    item = table->item(row, static_cast<int>(StepHeader::MeasureRequestValue));
    item->setText(it.measureRequest.itemValue.toString());

    item = table->item(row, static_cast<int>(StepHeader::SuccessResponseItemID));
    item->setText(it.successResponse.itemID);
    item = table->item(row, static_cast<int>(StepHeader::SuccessResponseValue));
    item->setText(it.successResponse.itemValue.toString());

    item = table->item(row, static_cast<int>(StepHeader::FailedResponseItemID));
    item->setText(it.failedResponse.itemID);
    item = table->item(row, static_cast<int>(StepHeader::FailedResponseValue));
    item->setText(it.failedResponse.itemValue.toString());

    para.stepType = stepTypeStringMap.key(it.interaction);

    ui->stackedWidget_2->setCurrentIndex(static_cast<int>(para.stepType));
}

void WidgetAssyProcessStation::slotBarcodeItemDoubleClicked()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableBarcode;
    QTableWidgetItem *item = nullptr;
    row = table->currentRow();

    ScanBarcodeItem &it = para.barcodeItemList[row];

    DialogStepScanBarcode dialog(this);

    dialog.setMaterialIDList(materialIDList);
    dialog.setStationIDList(stationIDList);
    dialog.setDacItemIDList(dacItemIDList);
    dialog.setOpcItemIDList(opcItemIDList);

    dialog.setWidgetData(it);

    if (QDialog::Accepted != dialog.exec())
        return;

    it = dialog.getWidgetData();

    item = table->item(row, static_cast<int>(BarcodeHeader::IsDisabled));
    item->setText(it.isDisabled ? tr("屏蔽") : tr("启用"));
    item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);

    item = table->item(row, static_cast<int>(BarcodeHeader::BarcodeType));
    item->setText(barcodeTypeStringMap.value(it.barcodeType));

    item = table->item(row, static_cast<int>(BarcodeHeader::BarcodeID));
    item->setText(it.barcodeID);

    item = table->item(row, static_cast<int>(BarcodeHeader::ReadBarcode));
    item->setText(it.readBarcode.itemID);

    item = table->item(row, static_cast<int>(BarcodeHeader::WriteDisplay));
    item->setText(it.writeDisplay.itemID);

    item = table->item(row, static_cast<int>(BarcodeHeader::WriteResult));
    item->setText(it.writeResult.itemID);

    item = table->item(row, static_cast<int>(BarcodeHeader::ResultOK));
    item->setText(it.resultOK);

    item = table->item(row, static_cast<int>(BarcodeHeader::ResultNG));
    item->setText(it.resultNG);
}

void WidgetAssyProcessStation::slotMeasureItemDoubleClicked()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableMeasure;
    QTableWidgetItem *item = nullptr;
    row = table->currentRow();

    MeasureDataItem &it = para.measureItemList[row];

    DialogStepMeasureData dialog(this);

    dialog.setMeasureIDList(measureIDList);
    dialog.setDacItemIDList(dacItemIDList);
    dialog.setOpcItemIDList(opcItemIDList);

    dialog.setWidgetData(it);

    if (QDialog::Accepted != dialog.exec())
        return;

    it = dialog.getWidgetData();

    item = table->item(row, static_cast<int>(MeasureHeader::IsDisabled));
    item->setText(it.isDisabled ? tr("屏蔽") : tr("启用"));
    item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);

    item = table->item(row, static_cast<int>(MeasureHeader::MeasureID));
    item->setText(it.measureID);

    item = table->item(row, static_cast<int>(MeasureHeader::OffsetValue));
    item->setText(it.offsetValue);

    item = table->item(row, static_cast<int>(MeasureHeader::ReadMeasure));
    item->setText(it.readMeasure.itemID);

    item = table->item(row, static_cast<int>(MeasureHeader::WriteDisplay));
    item->setText(it.writeDisplay.itemID);

    item = table->item(row, static_cast<int>(MeasureHeader::WriteResult));
    item->setText(it.writeResult.itemID);

    item = table->item(row, static_cast<int>(MeasureHeader::ResultOK));
    item->setText(it.resultOK);

    item = table->item(row, static_cast<int>(MeasureHeader::ResultNG));
    item->setText(it.resultNG);
}

void WidgetAssyProcessStation::slotAddnewStepItem()
{
    DialogProcessStepData dialog(this);

    dialog.setOpcItemIDList(opcItemIDList);

    if (QDialog::Accepted != dialog.exec())
        return;

    QTableWidget *table = ui->tableStep;
    QTableWidgetItem *item = nullptr;
    int row = table->rowCount();
    table->insertRow(row);

    StepItem it = dialog.getWidgetData();

    StepPara para;
    para.stepItem = it;
    para.stepType = stepTypeStringMap.key(it.interaction);
    stepParaList.push_back(para);

    item = new QTableWidgetItem(it.isDisabled ? tr("跳过") : tr("启用"));
    item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, static_cast<int>(StepHeader::IsDisabled), item);

    item = new QTableWidgetItem(it.interaction);
    table->setItem(row, static_cast<int>(StepHeader::Interaction), item);
    item = new QTableWidgetItem(it.description);
    table->setItem(row, static_cast<int>(StepHeader::Description), item);
    item = new QTableWidgetItem(QString::number(it.maxMeasure));
    table->setItem(row, static_cast<int>(StepHeader::MaxMeasure), item);

    item = new QTableWidgetItem(it.startRequest.itemID);
    table->setItem(row, static_cast<int>(StepHeader::StartRequestItemID), item);
    item = new QTableWidgetItem(it.startRequest.itemValue.toString());
    table->setItem(row, static_cast<int>(StepHeader::StartRequestValue), item);

    item = new QTableWidgetItem(it.startResponse.itemID);
    table->setItem(row, static_cast<int>(StepHeader::StartResponseItemID), item);
    item = new QTableWidgetItem(it.startResponse.itemValue.toString());
    table->setItem(row, static_cast<int>(StepHeader::StartResponseValue), item);

    item = new QTableWidgetItem(it.skipResponse.itemID);
    table->setItem(row, static_cast<int>(StepHeader::SkipResponseItemID), item);
    item = new QTableWidgetItem(it.skipResponse.itemValue.toString());
    table->setItem(row, static_cast<int>(StepHeader::SkipResponseValue), item);

    item = new QTableWidgetItem(it.measureRequest.itemID);
    table->setItem(row, static_cast<int>(StepHeader::MeasureRequestItemID), item);
    item = new QTableWidgetItem(it.measureRequest.itemValue.toString());
    table->setItem(row, static_cast<int>(StepHeader::MeasureRequestValue), item);

    item = new QTableWidgetItem(it.successResponse.itemID);
    table->setItem(row, static_cast<int>(StepHeader::SuccessResponseItemID), item);
    item = new QTableWidgetItem(it.successResponse.itemValue.toString());
    table->setItem(row, static_cast<int>(StepHeader::SuccessResponseValue), item);

    item = new QTableWidgetItem(it.failedResponse.itemID);
    table->setItem(row, static_cast<int>(StepHeader::FailedResponseItemID), item);
    item = new QTableWidgetItem(it.failedResponse.itemValue.toString());
    table->setItem(row, static_cast<int>(StepHeader::FailedResponseValue), item);

    table->selectRow(row);
}

void WidgetAssyProcessStation::slotDeleteStepItem()
{
    QTableWidget *table = ui->tableStep;
    int row = table->currentRow();

    if (row == -1)
        return;

    stepParaList.removeAt(row);
    table->removeRow(row);
}

void WidgetAssyProcessStation::slotMoveupStepItem()
{
    QTableWidget *table = ui->tableStep;
    int row = table->currentRow();

    if (row == -1 || row == 0)
        return;

    stepParaList.swap(row, row - 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row - 1, col);
        table->setItem(row, col, item2);
        table->setItem(row - 1, col, item1);
    }

    table->selectRow(row - 1);
}

void WidgetAssyProcessStation::slotMovedownStepItem()
{
    QTableWidget *table = ui->tableStep;
    int row = table->currentRow();
    int count = table->rowCount();

    if (row == -1 || row == count - 1)
        return;

    stepParaList.swap(row, row + 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row + 1, col);
        table->setItem(row, col, item2);
        table->setItem(row + 1, col, item1);
    }

    table->selectRow(row + 1);
}

void WidgetAssyProcessStation::slotAddnewBarcodeItem()
{
    DialogStepScanBarcode dialog(this);

    dialog.setMaterialIDList(materialIDList);
    dialog.setStationIDList(stationIDList);
    dialog.setDacItemIDList(dacItemIDList);
    dialog.setOpcItemIDList(opcItemIDList);

    if (QDialog::Accepted != dialog.exec())
        return;

    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableBarcode;
    QTableWidgetItem *item = nullptr;
    row = table->rowCount();
    table->insertRow(row);

    ScanBarcodeItem it = dialog.getWidgetData();
    para.barcodeItemList.push_back(it);

    item = new QTableWidgetItem(it.isDisabled ? tr("屏蔽") : tr("启用"));
    item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, static_cast<int>(BarcodeHeader::IsDisabled), item);

    item = new QTableWidgetItem(barcodeTypeStringMap.value(it.barcodeType));
    table->setItem(row, static_cast<int>(BarcodeHeader::BarcodeType), item);

    item = new QTableWidgetItem(it.barcodeID);
    table->setItem(row, static_cast<int>(BarcodeHeader::BarcodeID), item);

    item = new QTableWidgetItem(it.readBarcode.itemID);
    table->setItem(row, static_cast<int>(BarcodeHeader::ReadBarcode), item);

    item = new QTableWidgetItem(it.writeDisplay.itemID);
    table->setItem(row, static_cast<int>(BarcodeHeader::WriteDisplay), item);

    item = new QTableWidgetItem(it.writeResult.itemID);
    table->setItem(row, static_cast<int>(BarcodeHeader::WriteResult), item);

    item = new QTableWidgetItem(it.resultOK);
    table->setItem(row, static_cast<int>(BarcodeHeader::ResultOK), item);

    item = new QTableWidgetItem(it.resultNG);
    table->setItem(row, static_cast<int>(BarcodeHeader::ResultNG), item);

    table->selectRow(row);
}

void WidgetAssyProcessStation::slotAddnewMeasureItem()
{
    DialogStepMeasureData dialog(this);

    dialog.setMeasureIDList(measureIDList);
    dialog.setDacItemIDList(dacItemIDList);
    dialog.setOpcItemIDList(opcItemIDList);

    if (QDialog::Accepted != dialog.exec())
        return;

    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableMeasure;
    QTableWidgetItem *item = nullptr;
    row = table->rowCount();
    table->insertRow(row);

    MeasureDataItem it = dialog.getWidgetData();
    para.measureItemList.push_back(it);

    item = new QTableWidgetItem(it.isDisabled ? tr("屏蔽") : tr("启用"));
    item->setTextColor(it.isDisabled ? Qt::red : Qt::blue);
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, static_cast<int>(MeasureHeader::IsDisabled), item);

    item = new QTableWidgetItem(it.measureID);
    table->setItem(row, static_cast<int>(MeasureHeader::MeasureID), item);

    item = new QTableWidgetItem(it.offsetValue);
    table->setItem(row, static_cast<int>(MeasureHeader::OffsetValue), item);

    item = new QTableWidgetItem(it.readMeasure.itemID);
    table->setItem(row, static_cast<int>(MeasureHeader::ReadMeasure), item);

    item = new QTableWidgetItem(it.writeDisplay.itemID);
    table->setItem(row, static_cast<int>(MeasureHeader::WriteDisplay), item);

    item = new QTableWidgetItem(it.writeResult.itemID);
    table->setItem(row, static_cast<int>(MeasureHeader::WriteResult), item);

    item = new QTableWidgetItem(it.resultOK);
    table->setItem(row, static_cast<int>(MeasureHeader::ResultOK), item);

    item = new QTableWidgetItem(it.resultNG);
    table->setItem(row, static_cast<int>(MeasureHeader::ResultNG), item);

    table->selectRow(row);
}

void WidgetAssyProcessStation::slotDeleteBarcodeItem()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableBarcode;
    row = table->currentRow();

    if (row == -1)
        return;

    para.barcodeItemList.removeAt(row);
    table->removeRow(row);
}

void WidgetAssyProcessStation::slotDeleteMeasureItem()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableMeasure;
    row = table->currentRow();

    if (row == -1)
        return;

    para.measureItemList.removeAt(row);
    table->removeRow(row);
}

void WidgetAssyProcessStation::slotMoveupBarcodeItem()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableBarcode;
    row = table->currentRow();

    if (row == -1 || row == 0)
        return;

    para.barcodeItemList.swap(row, row - 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row - 1, col);
        table->setItem(row, col, item2);
        table->setItem(row - 1, col, item1);
    }

    table->selectRow(row - 1);
}

void WidgetAssyProcessStation::slotMoveupMeasureItem()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableMeasure;
    row = table->currentRow();

    if (row == -1 || row == 0)
        return;

    para.measureItemList.swap(row, row - 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row - 1, col);
        table->setItem(row, col, item2);
        table->setItem(row - 1, col, item1);
    }

    table->selectRow(row - 1);
}

void WidgetAssyProcessStation::slotMovedownBarcodeItem()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableBarcode;
    row = table->currentRow();

    if (row == -1 || row == table->rowCount() - 1)
        return;

    para.barcodeItemList.swap(row, row + 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row + 1, col);
        table->setItem(row, col, item2);
        table->setItem(row + 1, col, item1);
    }

    table->selectRow(row + 1);
}

void WidgetAssyProcessStation::slotMovedownMeasureItem()
{
    int row = ui->tableStep->currentRow();
    StepPara &para = stepParaList[row];

    QTableWidget *table = ui->tableMeasure;
    row = table->currentRow();

    if (row == -1 || row == table->rowCount() - 1)
        return;

    para.measureItemList.swap(row, row + 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row + 1, col);
        table->setItem(row, col, item2);
        table->setItem(row + 1, col, item1);
    }

    table->selectRow(row + 1);
}
