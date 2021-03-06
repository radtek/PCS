#ifndef XYSCANNERSERIAL_H
#define XYSCANNERSERIAL_H

#include "CommDevice.h"

class ScannerHONEYWELL : public CommDevice
{
    Q_OBJECT
public:
    enum class ItemType
    {
        Barcode,          //条码
    };
public:
    explicit ScannerHONEYWELL(CommManager *manager);
    virtual ~ScannerHONEYWELL();

protected:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);
    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
private:
    QMap<ItemType, QString> itemTypeMap;
};

#endif // XYSCANNERSERIAL_H
