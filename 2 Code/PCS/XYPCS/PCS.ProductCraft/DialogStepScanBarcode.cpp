#include "DialogStepScanBarcode.h"
#include "ui_DialogStepScanBarcode.h"

DialogStepScanBarcode::DialogStepScanBarcode(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogStepScanBarcode)
{
    ui->setupUi(this);

    initialWidget();
}

DialogStepScanBarcode::~DialogStepScanBarcode()
{
    delete ui;
}

void DialogStepScanBarcode::initialWidget()
{
    do
    {
        ui->comboBarcodeType->addItems(barcodeTypeStringMap.values());
        ui->comboBarcodeType->setCurrentIndex(-1);
        ui->comboMeasureType->addItems(measureTypeStringMap.values());
        ui->comboMeasureType->setCurrentIndex(-1);

        ui->editResultOK->setText("1");
        ui->editResultNG->setText("0");
    }
    while (0);

    do
    {
        connect(ui->checkIsDisabled, SIGNAL(toggled(bool)), this, SLOT(slotCheckDisabled(bool)));
        connect(ui->comboBarcodeType, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectBarcodeType(QString)));
        connect(ui->comboMeasureType, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectMeasureType(QString)));

        connect(ui->buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    }
    while (0);
}

void DialogStepScanBarcode::slotCheckDisabled(bool check)
{
    ui->frame->setEnabled(!check);
}

int DialogStepScanBarcode::exec()
{
    if (ui->comboBarcodeType->currentText().isEmpty())
        ui->comboBarcodeType->setCurrentIndex(0);

    if (ui->comboMeasureType->currentText().isEmpty())
        ui->comboMeasureType->setCurrentIndex(0);

    return QDialog::exec();
}

void DialogStepScanBarcode::slotSelectBarcodeType(const QString &text)
{
    switch (barcodeTypeStringMap.key(text))
    {
    case BarcodeType::Material:
        ui->comboBarcodeID->clear();
        ui->comboBarcodeID->addItems(materialIDList);
        ui->comboBarcodeID->setCurrentIndex(-1);
        ui->labelBarcodeID->setText(tr("物料编号："));
        break;

    case BarcodeType::Transfer:
        ui->comboBarcodeID->clear();
        ui->comboBarcodeID->addItems(stationIDList);
        ui->comboBarcodeID->setCurrentIndex(-1);
        ui->labelBarcodeID->setText(tr("工位编号："));
        break;
    }
}

void DialogStepScanBarcode::slotSelectMeasureType(const QString &text)
{
    switch (measureTypeStringMap.key(text))
    {
    case MeasureType::ByDAC:
        ui->comboReadBarcode->clear();
        ui->comboReadBarcode->addItems(dacItemIDList);
        ui->comboReadBarcode->setCurrentIndex(-1);
        ui->labelReadBarcode->setText(tr("采集设备编号"));
        break;

    case MeasureType::ByOPC:
        ui->comboReadBarcode->clear();
        ui->comboReadBarcode->addItems(opcItemIDList);
        ui->comboReadBarcode->setCurrentIndex(-1);
        ui->labelReadBarcode->setText(tr("数据读取地址"));
        break;
    }
}

void DialogStepScanBarcode::setMaterialIDList(const QStringList &list)
{
    materialIDList = list;
}

void DialogStepScanBarcode::setStationIDList(const QStringList &list)
{
    stationIDList = list;
}

void DialogStepScanBarcode::setDacItemIDList(const QStringList &list)
{
    dacItemIDList = list;
}

void DialogStepScanBarcode::setOpcItemIDList(const QStringList &list)
{
    opcItemIDList = list;

    ui->comboWriteDisplay->addItems(list);
    ui->comboWriteResult->addItems(list);
    ui->comboWriteDisplay->setCurrentIndex(-1);
    ui->comboWriteResult->setCurrentIndex(-1);
}

void DialogStepScanBarcode::setWidgetData(const ScanBarcodeItem &item)
{
    ui->checkIsDisabled->setChecked(item.isDisabled);

    ui->comboBarcodeType->setCurrentText(barcodeTypeStringMap.value(item.barcodeType));
    ui->comboBarcodeID->setCurrentText(item.barcodeID);

    ui->comboMeasureType->setCurrentText(measureTypeStringMap.value(item.measureType));
    ui->comboReadBarcode->setCurrentText(item.readBarcode.itemID);

    ui->comboWriteDisplay->setCurrentText(item.writeDisplay.itemID);
    ui->comboWriteResult->setCurrentText(item.writeResult.itemID);

    ui->editResultOK->setText(item.resultOK);
    ui->editResultNG->setText(item.resultNG);
}

ScanBarcodeItem DialogStepScanBarcode::getWidgetData() const
{
    ScanBarcodeItem item;

    item.isDisabled = ui->checkIsDisabled->isChecked();

    item.barcodeType = barcodeTypeStringMap.key(ui->comboBarcodeType->currentText());
    item.barcodeID = ui->comboBarcodeID->currentText();

    item.measureType = measureTypeStringMap.key(ui->comboMeasureType->currentText());
    item.readBarcode.itemID = ui->comboReadBarcode->currentText();

    item.writeDisplay.itemID = ui->comboWriteDisplay->currentText();
    item.writeResult.itemID = ui->comboWriteResult->currentText();

    item.resultOK = ui->editResultOK->text();
    item.resultNG = ui->editResultNG->text();

    return item;
}
