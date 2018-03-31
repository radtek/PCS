#include "OPCGroup.h"
#include "OPCServer.h"
#include "OPCCallback.h"
#include <QDebug>

XYOPCGroup::XYOPCGroup(IOPCGroupStateMgt *pGroup, XYOPCServer *pServer)
    : pIOPCGroupStateMgt(pGroup)
    , pOPCServer(pServer)
    , pOPCCallback(NULL), dwCookie(0L)
    , pOVCFunc(NULL), pORCFunc(NULL), pOWCFunc(NULL), pOCCFunc(NULL)
    , pOVCData(NULL), pORCData(NULL), pOWCData(NULL), pOCCData(NULL)
{
    OPCGROUPSTATE state;
    GetGroupState(&state);
    GroupState.szName = ComStringAlloc(state.szName);
    GroupState.hServerGroup = state.hServerGroup;
    SetGroupState(&state);

    HRESULT hResult;

    hResult = pIOPCGroupStateMgt->QueryInterface(IID_IOPCItemMgt,
                                                 (void **)&pIOPCItemMgt);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCGroupStateMgt QueryInterface() error:" << hResult;
    }
    else
    {
        hResult = pIOPCItemMgt->QueryInterface(IID_IOPCSyncIO,
                                               (void **)&pIOPCSyncIO);

        if (FAILED(hResult))
        {
            qCritical() << "IOPCGroupStateMgt QueryInterface() error:" << hResult;
        }


        hResult = pIOPCItemMgt->QueryInterface(IID_IOPCAsyncIO2,
                                               (void **)&pIOPCAsyncIO2);

        if (FAILED(hResult))
        {
            qCritical() << "IOPCGroupStateMgt QueryInterface() error:" << hResult;
        }
    }

    hResult = pIOPCGroupStateMgt->QueryInterface(IID_IConnectionPointContainer,
                                                 (void **)&pIConnectionPointContainer);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCGroupStateMgt QueryInterface() error:" << hResult;
    }
    else
    {
        hResult = pIConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback,
                                                                  &pIConnectionPoint);

        if (FAILED(hResult))
        {
            qCritical() << "IConnectionPointContainer FindConnectionPoint() error:" << hResult;
        }
    }
}

XYOPCGroup::~XYOPCGroup()
{
    DeleteItemAttributes();
    DeleteItemStates();

    ComStringFree(GroupState.szName);

    ReleaseInterface(pOPCCallback);
    ReleaseInterface(pIConnectionPoint);
    ReleaseInterface(pIConnectionPointContainer);
    ReleaseInterface(pIOPCSyncIO);
    ReleaseInterface(pIOPCAsyncIO2);
    ReleaseInterface(pIOPCItemMgt);
    ReleaseInterface(pIOPCGroupStateMgt);
}

XYOPCGroup *XYOPCGroup::CloneGroup(LPCWSTR szName)
{
    if (pIOPCGroupStateMgt == NULL)
    {
        qCritical() << "IOPCGroupStateMgt has not been created!";
        return NULL;
    }

    HRESULT hResult;

    IOPCGroupStateMgt *pIOPCCloneStateMgt;
    hResult = pIOPCGroupStateMgt->CloneGroup(szName,
                                             IID_IOPCGroupStateMgt,
                                             (LPUNKNOWN *)&pIOPCCloneStateMgt);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCGroupStateMgt CloneGroup() error";
        return NULL;
    }

    XYOPCGroup *pClone = new XYOPCGroup(pIOPCCloneStateMgt, pOPCServer);

    pOPCServer->AddGroup(pClone);

    return pClone;
}

bool XYOPCGroup::SetGroupState(OPCGROUPSTATE *pState)
{
    if (pIOPCGroupStateMgt == NULL)
    {
        qCritical() << "IOPCGroupStateMgt has not been created!";
        return false;
    }

    if (pState == NULL)
    {
        qCritical() << "SetGroupState pState is NULL!";
        return false;
    }

    HRESULT hResult;

    hResult = pIOPCGroupStateMgt->SetName(pState->szName);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCGroupStateMgt SetName() error";
        return false;
    }

    GroupState.szName = ComStringRealloc(GroupState.szName, pState->szName);

    DWORD dwRevisedUpdateRate = 0;
    pState->hClientGroup = GroupState.hServerGroup;
    hResult = pIOPCGroupStateMgt->SetState(&pState->dwUpdateRate,
                                           &dwRevisedUpdateRate,
                                           &pState->bActive,
                                           &pState->lTimeBias,
                                           &pState->fPercentDeadband,
                                           &pState->dwLCID,
                                           &pState->hClientGroup);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCGroupStateMgt SetState() error";
        return false;
    }

    GroupState.dwUpdateRate = dwRevisedUpdateRate;
    GroupState.bActive = pState->bActive;
    GroupState.lTimeBias = pState->lTimeBias;
    GroupState.fPercentDeadband = pState->fPercentDeadband;
    GroupState.dwLCID = pState->dwLCID;
    GroupState.hClientGroup = pState->hClientGroup;

    return true;
}

void XYOPCGroup::GetGroupState(OPCGROUPSTATE *pState)
{
    if (pIOPCGroupStateMgt == NULL)
    {
        qCritical() << "IOPCGroupStateMgt has not been created!";
        return;
    }

    if (pState == NULL)
    {
        qCritical() << "GetGroupState pState is NULL!";
        return;
    }

    HRESULT hResult;

    hResult = pIOPCGroupStateMgt->GetState(&pState->dwUpdateRate,
                                           &pState->bActive,
                                           &pState->szName,
                                           &pState->lTimeBias,
                                           &pState->fPercentDeadband,
                                           &pState->dwLCID,
                                           &pState->hClientGroup,
                                           &pState->hServerGroup);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCGroupStateMgt GetState() error:" << hResult;
        return;
    }
}

bool XYOPCGroup::AddItems(DWORD dwCount, OPCITEMATTRIBUTES *pAttributes)
{
    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return false;
    }

    if (pAttributes == NULL)
    {
        qCritical() << "AddItems pAttributes is NULL!";
        return false;
    }

    if (dwCount == 0)
    {
        qCritical() << "AddItems dwCount is zero!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCITEMDEF *pItemArray = ComStructAlloc<OPCITEMDEF>(dwCount);
    OPCITEMRESULT *pAddResults;

    for (DWORD i = 0; i < dwCount; ++i)
    {
        pItemArray[i].szAccessPath = pAttributes[i].szAccessPath;
        pItemArray[i].szItemID = pAttributes[i].szItemID;
        pItemArray[i].bActive = pAttributes[i].bActive;
        pItemArray[i].hClient = pAttributes[i].hClient;
        pItemArray[i].dwBlobSize = pAttributes[i].dwBlobSize;
        pItemArray[i].pBlob = pAttributes[i].pBlob;
        pItemArray[i].vtRequestedDataType = pAttributes[i].vtRequestedDataType;
    }

    hResult = pIOPCItemMgt->AddItems(dwCount,
                                     pItemArray,
                                     &pAddResults,
                                     &phErrors);

    ComStructFree(pItemArray);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCItemMgt AddItems() error:" << hResult;
        return false;
    }

    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);
    OPCHANDLE *phClient = ComStructAlloc<OPCHANDLE>(dwCount);

    for (DWORD i = 0; i < dwCount; ++i)
    {
        pAttributes[i].hServer = pAddResults[i].hServer;
        pAttributes[i].hClient = pAddResults[i].hServer;
        pAttributes[i].vtCanonicalDataType = pAddResults[i].vtCanonicalDataType;
        pAttributes[i].dwAccessRights = pAddResults[i].dwAccessRights;
        pAttributes[i].dwBlobSize = pAddResults[i].dwBlobSize;
        pAttributes[i].pBlob = pAddResults[i].pBlob;

        phServer[i] = pAddResults[i].hServer;
        phClient[i] = pAddResults[i].hServer;

    }

    ComStructFree(pAddResults);
    ComStructFree(phErrors);

    hResult = pIOPCItemMgt->SetClientHandles(dwCount,
                                             phServer,
                                             phClient,
                                             &phErrors);

    ComStructFree(phServer);
    ComStructFree(phClient);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCItemMgt SetClientHandles() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    qDebug() << "AddItems successfully.";
    return true;
}

bool XYOPCGroup::ValidateItems(DWORD dwCount, OPCITEMATTRIBUTES *pAttributes)
{
    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return false;
    }

    if (pAttributes == NULL)
    {
        qCritical() << "ValidateItems pAttributes is NULL!";
        return false;
    }

    if (dwCount == 0)
    {
        qCritical() << "ValidateItems dwCount is zero!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCITEMDEF *pItemArray = ComStructAlloc<OPCITEMDEF>(dwCount);
    BOOL bBlobUpdate = FALSE;
    OPCITEMRESULT *pValidationResults;

    for (DWORD i = 0; i < dwCount; ++i)
    {
        pItemArray[i].szAccessPath = pAttributes[i].szAccessPath;
        pItemArray[i].szItemID = pAttributes[i].szItemID;
        pItemArray[i].bActive = pAttributes[i].bActive;
        pItemArray[i].hClient = pAttributes[i].hClient;
        pItemArray[i].dwBlobSize = pAttributes[i].dwBlobSize;
        pItemArray[i].pBlob = pAttributes[i].pBlob;
        pItemArray[i].vtRequestedDataType = pAttributes[i].vtRequestedDataType;
    }

    hResult = pIOPCItemMgt->ValidateItems(dwCount,
                                          pItemArray,
                                          bBlobUpdate,
                                          &pValidationResults,
                                          &phErrors);

    ComStructFree(pItemArray);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCItemMgt ValidateItems() error:" << hResult;
        return false;
    }

    for (DWORD i = 0; i < dwCount; ++i)
    {
        pAttributes[i].hServer = pValidationResults[i].hServer;
        pAttributes[i].vtCanonicalDataType = pValidationResults[i].vtCanonicalDataType;
        pAttributes[i].dwAccessRights = pValidationResults[i].dwAccessRights;
        pAttributes[i].dwBlobSize = pValidationResults[i].dwBlobSize;
        pAttributes[i].pBlob = pValidationResults[i].pBlob;
    }

    ComStructFree(pValidationResults);
    ComStructFree(phErrors);

    qDebug() << "ValidateItems successfully.";
    return true;
}

void XYOPCGroup::RemoveItems(DWORD dwCount, OPCHANDLE *phServer)
{
    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return;
    }

    if (dwCount == 0)
    {
        qCritical() << "RemoveItems dwCount is zero!";
        return;
    }

    HRESULT hResult;
    HRESULT *phErrors;

    hResult = pIOPCItemMgt->RemoveItems(dwCount,
                                        phServer,
                                        &phErrors);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCItemMgt RemoveItems() error:" << hResult;
        return;
    }

    ComStructFree(phErrors);

    qDebug() << "RemoveItems successfully.";
}


void XYOPCGroup::RemoveAllItems()
{
    BrowseItemAttributes();

    DWORD dwCount = ItemAttributes.size();
    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);
    for (DWORD i = 0; i < dwCount; ++i)
    {
        phServer[i] = ItemAttributes.at(i)->hServer;
    }

    RemoveItems(dwCount, phServer);

    ComStructFree(phServer);
}

bool XYOPCGroup::SetActiveState(DWORD dwCount, OPCHANDLE *phServer, BOOL bActive)
{
    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return false;
    }

    if (dwCount == 0 || phServer == NULL)
    {
        qCritical() << "SetActiveState parameters invalid!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    hResult = pIOPCItemMgt->SetActiveState(dwCount,
                                           phServer,
                                           bActive,
                                           &phErrors);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCItemMgt SetActiveState() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    return true;
}

bool XYOPCGroup::SetClientHandle(DWORD dwCount, OPCHANDLE *phServer, OPCHANDLE *phClient)
{
    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return false;
    }

    if (dwCount == 0 || phServer == NULL || phClient == NULL)
    {
        qCritical() << "SetActiveState parameters invalid!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    hResult = pIOPCItemMgt->SetClientHandles(dwCount,
                                             phServer,
                                             phClient,
                                             &phErrors);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCItemMgt SetClientHandle() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    return true;
}

bool XYOPCGroup::SetDataType(DWORD dwCount, OPCHANDLE *phServer, VARTYPE *pvtRequested)
{
    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return false;
    }

    if (dwCount == 0 || phServer == NULL || pvtRequested == NULL)
    {
        qCritical() << "SetActiveState parameters invalid!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    hResult = pIOPCItemMgt->SetDatatypes(dwCount,
                                         phServer,
                                         pvtRequested,
                                         &phErrors);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCItemMgt SetClientHandle() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    return true;
}


ItemAttributeList XYOPCGroup::BrowseItemAttributes()
{
    DeleteItemAttributes();

    if (pIOPCItemMgt == NULL)
    {
        qCritical() << "IOPCItemMgt has not been created!";
        return ItemAttributes;
    }

    HRESULT hResult;
    IEnumOPCItemAttributes *pIEnumOPCItemAttributes;
    hResult = pIOPCItemMgt->CreateEnumerator(IID_IEnumOPCItemAttributes,
                                             (LPUNKNOWN *)&pIEnumOPCItemAttributes);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCItemMgt CreateEnumerator() error:" << hResult;
        return ItemAttributes;
    }

    OPCITEMATTRIBUTES *pAttributes;
    DWORD dwCount = 0;

    pIEnumOPCItemAttributes->Next(0xFFFF, &pAttributes, &dwCount);

    for (DWORD i = 0; i < dwCount; ++i)
    {
        ItemAttributes.append(pAttributes + i);
    }

    pIEnumOPCItemAttributes->Release();

    return ItemAttributes;
}

ItemStateList XYOPCGroup::BrowseItemStates()
{
    DeleteItemStates();

    if (pIOPCSyncIO == NULL)
    {
        qCritical() << "IOPCSyncIO has not been created!";
        return ItemStates;
    }

    if (ItemAttributes.size() == 0)
    {
        qCritical() << "IOPCSyncIO item size is zero!";
        return ItemStates;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCDATASOURCE dwSource = OPC_DS_DEVICE;
    DWORD dwCount = ItemAttributes.size();
    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);
    for (DWORD i = 0; i < dwCount; ++i)
    {
        phServer[i] = ItemAttributes.at(i)->hServer;
    }
    OPCITEMSTATE *pItemStates;

    hResult = pIOPCSyncIO->Read(dwSource,
                                dwCount,
                                phServer,
                                &pItemStates,
                                &phErrors);

    ComStructFree(phServer);

    if (FAILED(hResult))
    {
        qCritical() << "IOPCSyncIO Read() error:" << hResult;
        return ItemStates;
    }

    for (DWORD i = 0; i < dwCount; ++i)
    {
        ItemStates.append(pItemStates + i);
    }

    return ItemStates;
}

void XYOPCGroup::DeleteItemAttributes()
{
    if (!ItemAttributes.isEmpty())
    {
        ComStructFree(ItemAttributes.at(0));
        ItemAttributes.clear();
    }
}

void XYOPCGroup::DeleteItemStates()
{
    if (!ItemStates.isEmpty())
    {
        ComStructFree(ItemStates.at(0));
        ItemStates.clear();
    }
}

LPCWSTR XYOPCGroup::GetItemIDByHandle(OPCHANDLE hClient)
{
    for (auto attribute : ItemAttributes)
        if (attribute->hClient == hClient)
            return attribute->szItemID;

    return NULL;
}

VARTYPE XYOPCGroup::GetItemTypeByItemID(LPCWSTR szItemID)
{
    for (auto attribute : ItemAttributes)
        if (0 == wcscmp(attribute->szItemID, szItemID))
            return attribute->vtCanonicalDataType;

    return VT_EMPTY;
}

bool XYOPCGroup::AddCallback()
{
    if (pIConnectionPoint == NULL)
    {
        qCritical() << "IConnectionPoint has not been created!";
        return false;
    }

    if (pOPCCallback != NULL)
    {
        qCritical() << "OPCCallback has been created!";
        return true;
    }

    HRESULT hResult;
    pOPCCallback = new XYOPCCallback(this);

    hResult = pIConnectionPoint->Advise(pOPCCallback, &dwCookie);

    if (FAILED(hResult))
    {
        qCritical() << "IConnectionPoint Advise() error:" << hResult;
//        delete pOPCCallback;
        pOPCCallback = NULL;
        dwCookie = 0L;
        return false;
    }

    qDebug() << "Add Callback successfully.";

    return true;
}

void XYOPCGroup::RemoveCallback()
{
    if (pIConnectionPoint == NULL)
    {
        qCritical() << "IConnectionPoint has not been created!";
        return;
    }

    if (pOPCCallback == NULL)
    {
        qCritical() << "OPCCallback has not been created!";
        return;
    }

    HRESULT hResult;

    hResult = pIConnectionPoint->Unadvise(dwCookie);

    if (FAILED(hResult))
    {
        qCritical() << "IConnectionPoint Unadvise() error:" << hResult;
        return;
    }

//    delete pOPCCallback;
    pOPCCallback = NULL;
    dwCookie = 0L;

    qDebug() << "Remove Callback successfully.";
}

bool XYOPCGroup::ReadSync(DWORD dwCount, LPCWSTR *szItemIDs, VARIANT *pvValues)
{
    if (pIOPCSyncIO == NULL)
    {
        qCritical() << "IOPCSyncIO has not been created!";
        return false;
    }

    if (dwCount == 0)
    {
        qCritical() << "IOPCSyncIO dwCount is zero!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCDATASOURCE dwSource = OPC_DS_DEVICE;
    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);
    for (DWORD i = 0; i < dwCount; ++i)
    {
        for (auto pAttribute : ItemAttributes)
        {
            if (0 == wcscmp(pAttribute->szItemID, szItemIDs[i]))
            {
                phServer[i] = pAttribute->hServer;
                break;
            }
        }
    }
    OPCITEMSTATE *pItemStates;

    hResult = pIOPCSyncIO->Read(dwSource,
                                dwCount,
                                phServer,
                                &pItemStates,
                                &phErrors);

    ComStructFree(phServer);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCSyncIO Read() error:" << hResult;
        return false;
    }

    for (DWORD i = 0; i < dwCount; ++i)
    {
        pvValues[i] = pItemStates[i].vDataValue;
    }

    ComStructFree(pItemStates);
    ComStructFree(phErrors);

    return true;
}

bool XYOPCGroup::WriteSync(DWORD dwCount, LPCWSTR *szItemIDs, VARIANT *pvValues)
{
    if (pIOPCSyncIO == NULL)
    {
        qCritical() << "IOPCSyncIO has not been created!";
        return false;
    }

    if (dwCount == 0)
    {
        qCritical() << "IOPCSyncIO dwCount is zero!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);

    for (DWORD i = 0; i < dwCount; ++i)
    {
        for (auto pAttribute : ItemAttributes)
        {
            if (0 == wcscmp(pAttribute->szItemID, szItemIDs[i]))
            {
                phServer[i] = pAttribute->hServer;
                break;
            }
        }
    }

    hResult = pIOPCSyncIO->Write(dwCount,
                                 phServer,
                                 pvValues,
                                 &phErrors);

    ComStructFree(phServer);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCSyncIO WriteSync() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    return true;
}

bool XYOPCGroup::ReadAsync(DWORD dwCount, LPCWSTR *szItemIDs)
{
    if (pIOPCAsyncIO2 == NULL)
    {
        qCritical() << "IOPCAsyncIO2 has not been created!";
        return false;
    }

    if (dwCount == 0)
    {
        qCritical() << "IOPCSyncIO dwCount is zero!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);
    DWORD dwTransactionID = 0;
    DWORD dwCancelID = 0;
    for (DWORD i = 0; i < dwCount; ++i)
    {
        for (auto pAttribute : ItemAttributes)
        {
            if (0 == wcscmp(pAttribute->szItemID, szItemIDs[i]))
            {
                phServer[i] = pAttribute->hServer;
                break;
            }
        }
    }

    hResult = pIOPCAsyncIO2->Read(dwCount,
                                  phServer,
                                  dwTransactionID,
                                  &dwCancelID,
                                  &phErrors);

    ComStructFree(phServer);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCAsyncIO2 Read() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    return true;
}

bool XYOPCGroup::WriteAsync(DWORD dwCount, LPCWSTR *szItemIDs, VARIANT *pvValues)
{
    if (pIOPCAsyncIO2 == NULL)
    {
        qCritical() << "IOPCAsyncIO2 has not been created!";
        return false;
    }

    if (dwCount == 0)
    {
        qCritical() << "IOPCSyncIO dwCount is zero!";
        return false;
    }

    HRESULT hResult;
    HRESULT *phErrors;
    OPCHANDLE *phServer = ComStructAlloc<OPCHANDLE>(dwCount);
    DWORD dwTransactionID = 0;
    DWORD dwCancelID = 0;
    for (DWORD i = 0; i < dwCount; ++i)
    {
        for (auto pAttribute : ItemAttributes)
        {
            if (0 == wcscmp(pAttribute->szItemID, szItemIDs[i]))
            {
                phServer[i] = pAttribute->hServer;
                break;
            }
        }
    }

    hResult = pIOPCAsyncIO2->Write(dwCount,
                                   phServer,
                                   pvValues,
                                   dwTransactionID,
                                   &dwCancelID,
                                   &phErrors);

    ComStructFree(phServer);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qCritical() << "IOPCAsyncIO2 Write() error:" << hResult;
        return false;
    }

    ComStructFree(phErrors);

    return true;
}

void XYOPCGroup::OnValueChange(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState)
{ pOVCFunc(pOVCData, pHeader, pState); }
void XYOPCGroup::OnReadComplete(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState)
{ pORCFunc(pORCData, pHeader, pState); }
void XYOPCGroup::OnWriteComplete(OPCGROUPHEADER *pHeader)
{ pOWCFunc(pOWCData, pHeader); }
void XYOPCGroup::OnCancelComplete(OPCGROUPHEADER *pHeader)
{ pOCCFunc(pOCCData, pHeader); }

void XYOPCGroup::AddDataChangeFunc(ONVALUECHANGEFUNC pFunc, void *pData)
{ pOVCFunc = pFunc; pOVCData = pData; }
void XYOPCGroup::AddReadCompleteFunc(ONREADCOMPLETEFUNC pFunc, void *pData)
{ pORCFunc = pFunc; pORCData = pData; }
void XYOPCGroup::AddWriteCompleteFunc(ONWRITECOMPLETEFUNC pFunc, void *pData)
{ pOWCFunc = pFunc; pOWCData = pData; }
void XYOPCGroup::AddCancelCompleteFunc(ONCANCELCOMPLETEFUNC pFunc, void *pData)
{ pOCCFunc = pFunc; pOCCData = pData; }



























