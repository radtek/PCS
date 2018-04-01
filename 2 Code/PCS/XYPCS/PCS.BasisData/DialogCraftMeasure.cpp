#include "DialogCraftMeasure.h"
#include "WorkManager.h"
#include "ui_DialogCraftMeasure.h"
#include <QDebug>
#include <QMessageBox>

DialogCraftMeasure::DialogCraftMeasure(const QSqlRecord &record, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCraftMeasure)
    , dbRecord(record)
{
    ui->setupUi(this);
    initialDialog();
    connect(ui->comboBoxMeasureType, SIGNAL(currentTextChanged(const QString &)), this, SLOT(slotSelectMeasure(const QString &)));
    QObject::connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
    QObject::connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

DialogCraftMeasure::~DialogCraftMeasure()
{
    delete ui;
}

void DialogCraftMeasure::slotSelectMeasure(const QString &MeasureType)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [MeasureName]
                  ,[Unit]
                  FROM [PCS_Base_Measure] WHERE [MeasureType]=? )");
    query.addBindValue(MeasureType);
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->lineEditMeasureName->setText(query.value("MeasureName").toString());
        ui->lineEditUnit->setText(query.value("Unit").toString());
    }
}

void DialogCraftMeasure::slotAccept()
{
    if (ui->comboBoxMeasureType->currentText().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择测试类型");
        return;
    }
    if (ui->lineEditMeasureSerial->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入测试序号");
        return;
    }
    if (ui->lineEditStandardValue->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入标准值");
        return;
    }
    if (ui->lineEditUpperLimit->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入上限值");
        return;
    }
    if (ui->lineEditLowerLimit->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入下限值");
        return;
    }
    measureType = ui->comboBoxMeasureType->currentText();
    measureSerial = ui->lineEditMeasureSerial->text();
    standardValue = ui->lineEditStandardValue->text();
    upperLimit = ui->lineEditUpperLimit->text();
    lowerLimit = ui->lineEditLowerLimit->text();

    accept();
}

void DialogCraftMeasure::initialDialog()
{
    ui->comboBoxMeasureType->clear();
    ui->lineEditMeasureName->clear();
    ui->lineEditUnit->clear();
    ui->lineEditMeasureSerial->clear();
    ui->lineEditStandardValue->clear();
    ui->lineEditUpperLimit->clear();
    ui->lineEditLowerLimit->clear();
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT DISTINCT [MeasureType]
                  FROM [PCS_Base_Measure] )");
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->comboBoxMeasureType->addItem(query.value("MeasureType").toString());
    }

    ui->lineEditMeasureSerial->setText(dbRecord.value("MeasureOrder").toString());
    ui->lineEditStandardValue->setText(dbRecord.value("StandardValue").toString());
    ui->lineEditUpperLimit->setText(dbRecord.value("UpperLimit").toString());
    ui->lineEditLowerLimit->setText(dbRecord.value("LowerLimit").toString());
    if (!dbRecord.value("MeasureType").toString().isEmpty())
    {
        ui->comboBoxMeasureType->setCurrentText(dbRecord.value("MeasureType").toString());
        slotSelectMeasure(dbRecord.value("MeasureType").toString());
        ui->comboBoxMeasureType->setEnabled(false);
    }
    else
    {
        ui->comboBoxMeasureType->setCurrentIndex(-1);
        ui->comboBoxMeasureType->setEnabled(true);
    }
}
