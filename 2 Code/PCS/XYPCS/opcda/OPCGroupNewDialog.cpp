#include "OPCGroupNewDialog.h"
#include "ui_OPCGroupNewDialog.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include <QMessageBox>

OPCGroupNewDialog::OPCGroupNewDialog(XYOPCServer *server, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OPCGroupNewDialog)
    , server(server)
    , group(nullptr)
{
    ui->setupUi(this);

    initialWidget();
}

OPCGroupNewDialog::~OPCGroupNewDialog()
{
    delete ui;
}

void OPCGroupNewDialog::initialWidget()
{
    this->setWindowTitle("New Group");
    this->setMinimumHeight(320);
    this->setMinimumHeight(260);

    ui->editGroupName->clear();
    ui->checkActive->setChecked(true);
    ui->spinUpdateRate->setValue(100);
    ui->spinTimeBias->setValue(0);
    ui->spinDeadband->setValue(0);
    ui->spinLanguageID->setValue(0x0409);

    connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->buttonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

void OPCGroupNewDialog::accept()
{
    OPCGROUPSTATE state;
    state.szName = (LPWSTR)ui->editGroupName->text().unicode();
    state.bActive = ui->checkActive->isChecked() ? TRUE : FALSE;
    state.dwUpdateRate = (DWORD)ui->spinUpdateRate->value();
    state.lTimeBias = (LONG)ui->spinTimeBias->value();
    state.fPercentDeadband = (FLOAT)ui->spinDeadband->value();
    state.dwLCID = (DWORD)ui->spinLanguageID->value();
    state.hClientGroup = 0L;
    state.hServerGroup = 0L;

    group = server->AddGroup(&state);

    if (group == nullptr)
    {
        QMessageBox::critical(this, "Error", "Add new group failed!");
        return;
    }

    QDialog::accept();
}
