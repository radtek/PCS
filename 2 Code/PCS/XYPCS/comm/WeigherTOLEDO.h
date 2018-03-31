#ifndef XYSERIALSCALE_H
#define XYSERIALSCALE_H

#include "CommDevice.h"

class WeigherTOLEDO : public CommDevice
{
    Q_OBJECT
public:
    enum class ItemType
    {
        Reset = 1,   //复位
        Zero = 2,    //去皮
        Read = 3,    //读取
    };

public:
    explicit WeigherTOLEDO(CommManager *manager);
    virtual ~WeigherTOLEDO();
protected:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);
    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
private:
    bool isStable;

    QMap<ItemType, QString> itemTypeMap;
};

#endif // XYSERIALSCALE_H
