#include "OhmmeterKEITHLY.h"

OhmmeterKEITHLY::OhmmeterKEITHLY(CommManager *manager)
    : CommDevice(manager)
    , testItem(ItemType::ShortRes)
{
    itemTypeMap.insert(ItemType::ShortRes,  "短路电阻");
    itemTypeMap.insert(ItemType::BridgeRes, "工作电阻");
    itemTypeMap.insert(ItemType::HipotRes,  "绝缘电阻");

    setItemTypeList(itemTypeMap.values());
}

OhmmeterKEITHLY::~OhmmeterKEITHLY()
{

}

void OhmmeterKEITHLY::recvData(const QByteArray &data)
{
    qDebug() << "XYSerialResistance recv data" << data;

    QByteArray temp(data);
    temp.replace("\x13", "");
    temp.replace("\x11", "");

    if (temp.size() == 0)
        return;

    double resistance = temp.trimmed().toDouble();

    switch (testItem)
    {
    case ItemType::ShortRes:        //mΩ
        resistance *= 1E3;
        break;

    case ItemType::BridgeRes:       //Ω
        break;

    case ItemType::HipotRes:       //MΩ
        resistance /= 1E6;
        break;
    }

    if (resistance >= 1E10)
        resistance = 1E10 - 1;

    if (resistance < 0.0)
        resistance = 0;

    commRecvData(itemTypeMap.value(testItem), resistance);

}

void OhmmeterKEITHLY::sendData(const QByteArray &data)
{
    CommDevice::sendData(data);
}

void OhmmeterKEITHLY::commRecvData(const QString &type, const QVariant &value)
{
    CommDevice::commRecvData(type, value);
}

void OhmmeterKEITHLY::commSendData(const QString &type, const QVariant &value)
{
    Q_UNUSED(value);
    QByteArray data;

    testItem = itemTypeMap.key(type);

    switch (testItem)
    {
    case ItemType::ShortRes:
    case ItemType::BridgeRes:
        data.append("*RST\r");
        data.append("FORM:ELEM READ\r");
        data.append("ROUT:OPEN:ALL\r");
        data.append("SOUR:CURR 0.05,(@127)\r");
        data.append("CALC1:FORM S1I\r");
        data.append("CALC1:STAT ON\r");
        data.append("ROUT:MULT:CLOS (@101,102,117,118,121)\r");
        data.append("READ?\r");
        data.append("ROUT:MULT:OPEN (@121)\r");
        break;

    case ItemType::HipotRes:
        data.append("*RST\r");
        data.append("FORM:ELEM READ\r");
        data.append("ROUT:OPEN:ALL\r");
        data.append("SOUR:VOLT 500,(@128)\r");
        data.append("CALC1:FORM S1V\r");
        data.append("CALC1:STAT ON\r");
        data.append("ROUT:MULT:CLOS (@101,103,108,116,118,121,122,123)\r");
        data.append("READ?\r");
        data.append("ROUT:MULT:OPEN (@121)\r");
        break;
    }

    sendData(data);
}
