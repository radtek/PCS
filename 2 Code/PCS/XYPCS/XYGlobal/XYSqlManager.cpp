/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYSqlManager.cpp
 *
 * @example
 *
 * @brief
 *  Default configure file is `sqlconfig.xml` and `sqlnaming.xml`.
 *
 * @date 2018-03-27 @author weiheng             @note create this file
 * *****************************************************************************
 */

#include "XYSqlManager.h"

#define DEFAULT_CONFIG_CONFIGSQL "sqlconfig.xml"
#define DEFAULT_CONFIG_NAMINGSQL "sqlnaming.xml"

Q_GLOBAL_STATIC(XYSqlManager, theInstance);

XYSqlManager *XYSqlManager::instance()
{
    return theInstance();
}

bool XYSqlManager::SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantMap &dbParam)
{
    qDebug().noquote() << "\n==========\n"
                       << dbSQL << "\n----------\n"
                       << dbParam << "\n==========";
    dbQuery.clear();

    dbQuery.prepare(dbSQL);
    for (auto it = dbParam.begin(); it != dbParam.end(); ++it)
    {
        dbQuery.bindValue(it.key(), it.value());
    }

    if (!dbQuery.exec())
    {
        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}.").arg(dbQuery.lastError().text());
        return false;
    }

    return true;
}

bool XYSqlManager::SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantList &dbParam)
{
    qDebug().noquote() << "\n==========\n"
                       << dbSQL << "\n----------\n"
                       << dbParam << "\n==========";
    dbQuery.clear();

    dbQuery.prepare(dbSQL);
    for (auto it = dbParam.begin(); it != dbParam.end(); ++it)
    {
        dbQuery.addBindValue(*it);
    }

    if (!dbQuery.exec())
    {
        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}.").arg(dbQuery.lastError().text());
        return false;
    }

    return true;
}

XYSqlManager::XYSqlManager()
{
}

XYSqlManager::~XYSqlManager()
{
}

bool XYSqlManager::Initialize()
{
    initConfigSQL();
    initNamingSQL();

    return init();
}

QString XYSqlManager::getSqlByName(const QString &name)
{
    qDebug() << QString("[SQL] NamingSQL:[%1].").arg(name);

    if (mapNamingSQL.contains(name))
    {
        return mapNamingSQL[name]->StrSQL;
    }
    return QString();
}

void XYSqlManager::initConfigSQL()
{
    mapConfigSQL.clear();

    QDomDocument xmldoc;

    QFile file(DEFAULT_CONFIG_CONFIGSQL);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning().noquote() << QString("[XML] Open file error, FileName[%1].").arg(file.fileName());
        return;
    }

    QString errorStr;
    int errorLine = 0;
    int errorCol = 0;
    if (!xmldoc.setContent(&file, true, &errorStr, &errorLine, &errorCol))
    {
        qWarning().noquote() << QString("[XML] Parse file error, FileName[%1], Error[%2], Position[ln:%3 col:%4].").arg(file.fileName()).arg(errorStr).arg(errorLine).arg(errorCol);
        file.close();
        return;
    }
    file.close();

    QDomElement xmlElemRoot = xmldoc.documentElement();
    QDomElement xmlElemItemSQL = xmlElemRoot.firstChildElement("ItemSQL");
    while (!xmlElemItemSQL.isNull())
    {
        do
        {
            QString id = xmlElemItemSQL.attribute("id");
            QString type = xmlElemItemSQL.attribute("type");
            QString enable = xmlElemItemSQL.attribute("enable");
            QString force = xmlElemItemSQL.attribute("force");

            if (id.isEmpty() || type.isEmpty() || mapConfigSQL.contains(id))
            {
                break;
            }

            QDomElement xmlElemAddr = xmlElemItemSQL.firstChildElement("Addr");
            if (xmlElemAddr.isNull())
            {
                break;
            }
            QString Addr = xmlElemAddr.text();

            QDomElement xmlElemPort = xmlElemItemSQL.firstChildElement("Port");
            if (xmlElemPort.isNull())
            {
                break;
            }
            QString Port = xmlElemPort.text();

            QDomElement xmlElemUser = xmlElemItemSQL.firstChildElement("User");
            if (xmlElemUser.isNull())
            {
                break;
            }
            QString User = xmlElemUser.text();

            QDomElement xmlElemPass = xmlElemItemSQL.firstChildElement("Pass");
            if (xmlElemPass.isNull())
            {
                break;
            }
            QString Pass = xmlElemPass.text();

            QDomElement xmlElemName = xmlElemItemSQL.firstChildElement("Name");
            if (xmlElemName.isNull())
            {
                break;
            }
            QString Name = xmlElemName.text();

            do
            {
                TConfigSQLPtr configSQL(new TConfigSQL());
                configSQL->type = type;
                configSQL->isEnable = enable.toInt();
                configSQL->isForce = force.toInt();
                configSQL->address = Addr;
                configSQL->port = Port.toInt();
                configSQL->username = User;
                configSQL->password = Pass;
                configSQL->database = Name;

                mapConfigSQL.insert(id, configSQL);
            } while (0);
        } while (0);

        xmlElemItemSQL = xmlElemItemSQL.nextSiblingElement();
    }
}

void XYSqlManager::initNamingSQL()
{
    mapNamingSQL.clear();

    QDomDocument xmldoc;

    QFile file(DEFAULT_CONFIG_NAMINGSQL);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning().noquote() << QString("[XML] Open file error, FileName[%1].").arg(file.fileName());
        return;
    }

    QString errorStr;
    int errorLine = 0;
    int errorCol = 0;
    if (!xmldoc.setContent(&file, true, &errorStr, &errorLine, &errorCol))
    {
        qWarning().noquote() << QString("[XML] Parse file error, FileName[%1], Error[%2], Position[ln:%3 col:%4].").arg(file.fileName()).arg(errorStr).arg(errorLine).arg(errorCol);
        file.close();
        return;
    }
    file.close();

    QDomElement xmlElemRoot = xmldoc.documentElement();
    QDomElement xmlElemItemSQL = xmlElemRoot.firstChildElement("ItemSQL");
    while (!xmlElemItemSQL.isNull())
    {
        while (!xmlElemItemSQL.isNull())
        {
            do
            {
                QString Name = xmlElemItemSQL.attribute("name");
                if (Name.isEmpty() || mapNamingSQL.contains(Name))
                {
                    break;
                }

                QDomElement xmlElemStrSQL = xmlElemItemSQL.firstChildElement("StrSQL");
                if (xmlElemStrSQL.isNull())
                {
                    break;
                }
                QString StrSQL = xmlElemStrSQL.text();

                do
                {
                    TNamingSQLPtr namingSQL(new TNamingSQL());
                    namingSQL->Name = Name;
                    namingSQL->StrSQL = StrSQL;

                    mapNamingSQL.insert(Name, namingSQL);
                } while (0);
            } while (0);

            xmlElemItemSQL = xmlElemItemSQL.nextSiblingElement();
        }
    }
}

bool XYSqlManager::init()
{
    for (auto it = mapConfigSQL.begin(); it != mapConfigSQL.end(); ++it)
    {
        QSharedPointer<TConfigSQL> configSQL = it.value();
        QSqlDatabase dbSQL;

        if (!configSQL->isEnable)
        {
            continue;
        }
        if (configSQL->type == "QMSSQL")
        {
            dbSQL = QSqlDatabase::addDatabase("QODBC", it.key());
            QString dsn = QString("Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;DATABASE=%5;")
                              .arg(configSQL->address)
                              .arg(configSQL->port)
                              .arg(configSQL->username)
                              .arg(configSQL->password)
                              .arg(configSQL->database);
            dbSQL.setDatabaseName(dsn);
            dbSQL.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=5;SQL_ATTR_CONNECTION_TIMEOUT=5;");
        }
        else
        {
            dbSQL = QSqlDatabase::addDatabase(configSQL->type, it.key());
            dbSQL.setHostName(configSQL->address);
            dbSQL.setPort(configSQL->port);
            dbSQL.setUserName(configSQL->username);
            dbSQL.setPassword(configSQL->password);
            dbSQL.setDatabaseName(configSQL->database);
            dbSQL.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=5;SQL_ATTR_CONNECTION_TIMEOUT=5;");
        }

        bool isOpen = dbSQL.open();
        if (isOpen)
        {
            qInfo().noquote() << QString("Open database success, NAME[%1], TYPE[%2], SERVER[%3], PORT[%4], UID[%5], PWD[%6], DATABASE[%7]")
                                     .arg(it.key())
                                     .arg(configSQL->type)
                                     .arg(configSQL->address)
                                     .arg(configSQL->port)
                                     .arg(configSQL->username)
                                     .arg(configSQL->password)
                                     .arg(configSQL->database);
        }
        else
        {
            qWarning().noquote() << QString("Open database failed, NAME[%1], TYPE[%2], SERVER[%3], PORT[%4], UID[%5], PWD[%6], DATABASE[%7], Error:%8")
                                        .arg(it.key())
                                        .arg(configSQL->type)
                                        .arg(configSQL->address)
                                        .arg(configSQL->port)
                                        .arg(configSQL->username)
                                        .arg(configSQL->password)
                                        .arg(configSQL->database)
                                        .arg(dbSQL.lastError().text());
        }
        if (configSQL->isForce && !isOpen)
        {
            return false;
        }
    }
    return true;
}
