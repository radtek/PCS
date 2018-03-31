#include "DialogProcessStepData.h"
#include "ui_DialogProcessStepData.h"

DialogProcessStepData::DialogProcessStepData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogProcessStepData)
{
    ui->setupUi(this);

    initialWidget();
}

DialogProcessStepData::~DialogProcessStepData()
{
    delete ui;
}

void DialogProcessStepData::initialWidget()
{
    do
    {
        ui->comboInteraction->addItems(stepTypeStringMap.values());
        ui->comboInteraction->setCurrentIndex(-1);
        ui->editMaxMeasure->setValidator(new QIntValidator(0, 99));
        ui->editMaxMeasure->setText("0");
    }
    while (0);

    do
    {
        connect(ui->buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    }
    while (0);
}

void DialogProcessStepData::setOpcItemIDList(const QStringList &list)
{
    ui->comboStartRequset->addItems(list);
    ui->comboStartResponse->addItems(list);
    ui->comboSkipResponse->addItems(list);

    ui->comboMeasureRequest->addItems(list);
    ui->comboSuccessResponse->addItems(list);
    ui->comboFailedResponse->addItems(list);

    ui->comboStartRequset->setCurrentIndex(-1);
    ui->comboStartResponse->setCurrentIndex(-1);
    ui->comboSkipResponse->setCurrentIndex(-1);

    ui->comboMeasureRequest->setCurrentIndex(-1);
    ui->comboSuccessResponse->setCurrentIndex(-1);
    ui->comboFailedResponse->setCurrentIndex(-1);
}

void DialogProcessStepData::setWidgetData(const StepItem &item)
{
    ui->comboInteraction->setCurrentText(item.interaction);
    ui->editDescription->setText(item.description);
    ui->editMaxMeasure->setText(QString::number(item.maxMeasure));
    ui->checkIsDisabled->setChecked(item.isDisabled);

    ui->comboStartRequset->setCurrentText(item.startRequest.itemID);
    ui->comboStartResponse->setCurrentText(item.startResponse.itemID);
    ui->comboSkipResponse->setCurrentText(item.skipResponse.itemID);
    ui->comboMeasureRequest->setCurrentText(item.measureRequest.itemID);
    ui->comboSuccessResponse->setCurrentText(item.successResponse.itemID);
    ui->comboFailedResponse->setCurrentText(item.failedResponse.itemID);

    ui->editStartRequset->setText(item.startRequest.itemValue.toString());
    ui->editStartResponse->setText(item.startResponse.itemValue.toString());
    ui->editSkipResponse->setText(item.skipResponse.itemValue.toString());
    ui->editMeasureRequest->setText(item.measureRequest.itemValue.toString());
    ui->editSuccessResponse->setText(item.successResponse.itemValue.toString());
    ui->editFailedResponse->setText(item.failedResponse.itemValue.toString());
}

StepItem DialogProcessStepData::getWidgetData() const
{
    StepItem para;

    para.interaction = ui->comboInteraction->currentText();
    para.description = ui->editDescription->text();
    para.maxMeasure = ui->editMaxMeasure->text().toInt();
    para.isDisabled = ui->checkIsDisabled->isChecked();

    para.startRequest.itemID = ui->comboStartRequset->currentText();
    para.startResponse.itemID = ui->comboStartResponse->currentText();
    para.skipResponse.itemID = ui->comboSkipResponse->currentText();
    para.measureRequest.itemID = ui->comboMeasureRequest->currentText();
    para.successResponse.itemID = ui->comboSuccessResponse->currentText();
    para.failedResponse.itemID = ui->comboFailedResponse->currentText();

    para.startRequest.itemValue = ui->editStartRequset->text();
    para.startResponse.itemValue = ui->editStartResponse->text();
    para.skipResponse.itemValue = ui->editSkipResponse->text();
    para.measureRequest.itemValue = ui->editMeasureRequest->text();
    para.successResponse.itemValue = ui->editSuccessResponse->text();
    para.failedResponse.itemValue = ui->editFailedResponse->text();

    return para;
}
