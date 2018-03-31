#ifndef XYSERIALCOMMON_H
#define XYSERIALCOMMON_H

#include "CommDefine.h"
#include "CommManager.h"
#include "CommSerialPort.h"
#include "CommTcpSocket.h"

class CommDevice : public QObject
{
    Q_OBJECT
public:
    explicit CommDevice(CommManager *manager);
    virtual ~CommDevice();
public:
    void startThread();
    void endThread();
public:
    virtual void recvData(const QByteArray &data);
    virtual void sendData(const QByteArray &data);

    virtual void commRecvData(const QString &type, const QVariant &value);
    virtual void commSendData(const QString &type, const QVariant &value);
public slots:
    void recvSerialData(const QByteArray &data);
    void recvSerialError(const QString &text);

signals:
    void openPort(const QString &portname,
                  const QString &baudrate,
                  const QString &databits,
                  const QString &parity,
                  const QString &stopbits);
    void openPort(const QString &portname,
                  const QString &portinfo);
    void closePort();

    void sendSerialData(const QByteArray &data);
public:
    inline QString getStationID() const { return stationID; }
    inline QString getDeviceID() const { return deviceID; }
    inline QString getDeviceType() const { return deviceType; }
    inline QStringList getItemTypeList() const { return itemTypeList; }

    inline void setStationID(const QString &id) { stationID = id; }
    inline void setDeviceID(const QString &id) { deviceID = id; }
    inline void setDeviceType(const QString &type) { deviceType = type; }
    inline void setItemTypeList(const QStringList &list) { itemTypeList = list; }

    inline void setPortName(const QString &portname) { serialParam.portname = portname;}
    inline void setBaudRate(const QString &baudrate) { serialParam.baudrate = baudrate;}
    inline void setDataBits(const QString &databits) { serialParam.databits = databits;}
    inline void setParity(const QString &parity) { serialParam.parity = parity;}
    inline void setStopBits(const QString &stopbits) { serialParam.stopbits = stopbits;}

    inline void setSocketAddr(const QString &address)  { socketParam.address = address;}
    inline void setSocketPort(const QString &port)  { socketParam.port = quint16(port.toUInt());}
protected:
    QString stationID;
    QString deviceID;
    QString deviceType;
    QStringList itemTypeList;

    SerialPortParam serialParam;
    TcpSocketParam socketParam;

    CommManager *manager;
    CommSerialPort *serial;
    QThread *thread;
};

#endif // XYSERIALCOMMON_H
