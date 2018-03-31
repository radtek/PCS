#ifndef OPCSERVERLIST_H
#define OPCSERVERLIST_H

#include "OPCDefine.h"

class XYOPCServerList
{
public:
    XYOPCServerList(IOPCServerList *pServerList, LPCWSTR szName);
    ~XYOPCServerList();

    inline void ReleaseInterface(IUnknown *pIUnknown)
    { if (pIUnknown != NULL) pIUnknown->Release(); }

    ServerInfoList BrowseServerList();

private:
    IOPCServerList *pIOPCServerList;
    LPWSTR szMachineName;
};

#endif // OPCSERVERLIST_H
