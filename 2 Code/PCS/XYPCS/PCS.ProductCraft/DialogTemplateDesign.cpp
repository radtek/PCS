#include "DialogTemplateDesign.h"
#include "ui_DialogTemplateDesign.h"

DialogTemplateDesign::DialogTemplateDesign(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogTemplateDesign)
{
    ui->setupUi(this);

    initialWidget();
}

DialogTemplateDesign::~DialogTemplateDesign()
{
    delete ui;
}

void DialogTemplateDesign::initialWidget()
{
    do
    {
        ui->comboItemType->lineEdit()->setReadOnly(true);
        ui->comboItemName->lineEdit()->setReadOnly(true);
        ui->editItemText->setReadOnly(true);
    }
    while (0);

    do
    {
        connect(ui->comboItemType, SIGNAL(currentTextChanged(QString)), this, SLOT(slotItemTypeChanged(QString)));

        connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
    }
    while (0);
}

QString DialogTemplateDesign::getItemType() const
{
    return ui->comboItemType->currentText();
}

QString DialogTemplateDesign::getItemName() const
{
    return ui->comboItemName->currentText();
}

QString DialogTemplateDesign::getItemText() const
{
    return ui->editItemText->text();
}

void DialogTemplateDesign::setItemType(const QString &type)
{
    ui->comboItemType->setCurrentText(type);
}

void DialogTemplateDesign::setItemName(const QString &name)
{
    ui->comboItemName->setCurrentText(name);
}

void DialogTemplateDesign::setItemText(const QString &text)
{
    ui->editItemText->setText(text);
}

void DialogTemplateDesign::setItemTypeList(const QStringList &typeList)
{
    ui->comboItemType->addItems(typeList);
}

void DialogTemplateDesign::setItemNameList(const QStringList &nameList)
{
    ui->comboItemName->addItems(nameList);
}

void DialogTemplateDesign::slotItemTypeChanged(const QString &type)
{
    switch (templateItemTypeMap.key(type))
    {
    case TemplateItemType::FixedText:
        ui->editItemText->clear();
        ui->editItemText->setReadOnly(false);
        break;
    case TemplateItemType::Barcode:
        ui->editItemText->clear();
        ui->editItemText->setReadOnly(true);
        break;
    case TemplateItemType::DateFormat:
        ui->editItemText->setText("YYYY/MM/DD");
        ui->editItemText->setReadOnly(false);
        break;
    case TemplateItemType::SerialNo10:
        ui->editItemText->setText("%04d");
        ui->editItemText->setReadOnly(false);
        break;
    case TemplateItemType::SerialNo16:
        ui->editItemText->setText("%04x");
        ui->editItemText->setReadOnly(false);
        break;
    case TemplateItemType::PackWeight:
        ui->editItemText->setText("%0.2f");
        ui->editItemText->setReadOnly(false);
        break;
    case TemplateItemType::PackNumber:
        ui->editItemText->setText("%04d");
        ui->editItemText->setReadOnly(false);
        break;
    }
}
