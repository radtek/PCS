#ifndef OPCMANAGER_H
#define OPCMANAGER_H

#include "GlobalData.h"
#include "OPCDefine.h"

class XYOPCServerList;
class XYOPCServer;

class XYOPCManager : public QObject
{
    Q_OBJECT
private:
    explicit XYOPCManager();
    virtual ~XYOPCManager();

public:
    static XYOPCManager *instance()
    {
        static XYOPCManager manager;
        return &manager;
    }

    static void __stdcall OnValueChange(void *pOwner, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
    static void __stdcall OnReadComplete(void *pOwner, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
    static void __stdcall OnWriteComplete(void *pOwner, OPCGROUPHEADER *pHeader);
    static void __stdcall OnCancelComplete(void *pOwner, OPCGROUPHEADER *pHeader);

public slots:
    void opcWriteValue(const QString &stationID, const DataDefine &data);
    void opcReadValue(const QString &stationID, const DataDefine &data);

    //    void opcWriteValue(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
    //    void opcReadValue(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
signals:
    void opcValueChanged(const QString &stationID, const DataDefine &data);
    void opcReadComplete(const QString &stationID, const DataDefine &data);

    //    void opcValueChanged(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
    //    void opcReadComplete(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
public:
    XYOPCServerList *CreateServerList(LPCWSTR szMachineName);
    void ReleaseServerList(XYOPCServerList *pServerList);
    void ReleaseAllServerList();

    XYOPCServer *CreateServer(OPCSERVERINFO *pInfo);
    void ReleaseServer(XYOPCServer *pServer);
    void ReleaseAllServer();

    inline OPCServerListList GetServerListList() const { return ServerListList; }
    inline OPCServerList GetServerList() const { return ServerList; }

    QStringList getStationIDList() const;
    QStringList getItemIDList(const QString &stationID) const;

    ItemAttributeList getItemAttrList(const QString &stationID) const;

    bool ReadXmlFile(const QString &filename);
    bool SaveXmlFile(const QString &filename);

    void enableCallBack();
    void disableCallBack();

private:
    OPCServerListList ServerListList;
    OPCServerList ServerList;
};

#define qOPCManager (XYOPCManager::instance())

#endif    // OPCMANAGER_H
