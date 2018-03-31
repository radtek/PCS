#include "DialogMaterialInput.h"
#include "WorkManager.h"
#include "ui_DialogMaterialInput.h"
DialogMaterialInput::DialogMaterialInput(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogMaterialInput)
{
    ui->setupUi(this);

    initialWidget();
}

DialogMaterialInput::~DialogMaterialInput()
{
    delete ui;
}

void DialogMaterialInput::initialWidget()
{
    do
    {
        ui->comboStationID->lineEdit()->setReadOnly(true);
        ui->comboMaterialID->lineEdit()->setReadOnly(true);

        ui->comboStationID->addItems(qWorkManager->getStationIDList());
        ui->comboStationID->setCurrentIndex(-1);

        ui->editBatchQuantity->setValidator(new QIntValidator(0, 99999));
    } while (0);

    do
    {
        connect(ui->comboStationID, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectStationID()));
        connect(ui->comboMaterialID, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectMaterialID()));
        connect(ui->editMaterialBatch, SIGNAL(returnPressed()), ui->editBatchQuantity, SLOT(setFocus()));
        connect(ui->editBatchQuantity, SIGNAL(returnPressed()), this, SLOT(accept()));
        connect(ui->editPackageBatch, SIGNAL(returnPressed()), this, SLOT(accept()));

        connect(ui->buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    } while (0);
}

void DialogMaterialInput::setStationID(const QString &stationID)
{
    ui->comboStationID->setCurrentText(stationID);
}

void DialogMaterialInput::setMaterialID(const QString &materialID)
{
    ui->comboMaterialID->setCurrentText(materialID);
}

QString DialogMaterialInput::getStationID() const
{
    return ui->comboStationID->currentText();
}

QString DialogMaterialInput::getMaterialID() const
{
    return ui->comboMaterialID->currentText();
}

QString DialogMaterialInput::getMaterialBatch() const
{
    return ui->editMaterialBatch->text();
}

int DialogMaterialInput::getBatchQuantity() const
{
    return ui->editBatchQuantity->text().toInt();
}

QString DialogMaterialInput::getPackageBatch() const
{
    return ui->editPackageBatch->text();
}

void DialogMaterialInput::slotSelectStationID()
{
    QString craftID = qWorkManager->getCraftID();
    QString stationID = ui->comboStationID->currentText();

    ui->comboMaterialID->clear();
    ui->comboMaterialID->addItems(qWorkManager->getMaterialIDList(craftID, stationID));
    ui->comboMaterialID->setCurrentIndex(-1);

    ui->editMaterialBatch->clear();
    ui->editBatchQuantity->clear();
    ui->editMaterialBatch->setFocus();

    //    ui->editPackageBatch->clear();
    //    ui->editPackageBatch->setFocus();
}

void DialogMaterialInput::slotSelectMaterialID()
{
    ui->editMaterialBatch->clear();
    ui->editBatchQuantity->clear();
    ui->editMaterialBatch->setFocus();

    //    ui->editPackageBatch->clear();
    //    ui->editPackageBatch->setFocus();
}
