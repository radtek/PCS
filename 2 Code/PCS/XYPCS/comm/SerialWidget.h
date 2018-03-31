#ifndef SCANNERCOMMWIDGET_H
#define SCANNERCOMMWIDGET_H

#include "CommDefine.h"

namespace Ui
{
class XYSerialWidget;
}

class CommSerialPort;

class XYSerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XYSerialWidget(CommSerialPort *s, QWidget *parent = nullptr);
    virtual ~XYSerialWidget();

public slots:
    void openSerialPort();
    void closeSerialPort();
    void sendSerialData();

    void recvSerialError(const QString &error);
    void recvSerialData(const QByteArray &data);

signals:
    void openPort(const QString &portname,
                  const QString &baudrate,
                  const QString &databits,
                  const QString &parity,
                  const QString &stopbits);
    void closePort();

    void sendSerialData(const QByteArray &data);

private:
    void initialSignal();
    void initialWidget();
private:
    Ui::XYSerialWidget *ui;
    CommSerialPort *serial;
};

#endif // SCANNERCOMMWIDGET_H
