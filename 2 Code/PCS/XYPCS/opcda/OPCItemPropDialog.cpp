#include "OPCItemPropDialog.h"
#include "ui_OPCItemPropDialog.h"

OPCItemPropDialog::OPCItemPropDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OPCItemPropDialog)
{
    ui->setupUi(this);
}

OPCItemPropDialog::~OPCItemPropDialog()
{
    delete ui;
}
