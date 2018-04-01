#include "PCSApplication.h"
#include "DialogLogin.h"
#include "GlobalData.h"
#include "MainWindow.h"

#include "XYSearchLineEdit.h"
#include "XYTextFieldLineEdit.h"

int main(int argc, char *argv[])
{
    PCSApplication app(argc, argv);

    // 日志初始化
    qInstallMessageHandler(XYLogManager::OutputMessage);

    if (!app.initialize())
    {
        return 0;
    }

    return app.exec();
}

PCSApplication *PCSApplication::instance()
{
    return static_cast<PCSApplication *>(QCoreApplication::instance());
}

PCSApplication::PCSApplication(int argc, char **argv)
    : QApplication(argc, argv)
    , localServer(Q_NULLPTR)
    , online(true)
    , checkTimer(new QTimer)
{
    QApplication::setApplicationName("PCS");
    QApplication::setFont(QFont("Microsoft Yahei", 9));
    QApplication::setStyle("Fusion");
}

PCSApplication::~PCSApplication()
{
}

bool PCSApplication::initialize()
{
    qCritical() << "程序开始启动……";

    // 配置初始化
    cfgManager = qCfgManager;

    // 检测程序是否开启,开启本地服务
    if (!isLocalServerOpen() || !openLocalServer())
    {
        return false;
    }

    // 开启数据库
    if (!qSqlManager->Initialize(qCfgManager->getPathSqlConn(), qCfgManager->getPathSqlName()))
    {
        MessageBox_Critical("数据库打开失败！");
        this->processEvents();
        return false;
    }

    if (cfgManager->isNeedLogin())
    {
        DialogLogin dialog;
        if (QDialog::Accepted != dialog.exec())
        {
            return false;
        }
    }

    mainWindow = new MainWindow();
    mainWindow->showMaximized();

    return true;
}

bool PCSApplication::isOnline()
{
    QMutexLocker locker(&onlineMutex);

    return online;
}

void PCSApplication::setOnline(bool value)
{
    QMutexLocker locker(&onlineMutex);

    if (online)
    {
        online = value;
    }
}

bool PCSApplication::isLocalServerOpen()
{
    QLocalSocket socket;
    socket.connectToServer(QCoreApplication::applicationName());
    if (socket.waitForConnected(500))
    {
        qWarning() << "[SYSTEM] System local connect success.";

        // 连接上，进入命令行模式
        QStringList argv = QCoreApplication::arguments();
        if (argv.count() > 1)
        {
            QTextStream stream(&socket);

            for (int i = 0; i < argv.size(); ++i)
            {
                stream << argv[i] << " ";
            }
            stream.flush();
            socket.waitForBytesWritten();
        }
        else
        {
            qCritical() << "[SYSTEM] System already run...";
            MessageBox_Critical("程序已经打开！");
            this->processEvents();
        }
        socket.close();
        return false;
    }
    return true;
}

bool PCSApplication::openLocalServer()
{
    localServer = new QLocalServer(this);
    QObject::connect(localServer, SIGNAL(newConnection()), this, SLOT(slotLocalSocketNewConnection()));
    if (localServer->listen(QCoreApplication::applicationName()))
    {
        qDebug() << "[SYSTEM] System local listen success.";
        return true;
    }
    else
    {
        qCritical() << "[SYSTEM] System local listen failed." << localServer->errorString();
        if (localServer->serverError() == QAbstractSocket::AddressInUseError || QFile::exists(localServer->serverName()))
        {
            QFile::remove(localServer->serverName());
            if (localServer->listen(QCoreApplication::applicationName()))
            {
                qDebug() << "[SYSTEM] System try to listen success.";
                return true;
            }
        }
    }
    return false;
}

bool PCSApplication::execCommand(const QString &command)
{
    qDebug() << "Recieve command:" << command;

    return true;
}

void PCSApplication::slotLocalSocketNewConnection()
{
    qInfo() << "slotLocalSocketNewConnection";

    QLocalSocket *socket = localServer->nextPendingConnection();
    if (!socket)
    {
        qWarning() << "socket is invalid!";
        return;
    }
    socket->waitForReadyRead(500);

    QString command = socket->readAll();
    execCommand(command);

    delete socket;
}

void PCSApplication::slotCheckSystemOnline()
{
    qInfo() << "slotCheckSystemOnline";

    if (isOnline())
    {
        return;
    }

    MessageBox_Critical1("连接中控服务器失败，系统即将关闭");
    QCoreApplication::quit();
}
