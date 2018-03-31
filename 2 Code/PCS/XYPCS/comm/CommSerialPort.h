#ifndef XYSERIALPORT_H
#define XYSERIALPORT_H

#include "CommDefine.h"

class CommSerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit CommSerialPort(QObject *parent = nullptr);
    virtual ~CommSerialPort();

public:
    //获取串口属性列表
    static QStringList getPortNameList();
    static QStringList getBaudRateList();
    static QStringList getDataBitsList();
    static QStringList getParityList();
    static QStringList getStopBitsList();

    //字符串转属性
    static BaudRate baudrateFromString(const QString &string);
    static DataBits databitsFromString(const QString &string);
    static Parity parityFromString(const QString &string);
    static StopBits stopbitsFromString(const QString &string);
public slots:
    //打开串口
    bool openPort(const QString &portname,
                  const QString &baudrate,
                  const QString &databits,
                  const QString &parity,
                  const QString &stopbits);
    //打开串口，portinfo格式：9600,8,N,1
    bool openPort(const QString &portname,
                  const QString &portinfo);

    void closePort();

    //串口发出，接收数据
    void receiveData();
    //串口发出，出现错误
    void occurrError(QSerialPort::SerialPortError error);
    //窗体发出，发送数据
    void sendSerialData(const QByteArray &data);
signals:
    //接收数据，发出信号
    void recvSerialData(const QByteArray &data);
    //出现错误，发出信号
    void recvSerialError(const QString &error);

public:
    inline void setWaitTime(int msec) { waitTime = msec; }
    inline void setChannelID(int id) { channelID = id; }
private:
    int waitTime;
    int channelID;
};

#endif // XYSERIALPORT_H
