#include "DialogStepMeasureData.h"
#include "ui_DialogStepMeasureData.h"

DialogStepMeasureData::DialogStepMeasureData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogStepMeasureData)
{
    ui->setupUi(this);

    initialWidget();
}

DialogStepMeasureData::~DialogStepMeasureData()
{
    delete ui;
}

void DialogStepMeasureData::initialWidget()
{
    do
    {
        ui->comboMeasureType->addItems(measureTypeStringMap.values());
        ui->comboMeasureType->setCurrentIndex(-1);

        ui->editOffsetValue->setValidator(new QDoubleValidator());
        ui->editOffsetValue->setText("0.0");

        ui->editResultOK->setText("1");
        ui->editResultNG->setText("0");
    }
    while (0);

    do
    {
        connect(ui->checkIsDisabled, SIGNAL(toggled(bool)), this, SLOT(slotCheckDisabled(bool)));
        connect(ui->comboMeasureType, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectMeasureType(QString)));

        connect(ui->buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    }
    while (0);
}

void DialogStepMeasureData::slotCheckDisabled(bool check)
{
    ui->frame->setEnabled(!check);
}

int DialogStepMeasureData::exec()
{
    if (ui->comboMeasureType->currentText().isEmpty())
        ui->comboMeasureType->setCurrentIndex(0);

    return QDialog::exec();
}

void DialogStepMeasureData::slotSelectMeasureType(const QString &text)
{
    switch (measureTypeStringMap.key(text))
    {
    case MeasureType::ByDAC:
        ui->comboReadMeasure->clear();
        ui->comboReadMeasure->addItems(dacItemIDList);
        ui->comboReadMeasure->setCurrentIndex(-1);
        ui->labelReadMeasure->setText(tr("采集设备编号："));
        break;

    case MeasureType::ByOPC:
        ui->comboReadMeasure->clear();
        ui->comboReadMeasure->addItems(opcItemIDList);
        ui->comboReadMeasure->setCurrentIndex(-1);
        ui->labelReadMeasure->setText(tr("数据读取地址："));
        break;
    }
}

void DialogStepMeasureData::setMeasureIDList(const QStringList &list)
{
    ui->comboMeasureID->addItems(list);
    ui->comboMeasureID->setCurrentIndex(-1);
}

void DialogStepMeasureData::setDacItemIDList(const QStringList &list)
{
    dacItemIDList = list;
}

void DialogStepMeasureData::setOpcItemIDList(const QStringList &list)
{
    opcItemIDList = list;

    ui->comboWriteDisplay->addItems(list);
    ui->comboWriteResult->addItems(list);
    ui->comboWriteDisplay->setCurrentIndex(-1);
    ui->comboWriteResult->setCurrentIndex(-1);
}

void DialogStepMeasureData::setWidgetData(const MeasureDataItem &item)
{
    ui->checkIsDisabled->setChecked(item.isDisabled);

    ui->comboMeasureID->setCurrentText(item.measureID);
    ui->editOffsetValue->setText(item.offsetValue);

    ui->comboMeasureType->setCurrentText(measureTypeStringMap.value(item.measureType));
    ui->comboReadMeasure->setCurrentText(item.readMeasure.itemID);

    ui->comboWriteDisplay->setCurrentText(item.writeDisplay.itemID);
    ui->comboWriteResult->setCurrentText(item.writeResult.itemID);

    ui->editResultOK->setText(item.resultOK);
    ui->editResultNG->setText(item.resultNG);
}

MeasureDataItem DialogStepMeasureData::getWidgetData() const
{
    MeasureDataItem item;

    item.isDisabled = ui->checkIsDisabled->isChecked();

    item.measureID = ui->comboMeasureID->currentText();
    item.offsetValue = ui->editOffsetValue->text();

    item.measureType = measureTypeStringMap.key(ui->comboMeasureType->currentText());
    item.readMeasure.itemID = ui->comboReadMeasure->currentText();

    item.writeDisplay.itemID = ui->comboWriteDisplay->currentText();
    item.writeResult.itemID = ui->comboWriteResult->currentText();

    item.resultOK = ui->editResultOK->text();
    item.resultNG = ui->editResultNG->text();

    return item;
}

