#include "DialogOperatorLogin.h"
#include "WorkManager.h"
#include "ui_DialogOperatorLogin.h"
DialogOperatorLogin::DialogOperatorLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogOperatorLogin)
{
    ui->setupUi(this);

    initialWidget();
}

DialogOperatorLogin::~DialogOperatorLogin()
{
    delete ui;
}

void DialogOperatorLogin::initialWidget()
{
    do
    {
        ui->comboStationID->lineEdit()->setReadOnly(true);
        ui->comboStationID->addItems(qWorkManager->getStationIDList());
        ui->comboStationID->setCurrentIndex(-1);
    } while (0);

    do
    {
        connect(ui->comboStationID, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectStationID()));
        connect(ui->editOperatorCard, SIGNAL(returnPressed()), this, SLOT(accept()));

        connect(ui->buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    } while (0);
}

void DialogOperatorLogin::setStationID(const QString &stationID)
{
    ui->comboStationID->setCurrentText(stationID);
}

QString DialogOperatorLogin::getStationID() const
{
    return ui->comboStationID->currentText();
}

QString DialogOperatorLogin::getOperatorCard() const
{
    return ui->editOperatorCard->text();
}

void DialogOperatorLogin::slotSelectStationID()
{
    ui->editOperatorCard->clear();
    ui->editOperatorCard->setFocus();
}
