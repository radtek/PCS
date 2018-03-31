#include "OPCGroupEditDialog.h"
#include "ui_OPCGroupEditDialog.h"
#include "OPCGroup.h"
#include <QMessageBox>

OPCGroupEditDialog::OPCGroupEditDialog(XYOPCGroup *group, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OPCGroupEditDialog)
    , type(GroupProperties)
    , group(group)
{
    ui->setupUi(this);

    initialWidget();
    setGroupState();
    setEditType(type);
}

OPCGroupEditDialog::~OPCGroupEditDialog()
{
    delete ui;
}


void OPCGroupEditDialog::initialWidget()
{
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

void OPCGroupEditDialog::setGroupState()
{
    if (group == nullptr) return;

    OPCGROUPSTATE state;
    group->GetGroupState(&state);

    ui->editGroupName->setText(QString::fromWCharArray(state.szName));
    ui->checkActive->setChecked(state.bActive == TRUE);
    ui->spinUpdateRate->setValue(int(state.dwUpdateRate));
    ui->spinTimeBias->setValue(int(state.lTimeBias));
    ui->spinDeadband->setValue(int(state.fPercentDeadband));
    ui->spinLanguageID->setValue(int(state.dwLCID));
}

void OPCGroupEditDialog::setEditType(GroupEditType type)
{
    this->type = type;

    switch (type)
    {
    case GroupClone:
        this->setWindowTitle("Clone Group");
        ui->editGroupName->clear();
        ui->editGroupName->setReadOnly(false);
        ui->spinUpdateRate->setReadOnly(true);
        ui->spinTimeBias->setReadOnly(true);
        ui->spinDeadband->setReadOnly(true);
        ui->spinLanguageID->setReadOnly(true);
        break;
    case GroupModify:
        this->setWindowTitle("Modify Group");
        ui->editGroupName->setReadOnly(false);
        ui->spinUpdateRate->setReadOnly(false);
        ui->spinTimeBias->setReadOnly(false);
        ui->spinDeadband->setReadOnly(false);
        ui->spinLanguageID->setReadOnly(false);
        break;
    case GroupProperties:
        this->setWindowTitle("Group Properties");
        ui->editGroupName->setReadOnly(true);
        ui->spinUpdateRate->setReadOnly(true);
        ui->spinTimeBias->setReadOnly(true);
        ui->spinDeadband->setReadOnly(true);
        ui->spinLanguageID->setReadOnly(true);
        break;
    }
}

void OPCGroupEditDialog::accept()
{
    switch (type)
    {
    case GroupClone:
    {
        LPCWSTR szName = (LPCWSTR)ui->editGroupName->text().unicode();
        XYOPCGroup *pGroup = group->CloneGroup(szName);
        if (pGroup == NULL)
        {
            QMessageBox::critical(this, "Error", "Clone the group failed!");
            return;
        }
        group = pGroup;
        break;
    }
    case GroupModify:
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
        if (!group->SetGroupState(&state))
        {
            QMessageBox::critical(this, "Error", "Modify the group failed!");
            return;
        }
        break;
    }
    default:
        break;
    }

    QDialog::accept();
}
