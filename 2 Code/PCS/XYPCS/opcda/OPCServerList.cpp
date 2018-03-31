#include "OPCServerList.h"
#include <QUuid>
#include <QDebug>

XYOPCServerList::XYOPCServerList(IOPCServerList *pServerList, LPCWSTR szName)
{
    pIOPCServerList = pServerList;
    szMachineName = ComStringAlloc(szName);
}

XYOPCServerList::~XYOPCServerList()
{
    ComStringFree(szMachineName);

    ReleaseInterface(pIOPCServerList);
}

ServerInfoList XYOPCServerList::BrowseServerList()
{
    ServerInfoList infoList;

    if (pIOPCServerList == NULL)
    {
        qDebug() << "IOPCServerList has not been created!";
        return infoList;
    }

    HRESULT hResult;
    CATID catid[2];
    ULONG ulSize = 2;
    catid[0] = CATID_OPCDAServer10;
    catid[1] = CATID_OPCDAServer20;
    IOPCEnumGUID *pIOPCEnumGUID;

    hResult = pIOPCServerList->EnumClassesOfCategories(ulSize,
                                                       catid,
                                                       ulSize,
                                                       catid,
                                                       (IEnumGUID **)&pIOPCEnumGUID);

    if (FAILED(hResult) || hResult == S_FALSE)
    {
        qDebug() << "IOPCServerList EnumClassesOfCategories() error:" << hResult;
        return infoList;
    }

    do
    {
        CLSID clsid;
        ULONG ulCount = 0;

        hResult = pIOPCEnumGUID->Next(1, &clsid, &ulCount);

        if (ulCount > 0)
        {
            LPOLESTR szProgID;
            LPOLESTR szUserType;

            pIOPCServerList->GetClassDetails(clsid, &szProgID, &szUserType);

            OPCSERVERINFO *pInfo = ComStructAlloc<OPCSERVERINFO>();
            pInfo->clsid = clsid;
            pInfo->szProgID = ComStringAlloc(szProgID);
            pInfo->szUserType = ComStringAlloc(szUserType);
            pInfo->szMachineName = ComStringAlloc(szMachineName);
            infoList.append(pInfo);

            ComStringFree(szProgID);
            ComStringFree(szUserType);
        }

    } while (hResult == S_OK);

    pIOPCEnumGUID->Release();

    qDebug() << "Browse OPCServerList successfully.";

    return infoList;
}
