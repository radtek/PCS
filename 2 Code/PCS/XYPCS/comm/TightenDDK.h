#ifndef XYTIGHTENSERIAL_H
#define XYTIGHTENSERIAL_H

#include "CommDevice.h"

class TightenDDK : public CommDevice
{
    Q_OBJECT
public:
    enum class ItemType
    {
        Torque,          //扭矩
        Angle,          //落座角
    };
public:
    explicit TightenDDK(CommManager *manager);
    virtual ~TightenDDK();

protected:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);
    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
private:
    QMap<ItemType, QString> itemTypeMap;
};

#endif // XYTIGHTENSERIAL_H
