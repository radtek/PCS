#ifndef COMMDEFINE_H
#define COMMDEFINE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QDebug>

#include <QThread>
#include <QWidget>

#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTcpServer>
#include <QTcpSocket>

extern const QString COMM_CONFIG_XML;

//设备型号
enum class DeviceModel
{
    Scanner_HONEYWELL,              //条码枪，HONEYWELL
    Scanner_DATALOGIC,              //条码枪，DATALOGIC
    Tighten_DDK,                    //拧紧枪，DDK
    Tighten_ATLAS,                  //拧紧枪，ATLAS
    Tighten_DEPRAG,                 //拧紧枪，DEPRAG
    Clamp_OETIKER,                  //大力钳，OETIKER
    Ohmmeter_KEITHLY,               //欧姆表，KEITHLY
    Ohmmeter_CHROMA,                //欧姆表，CHROMA
    Ohmmeter_KEYSIGHT,              //欧姆表，KEYSIGHT
    Balance_TOLEDO,                 //电子天平，TOLEDO
    Weigher_TOLEDO,                 //电子台秤，TOLEDO
};

extern const QMap<DeviceModel, QString> deviceModelMap;

//串口参数
struct SerialPortParam
{
    QString portname;
    QString baudrate;
    QString databits;
    QString parity;
    QString stopbits;
};

//网口参数
struct TcpSocketParam
{
    QString address;
    quint16 port;
};


class CommManager;
class CommDevice;
class CommSerialPort;
class CommTcpSocket;

typedef QList<CommDevice *> CommDeviceList;


#endif // COMMDEFINE_H
