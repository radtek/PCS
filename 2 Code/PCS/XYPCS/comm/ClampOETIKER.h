#ifndef XYCLAMPERSERIAL_H
#define XYCLAMPERSERIAL_H

#include "CommDevice.h"

class ClampOETIKER : public CommDevice
{
    Q_OBJECT
public:
    enum class ItemType
    {
        Force,          //夹紧力
        Space,          //间隙
    };
public:
    explicit ClampOETIKER(CommManager *manager);
    virtual ~ClampOETIKER();

protected:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);

    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
private:
    QMap<ItemType, QString> itemTypeMap;
};

#endif // XYCLAMPERSERIAL_H
