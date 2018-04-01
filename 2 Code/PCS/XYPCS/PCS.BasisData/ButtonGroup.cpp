#include "ButtonGroup.h"
#include "ui_ButtonGroup.h"

ButtonGroup::ButtonGroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
    connect(ui->buttonAdd, SIGNAL(clicked()), this, SIGNAL(signalAdd()));
    connect(ui->buttonModify, SIGNAL(clicked()), this, SIGNAL(signalModify()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SIGNAL(signalDelete()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SIGNAL(signalSave()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SIGNAL(signalCancel()));
    setGroupState(GroupState::Unselect);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::setGroupState(GroupState state)
{
    switch (state)
    {
    case GroupState::Unselect:
        ui->buttonAdd->setEnabled(true);
        ui->buttonModify->setEnabled(false);
        ui->buttonDelete->setEnabled(false);
        ui->buttonSave->setEnabled(false);
        ui->buttonCancel->setEnabled(false);
        break;
    case GroupState::Select:
        ui->buttonAdd->setEnabled(true);
        ui->buttonModify->setEnabled(true);
        ui->buttonDelete->setEnabled(true);
        ui->buttonSave->setEnabled(false);
        ui->buttonCancel->setEnabled(false);
        break;
    case GroupState::Edit:
        ui->buttonAdd->setEnabled(false);
        ui->buttonModify->setEnabled(false);
        ui->buttonDelete->setEnabled(false);
        ui->buttonSave->setEnabled(true);
        ui->buttonCancel->setEnabled(true);
        break;
    }
}
