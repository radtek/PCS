#include "TightenDDK.h"

TightenDDK::TightenDDK(CommManager *manager)
    : CommDevice(manager)
{
    itemTypeMap.insert(ItemType::Torque, "扭矩");
    itemTypeMap.insert(ItemType::Angle,  "落座角");

    setItemTypeList(itemTypeMap.values());
}

TightenDDK::~TightenDDK()
{

}

void TightenDDK::recvData(const QByteArray &data)
{
    qDebug() << "XYSerialTighten recv data" << data;

    if (data.size() != 78)
        return;

    double torque = data.mid(28, 5).trimmed().toDouble();
    double angle = data.mid(22, 4).trimmed().toDouble();

    commRecvData(itemTypeMap.value(ItemType::Torque), torque);
    commRecvData(itemTypeMap.value(ItemType::Angle), angle);
}

void TightenDDK::sendData(const QByteArray &data)
{
    CommDevice::sendData(data);
}

void TightenDDK::commRecvData(const QString &type, const QVariant &value)
{
    CommDevice::commRecvData(type, value);
}

void TightenDDK::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(type); Q_UNUSED(value);
}
