#include "WidgetMainStatus.h"
#include "ui_WidgetMainStatus.h"

WidgetMainStatus::WidgetMainStatus(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetMainStatus)
{
    ui->setupUi(this);
}

WidgetMainStatus::~WidgetMainStatus()
{
    delete ui;
}
