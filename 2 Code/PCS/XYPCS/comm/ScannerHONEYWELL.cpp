#include "ScannerHONEYWELL.h"

ScannerHONEYWELL::ScannerHONEYWELL(CommManager *manager)
    : CommDevice(manager)
{
    itemTypeMap.insert(ItemType::Barcode,  "条码");

    setItemTypeList(itemTypeMap.values());
}

ScannerHONEYWELL::~ScannerHONEYWELL()
{

}


void ScannerHONEYWELL::recvData(const QByteArray &data)
{
    qDebug() << "XYSerialScanner recv data" << data;

    if (data.size() == 0)
        return;

    QString barcode = data.trimmed();

    commRecvData(itemTypeMap.value(ItemType::Barcode), barcode);
}

void ScannerHONEYWELL::sendData(const QByteArray &data)
{
    CommDevice::sendData(data);
}

void ScannerHONEYWELL::commRecvData(const QString &type, const QVariant &value)
{
    CommDevice::commRecvData(type, value);
}

void ScannerHONEYWELL::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(type); Q_UNUSED(value);
}
