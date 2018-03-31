#include "DialogOrderPlanRemark.h"
#include "ui_DialogOrderPlanRemark.h"

DialogOrderPlanRemark::DialogOrderPlanRemark(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogOrderPlanRemark)
{
    ui->setupUi(this);

    initialWidget();
}

DialogOrderPlanRemark::~DialogOrderPlanRemark()
{
    delete ui;
}

void DialogOrderPlanRemark::initialWidget()
{
    connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

QString DialogOrderPlanRemark::getRemark() const
{
    return ui->editRemark->toPlainText();
}

void DialogOrderPlanRemark::setReason(const QString &reason)
{
    ui->labelReason->setText(reason);
}

void DialogOrderPlanRemark::setNotice(const QString &notice)
{
    ui->labelNotice->setText(notice);
}

