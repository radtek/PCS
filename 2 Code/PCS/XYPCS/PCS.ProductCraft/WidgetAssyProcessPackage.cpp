#include "WidgetAssyProcessPackage.h"
#include "CommManager.h"
#include "ui_WidgetAssyProcessPackage.h"

WidgetAssyProcessPackage::WidgetAssyProcessPackage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAssyProcessPackage)
{
    ui->setupUi(this);

    initialWidget();
}

WidgetAssyProcessPackage::~WidgetAssyProcessPackage()
{
    delete ui;
}

void WidgetAssyProcessPackage::initialWidget()
{
    connect(ui->buttonSaveParam, SIGNAL(clicked()), this, SLOT(saveParameter()));

    connect(ui->checkAssemblyEnable, SIGNAL(toggled(bool)), this, SLOT(slotAssemblyToggled(bool)));
    connect(ui->checkPackageEnable, SIGNAL(toggled(bool)), this, SLOT(slotPackageToggled(bool)));
}

void WidgetAssyProcessPackage::loadParameter(const QString &craftID, const QString &stationID)
{
    this->craftID = craftID;
    this->stationID = stationID;

    do
    {
        ui->comboScaleDeviceID->clear();
        ui->comboPrevStationID->clear();
    } while (0);

    do
    {
        stationIDList = qWorkManager->getStationIDList();
        deviceIDList = qCommManager->getDeviceIDList(stationID);

        ui->comboScaleDeviceID->addItems(deviceIDList);
        ui->comboPrevStationID->addItems(stationIDList);
    } while (0);

    QString filename = QString("%1/%2.%3.xml")
                           .arg(PROCESS_PACKAGE_PATH)
                           .arg(craftID)
                           .arg(stationID);

    if (AssyPackage_ReadXmlFile(filename, preparePara, assemblyPara, packagePara))
    {
        setWidgetData();
    }
    else
    {
        MessageBox_Critical("参数读取失败！");
    }
}

void WidgetAssyProcessPackage::saveParameter()
{
    getWidgetData();

    QString filename = QString("%1/%2.%3.xml")
                           .arg(PROCESS_PACKAGE_PATH)
                           .arg(craftID)
                           .arg(stationID);

    if (AssyPackage_SaveXmlFile(filename, preparePara, assemblyPara, packagePara))
    {
        MessageBox_Information("参数保存成功。");
    }
    else
    {
        MessageBox_Critical("参数保存失败！");
    }
}

void WidgetAssyProcessPackage::setWidgetData()
{
    do
    {
        ui->checkOperatorLogin->setChecked(preparePara.enableOperatorLogin);
        ui->checkMaterialInput->setChecked(preparePara.enableMaterialInput);
        ui->checkFixtureCheck->setChecked(preparePara.enableFixtureCheck);
        ui->checkMachineCheck->setChecked(preparePara.enableMachineCheck);
        ui->checkProductCheck->setChecked(preparePara.enableProductCheck);
    } while (0);

    do
    {
        ui->checkAssemblyEnable->setChecked(assemblyPara.enable);
        ui->checkAssemblyRescan->setChecked(assemblyPara.rescanBarcode);
        ui->checkAssemblyVerify->setChecked(assemblyPara.verifyBarcode);
        ui->spinAssyPrintCount->setValue(assemblyPara.printCount);
        ui->spinAssyRescanCount->setValue(assemblyPara.rescanCount);
        ui->comboPrevStationID->setCurrentText(assemblyPara.prevStationID);

        ui->checkPackageEnable->setChecked(packagePara.enable);
        ui->checkPackageRescan->setChecked(packagePara.rescanBarcode);
        ui->checkPackageVerify->setChecked(packagePara.verifyBarcode);
        ui->spinPackPrintCount->setValue(packagePara.printCount);
        ui->spinPackRescanCount->setValue(packagePara.rescanCount);
        ui->editPackageMarker->setText(packagePara.packageMarker);
        ui->comboScaleDeviceID->setCurrentText(packagePara.scaleDeviceID);
        ui->spinPackageNumber->setValue(packagePara.packageNumber);
        ui->dspinSingleWeight->setValue(packagePara.singleWeight);
        ui->dspinOffsetWeight->setValue(packagePara.offsetWeight);
    } while (0);

    do
    {
        slotAssemblyToggled(assemblyPara.enable);
        slotPackageToggled(packagePara.enable);
    } while (0);
}

void WidgetAssyProcessPackage::getWidgetData()
{
    do
    {
        preparePara.enableOperatorLogin = ui->checkOperatorLogin->isChecked();
        preparePara.enableMaterialInput = ui->checkMaterialInput->isChecked();
        preparePara.enableFixtureCheck = ui->checkFixtureCheck->isChecked();
        preparePara.enableMachineCheck = ui->checkMachineCheck->isChecked();
        preparePara.enableProductCheck = ui->checkProductCheck->isChecked();
    } while (0);

    do
    {
        assemblyPara.enable = ui->checkAssemblyEnable->isChecked();
        assemblyPara.rescanBarcode = ui->checkAssemblyRescan->isChecked();
        assemblyPara.verifyBarcode = ui->checkAssemblyVerify->isChecked();
        assemblyPara.printCount = ui->spinAssyPrintCount->value();
        assemblyPara.rescanCount = ui->spinAssyRescanCount->value();
        assemblyPara.prevStationID = ui->comboPrevStationID->currentText();
    } while (0);

    do
    {
        packagePara.enable = ui->checkPackageEnable->isChecked();
        packagePara.rescanBarcode = ui->checkPackageRescan->isChecked();
        packagePara.verifyBarcode = ui->checkPackageVerify->isChecked();
        packagePara.printCount = ui->spinPackPrintCount->value();
        packagePara.rescanCount = ui->spinPackRescanCount->value();
        packagePara.packageMarker = ui->editPackageMarker->text();
        packagePara.scaleDeviceID = ui->comboScaleDeviceID->currentText();
        packagePara.packageNumber = ui->spinPackageNumber->value();
        packagePara.singleWeight = ui->dspinSingleWeight->value();
        packagePara.offsetWeight = ui->dspinOffsetWeight->value();
    } while (0);
}

void WidgetAssyProcessPackage::slotAssemblyToggled(bool checked)
{
    ui->assemblyWidget_1->setEnabled(checked);
    ui->assemblyWidget_2->setEnabled(checked);
}

void WidgetAssyProcessPackage::slotPackageToggled(bool checked)
{
    ui->packageWidget_1->setEnabled(checked);
    ui->packageWidget_2->setEnabled(checked);
}
