#include "WeigherTOLEDO.h"

WeigherTOLEDO::WeigherTOLEDO(CommManager *manager)
    : CommDevice(manager)
    , isStable(false)
{
    itemTypeMap.insert(ItemType::Reset, "复位");
    itemTypeMap.insert(ItemType::Zero,  "清零");
    itemTypeMap.insert(ItemType::Read,  "读取");

    setItemTypeList(itemTypeMap.values());
}

WeigherTOLEDO::~WeigherTOLEDO()
{

}

void WeigherTOLEDO::recvData(const QByteArray &data)
{
//    qDebug() << "XYSerialScale recv data" << data;

    if (data.at(0) == 'I')
    {
        QByteArray temp = data.trimmed();
        temp.remove(0, 5);

        commRecvData(itemTypeMap.value(ItemType::Reset), temp);
    }

    if (data.at(0) == 'Z')
    {
        commRecvData(itemTypeMap.value(ItemType::Zero), data.at(2));
        return;
    }

    if (data.at(2) == 'D')
    {
        isStable = false;
        return;
    }

    if (data.at(2) == 'S' && !isStable)
    {
        double weight = data.mid(3, 12).trimmed().toDouble();

        isStable = true;

        commRecvData(itemTypeMap.value(ItemType::Read), weight);
    }
}

void WeigherTOLEDO::sendData(const QByteArray &data)
{
    CommDevice::sendData(data);
}

void WeigherTOLEDO::commRecvData(const QString &type, const QVariant &value)
{
    CommDevice::commRecvData(type, value);
}

void WeigherTOLEDO::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(value);
    QByteArray data;

    switch (itemTypeMap.key(type))
    {
    case ItemType::Reset:
        data = "@ \r\n";
        break;

    case ItemType::Zero:
        data = "Z \r\n";
        break;

    case ItemType::Read:
        data = "SIR \r\n";
        break;
    }

    sendData(data);
}
