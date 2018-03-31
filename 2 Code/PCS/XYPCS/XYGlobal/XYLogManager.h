/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLogManager.h
 * @brief   日志输出类
 * @version 1.0
 * @note
 *  不采用独立线程，及时输出到控制台和文件
 * *****************************************************************************
 */

#ifndef XYTECH_XYLOGMANAGER_H
#define XYTECH_XYLOGMANAGER_H

#include <QtCore>

class XYLogManagerPrivate;

class XYLogManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(XYLogManager)
    Q_DECLARE_PRIVATE(XYLogManager)

public:
    enum ELogLevel
    {
        LG_ALL = 0,
        LG_DBG,
        LG_INF,
        LG_WAR,
        LG_ERR,
        LG_OFF,
    };

public:
    static XYLogManager *instance();
    static void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf);

public:
    XYLogManager();
    virtual ~XYLogManager();

private:
    void execMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &buf);

public Q_SLOTS:
    void reloadConfig();

private:
    const QScopedPointer<XYLogManagerPrivate> d_ptr;
    QScopedPointer<QTimer> d_timer;
};

#endif
