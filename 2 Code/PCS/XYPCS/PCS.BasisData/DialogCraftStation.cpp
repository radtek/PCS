#include "DialogCraftStation.h"
#include "WorkManager.h"
#include "ui_DialogCraftStation.h"

DialogCraftStation::DialogCraftStation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCraftStation)
{
    ui->setupUi(this);
}

DialogCraftStation::~DialogCraftStation()
{
    delete ui;
}

void DialogCraftStation::slotAccept()
{
}
