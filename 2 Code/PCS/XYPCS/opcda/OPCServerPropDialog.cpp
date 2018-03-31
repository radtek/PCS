#include "OPCServerPropDialog.h"
#include "ui_OPCServerPropDialog.h"
#include "OPCServer.h"

OPCServerPropDialog::OPCServerPropDialog(XYOPCServer *pServer, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OPCServerPropDialog)
    , server(pServer)
{
    ui->setupUi(this);

    initialWidget();

    setServerProp();
}

OPCServerPropDialog::~OPCServerPropDialog()
{
    delete ui;
}

void OPCServerPropDialog::initialWidget()
{
    this->setWindowTitle("Server Properties");
    this->setMinimumWidth(360);
    this->setMinimumHeight(520);

    ui->editProgID->setReadOnly(true);
    ui->editVendor->setReadOnly(true);
    ui->editUserType->setReadOnly(true);
    ui->editMachineName->setReadOnly(true);
    ui->editCLSID->setReadOnly(true);
    ui->editState->setReadOnly(true);
    ui->editStartTime->setReadOnly(true);
    ui->editCurrentTime->setReadOnly(true);
    ui->editUpdateTime->setReadOnly(true);
    ui->editGroupCount->setReadOnly(true);
    ui->editBaudWidth->setReadOnly(true);
    ui->editMajorVersion->setReadOnly(true);
    ui->editMinorVersion->setReadOnly(true);
    ui->editBuildNumber->setReadOnly(true);

    connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->buttonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

void OPCServerPropDialog::setServerProp()
{
    if (server == Q_NULLPTR)
        return;

    OPCSERVERINFO info;
    server->GetServerInfo(&info);
    OPCSERVERSTATUS status;
    server->GetServerStatus(&status);

    ui->editProgID->setText(QString::fromWCharArray(info.szProgID));
    ui->editVendor->setText(QString::fromWCharArray(status.szVendorInfo));
    ui->editUserType->setText(QString::fromWCharArray(info.szUserType));
    ui->editMachineName->setText(QString::fromWCharArray(info.szMachineName));
    ui->editCLSID->setText(QUuid(info.clsid).toString());
    ui->editState->setText(SERVERSTATE_to_String(status.dwServerState));
    ui->editStartTime->setText(FILETIME_to_QDateTime(status.ftStartTime).toString());
    ui->editCurrentTime->setText(FILETIME_to_QDateTime(status.ftCurrentTime).toString());
    ui->editUpdateTime->setText(FILETIME_to_QDateTime(status.ftLastUpdateTime).toString());
    ui->editGroupCount->setText(QString::number(status.dwGroupCount));
    ui->editBaudWidth->setText(QString::number(status.dwBandWidth));
    ui->editMajorVersion->setText(QString::number(status.wMajorVersion));
    ui->editMinorVersion->setText(QString::number(status.wMinorVersion));
    ui->editBuildNumber->setText(QString::number(status.wBuildNumber));
}
