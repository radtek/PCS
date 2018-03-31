#ifndef COMMTCPSOCKET_H
#define COMMTCPSOCKET_H

#include "CommDefine.h"

class CommTcpSocket : public QTcpSocket
{
public:
    explicit CommTcpSocket(QObject *parent = nullptr);
    virtual ~CommTcpSocket();
};

#endif // COMMTCPSOCKET_H
