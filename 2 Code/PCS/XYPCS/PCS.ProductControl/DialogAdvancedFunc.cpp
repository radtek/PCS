#include "DialogAdvancedFunc.h"
#include "WorkManager.h"
#include "ui_DialogAdvancedFunc.h"
DialogAdvancedFunc::DialogAdvancedFunc(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAdvancedFunc)
{
    ui->setupUi(this);
}

DialogAdvancedFunc::~DialogAdvancedFunc()
{
    delete ui;
}
