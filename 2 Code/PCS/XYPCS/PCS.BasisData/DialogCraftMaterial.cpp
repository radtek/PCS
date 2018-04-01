#include "DialogCraftMaterial.h"
#include "WorkManager.h"
#include "ui_DialogCraftMaterial.h"
#include <QDebug>
#include <QMessageBox>

DialogCraftMaterial::DialogCraftMaterial(const QSqlRecord &record, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCraftMaterial)
    , dbRecord(record)
{
    ui->setupUi(this);
    initialDialog();
    connect(ui->comboBoxMaterialID, SIGNAL(currentTextChanged(const QString &)), this, SLOT(slotSelectMaterial(const QString &)));
    QObject::connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
    QObject::connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

DialogCraftMaterial::~DialogCraftMaterial()
{
    delete ui;
}

void DialogCraftMaterial::slotSelectMaterial(const QString &MaterialID)
{
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [MaterialName]
                  ,[BarcodeRule]
                  FROM [PCS_Base_Material] WHERE [MaterialID]=? )");
    query.addBindValue(MaterialID);
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->lineEditMaterialName->setText(query.value("MaterialName").toString());
        ui->lineEditBarcodeRule->setText(query.value("BarcodeRule").toString());
    }
}

void DialogCraftMaterial::slotAccept()
{
    if (ui->comboBoxMaterialID->currentText().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择物料编号");
        return;
    }
    if (ui->lineEditSingleConsumeQuantity->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入单件消耗数量");
        return;
    }
    if (ui->lineEditShortWarningQuantity->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入缺料预警数量");
        return;
    }
    materialID = ui->comboBoxMaterialID->currentText();
    singleConsumeQuantity = ui->lineEditSingleConsumeQuantity->text();
    shortWarningQuantity = ui->lineEditShortWarningQuantity->text();

    accept();
}

void DialogCraftMaterial::initialDialog()
{
    ui->comboBoxMaterialID->clear();
    ui->lineEditBarcodeRule->clear();
    ui->lineEditMaterialName->clear();
    ui->lineEditSingleConsumeQuantity->clear();
    ui->lineEditShortWarningQuantity->clear();
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT DISTINCT [MaterialID]
                  FROM [PCS_Base_Material] )");
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->comboBoxMaterialID->addItem(query.value("MaterialID").toString());
    }

    ui->lineEditSingleConsumeQuantity->setText(dbRecord.value("SingleConsumeQuantity").toString());
    ui->lineEditShortWarningQuantity->setText(dbRecord.value("ShortWarningQuantity").toString());
    if (!dbRecord.value("MaterialID").toString().isEmpty())
    {
        ui->comboBoxMaterialID->setCurrentText(dbRecord.value("MaterialID").toString());
        slotSelectMaterial(dbRecord.value("MaterialID").toString());
        ui->comboBoxMaterialID->setEnabled(false);
    }
    else
    {
        ui->comboBoxMaterialID->setCurrentIndex(-1);
        ui->comboBoxMaterialID->setEnabled(true);
    }
}
