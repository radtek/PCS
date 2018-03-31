/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLogManager.cpp
 *
 * @example
 * @code
 * int main()
 * {
 *      qInstallMessageHandler(XYLogManager::OutputMessage);
 *      qDebug() << "Test Debug()";
 *      qInfo() << "Test Info()";
 * }
 * @endcode
 *
 * @brief
 *  Default configure file is `config.ini`.
 *
 * @date 2018-03-27 @author weiheng             @note create this file
 * @date 2018-03-28 @author weiheng             @note remove level `qFatal`
 * @date 2018-03-30 @author weiheng             @note add console color
 * *****************************************************************************
 */

#include "XYLogManager.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#define DEFAULT_CONFIG_LOGMANAGER "config.ini"

#define DEFAULT_CONFIG_LG_DIR "./run/"
#define DEFAULT_CONFIG_LG_FILE "mes"
#define DEFAULT_CONFIG_LG_SUFFIX ".log"
#define DEFAULT_CONFIG_LG_MAXCOUNT 100
#define DEFAULT_CONFIG_LG_MAXSIZE 10
#define DEFAULT_CONFIG_LG_LEVEL static_cast<quint32>(XYLogManager::LG_WAR)
#define DEFAULT_CONFIG_LG_PATTERN                                                                                         \
    "%{time yyyy-MM-dd HH:mm:ss.zzz} "                                                                                    \
    "%{if-debug}DBG%{endif}%{if-info}INF%{endif}%{if-warning}WAR%{endif}%{if-critical}ERR%{endif}%{if-fatal}CRI%{endif} " \
    "PID:%{pid} TID:%{threadid} "                                                                                         \
    "%{message}"

#define TAF_CONFIG_LGGROUP "LOG"
#define TAG_CONFIG_LG_PATH "Path"
#define TAG_CONFIG_LG_FILE "File"
#define TAG_CONFIG_LG_MCNT "MaxCount"
#define TAG_CONFIG_LG_MSIZ "MaxSize"
#define TAG_CONFIG_LG_DLVL "DefaultLevel"
#define TAG_CONFIG_LG_PATT "Pattern"

#define CONSOLE_COLOR_BEGIN_DEBUG "\033[0m"
#define CONSOLE_COLOR_BEGIN_INFO "\033[0m"
#define CONSOLE_COLOR_BEGIN_WARNING "\033[47;31;1m"
#define CONSOLE_COLOR_BEGIN_CRITICAL "\033[40;33;1m"

#define CONSOLE_COLOR_END "\033[0m"

const QMap<QtMsgType, QString> consoleColorBegin = {
    {QtDebugMsg, CONSOLE_COLOR_BEGIN_DEBUG},
    {QtInfoMsg, CONSOLE_COLOR_BEGIN_INFO},
    {QtWarningMsg, CONSOLE_COLOR_BEGIN_WARNING},
    {QtCriticalMsg, CONSOLE_COLOR_BEGIN_CRITICAL},
    {QtFatalMsg, CONSOLE_COLOR_BEGIN_CRITICAL},
};

const QMap<QtMsgType, QString> consoleColorEnd = {
    {QtDebugMsg, CONSOLE_COLOR_END},
    {QtInfoMsg, CONSOLE_COLOR_END},
    {QtWarningMsg, CONSOLE_COLOR_END},
    {QtCriticalMsg, CONSOLE_COLOR_END},
    {QtFatalMsg, CONSOLE_COLOR_END},
};

class XYLogManagerPrivate
{
    Q_DISABLE_COPY(XYLogManagerPrivate)
    Q_DECLARE_PUBLIC(XYLogManager)

public:
    XYLogManagerPrivate(XYLogManager *q);
    ~XYLogManagerPrivate();

    void init();
    void changeDir();
    void changeFile();
    void write(const QString &message);

    XYLogManager *const q_ptr;

    QMutex mutex;
    QFile file;

    QString dirName;
    QString fileName;
    quint32 maxCount;
    quint32 maxSize;
    quint32 currLevel;
};

XYLogManagerPrivate::XYLogManagerPrivate(XYLogManager *q)
    : q_ptr(q)
    , mutex(QMutex::Recursive)
{
}

XYLogManagerPrivate::~XYLogManagerPrivate()
{
    file.close();
}

void XYLogManagerPrivate::init()
{
    dirName = DEFAULT_CONFIG_LG_DIR;
    fileName = DEFAULT_CONFIG_LG_FILE;
    maxCount = DEFAULT_CONFIG_LG_MAXCOUNT;
    maxSize = DEFAULT_CONFIG_LG_MAXSIZE;
    currLevel = DEFAULT_CONFIG_LG_LEVEL;

    changeDir();
}

void XYLogManagerPrivate::changeDir()
{
    QMutexLocker locker(&mutex);

    QDir dir(".");
    if (dir.mkpath(dirName))
    {
        file.close();
        file.setFileName(dirName + fileName + DEFAULT_CONFIG_LG_SUFFIX);
        file.open(QIODevice::Append);
    }
}

void XYLogManagerPrivate::changeFile()
{
    QMutexLocker locker(&mutex);

    QString strOldName = dirName + fileName;
    QString strNewName = strOldName + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QFile::rename(strOldName + DEFAULT_CONFIG_LG_SUFFIX, strNewName + DEFAULT_CONFIG_LG_SUFFIX);

    QDir dir(dirName);
    QStringList entryList = dir.entryList(QStringList(fileName + "*"));
    int num = entryList.size() - static_cast<int>(maxCount);
    if (num > 0)
    {
        for (int i = 0; i < num; ++i)
        {
            QFile::remove(dirName + entryList[i]);
        }
    }
}

void XYLogManagerPrivate::write(const QString &message)
{
    QMutexLocker locker(&mutex);

    if (file.isOpen())
    {
        if (file.size() > maxSize * (1 * 1024 * 1024))
        {
            file.close();
            changeFile();
        }
    }
    else
    {
        changeDir();
    }

    if (file.isOpen())
    {
        QTextStream stream(&file);
        stream << message << endl;
        stream.flush();
    }
}

///////////////////////////////////////////////////////////////////////////////

Q_GLOBAL_STATIC(XYLogManager, theInstance);

XYLogManager *XYLogManager::instance()
{
    return theInstance();
}

void XYLogManager::OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf)
{
    XYLogManager::instance()->execMessageHandler(type, context, buf);
}

XYLogManager::XYLogManager()
    : d_ptr(new XYLogManagerPrivate(this))
    , d_timer(new QTimer)
{
    d_func()->init();

    reloadConfig();

    QObject::connect(d_timer.data(), &QTimer::timeout, this, &XYLogManager::reloadConfig);
    d_timer->start(30000);

    /// qInstallMessageHandler最好不要在该函数中设置，不然会导致初始化时多次调用instance引起死锁。
}

XYLogManager::~XYLogManager()
{
}

void XYLogManager::execMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &buf)
{
    QString message = qFormatLogMessage(type, context, buf);

#ifdef Q_OS_WIN
    QString console = consoleColorBegin[type] + message + consoleColorEnd[type] + QLatin1Char('\n');
    OutputDebugString(reinterpret_cast<const wchar_t *>(console.utf16()));
#else
    fprintf(stderr, "%s\n", message.toLocal8Bit().constData());
    fflush(stderr);
#endif

    Q_D(XYLogManager);
    if (d->currLevel == LG_OFF)
    {
        return;
    }

    if (d->currLevel != LG_ALL)
    {
        quint32 msgLevel = d->currLevel;
        switch (type)
        {
        case QtDebugMsg:
            msgLevel = static_cast<int>(ELogLevel::LG_DBG);
            break;
        case QtInfoMsg:
            msgLevel = static_cast<int>(ELogLevel::LG_INF);
            break;
        case QtWarningMsg:
            msgLevel = static_cast<int>(ELogLevel::LG_WAR);
            break;
        case QtCriticalMsg:
        default:
            msgLevel = static_cast<int>(ELogLevel::LG_ERR);
            break;
        }
        if (msgLevel < d->currLevel)
        {
            return;
        }
    }

    d->write(message);
}

void XYLogManager::reloadConfig()
{
    QString pattern = DEFAULT_CONFIG_LG_PATTERN;
    QString dirName = DEFAULT_CONFIG_LG_DIR;
    QString fileName = DEFAULT_CONFIG_LG_FILE;
    quint32 maxCount = DEFAULT_CONFIG_LG_MAXCOUNT;
    quint32 maxSize = DEFAULT_CONFIG_LG_MAXSIZE;
    quint32 DefaultLevel = DEFAULT_CONFIG_LG_LEVEL;

    bool bSuccess = true;
    do
    {
        QSettings settings(DEFAULT_CONFIG_LOGMANAGER, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        if (settings.status() != QSettings::NoError)
        {
            bSuccess = false;
            break;
        }

        settings.beginGroup(TAF_CONFIG_LGGROUP);

        pattern = settings.value(TAG_CONFIG_LG_PATT, DEFAULT_CONFIG_LG_PATTERN).toString();
        if (pattern.isEmpty())
        {
            pattern = DEFAULT_CONFIG_LG_PATTERN;
        }

        dirName = settings.value(TAG_CONFIG_LG_PATH, DEFAULT_CONFIG_LG_DIR).toString();
        if (dirName.right(1) != QLatin1Char('/') && dirName.right(1) != QLatin1Char('\\'))
        {
            dirName.append(QLatin1Char('/'));
        }

        fileName = settings.value(TAG_CONFIG_LG_FILE, DEFAULT_CONFIG_LG_FILE).toString();
        if (fileName.isEmpty())
        {
            fileName = DEFAULT_CONFIG_LG_FILE;
        }

        maxCount = settings.value(TAG_CONFIG_LG_MCNT, DEFAULT_CONFIG_LG_MAXCOUNT).toUInt();

        maxSize = settings.value(TAG_CONFIG_LG_MSIZ, DEFAULT_CONFIG_LG_MAXSIZE).toUInt();
        if (maxSize == 0)
        {
            maxSize = DEFAULT_CONFIG_LG_MAXSIZE;
        }

        DefaultLevel = settings.value(TAG_CONFIG_LG_DLVL, DEFAULT_CONFIG_LG_LEVEL).toUInt();
        if (DefaultLevel > static_cast<quint32>(ELogLevel::LG_OFF))
        {
            DefaultLevel = DEFAULT_CONFIG_LG_LEVEL;
        }

        settings.endGroup();
    } while (0);

    if (bSuccess)
    {
        Q_D(XYLogManager);

        QMutexLocker locker(&d->mutex);

        d->maxCount = maxCount;
        d->maxSize = maxSize;
        d->currLevel = static_cast<ELogLevel>(DefaultLevel);
        if (d->dirName != dirName || d->fileName != fileName)
        {
            d->dirName = dirName;
            d->fileName = fileName;
            d->changeDir();
        }

        // DEBUG级别以下打印文件和行号
        if (d->currLevel <= LG_DBG)
        {
            if (!pattern.contains("%{file}"))
            {
                pattern.append("\t\t(%{file}:%{line})");
            }
        }
        qSetMessagePattern(pattern);
    }
}
