#ifndef XYRESISTANCESERIAL_H
#define XYRESISTANCESERIAL_H

#include "CommDevice.h"

class OhmmeterKEITHLY : public CommDevice
{
    Q_OBJECT
public:
    enum class ItemType
    {
        ShortRes = 1,
        BridgeRes = 2,
        HipotRes = 3,
    };

public:
    explicit OhmmeterKEITHLY(CommManager *manager);
    virtual ~OhmmeterKEITHLY();

protected:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);
    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
private:
    ItemType testItem;

    QMap<ItemType, QString> itemTypeMap;
};

#endif // XYRESISTANCESERIAL_H
