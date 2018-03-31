#include "CommDevice.h"

CommDevice::CommDevice(CommManager *manager)
    : manager(manager)
    , serial(new CommSerialPort)
    , thread(new QThread)
{
    connect(this, SIGNAL(openPort(QString, QString, QString, QString, QString)),
            serial, SLOT(openPort(QString, QString, QString, QString, QString)));
    connect(this, SIGNAL(openPort(QString, QString)), serial, SLOT(openPort(QString, QString)));
    connect(this, SIGNAL(closePort()), serial, SLOT(closePort()));

    connect(serial, SIGNAL(recvSerialData(QByteArray)), this, SLOT(recvSerialData(QByteArray)));
    connect(serial, SIGNAL(recvSerialError(QString)), this, SLOT(recvSerialError(QString)));
    connect(this, SIGNAL(sendSerialData(QByteArray)), serial, SLOT(sendSerialData(QByteArray)));
}

CommDevice::~CommDevice()
{
    endThread();

    delete thread;
    delete serial;
}

void CommDevice::startThread()
{
    serial->moveToThread(thread);
    thread->start();

    openPort(serialParam.portname,
             serialParam.baudrate,
             serialParam.databits,
             serialParam.parity,
             serialParam.stopbits);
}

void CommDevice::endThread()
{
    closePort();

    thread->quit();

    while (thread->isRunning());
}

void CommDevice::recvSerialData(const QByteArray &data)
{
    recvData(data);
}

void CommDevice::recvData(const QByteArray &data)
{
    Q_UNUSED(data);
}

void CommDevice::sendData(const QByteArray &data)
{
    emit sendSerialData(data);
}

void CommDevice::recvSerialError(const QString &text)
{
    ErrorDefine error;

    error.deviceID = deviceID;
    error.errorText = text;

    emit manager->commErrorOccurred(stationID, error);
}

void CommDevice::commRecvData(const QString &type, const QVariant &value)
{
    DataDefine data;

    data.itemID = QString("%1,%2").arg(deviceID).arg(type);
    data.itemValue = value;

    emit manager->commDataReceived(stationID, data);
}

void CommDevice::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(type); Q_UNUSED(value);
}
