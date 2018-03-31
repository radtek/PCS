#include "WidgetAssyProcessProduce.h"
#include "ui_WidgetAssyProcessProduce.h"

WidgetAssyProcessProduce::WidgetAssyProcessProduce(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAssyProcessProduce)
{
    ui->setupUi(this);

    initialWidget();
}

WidgetAssyProcessProduce::~WidgetAssyProcessProduce()
{
    delete ui;
}

void WidgetAssyProcessProduce::initialWidget()
{
    connect(ui->buttonSaveParam, SIGNAL(clicked()), this, SLOT(saveParameter()));
}

void WidgetAssyProcessProduce::loadParameter(const QString &craftID)
{
    this->craftID = craftID;

    QString filename = QString("%1/%2.xml")
                       .arg(PROCESS_PRODUCE_PATH)
                       .arg(craftID);

    if (AssyProduce_ReadXmlFile(filename, producePara))
    {
        setWidgetData();
    }
    else
    {
        MessageBox_Critical("参数读取失败！");
    }
}

void WidgetAssyProcessProduce::saveParameter()
{
    getWidgetData();

    QString filename = QString("%1/%2.xml")
                       .arg(PROCESS_PRODUCE_PATH)
                       .arg(craftID);

    if (AssyProduce_SaveXmlFile(filename, producePara))
    {
        MessageBox_Information("参数保存成功。");
    }
    else
    {
        MessageBox_Critical("参数保存失败！");
    }
}

void WidgetAssyProcessProduce::setWidgetData()
{
    do
    {
        ui->checkFirstProductQA->setChecked(producePara.enableFirstProductQA);
        ui->checkFinalProductQA->setChecked(producePara.enableFinalProductQA);
    }
    while (0);

    do
    {
        ui->checkMistakeProof01->setChecked(producePara.enableMistakeProof01);
        ui->checkMistakeProof02->setChecked(producePara.enableMistakeProof02);
        ui->checkMistakeProof03->setChecked(producePara.enableMistakeProof03);
        ui->checkMistakeProof04->setChecked(producePara.enableMistakeProof04);
        ui->checkMistakeProof05->setChecked(producePara.enableMistakeProof05);
        ui->checkMistakeProof06->setChecked(producePara.enableMistakeProof06);
        ui->checkMistakeProof07->setChecked(producePara.enableMistakeProof07);
        ui->checkMistakeProof08->setChecked(producePara.enableMistakeProof08);
        ui->checkMistakeProof09->setChecked(producePara.enableMistakeProof09);
        ui->checkMistakeProof10->setChecked(producePara.enableMistakeProof10);
        ui->checkMistakeProof11->setChecked(producePara.enableMistakeProof11);
    }
    while (0);
}

void WidgetAssyProcessProduce::getWidgetData()
{
    do
    {
        producePara.enableFirstProductQA = ui->checkFirstProductQA->isChecked();
        producePara.enableFinalProductQA = ui->checkFinalProductQA->isChecked();
    }
    while (0);

    do
    {
        producePara.enableMistakeProof01 = ui->checkMistakeProof01->isChecked();
        producePara.enableMistakeProof02 = ui->checkMistakeProof02->isChecked();
        producePara.enableMistakeProof03 = ui->checkMistakeProof03->isChecked();
        producePara.enableMistakeProof04 = ui->checkMistakeProof04->isChecked();
        producePara.enableMistakeProof05 = ui->checkMistakeProof05->isChecked();
        producePara.enableMistakeProof06 = ui->checkMistakeProof06->isChecked();
        producePara.enableMistakeProof07 = ui->checkMistakeProof07->isChecked();
        producePara.enableMistakeProof08 = ui->checkMistakeProof08->isChecked();
        producePara.enableMistakeProof09 = ui->checkMistakeProof09->isChecked();
        producePara.enableMistakeProof10 = ui->checkMistakeProof10->isChecked();
        producePara.enableMistakeProof11 = ui->checkMistakeProof11->isChecked();
    }
    while (0);
}
