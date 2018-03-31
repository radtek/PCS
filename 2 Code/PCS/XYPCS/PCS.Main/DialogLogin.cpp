#include "DialogLogin.h"
#include "ui_DialogLogin.h"
#include <GlobalData.h>
#include <PCSApplication.h>

DialogLogin::DialogLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->initUI();

    QObject::connect(ui->labelMinimize, SIGNAL(clicked()), this, SLOT(showMinimized()));
    QObject::connect(ui->labelReject, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(ui->buttonLogin, SIGNAL(clicked()), this, SLOT(Slot_QPushButton_clicked_Login()));
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

void DialogLogin::initUI()
{
    do
    {
        if (!PCSApplication::instance()->isOnline())
        {
            ui->buttonLogin->setText("离线登录");
        }

        ui->labelMainTitle->setText(qCfgManager->getMainTitle());
        ui->labelSubTitle->setText(qCfgManager->getSubTitle());
        ui->labelWorkShop->setText(QString("车间：%1").arg(qCfgManager->getWorkShop()));
        ui->labelWorkLine->setText(QString("产线：%1").arg(qCfgManager->getWorkLine()));
        ui->labelVersion->setText(QString("版本：%1").arg(qCfgManager->getVersion()));
    } while (0);

    // 设置背景图片
    do
    {
        this->setAutoFillBackground(true);

        QPalette palette;
        QPixmap pixmap(":login_background");
        palette.setBrush(QPalette::Window, QBrush(pixmap));
        palette.setColor(QPalette::WindowText, Qt::white);
        this->setPalette(palette);
    } while (0);

    ui->labelMinimize->setStaticPixmap(QPixmap(":/login_minimize"));
    ui->labelMinimize->setActivePixmap(QPixmap(":/login_minimize_active"));
    ui->labelMinimize->setClickPixmap(QPixmap(":/login_minimize_click"));

    ui->labelReject->setStaticPixmap(QPixmap(":/login_cancel"));
    ui->labelReject->setActivePixmap(QPixmap(":/login_cancel_active"));
    ui->labelReject->setClickPixmap(QPixmap(":/login_cancel_click"));
}

void DialogLogin::Slot_QPushButton_clicked_Login()
{
    qDebug() << "Slot_QPushButton_clicked_Login";

    QString strUser = ui->comboUser->currentText();
    QString strPass = ui->editPassword->text();

    if (strUser == strPass)
    {
        accept();
    }
    else
    {
        MessageBox_Warning("用户名密码错误！");
    }
}
