#include "DialogDateFormatInfo.h"
#include "ui_DialogDateFormatInfo.h"

DialogDateFormatInfo::DialogDateFormatInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDateFormatInfo)
{
    ui->setupUi(this);

    connect(ui->buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
}

DialogDateFormatInfo::~DialogDateFormatInfo()
{
    delete ui;
}
