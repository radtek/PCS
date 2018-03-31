#include "SerialWidget.h"
#include "ui_SerialWidget.h"
#include "CommSerialPort.h"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QTime>

XYSerialWidget::XYSerialWidget(CommSerialPort *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XYSerialWidget)
    , serial(s)
{
    ui->setupUi(this);

    Q_ASSERT(s != nullptr);

    initialWidget();
    initialSignal();
}

XYSerialWidget::~XYSerialWidget()
{
    delete ui;
}

void XYSerialWidget::initialSignal()
{
    connect(ui->button_ClearRecv, SIGNAL(clicked()), ui->list_RecvData, SLOT(clear()));
    connect(ui->button_ClearSend, SIGNAL(clicked()), ui->list_SendData, SLOT(clear()));
    connect(ui->button_OpenPort, SIGNAL(clicked()), this, SLOT(openSerialPort()));
    connect(ui->button_ClosePort, SIGNAL(clicked()), this, SLOT(closeSerialPort()));
    connect(ui->button_SendData,SIGNAL(clicked()), this, SLOT(sendSerialData()));

    connect(serial, SIGNAL(recvSerialData(QByteArray)), this, SLOT(recvSerialData(QByteArray)));
    connect(serial, SIGNAL(recvSerialError(QString)), this, SLOT(recvSerialError(QString)));
    connect(this, SIGNAL(sendSerialData(QByteArray)), serial, SLOT(sendSerialData(QByteArray)));
    connect(this, SIGNAL(openPort(QString,QString,QString,QString,QString)),
            serial, SLOT(openPort(QString,QString,QString,QString,QString)));
    connect(this, SIGNAL(closePort()), serial, SLOT(closePort()));

}

void XYSerialWidget::initialWidget()
{
    ui->combo_PortName->addItems(CommSerialPort::getPortNameList());
    ui->combo_BaudRate->addItems(CommSerialPort::getBaudRateList());
    ui->combo_DataBits->addItems(CommSerialPort::getDataBitsList());
    ui->combo_Parity->addItems(CommSerialPort::getParityList());
    ui->combo_StopBits->addItems(CommSerialPort::getStopBitsList());

    ui->combo_BaudRate->setCurrentIndex(3);
    ui->combo_DataBits->setCurrentIndex(3);
    ui->combo_Parity->setCurrentIndex(0);
    ui->combo_StopBits->setCurrentIndex(0);
}

void XYSerialWidget::openSerialPort()
{
    emit openPort(ui->combo_PortName->currentText(),
                  ui->combo_BaudRate->currentText(),
                  ui->combo_DataBits->currentText(),
                  ui->combo_Parity->currentText(),
                  ui->combo_StopBits->currentText());

}

void XYSerialWidget::closeSerialPort()
{
    emit closePort();
}

void XYSerialWidget::recvSerialData(const QByteArray &data)
{
    qDebug() << "XYSerialWidget:" << QThread::currentThreadId();

    QListWidget *list = ui->list_RecvData;

    QByteArray temp = data;

    temp.replace("\r", "<CR>");
    temp.replace("\n", "<LF>");
    temp.replace("\t", "<TAB>");

    QString text = QString("%1 : %2")
                   .arg(QTime::currentTime().toString())
                   .arg(QString(temp.data()));

    list->addItem(text);
    int row = list->count();
    if (row > 50)
    {
        delete list->takeItem(0);
        row = 50;
    }
    list->setCurrentRow(row - 1);
}

void XYSerialWidget::sendSerialData()
{
    QListWidget *list = ui->list_SendData;
    QString temp = ui->edit_SendData->text();

    QString text = QString("%1 : %2")
                   .arg(QTime::currentTime().toString())
                   .arg(temp);

    list->addItem(text);
    int row = list->count();
    if (row > 50)
    {
        delete list->takeItem(0);
        row = 50;
    }
    list->setCurrentRow(row - 1);

    QByteArray data = temp.toLocal8Bit();

    data.replace("<CR>", "\r");
    data.replace("<LF>", "\n");
    data.replace("<TAB>", "\t");

    emit sendSerialData(data);
}

void XYSerialWidget::recvSerialError(const QString &error)
{
    QMessageBox::critical(this,"Error",error);
}
