#ifndef OPCDATACALLBACK_H
#define OPCDATACALLBACK_H

#include "OPCDefine.h"

class XYOPCGroup;

class XYOPCCallback : public IOPCDataCallback
{
public:
    explicit XYOPCCallback(XYOPCGroup *pGroup);
    virtual ~XYOPCCallback();

public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    virtual HRESULT STDMETHODCALLTYPE OnDataChange(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors);

    virtual HRESULT STDMETHODCALLTYPE OnReadComplete(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors);

    virtual HRESULT STDMETHODCALLTYPE OnWriteComplete(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMastererr,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *pClienthandles,
        /* [size_is][in] */ HRESULT *pErrors);

    virtual HRESULT STDMETHODCALLTYPE OnCancelComplete(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup);

private:
    XYOPCGroup *pOPCGroup;
    DWORD dwRefCount;
};


#endif // OPCDATACALLBACK_H
