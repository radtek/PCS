#ifndef OPCGROUP_H
#define OPCGROUP_H

#include "OPCDefine.h"

Q_DECLARE_METATYPE(XYOPCGroup *)

typedef void (__stdcall *ONVALUECHANGEFUNC)(void *, OPCGROUPHEADER *, OPCITEMSTATE *);
typedef void (__stdcall *ONREADCOMPLETEFUNC)(void *, OPCGROUPHEADER *, OPCITEMSTATE *);
typedef void (__stdcall *ONWRITECOMPLETEFUNC)(void *, OPCGROUPHEADER *);
typedef void (__stdcall *ONCANCELCOMPLETEFUNC)(void *, OPCGROUPHEADER *);

class XYOPCServer;
class XYOPCCallback;

class XYOPCGroup
{
public:
    XYOPCGroup(IOPCGroupStateMgt *pGroup, XYOPCServer *pServer);
    ~XYOPCGroup();

    inline XYOPCServer *GetServer()
    { return pOPCServer; }
    inline LPCWSTR GetName()
    { return GroupState.szName; }
    inline BOOL GetActive()
    { return GroupState.bActive; }
    inline BOOL GetMonitor()
    { return pOPCCallback != NULL; }
    inline OPCHANDLE GetServerHandle()
    { return GroupState.hServerGroup; }
    inline OPCHANDLE GetClientHandle()
    { return GroupState.hClientGroup; }

    inline void ReleaseInterface(IUnknown *pIUnknown)
    { if (pIUnknown != NULL) pIUnknown->Release(); }

    XYOPCGroup *CloneGroup(LPCWSTR szName);
    bool SetGroupState(OPCGROUPSTATE *pState);
    void GetGroupState(OPCGROUPSTATE *pState);

    bool AddItems(DWORD dwCount, OPCITEMATTRIBUTES *pAttributes);
    void RemoveItems(DWORD dwCount, OPCHANDLE *phServer);
    void RemoveAllItems();
    bool ValidateItems(DWORD dwCount, OPCITEMATTRIBUTES *pAttributes);

    bool SetActiveState(DWORD dwCount, OPCHANDLE *phServer, BOOL bActive);
    bool SetClientHandle(DWORD dwCount, OPCHANDLE *phServer, OPCHANDLE *phClient);
    bool SetDataType(DWORD dwCount, OPCHANDLE *phServer, VARTYPE *pvtRequested);

    ItemAttributeList BrowseItemAttributes();
    ItemStateList BrowseItemStates();

    LPCWSTR GetItemIDByHandle(OPCHANDLE hClient);
    VARTYPE GetItemTypeByItemID(LPCWSTR szItemID);

    bool AddCallback();
    void RemoveCallback();

    bool ReadSync(DWORD dwCount, LPCWSTR *szItemIDs, VARIANT *pvValues);
    bool WriteSync(DWORD dwCount, LPCWSTR *szItemIDs, VARIANT *pvValues);
    bool ReadAsync(DWORD dwCount, LPCWSTR *szItemIDs);
    bool WriteAsync(DWORD dwCount, LPCWSTR *szItemIDs, VARIANT *pvValues);

    void OnValueChange(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
    void OnReadComplete(OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
    void OnWriteComplete(OPCGROUPHEADER *pHeader);
    void OnCancelComplete(OPCGROUPHEADER *pHeader);

    void AddDataChangeFunc(ONVALUECHANGEFUNC pFunc, void *pData);
    void AddReadCompleteFunc(ONREADCOMPLETEFUNC pFunc, void *pData);
    void AddWriteCompleteFunc(ONWRITECOMPLETEFUNC pFunc, void *pData);
    void AddCancelCompleteFunc(ONCANCELCOMPLETEFUNC pFunc, void *pData);

private:
    void DeleteItemAttributes();
    void DeleteItemStates();
private:
    IOPCGroupStateMgt *pIOPCGroupStateMgt;
    IConnectionPointContainer *pIConnectionPointContainer;
    IConnectionPoint *pIConnectionPoint;
    IOPCItemMgt *pIOPCItemMgt;
    IOPCSyncIO *pIOPCSyncIO;
    IOPCAsyncIO2 *pIOPCAsyncIO2;

    XYOPCServer *pOPCServer;
    XYOPCCallback *pOPCCallback;

    DWORD dwCookie;
    OPCGROUPSTATE GroupState;

    ItemAttributeList ItemAttributes;
    ItemStateList ItemStates;

    ONVALUECHANGEFUNC pOVCFunc;
    ONREADCOMPLETEFUNC pORCFunc;
    ONWRITECOMPLETEFUNC pOWCFunc;
    ONCANCELCOMPLETEFUNC pOCCFunc;
    void *pOVCData;
    void *pORCData;
    void *pOWCData;
    void *pOCCData;
};


#endif // OPCGROUP_H
