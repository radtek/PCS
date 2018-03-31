#include "DialogDateCodeEdit.h"
#include "ui_DialogDateCodeEdit.h"

DialogDateCodeEdit::DialogDateCodeEdit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDateCodeEdit)
{
    ui->setupUi(this);

    connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

DialogDateCodeEdit::~DialogDateCodeEdit()
{
    delete ui;
}

void DialogDateCodeEdit::setDateFormula(const QString &formula)
{
    ui->editFormula->setText(formula);
}

QString DialogDateCodeEdit::getDateFormula() const
{
    return ui->editFormula->text();
}
