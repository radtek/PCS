#include "DialogBarcodeDesign.h"
#include "ui_DialogBarcodeDesign.h"

DialogBarcodeDesign::DialogBarcodeDesign(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogBarcodeDesign)
{
    ui->setupUi(this);

    initialWidget();
}

DialogBarcodeDesign::~DialogBarcodeDesign()
{
    delete ui;
}

void DialogBarcodeDesign::initialWidget()
{
    do
    {
        ui->comboItemType->lineEdit()->setReadOnly(true);
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
    }
    while (0);

    do
    {
        connect(ui->comboItemType, SIGNAL(currentTextChanged(QString)), this, SLOT(slotItemTypeChanged(QString)));
        connect(ui->editItemText, SIGNAL(textEdited(QString)), this, SLOT(slotItemTextChanged(QString)));

        connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
    }
    while (0);
}

QString DialogBarcodeDesign::getItemType() const
{
    return ui->comboItemType->currentText();
}

int DialogBarcodeDesign::getItemSize() const
{
    return ui->spinItemSize->value();
}

QString DialogBarcodeDesign::getItemText() const
{
    return ui->editItemText->text();
}

void DialogBarcodeDesign::setItemType(const QString &type)
{
    ui->comboItemType->setCurrentText(type);
}

void DialogBarcodeDesign::setItemSize(int size)
{
    ui->spinItemSize->setValue(size);
}

void DialogBarcodeDesign::setItemText(const QString &text)
{
    ui->editItemText->setText(text);
}

void DialogBarcodeDesign::setItemTypeList(const QStringList &typeList)
{
    ui->comboItemType->addItems(typeList);
}

void DialogBarcodeDesign::slotItemTypeChanged(const QString &type)
{
    switch (barcodeItemTypeMap.key(type))
    {
    case BarcodeItemType::FixedText:
        ui->spinItemSize->setValue(0);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(false);
        break;
    case BarcodeItemType::DateFormat:
        ui->spinItemSize->setValue(6);
        ui->editItemText->setText("YYMMDD");
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(false);
        break;
    case BarcodeItemType::DayOfYear:
        ui->spinItemSize->setValue(3);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::WeekOfYear:
        ui->spinItemSize->setValue(2);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::YearCode:
        ui->spinItemSize->setValue(1);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::MonthCode:
        ui->spinItemSize->setValue(1);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::DayCode:
        ui->spinItemSize->setValue(1);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::SerialNo10:
        ui->spinItemSize->setValue(4);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(false);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::SerialNo16:
        ui->spinItemSize->setValue(4);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(false);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::SerialNo36:
        ui->spinItemSize->setValue(4);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(false);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::CheckCode:
        ui->spinItemSize->setValue(1);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::BatchCode:
        ui->spinItemSize->setValue(1);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    case BarcodeItemType::RepairCode:
        ui->spinItemSize->setValue(1);
        ui->editItemText->clear();
        ui->spinItemSize->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
        break;
    }
}

void DialogBarcodeDesign::slotItemTextChanged(const QString &text)
{
    ui->spinItemSize->setValue(text.size());
}
