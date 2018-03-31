#include "OPCServer.h"
#include "OPCGroup.h"
#include <QDebug>

XYOPCServer::XYOPCServer(IOPCServer *pServer, OPCSERVERINFO *pInfo)
    : pIOPCServer(pServer)
{
    ServerInfo.clsid = pInfo->clsid;
    ServerInfo.szProgID = ComStringAlloc(pInfo->szProgID);
    ServerInfo.szUserType = ComStringAlloc(pInfo->szUserType);
    ServerInfo.szMachineName = ComStringAlloc(pInfo->szMachineName);

    HRESULT hResult;

    hResult = pIOPCServer->QueryInterface(IID_IOPCBrowseServerAddressSpace,
                                          (void **)&pIOPCBrowseServerAddressSpace);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCServer QueryInterface() error:" << hResult;
    }

    hResult = pIOPCServer->QueryInterface(IID_IOPCItemProperties,
                                          (void **)&pIOPCItemProperties);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCServer QueryInterface() error:" << hResult;
    }
}

XYOPCServer::~XYOPCServer()
{
    ComStringFree(ServerInfo.szProgID);
    ComStringFree(ServerInfo.szUserType);
    ComStringFree(ServerInfo.szMachineName);

    ReleaseInterface(pIOPCBrowseServerAddressSpace);
    ReleaseInterface(pIOPCItemProperties);
    ReleaseInterface(pIOPCServer);
}

void XYOPCServer::GetServerStatus(OPCSERVERSTATUS *pStatus)
{
    if (pIOPCServer == NULL)
    {
        qDebug() << "IOPCServer has not been created!";
        return;
    }

    HRESULT hResult;
    OPCSERVERSTATUS *pServerStatus;
    hResult = pIOPCServer->GetStatus(&pServerStatus);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCServer GetStatus() error!" << hResult;
        return;
    }

    *pStatus = *pServerStatus;

    ComStructFree(pServerStatus);
}

ItemBrowseList XYOPCServer::BrowseItemIDs(LPCWSTR szString, OPCBROWSETYPE dwType)
{
    ItemBrowseList browseList;

    if (pIOPCBrowseServerAddressSpace == NULL)
    {
        qDebug() << "IOPCBrowseServerAddressSpace has not been created!";
        return browseList;
    }

    HRESULT hResult;

    hResult = pIOPCBrowseServerAddressSpace->ChangeBrowsePosition(OPC_BROWSE_TO,
                                                                  szString);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCBrowseServerAddressSpace ChangeBrowsePosition() error!" << hResult;
        return browseList;
    }

    OPCNAMESPACETYPE dwNameSpaceType;

    hResult = pIOPCBrowseServerAddressSpace->QueryOrganization(&dwNameSpaceType);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCBrowseServerAddressSpace QueryOrganization() error!" << hResult;
        return browseList;
    }

    if (dwNameSpaceType == OPC_NS_FLAT)
    {
        qDebug() << "IOPCBrowseServerAddressSpace QueryOrganization() return OPC_NS_FLAT!";
        return browseList;
    }

    OPCBROWSETYPE dwBrowseFilterType = dwType;
    LPCWSTR szFilterCriteria = L"";
    VARTYPE vtDataTypeFilter = VT_EMPTY;
    DWORD dwAccessRightsFilter = 0;
    LPENUMSTRING pIEnumString = NULL;

    hResult = pIOPCBrowseServerAddressSpace->BrowseOPCItemIDs(dwBrowseFilterType,
                                                              szFilterCriteria,
                                                              vtDataTypeFilter,
                                                              dwAccessRightsFilter,
                                                              &pIEnumString);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qDebug() << "IOPCBrowseServerAddressSpace BrowseOPCItemIDs() error:" << hResult;
        return browseList;
    }

    do
    {
        LPWSTR szItemDataID;
        ULONG ulCount = 0;

        hResult = pIEnumString->Next(1, &szItemDataID, &ulCount);

        if (ulCount > 0)
        {
            LPWSTR szItemID;

            pIOPCBrowseServerAddressSpace->GetItemID(szItemDataID, &szItemID);

            OPCITEMBROWSE *pBrowse = ComStructAlloc<OPCITEMBROWSE>();
            pBrowse->szItemDataID = ComStringAlloc(szItemDataID);
            pBrowse->szItemID = ComStringAlloc(szItemID);
            browseList.append(pBrowse);

            ComStringFree(szItemID);
        }

        ComStringFree(szItemDataID);

    }
    while (hResult == S_OK);

    pIEnumString->Release();

    return browseList;
}

ItemPropertyList XYOPCServer::QueryItemProperties(LPCWSTR szItem)
{
    ItemPropertyList propertyList;

    if (pIOPCItemProperties == NULL)
    {
        qDebug() << "IOPCItemProperties has not been created!";
        return propertyList;
    }

    HRESULT hResult;
    LPWSTR szItemID = (LPWSTR)szItem;
    DWORD dwCount = 0;
    DWORD *pdwPropertyIDs;
    LPWSTR *pszDescriptions;
    VARTYPE *pvtDataTypes;

    hResult = pIOPCItemProperties->QueryAvailableProperties(szItemID,
                                                            &dwCount,
                                                            &pdwPropertyIDs,
                                                            &pszDescriptions,
                                                            &pvtDataTypes);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCItemProperties QueryAvailableProperties() error:" << hResult;
        return propertyList;
    }

    if (dwCount == 0)
    {
        qDebug() << "IOPCItemProperties QueryAvailableProperties() dwCount is zero!";
        return propertyList;
    }

    OPCITEMPROPERTY *pProperty = ComStructAlloc<OPCITEMPROPERTY>(dwCount);

    for (DWORD i = 0; i < dwCount ; ++i)
    {
        pProperty[i].dwPropertyID = pdwPropertyIDs[i];
        pProperty[i].vtDataType = pvtDataTypes[i];
        pProperty[i].szDescription = ComStringAlloc(pszDescriptions[i]);
    }

    VARIANT *pvData;
    HRESULT *phErrors;
    hResult = pIOPCItemProperties->GetItemProperties(szItemID,
                                                     dwCount,
                                                     pdwPropertyIDs,
                                                     &pvData,
                                                     &phErrors);

    if (FAILED(hResult))
    {
        qDebug() << "IOPCItemProperties GetItemProperties() error!" << hResult;
        ComStructFree(pProperty);
        return propertyList;
    }

    for (DWORD i = 0; i < dwCount ; ++i)
    {
        pProperty[i].vValue = pvData[i];
        propertyList.append(pProperty + i);
//        switch (pdwPropertyIDs[i])
//        {
//        case OPC_PROPERTY_DATATYPE:
//            propList[i].value = DATATYPE_to_QString(pvData[i].uiVal);
//            break;
//        case OPC_PROPERTY_QUALITY:
//            propList[i].value = QUALITY_to_QString(pvData[i].uiVal);
//            break;
//        case OPC_PROPERTY_TIMESTAMP:
//            propList[i].value = DATE_to_QDateTime(pvData[i].date).toString("yyyy-MM-dd hh:mm:ss.zzz");
//            break;
//        case OPC_PROPERTY_ACCESS_RIGHTS:
//            propList[i].value = RIGHTS_to_QString(pvData[i].ulVal);
//            break;
//        case OPC_PROPERTY_EU_TYPE:
//            propList[i].value = EUTYPE_to_QString(pvData[i].ulVal);
//            break;
//        default:
//            propList[i].value = VARIANT_to_QVariant(pvData[i]).toString();
//        }
    }

    ComStructFree(pdwPropertyIDs);
    ComStructFree(pszDescriptions);
    ComStructFree(pvtDataTypes);
    ComStructFree(pvData);
    ComStructFree(phErrors);

    return propertyList;
}


XYOPCGroup *XYOPCServer::AddGroup(OPCGROUPSTATE *pState)
{
    if (pIOPCServer == NULL)
    {
        qDebug() << "IOPCServer has not been created!";
        return NULL;
    }

    if (pState == NULL)
    {
        qDebug() << "AddGroup pState is NULL!";
        return NULL;
    }

    HRESULT hr;

    IOPCGroupStateMgt *pIOPCGroupStateMgt;
    DWORD dwRevisedUpdateRate = 0;
    hr = pIOPCServer->AddGroup(pState->szName,
                               pState->bActive,
                               pState->dwUpdateRate,
                               pState->hClientGroup,
                               &pState->lTimeBias,
                               &pState->fPercentDeadband,
                               pState->dwLCID,
                               &pState->hServerGroup,
                               &dwRevisedUpdateRate,
                               IID_IOPCGroupStateMgt,
                               (LPUNKNOWN *)&pIOPCGroupStateMgt);

    if (FAILED(hr))
    {
        qDebug() << "IOPCServer AddGroup() error";
        return NULL;
    }

    XYOPCGroup *pGroup = new XYOPCGroup(pIOPCGroupStateMgt, this);

    GroupList.append(pGroup);

    qDebug() << "AddGroup successfully.";

    return pGroup;
}

void XYOPCServer::AddGroup(XYOPCGroup *pGroup)
{
    GroupList.append(pGroup);
}

void XYOPCServer::RemoveGroup(XYOPCGroup *pGroup)
{
    if (pIOPCServer == NULL)
    {
        qDebug() << "IOPCServer has not been created!";
        return;
    }

    if (pGroup == NULL)
    {
        qDebug() << "RemoveGroup pGroup is NULL!";
        return;
    }

    pGroup->RemoveCallback();
    pGroup->RemoveAllItems();

    HRESULT hr;
    OPCHANDLE hServer = pGroup->GetServerHandle();
    BOOL bForce = TRUE;
    hr = pIOPCServer->RemoveGroup(hServer,
                                  bForce);

    if (FAILED(hr))
    {
        qDebug() << "IOPCServer RemoveGroup() error";
        return;
    }

    delete pGroup;

    GroupList.removeOne(pGroup);

    qDebug() << "RemoveGroup successfully.";
}

void XYOPCServer::RemoveAllGroup()
{
    for (auto pGroup : GroupList)
    {
        RemoveGroup(pGroup);
    }
}

XYOPCGroup *XYOPCServer::GetGroupByHandle(OPCHANDLE hClient)
{
    for (auto pGroup : GroupList)
        if (pGroup->GetClientHandle() == hClient)
            return pGroup;

    return NULL;
}
