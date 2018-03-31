#include "GCfgManager.h"
#include "GlobalData.h"

#define DEFAULT_CFGMANAGER_PATH "config.ini"

Q_GLOBAL_STATIC(GCfgManager, theInstance);

GCfgManager *GCfgManager::instance()
{
    return theInstance();
}

GCfgManager::GCfgManager()
    : settings(DEFAULT_CFGMANAGER_PATH, QSettings::IniFormat)
{
    settings.setIniCodec("UTF-8");

    loadConfigure();
}

GCfgManager::~GCfgManager()
{
}

bool GCfgManager::loadConfigure()
{
    if (settings.status() != QSettings::NoError)
    {
        qCritical().noquote() << QString("[INI] Read config file error, file{%1}, error{%1}").arg(settings.fileName()).arg(settings.status());
        return false;
    }

    // SOFTWARE
    do
    {
        settings.beginGroup("SOFTWARE");

        // Version
        softwareVersion = settings.value("Version").toString();
        qDebug() << "[CFGMANAGER] SOFTWARE.Version=" << softwareVersion;
        if (softwareVersion.isEmpty())
        {
            return false;
        }

        // MainTitle
        softwareMainTitle = settings.value("MainTitle").toString();
        qDebug() << "[CFGMANAGER] SOFTWARE.MainTitle=" << softwareMainTitle;
        if (softwareMainTitle.isEmpty())
        {
            return false;
        }

        // SubTitle
        softwareSubTitle = settings.value("SubTitle").toString();
        qDebug() << "[CFGMANAGER] SOFTWARE.SubTitle=" << softwareSubTitle;
        if (softwareSubTitle.isEmpty())
        {
            return false;
        }

        // Splash
        softwareSplash = settings.value("Splash").toString();
        qDebug() << "[CFGMANAGER] SOFTWARE.Splash=" << softwareSplash;

        settings.endGroup();

    } while (0);

    // SYSPATH
    do
    {
        settings.beginGroup("SYSPATH");

        // SqlConn
        sysPathSqlConn = settings.value("SqlConn").toString();
        qDebug() << "[CFGMANAGER] SYSPATH.SqlConn=" << sysPathSqlConn;
        if (sysPathSqlConn.isEmpty())
        {
            return false;
        }

        // SqlName
        sysPathSqlName = settings.value("SqlName").toString();
        qDebug() << "[CFGMANAGER] SYSPATH.SqlName=" << sysPathSqlName;

        sysPathOpcConfig = settings.value("OpcConfig").toString();
        sysPathCommConfig = settings.value("CommConfig").toString();
        sysPathAssyProcess = settings.value("AssyProcess").toString();
        sysPathLabelPrint = settings.value("LabelPrint").toString();
        sysPathDateCode = settings.value("DateCode").toString();

        settings.endGroup();
    } while (0);

    // WORK
    do
    {
        settings.beginGroup("WORK");

        // WorkShop
        workShop = settings.value("WorkShop").toString();
        qDebug() << "[CFGMANAGER] WORK.WorkShop=" << workShop;
        if (workShop.isEmpty())
        {
            return false;
        }

        // WorkLine
        workLine = settings.value("WorkLine").toString();
        qDebug() << "[CFGMANAGER] WORK.WorkLine=" << workLine;
        if (workLine.isEmpty())
        {
            return false;
        }

        settings.endGroup();
    } while (0);

    // SYSTEM
    do
    {
        settings.beginGroup("SYSTEM");

        sysNeedLogin = settings.value("NeedLogin", 1).toBool();
        qDebug() << "[CFGMANAGER] SYSTEM.NeedLogin=" << sysNeedLogin;

        settings.endGroup();
    } while (0);

    return true;
}
