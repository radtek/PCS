#include "OPCCallback.h"
#include "OPCGroup.h"
#include <QDebug>

XYOPCCallback::XYOPCCallback(XYOPCGroup *pGroup)
    : pOPCGroup(pGroup)
    , dwRefCount(0)
{

}

XYOPCCallback::~XYOPCCallback()
{

}

HRESULT STDMETHODCALLTYPE XYOPCCallback::QueryInterface(REFIID riid, void **ppv)
{
    *ppv = NULL;


    if (IID_IUnknown == riid)
        *ppv = reinterpret_cast<IUnknown *>(this);
    else if (IID_IOPCDataCallback == riid)
        *ppv = (IOPCDataCallback *)this;

    if (NULL != *ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }

    return ResultFromScode(E_NOINTERFACE);
}

ULONG STDMETHODCALLTYPE XYOPCCallback::AddRef(void)
{
    return ++dwRefCount;
}

ULONG STDMETHODCALLTYPE XYOPCCallback::Release(void)
{
    if (0 == --dwRefCount)
        delete this;

    return dwRefCount;
}

HRESULT STDMETHODCALLTYPE XYOPCCallback::OnDataChange(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE *phClientItems,
    /* [size_is][in] */ VARIANT *pvValues,
    /* [size_is][in] */ WORD *pwQualities,
    /* [size_is][in] */ FILETIME *pftTimeStamps,
    /* [size_is][in] */ HRESULT *pErrors)
{
    OPCGROUPHEADER *pHeader = ComStructAlloc<OPCGROUPHEADER>();
    pHeader->dwTransactionID = dwTransid;
    pHeader->hClientGroup = hGroup;
    pHeader->dwItemCount = dwCount;
    pHeader->hrStatus = hrMastererror;

    OPCITEMSTATE *pState = ComStructAlloc<OPCITEMSTATE>(dwCount);
    for (DWORD i = 0; i < dwCount; ++i)
    {
        pState[i].hClient = phClientItems[i];
        pState[i].vDataValue = pvValues[i];
        pState[i].wQuality = pwQualities[i];
        pState[i].ftTimeStamp = pftTimeStamps[i];
    }

    pOPCGroup->OnValueChange(pHeader, pState);

    ComStructFree(pHeader);
    ComStructFree(pState);
    return NO_ERROR;
}

HRESULT STDMETHODCALLTYPE XYOPCCallback::OnReadComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE *phClientItems,
    /* [size_is][in] */ VARIANT *pvValues,
    /* [size_is][in] */ WORD *pwQualities,
    /* [size_is][in] */ FILETIME *pftTimeStamps,
    /* [size_is][in] */ HRESULT *pErrors)
{
    OPCGROUPHEADER *pHeader = ComStructAlloc<OPCGROUPHEADER>();
    pHeader->dwTransactionID = dwTransid;
    pHeader->hClientGroup = hGroup;
    pHeader->dwItemCount = dwCount;
    pHeader->hrStatus = hrMastererror;

    OPCITEMSTATE *pState = ComStructAlloc<OPCITEMSTATE>(dwCount);
    for (DWORD i = 0; i < dwCount; ++i)
    {
        pState[i].hClient = phClientItems[i];
        pState[i].vDataValue = pvValues[i];
        pState[i].wQuality = pwQualities[i];
        pState[i].ftTimeStamp = pftTimeStamps[i];
    }

    pOPCGroup->OnReadComplete(pHeader, pState);

    ComStructFree(pHeader);
    ComStructFree(pState);
    return NO_ERROR;
}

HRESULT STDMETHODCALLTYPE XYOPCCallback::OnWriteComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMastererr,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE *pClienthandles,
    /* [size_is][in] */ HRESULT *pErrors)
{
    OPCGROUPHEADER *pHeader = ComStructAlloc<OPCGROUPHEADER>();
    pHeader->dwTransactionID = dwTransid;
    pHeader->hClientGroup = hGroup;
    pHeader->dwItemCount = dwCount;
    pHeader->hrStatus = hrMastererr;

    pOPCGroup->OnWriteComplete(pHeader);

    ComStructFree(pHeader);
    return NO_ERROR;
}

HRESULT STDMETHODCALLTYPE XYOPCCallback::OnCancelComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup)
{
    OPCGROUPHEADER *pHeader = ComStructAlloc<OPCGROUPHEADER>();
    pHeader->dwTransactionID = dwTransid;
    pHeader->hClientGroup = hGroup;

    pOPCGroup->OnCancelComplete(pHeader);

    ComStructFree(pHeader);
    return NO_ERROR;
}
