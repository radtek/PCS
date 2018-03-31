#include "CraftFormulaWidget.h"
#include "ui_CraftFormulaWidget.h"

CraftFormulaWidget::CraftFormulaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CraftFormulaWidget)
{
    ui->setupUi(this);
}

CraftFormulaWidget::~CraftFormulaWidget()
{
    delete ui;
}



