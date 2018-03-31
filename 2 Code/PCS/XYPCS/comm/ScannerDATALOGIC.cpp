#include "ScannerDATALOGIC.h"

ScannerDATALOGIC::ScannerDATALOGIC(CommManager *manager)
    : CommDevice(manager)
{
    itemTypeMap.insert(ItemType::Barcode,  "条码");

    setItemTypeList(itemTypeMap.values());
}

ScannerDATALOGIC::~ScannerDATALOGIC()
{

}


void ScannerDATALOGIC::recvData(const QByteArray &data)
{
    qDebug() << "XYSerialScanner recv data" << data;

    if (data.size() == 0)
        return;

    QString barcode = data.trimmed();

    commRecvData(itemTypeMap.value(ItemType::Barcode), barcode);
}

void ScannerDATALOGIC::sendData(const QByteArray &data)
{
    CommDevice::sendData(data);
}

void ScannerDATALOGIC::commRecvData(const QString &type, const QVariant &value)
{
    CommDevice::commRecvData(type, value);
}

void ScannerDATALOGIC::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(type); Q_UNUSED(value);
}
