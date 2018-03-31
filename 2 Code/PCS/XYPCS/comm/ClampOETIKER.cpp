#include "ClampOETIKER.h"

ClampOETIKER::ClampOETIKER(CommManager *manager)
    : CommDevice(manager)
{
    itemTypeMap.insert(ItemType::Force, "夹紧力");
    itemTypeMap.insert(ItemType::Space, "间隙");

    setItemTypeList(itemTypeMap.values());
}

ClampOETIKER::~ClampOETIKER()
{

}

void ClampOETIKER::recvData(const QByteArray &data)
{
    qDebug() << "XYSerialClamp recv data" << data;

    if (data.size() != 112)
        return;

    double force = data.split('\t').at(12).trimmed().toDouble();
    double space = data.split('\t').at(8).trimmed().toDouble();

    commRecvData(itemTypeMap.value(ItemType::Force), force);
    commRecvData(itemTypeMap.value(ItemType::Space), space);
}

void ClampOETIKER::sendData(const QByteArray &data)
{
    CommDevice::sendData(data);
}

void ClampOETIKER::commRecvData(const QString &type, const QVariant &value)
{
    CommDevice::commRecvData(type, value);
}

void ClampOETIKER::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(type); Q_UNUSED(value);
}
