#ifndef SCANNERDATALOGIC_H
#define SCANNERDATALOGIC_H

#include "CommDevice.h"

class ScannerDATALOGIC : public CommDevice
{
    Q_OBJECT
public:
    enum class ItemType
    {
        Barcode,          //条码
    };
public:
    explicit ScannerDATALOGIC(CommManager *manager);
    virtual ~ScannerDATALOGIC();

protected:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);
    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
private:
    QMap<ItemType, QString> itemTypeMap;
};

#endif // SCANNERDATALOGIC_H
