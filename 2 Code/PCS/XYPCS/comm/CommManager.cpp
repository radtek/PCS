#include "CommManager.h"
#include "ScannerHONEYWELL.h"
#include "ScannerDATALOGIC.h"
#include "TightenDDK.h"
#include "ClampOETIKER.h"
#include "OhmmeterKEITHLY.h"
#include "WeigherTOLEDO.h"

CommManager::CommManager()
{
}

CommManager::~CommManager()
{
    endCommThread();
    removeAllDevice();
}

QStringList CommManager::getStationIDList() const
{
    QStringList list;

    for (auto device : deviceList)
        if (!list.contains(device->getStationID()))
            list.append(device->getStationID());

    return list;
}

QStringList CommManager::getDeviceIDList(const QString &stationID) const
{
    QStringList list;

    for (auto device : deviceList)
        if (stationID == device->getStationID())
            list.append(device->getDeviceID());

    return list;
}

QStringList CommManager::getItemTypeList(const QString &deviceID) const
{
    QStringList list;

    for (auto device : deviceList)
    {
        if (deviceID == device->getDeviceID())
        {
            list.append(device->getItemTypeList());
            break;
        }
    }

    return list;
}

QStringList CommManager::getItemIDList(const QString &stationID) const
{
    QStringList list;

    for (auto device : deviceList)
    {
        if (stationID == device->getStationID())
        {
            QString deviceID = device->getDeviceID();

            for (auto itemType : device->getItemTypeList())
            {
                list.append(QString("%1,%2").arg(deviceID).arg(itemType));
            }
        }
    }

    return list;
}

CommDeviceList CommManager::getDeviceList() const
{
    return deviceList;
}

CommDeviceList CommManager::getDeviceList(const QString &stationID) const
{
    CommDeviceList list;

    for (auto device : deviceList)
        if (stationID == device->getStationID())
            list.append(device);

    return list;
}

CommDevice *CommManager::createDevice(const QString &stationID, const QString &deviceID, const QString &deviceType)
{
    CommDevice *device = nullptr;

    switch (deviceModelMap.key(deviceType))
    {
    case DeviceModel::Scanner_HONEYWELL:        //条码枪(HONEYWELL)
        device = new ScannerHONEYWELL(this);
        break;

    case DeviceModel::Scanner_DATALOGIC:        //条码枪(DATALOGIC)
        device = new ScannerDATALOGIC(this);
        break;

    case DeviceModel::Tighten_DDK:              //拧紧枪(DDK)
        device = new TightenDDK(this);
        break;

    case DeviceModel::Tighten_ATLAS:            //拧紧枪(ATLAS)
        device = new CommDevice(this);
        break;

    case DeviceModel::Tighten_DEPRAG:           //拧紧枪(DEPRAG)
        device = new CommDevice(this);
        break;

    case DeviceModel::Clamp_OETIKER:            //大力钳(OETIKER)
        device = new CommDevice(this);
        break;

    case DeviceModel::Ohmmeter_KEITHLY:         //欧姆表(KEITHLY)
        device = new OhmmeterKEITHLY(this);
        break;

    case DeviceModel::Ohmmeter_CHROMA:          //欧姆表(CHROMA)
        device = new CommDevice(this);
        break;

    case DeviceModel::Ohmmeter_KEYSIGHT:        //欧姆表(KEYSIGHT)
        device = new CommDevice(this);
        break;

    case DeviceModel::Balance_TOLEDO:           //电子天平(TOLEDO)
        device = new CommDevice(this);
        break;

    case DeviceModel::Weigher_TOLEDO:           //电子台秤(TOLEDO)
        device = new WeigherTOLEDO(this);
        break;
    }

    device->setStationID(stationID);
    device->setDeviceID(deviceID);
    device->setDeviceType(deviceType);

    deviceList.append(device);

    return device;
}

void CommManager::removeDevice(CommDevice *device)
{
    if (deviceList.contains(device))
    {
        delete device;
        deviceList.removeOne(device);
    }
}

void CommManager::removeAllDevice()
{
    for (auto device : deviceList)
    {
        delete device;
    }

    deviceList.clear();
}

void CommManager::commWriteData(const QString &stationID, const DataDefine &data)
{
    QString deviceID = data.itemID.split(',').at(0);
    QString itemType = data.itemID.split(',').at(1);

    for (auto serial : deviceList)
    {
        if (serial->getStationID() == stationID &&
            serial->getDeviceID() == deviceID)
        {
            serial->commSendData(itemType, data.itemValue);
            return;
        }
    }
}

bool CommManager::readXmlFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "readXmlFile() file open() failed! filename:" << filename;
        return false;
    }

    QDomDocument document;
    QString errMsg;
    int errLine, errCol;

    if (!document.setContent(&file, &errMsg, &errLine, &errCol))
    {
        qDebug() << QString("Parse xml failed! filename: %1, err msg: %2, err line: %3, err col: %4")
                 .arg(filename).arg(errMsg).arg(errLine).arg(errCol);

        file.close();
        return false;
    }

    file.close();

    QDomElement rootNode = document.documentElement();

    do
    {
        QDomElement serialNode = rootNode.firstChildElement("serial");

        while (!serialNode.isNull())
        {
            QString stationID = serialNode.attribute("stationID");
            QString deviceID = serialNode.attribute("deviceID");
            QString deviceType = serialNode.attribute("deviceType");

            auto device = createDevice(stationID, deviceID, deviceType);

            QString deviceParam = serialNode.attribute("deviceParam");
            device->setPortName(deviceParam.split(',').at(0));
            device->setBaudRate(deviceParam.split(',').at(1));
            device->setDataBits(deviceParam.split(',').at(2));
            device->setParity(deviceParam.split(',').at(3));
            device->setStopBits(deviceParam.split(',').at(4));

            serialNode = serialNode.nextSiblingElement("serial");
        }
    }
    while (0);

    do
    {
        QDomElement socketNode = rootNode.firstChildElement("socket");

        while (!socketNode.isNull())
        {
            QString stationID = socketNode.attribute("stationID");
            QString deviceID = socketNode.attribute("deviceID");
            QString deviceType = socketNode.attribute("deviceType");

            auto device = createDevice(stationID, deviceID, deviceType);

            QString deviceParam = socketNode.attribute("deviceParam");
            device->setSocketAddr(deviceParam.split(',').at(0));
            device->setSocketPort(deviceParam.split(',').at(1));

            socketNode = socketNode.nextSiblingElement("socket");
        }
    }
    while (0);

    return true;
}

bool CommManager::saveXmlFile(const QString &filename)
{
    Q_UNUSED(filename);
    return true;
}

void CommManager::startCommThread()
{
    for (auto device : deviceList)
        device->startThread();
}

void CommManager::endCommThread()
{
    for (auto device : deviceList)
        device->endThread();
}
