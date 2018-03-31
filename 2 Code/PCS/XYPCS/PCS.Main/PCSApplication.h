#ifndef PCSAPPLICATION_H
#define PCSAPPLICATION_H

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtWidgets>

class GCfgManager;
class MainWindow;
class PCSApplication : public QApplication
{
    Q_OBJECT

public:
    static PCSApplication *instance();

public:
    PCSApplication(int argc, char **argv);
    virtual ~PCSApplication();

public:
    bool initialize();
    bool isOnline();
    void setOnline(bool value);

private:
    bool isLocalServerOpen();
    bool openLocalServer();
    bool execCommand(const QString &command);

private Q_SLOTS:
    void slotLocalSocketNewConnection();
    void slotCheckSystemOnline();

private:
    QLocalServer *localServer;
    GCfgManager *cfgManager;

    MainWindow *mainWindow;

    volatile bool online;
    QMutex onlineMutex;

    QTimer *checkTimer;
};

#define qApplication PCSApplication::instance()

#endif
