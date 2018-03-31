#include "CommSerialPort.h"
#include <QMetaType>
#include <QThread>
#include <QDebug>

CommSerialPort::CommSerialPort(QObject *parent)
    : QSerialPort(parent)
    , waitTime(25)
    , channelID(0)
{
    qRegisterMetaType<QSerialPort::SerialPortError>("SerialError");
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(this, SIGNAL(errorOccurred(SerialError)), this, SLOT(occurrError(SerialError)));
}

CommSerialPort::~CommSerialPort()
{
    closePort();
}

QStringList CommSerialPort::getPortNameList()
{
    QStringList list;
    for (auto port : QSerialPortInfo::availablePorts())
        list << port.portName();
    return list;
}

QStringList CommSerialPort::getBaudRateList()
{
    QStringList list;
    list << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    return list;
}

QStringList CommSerialPort::getDataBitsList()
{
    QStringList list;
    list << "5" << "6" << "7" << "8";
    return list;
}

QStringList CommSerialPort::getParityList()
{
    QStringList list;
    list << "None" << "Even" << "Odd" << "Space" << "Mark";
    return list;
}

QStringList CommSerialPort::getStopBitsList()
{
    QStringList list;
    list << "1" << "1.5" << "2";
    return list;
}

QSerialPort::BaudRate CommSerialPort::baudrateFromString(const QString &string)
{
    if (string == "1200")
        return Baud1200;
    if (string == "2400")
        return Baud2400;
    if (string == "4800")
        return Baud4800;
    if (string == "9600")
        return Baud9600;
    if (string == "19200")
        return Baud19200;
    if (string == "38400")
        return Baud38400;
    if (string == "57600")
        return Baud57600;
    if (string == "115200")
        return Baud115200;
    return Baud9600;
}

QSerialPort::DataBits CommSerialPort::databitsFromString(const QString &string)
{
    if (string == "5")
        return Data5;
    if (string == "6")
        return Data6;
    if (string == "7")
        return Data7;
    if (string == "8")
        return Data8;
    return Data8;
}

QSerialPort::Parity CommSerialPort::parityFromString(const QString &string)
{
    if (string == "None" || string == "N")
        return NoParity;
    if (string == "Even" || string == "E")
        return EvenParity;
    if (string == "Odd" || string == "O")
        return OddParity;
    if (string == "Space" || string == "S")
        return SpaceParity;
    if (string == "Mark" || string == "M")
        return MarkParity;
    return NoParity;
}

QSerialPort::StopBits CommSerialPort::stopbitsFromString(const QString &string)
{
    if (string == "1")
        return OneStop;
    if (string == "1.5")
        return OneAndHalfStop;
    if (string == "2")
        return TwoStop;
    return OneStop;
}

bool CommSerialPort::openPort(const QString &portname,
                              const QString &baudrate,
                              const QString &databits,
                              const QString &parity,
                              const QString &stopbits)
{
    setPortName(portname);
    setBaudRate(baudrateFromString(baudrate));
    setDataBits(databitsFromString(databits));
    setParity(parityFromString(parity));
    setStopBits(stopbitsFromString(stopbits));

    qDebug() << portName() + " Port Open.";

    return open(ReadWrite);
}

bool CommSerialPort::openPort(const QString &portname,
                              const QString &portinfo)
{
    setPortName(portname);
    setBaudRate(baudrateFromString(portinfo.split(',').at(0)));
    setDataBits(databitsFromString(portinfo.split(',').at(1)));
    setParity(parityFromString(portinfo.split(',').at(2)));
    setStopBits(stopbitsFromString(portinfo.split(',').at(3)));

    return open(ReadWrite);
}

void CommSerialPort::closePort()
{
    if (isOpen())
        close();

    qDebug() << portName() + " Port Close.";
}

//数据接收事件
void CommSerialPort::receiveData()
{
    //等待数据全部到达，此时会有多次触发
    waitForReadyRead(waitTime);

    //如果可用数据为0，则返回
    if (0 == bytesAvailable())
        return;

    QByteArray data = readAll();

//    qDebug() << "XYSerialPort receiveData()" << data;
    //发送数据
    emit recvSerialData(data);
}

void CommSerialPort::sendSerialData(const QByteArray &data)
{
    clear();
    write(data);
}

//错误处理事件
void CommSerialPort::occurrError(QSerialPort::SerialPortError error)
{
    QString string;
    switch (error)
    {
    case QSerialPort::NoError :
        string = "No error occurred.";
        return;
    case QSerialPort::DeviceNotFoundError :
        string = "Attempting to open an non-existing device.";
        break;
    case QSerialPort::PermissionError :
        string = "Attempting to open an already opened device by another process "
                 "or a user not having enough permission and credentials to open.";
        break;
    case QSerialPort::OpenError :
        string = "Attempting to open an already opened device in this object.";
        break;
    case QSerialPort::NotOpenError :
        string = "The operation is executed that can only be successfully performed if the device is open.";
        break;
    case QSerialPort::ParityError :
        string = "Parity error detected by the hardware while reading data.";
        break;
    case QSerialPort::FramingError :
        string = "Framing error detected by the hardware while reading data.";
        break;
    case QSerialPort::BreakConditionError :
        string = "Break condition detected by the hardware on the input line.";
        break;
    case QSerialPort::WriteError :
        string = "An I/O error occurred while writing the data.";
        break;
    case QSerialPort::ReadError :
        string = "An I/O error occurred while reading the data.";
        break;
    case QSerialPort::ResourceError :
        string = "An I/O error occurred when a resource becomes unavailable.";
        break;
    case QSerialPort::UnsupportedOperationError :
        string = "The requested device operation is not supported or prohibited by the running operating system.";
        break;
    case QSerialPort::TimeoutError :
        string = "A timeout error occurred.";
        return;
    case QSerialPort::UnknownError :
        string = "An unidentified error occurred.";
        break;
    }

    QString port = portName().isEmpty() ? "COM" : portName();
    emit recvSerialError(QString("%1 : %2").arg(port).arg(string));
}






