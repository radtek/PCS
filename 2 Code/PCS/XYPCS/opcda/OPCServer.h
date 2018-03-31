#ifndef OPCSERVER_H
#define OPCSERVER_H

#include "OPCDefine.h"

Q_DECLARE_METATYPE(XYOPCServer *)

class XYOPCGroup;

class XYOPCServer
{
public:
    XYOPCServer(IOPCServer *pServer, OPCSERVERINFO *pInfo);
    ~XYOPCServer();

    inline LPCWSTR GetProgID() const
    { return ServerInfo.szProgID; }
    inline OPCGroupList GetGroupList() const
    { return GroupList;}

    inline void GetServerInfo(OPCSERVERINFO *pInfo)
    { if (pInfo != NULL) *pInfo = ServerInfo; }
    inline void ReleaseInterface(IUnknown *pIUnknown)
    { if (pIUnknown != NULL) pIUnknown->Release(); }

    void GetServerStatus(OPCSERVERSTATUS *pStatus);

    ItemBrowseList BrowseItemIDs(LPCWSTR szString, OPCBROWSETYPE dwType);
    ItemPropertyList QueryItemProperties(LPCWSTR szItem);

    XYOPCGroup *AddGroup(OPCGROUPSTATE *pState);
    void AddGroup(XYOPCGroup *pGroup);
    void RemoveGroup(XYOPCGroup *pGroup);
    void RemoveAllGroup();

    XYOPCGroup *GetGroupByHandle(OPCHANDLE hClient);
private:
    IOPCServer *pIOPCServer;
    IOPCItemProperties *pIOPCItemProperties;
    IOPCBrowseServerAddressSpace *pIOPCBrowseServerAddressSpace;

    OPCGroupList GroupList;

    OPCSERVERINFO ServerInfo;
};


#endif // OPCSERVER_H
