#include "DialogPowerLogin.h"
#include "ui_DialogPowerLogin.h"

DialogPowerLogin::DialogPowerLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogPowerLogin)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QObject::connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(Slot_QPushButton_clicked_Accept()));
}

DialogPowerLogin::~DialogPowerLogin()
{
    delete ui;
}

void DialogPowerLogin::setTitle(const QString &title)
{
    ui->labelTitle->setText(title);
}

void DialogPowerLogin::hideClose(bool hide)
{
    setWindowFlags(hide
                       ? Qt::WindowTitleHint
                       : Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

QString DialogPowerLogin::getQAUser() const
{
    return ui->editUsername->text();
}

void DialogPowerLogin::Slot_QPushButton_clicked_Accept()
{
    QString username = ui->editUsername->text();

    if (username.isEmpty())
    {
        ui->labelNotice->setText("请输入用户名！");
        ui->editUsername->setFocus();
        return;
    }

    QString password = ui->editPassword->text();

    if (password.isEmpty())
    {
        ui->labelNotice->setText("请输入密码！");
        ui->editPassword->setFocus();
        return;
    }

    bool success = true;

    if (!success)
    {
        ui->labelNotice->setText("用户校验失败！");
        ui->editUsername->setFocus();
        ui->editUsername->clear();
        ui->editPassword->clear();

        return;
    }

    accept();
}
