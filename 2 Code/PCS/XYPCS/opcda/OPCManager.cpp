#include "OPCManager.h"
#include "OPCServerList.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QDebug>

XYOPCManager::XYOPCManager()
{
    HRESULT hResult;

    hResult = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    if (FAILED(hResult))
    {
        qDebug() << "CoInitializeEx() error:" << hResult;
        return;
    }
    /*
        hResult = ::CoInitializeSecurity(NULL,
                                         -1,
                                         NULL,
                                         NULL,
                                         RPC_C_AUTHN_LEVEL_NONE,
                                         RPC_C_IMP_LEVEL_IMPERSONATE,
                                         NULL,
                                         EOAC_NONE,
                                         NULL);

        if (FAILED(hResult))
        {
            qDebug() << "CoInitializeSecurity() error:" << hResult;
            return;
        }
        */
}

XYOPCManager::~XYOPCManager()
{
    ReleaseAllServer();
    ReleaseAllServerList();

    ::CoUninitialize();
}

XYOPCServerList *XYOPCManager::CreateServerList(LPCWSTR szMachineName)
{
    HRESULT hResult;
    REFCLSID clsid = CLSID_OPCServerList;
    COSERVERINFO si;
    MULTI_QI mqi;
    memset(&si, 0, sizeof(COSERVERINFO));
    memset(&mqi, 0, sizeof(MULTI_QI));

    si.pwszName = (LPWSTR)szMachineName;
    mqi.hr = S_OK;
    mqi.pIID = &IID_IOPCServerList;
    mqi.pItf = NULL;

    hResult = CoCreateInstanceEx(clsid,
                                 NULL,
                                 CLSCTX_ALL,
                                 &si,
                                 1,
                                 &mqi);

    if (FAILED(hResult))
    {
        qDebug() << "OPCServerList CoCreateInstanceEx() error:" << hResult;
        return NULL;
    }

    IOPCServerList *pIOPCServerList = (IOPCServerList *)mqi.pItf;

    XYOPCServerList *pServerList = new XYOPCServerList(pIOPCServerList, szMachineName);

    ServerListList.append(pServerList);

    qDebug() << "Create OPCServerList successfully.";

    return pServerList;
}

void XYOPCManager::ReleaseServerList(XYOPCServerList *pServerList)
{
    if (pServerList == NULL)
    {
        qDebug() << "ReleaseServerList pServerList is NULL!";
        return;
    }

    delete pServerList;

    ServerListList.removeOne(pServerList);

    qDebug() << "Release OPCServerList successfully.";
}

void XYOPCManager::ReleaseAllServerList()
{
    for (auto pServerList : ServerListList)
    {
        ReleaseServerList(pServerList);
    }
}

XYOPCServer *XYOPCManager::CreateServer(OPCSERVERINFO *pInfo)
{
    HRESULT hResult;
    LPCOLESTR lpszProgID = pInfo->szProgID;
    CLSID clsid;
    // Given a ProgID, this looks up the associated CLSID in the registry
    hResult = ::CLSIDFromProgID(lpszProgID, &clsid);

    if (FAILED(hResult))
    {
        qDebug() << "CLSIDFromProgID() failed!" << hResult;
        return NULL;
    }

    // Create the OPC server object and querry for the IOPCServer interface of the object
    COSERVERINFO si;
    MULTI_QI mqi;
    memset(&si, 0, sizeof(COSERVERINFO));
    memset(&mqi, 0, sizeof(MULTI_QI));

    si.pwszName = (LPWSTR)pInfo->szMachineName;
    mqi.hr = S_OK;
    mqi.pIID = &IID_IOPCServer;
    mqi.pItf = NULL;

    hResult = ::CoCreateInstanceEx(clsid,
                                   NULL,
                                   CLSCTX_ALL,
                                   &si,
                                   1,
                                   &mqi);

    if (FAILED(hResult))
    {
        qDebug() << "OPCServer CoCreateInstanceEx() error:" << hResult;
        return NULL;
    }

    IOPCServer *pIOPCServer = (IOPCServer *)mqi.pItf;

    XYOPCServer *pServer = new XYOPCServer(pIOPCServer, pInfo);

    ServerList.append(pServer);

    qDebug() << "Create OPCServer successfully.";

    return pServer;
}

void XYOPCManager::ReleaseServer(XYOPCServer *pServer)
{
    if (pServer == NULL)
    {
        qDebug() << "ReleaseServer pServer is NULL!";
        return;
    }

    pServer->RemoveAllGroup();

    delete pServer;

    ServerList.removeOne(pServer);

    qDebug() << "Release OPCServer successfully.";
}

void XYOPCManager::ReleaseAllServer()
{
    for (auto pServer : ServerList)
    {
        ReleaseServer(pServer);
    }
}

bool XYOPCManager::ReadXmlFile(const QString &filename)
{
    ReleaseAllServer();
    ReleaseAllServerList();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "readXmlFile() file open() failed! filename:" << filename;
        return false;
    }

    QDomDocument document;
    QString errMsg;
    int errLine, errCol;
    if (!document.setContent(&file, &errMsg, &errLine, &errCol))
    {
        qDebug() << QString("Parse xml failed! filename: %1, err msg: %2, err line: %3, err col: %4")
                 .arg(filename).arg(errMsg).arg(errLine).arg(errCol);

        file.close();
        return false;
    }
    file.close();

    QDomElement root = document.documentElement();
    QDomElement server = root.firstChildElement("OPCServer");

    while (!server.isNull())
    {
        OPCSERVERINFO info;
        info.clsid = (GUID)QUuid(server.attribute("CLSID"));
        info.szProgID = (LPWSTR)server.attribute("ProgID").unicode();
        info.szUserType = (LPWSTR)server.attribute("UserType").unicode();
        info.szMachineName = (LPWSTR)server.attribute("MachineName").unicode();
        XYOPCServer *pServer = CreateServer(&info);
        if (pServer == NULL)
        {
            server = server.nextSiblingElement();
            continue;
        }

        QDomElement group = server.firstChildElement("OPCGroup");

        while (!group.isNull())
        {
            OPCGROUPSTATE state;
            state.szName = (LPWSTR)group.attribute("Name").unicode();
            state.bActive = (BOOL)group.attribute("Active").toInt();
            state.dwUpdateRate = (DWORD)group.attribute("UpdateRate").toULong();
            state.lTimeBias = (LONG)group.attribute("UpdateRate").toLong();
            state.fPercentDeadband = (FLOAT)group.attribute("UpdateRate").toFloat();
            state.dwLCID = (DWORD)group.attribute("LCID").toULong();
            state.hClientGroup = 0L;
            state.hServerGroup = 0L;
            XYOPCGroup *pGroup = pServer->AddGroup(&state);
            if (pGroup == NULL)
            {
                group = group.nextSiblingElement();
                continue;
            }

            QDomElement item = group.firstChildElement("OPCItem");

            while (!item.isNull())
            {
                OPCITEMATTRIBUTES attribute;
                attribute.szAccessPath = (LPWSTR)L"";
                attribute.szItemID = (LPWSTR)item.attribute("ItemID").unicode();
                attribute.bActive = (BOOL)item.attribute("Active").toInt();
                attribute.vtRequestedDataType = (VARTYPE)item.attribute("DataType").toUShort();
                attribute.hClient = 0L;
                attribute.hServer = 0L;
                attribute.dwBlobSize = 0L;
                attribute.pBlob = NULL;
                pGroup->AddItems(1, &attribute);

                item = item.nextSiblingElement("OPCItem");
            }

            pGroup->BrowseItemAttributes();

            pGroup->AddDataChangeFunc(XYOPCManager::OnValueChange, pServer);
            pGroup->AddReadCompleteFunc(XYOPCManager::OnReadComplete, pServer);
            pGroup->AddWriteCompleteFunc(XYOPCManager::OnWriteComplete, pServer);
            pGroup->AddCancelCompleteFunc(XYOPCManager::OnCancelComplete, pServer);

            group = group.nextSiblingElement("OPCGroup");
        }

        server = server.nextSiblingElement("OPCServer");
    }

    return true;
}

bool XYOPCManager::SaveXmlFile(const QString &filename)
{
    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    for (auto pServer : ServerList)
    {
        OPCSERVERINFO info;
        pServer->GetServerInfo(&info);
        QDomElement server = document.createElement("OPCServer");
        server.setAttribute("CLSID", QUuid(info.clsid).toString());
        server.setAttribute("ProgID", QString::fromWCharArray(info.szProgID));
        server.setAttribute("UserType", QString::fromWCharArray(info.szUserType));
        server.setAttribute("MachineName", QString::fromWCharArray(info.szMachineName));
        root.appendChild(server);

        for (auto pGroup : pServer->GetGroupList())
        {
            OPCGROUPSTATE state;
            pGroup->GetGroupState(&state);
            QDomElement group = document.createElement("OPCGroup");
            group.setAttribute("Name", QString::fromWCharArray(state.szName));
            group.setAttribute("Active", QString::number(state.bActive));
            group.setAttribute("UpdateRate", QString::number(state.dwUpdateRate));
            group.setAttribute("TimeBias", QString::number(state.lTimeBias));
            group.setAttribute("PercentDeadband", QString::number(state.fPercentDeadband));
            group.setAttribute("LCID", QString::number(state.dwLCID));
            server.appendChild(group);

            for (auto pItem : pGroup->BrowseItemAttributes())
            {
                QDomElement item = document.createElement("OPCItem");
                item.setAttribute("ItemID", QString::fromWCharArray(pItem->szItemID));
                item.setAttribute("Active", QString::number(pItem->bActive));
                item.setAttribute("DataType", QString::number(pItem->vtCanonicalDataType));
                group.appendChild(item);
            }
        }
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    document.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

//QStringList XYOPCManager::GetServerNameList() const
//{
//    QStringList nameList;
//    for (auto pServer : ServerList)
//    {
//        QString name = QString::fromWCharArray(pServer->GetProgID());
//        nameList.append(name);
//    }
//    return nameList;
//}

QStringList XYOPCManager::getStationIDList() const
{
    QStringList list;

    for (auto pServer : ServerList)
        for (auto pGroup : pServer->GetGroupList())
            list.append(QString::fromWCharArray(pGroup->GetName()));

    return list;
}

QStringList XYOPCManager::getItemIDList(const QString &stationID) const
{
    QStringList list;

    for (auto pServer : ServerList)
        for (auto pGroup : pServer->GetGroupList())
            if (stationID == QString::fromWCharArray(pGroup->GetName()))
                for (auto pItem : pGroup->BrowseItemAttributes())
                    list.append(QString::fromWCharArray(pItem->szItemID));

    return list;
}

ItemAttributeList XYOPCManager::getItemAttrList(const QString &stationID) const
{
    ItemAttributeList list;

    for (auto pServer : ServerList)
        for (auto pGroup : pServer->GetGroupList())
            if (stationID == QString::fromWCharArray(pGroup->GetName()))
                list.append(pGroup->BrowseItemAttributes());

    return list;

}

void XYOPCManager::enableCallBack()
{
    for (auto pServer : ServerList)
        for (auto pGroup : pServer->GetGroupList())
            pGroup->AddCallback();
}

void XYOPCManager::disableCallBack()
{
    for (auto pServer : ServerList)
        for (auto pGroup : pServer->GetGroupList())
            pGroup->RemoveCallback();
}

void XYOPCManager::OnValueChange(void *pOwner, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState)
{
    XYOPCServer *pServer = static_cast<XYOPCServer *>(pOwner);
    XYOPCGroup *pGroup = pServer->GetGroupByHandle(pHeader->hClientGroup);

    QString stationID = QString::fromWCharArray(pGroup->GetName());

    for (DWORD i = 0; i < pHeader->dwItemCount; ++i)
    {
        DataDefine opcData;

        opcData.itemID = QString::fromWCharArray(pGroup->GetItemIDByHandle(pState[i].hClient));
        opcData.itemValue = VARIANT_to_QVariant(pState[i].vDataValue);

        qDebug() << "XYOPCManager OnValueChange()" << opcData.itemID << opcData.itemValue;

        emit instance()->opcValueChanged(stationID, opcData);
    }
}

void XYOPCManager::OnReadComplete(void *pOwner, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState)
{
    XYOPCServer *pServer = static_cast<XYOPCServer *>(pOwner);
    XYOPCGroup *pGroup = pServer->GetGroupByHandle(pHeader->hClientGroup);

    QString stationID = QString::fromWCharArray(pGroup->GetName());

    for (DWORD i = 0; i < pHeader->dwItemCount; ++i)
    {
        DataDefine opcData;

        opcData.itemID = QString::fromWCharArray(pGroup->GetItemIDByHandle(pState[i].hClient));
        opcData.itemValue = VARIANT_to_QVariant(pState[i].vDataValue);

        qDebug() << "XYOPCManager OnReadComplete()" << opcData.itemID << opcData.itemValue;

        emit instance()->opcReadComplete(stationID, opcData);
    }
}

void XYOPCManager::OnWriteComplete(void *pOwner, OPCGROUPHEADER *pHeader)
{
    Q_UNUSED(pOwner); Q_UNUSED(pHeader)
}

void XYOPCManager::OnCancelComplete(void *pOwner, OPCGROUPHEADER *pHeader)
{
    Q_UNUSED(pOwner); Q_UNUSED(pHeader);
}

void XYOPCManager::opcWriteValue(const QString &stationID, const DataDefine &data)
{
    qDebug() << "XYOPCManager opcWriteValue()" << data.itemID << data.itemValue;

    for (auto pServer : ServerList)
    {
        for (auto pGroup : pServer->GetGroupList())
        {
            if (stationID == QString::fromWCharArray(pGroup->GetName()))
            {
                LPCWSTR szItemID = reinterpret_cast<LPCWSTR>(data.itemID.unicode());
                VARTYPE vtDataType = pGroup->GetItemTypeByItemID(szItemID);
                VARIANT szValue = ::QVariant_to_VARIANT(data.itemValue, vtDataType);
                pGroup->WriteAsync(1, &szItemID, &szValue);
                return;
            }
        }
    }
}

void XYOPCManager::opcReadValue(const QString &stationID, const DataDefine &data)
{
    qDebug() << "XYOPCManager opcReadValue()" << data.itemID;

    for (auto pServer : ServerList)
    {
        for (auto pGroup : pServer->GetGroupList())
        {
            if (stationID == QString::fromWCharArray(pGroup->GetName()))
            {
                LPCWSTR szItemID = reinterpret_cast<LPCWSTR>(data.itemID.unicode());
                pGroup->ReadAsync(1, &szItemID);
                return;
            }
        }
    }
}

















