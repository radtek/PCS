#ifndef PCS_MAIN_GCFGMANAGER_H
#define PCS_MAIN_GCFGMANAGER_H

#include <QtCore>

class GCfgManager
{
public:
    static GCfgManager *instance();

public:
    GCfgManager();
    ~GCfgManager();

private:
    bool loadConfigure();

private:
    QSettings settings;

    QString softwareVersion;
    QString softwareMainTitle;
    QString softwareSubTitle;
    QString softwareSplash;

    QString sysPathSqlConn;
    QString sysPathSqlName;
    QString sysPathSqlSync;
    QString sysPathOpcConfig;
    QString sysPathCommConfig;
    QString sysPathAssyProcess;
    QString sysPathLabelPrint;
    QString sysPathDateCode;

    QString workShop;
    QString workLine;

    bool sysNeedLogin;

public:
    inline const QString &getVersion() const
    {
        return softwareVersion;
    }
    inline const QString &getMainTitle() const
    {
        return softwareMainTitle;
    }
    inline const QString &getSubTitle() const
    {
        return softwareSubTitle;
    }
    inline const QString &getSplash() const
    {
        return softwareSplash;
    }
    inline const QString &getSqlConn() const
    {
        return sysPathSqlConn;
    }
    inline const QString &getSqlName() const
    {
        return sysPathSqlName;
    }
    inline const QString &getSqlSync() const
    {
        return sysPathSqlSync;
    }
    inline const QString &getOpcConfig() const
    {
        return sysPathOpcConfig;
    }
    inline const QString &getComConfig() const
    {
        return sysPathCommConfig;
    }
    inline const QString &getAssyProcess() const
    {
        return sysPathAssyProcess;
    }
    inline const QString &getLabelPrint() const
    {
        return sysPathLabelPrint;
    }
    inline const QString &getDateCode() const
    {
        return sysPathDateCode;
    }
    inline const QString &getWorkShop() const
    {
        return workShop;
    }
    inline const QString &getWorkLine() const
    {
        return workLine;
    }
    inline const bool &isNeedLogin() const
    {
        return sysNeedLogin;
    }
};

#define qCfgManager (GCfgManager::instance())

#endif
