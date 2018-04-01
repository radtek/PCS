#include "DialogCraftFixture.h"
#include "WorkManager.h"
#include "ui_DialogCraftFixture.h"
#include <QDebug>
#include <QMessageBox>

DialogCraftFixture::DialogCraftFixture(const QSqlRecord &record, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCraftFixture)
    , dbRecord(record)
{
    ui->setupUi(this);
    initialDialog();
    connect(ui->comboBoxFixtureType, SIGNAL(currentTextChanged(const QString &)), this, SLOT(slotSelectFixture(const QString &)));
    QObject::connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
    QObject::connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

DialogCraftFixture::~DialogCraftFixture()
{
    delete ui;
}

void DialogCraftFixture::slotAccept()
{
    if (ui->comboBoxFixtureType->currentText().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择工装类型");
        return;
    }
    if (ui->lineEditFixtureSerial->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请输入工装序号");
        return;
    }
    fixtureType = ui->comboBoxFixtureType->currentText();
    fixtureSerial = ui->lineEditFixtureSerial->text();

    accept();
}

void DialogCraftFixture::slotSelectFixture(const QString &FixtureType)
{
}

void DialogCraftFixture::initialDialog()
{
    ui->comboBoxFixtureType->clear();
    ui->lineEditFixtureSerial->clear();
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT DISTINCT [FixtureType]
                  FROM [PCS_Base_Fixture] )");
    if (!query.exec())
    {
        qDebug().noquote() << query.lastError().text();
        return;
    }
    while (query.next())
    {
        ui->comboBoxFixtureType->addItem(query.value("FixtureType").toString());
    }

    ui->lineEditFixtureSerial->setText(dbRecord.value("FixtureSerial").toString());
    if (!dbRecord.value("FixtureType").toString().isEmpty())
    {
        ui->comboBoxFixtureType->setCurrentText(dbRecord.value("FixtureType").toString());
        slotSelectFixture(dbRecord.value("FixtureType").toString());
        ui->comboBoxFixtureType->setEnabled(false);
    }
    else
    {
        ui->comboBoxFixtureType->setCurrentIndex(-1);
        ui->comboBoxFixtureType->setEnabled(true);
    }
}
