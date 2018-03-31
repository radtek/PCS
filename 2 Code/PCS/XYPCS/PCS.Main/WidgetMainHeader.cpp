#include "WidgetMainHeader.h"
#include "ui_WidgetMainHeader.h"

WidgetMainHeader::WidgetMainHeader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetMainHeader)
{
    ui->setupUi(this);
}

WidgetMainHeader::~WidgetMainHeader()
{
    delete ui;
}

void WidgetMainHeader::setWorkshopName(const QString &name)
{
    ui->labelWorkshop->setText(name);
}

void WidgetMainHeader::setWorklineName(const QString &name)
{
    ui->labelWorkline->setText(name);
}
