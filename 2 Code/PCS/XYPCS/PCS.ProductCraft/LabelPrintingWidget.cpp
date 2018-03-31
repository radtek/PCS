#include "LabelPrintingWidget.h"
#include "ui_LabelPrintingWidget.h"

LabelPrintingWidget::LabelPrintingWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::LabelPrintingWidget)
{
    ui->setupUi(this);
}

LabelPrintingWidget::~LabelPrintingWidget()
{
    delete ui;
}

