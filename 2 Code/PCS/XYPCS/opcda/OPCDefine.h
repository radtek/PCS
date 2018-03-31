#ifndef OPCDEFINE_H
#define OPCDEFINE_H

#include "opcguid.h"
#include "opcda.h"
#include "opccomn.h"
#include "opcerror.h"

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QUuid>
#include <QDebug>

extern const QString OPC_CONFIG_XML;

typedef struct tagOPCSERVERINFO
{
    CLSID clsid;
    LPWSTR szProgID;
    LPWSTR szUserType;
    LPWSTR szMachineName;
} OPCSERVERINFO;

typedef struct tagOPCGROUPSTATE
{
    LPWSTR szName;
    BOOL bActive;
    DWORD dwUpdateRate;
    LONG lTimeBias;
    FLOAT fPercentDeadband;
    DWORD dwLCID;
    OPCHANDLE hClientGroup;
    OPCHANDLE hServerGroup;
} OPCGROUPSTATE;

typedef struct tagOPCITEMBROWSE
{
    LPWSTR szItemID;
    LPWSTR szItemDataID;
} OPCITEMBROWSE;

class XYOPCManager;
class XYOPCServerList;
class XYOPCServer;
class XYOPCGroup;

Q_DECLARE_METATYPE(OPCSERVERINFO *)
Q_DECLARE_METATYPE(OPCGROUPSTATE *)
Q_DECLARE_METATYPE(OPCITEMBROWSE *)
Q_DECLARE_METATYPE(OPCITEMPROPERTY *)
Q_DECLARE_METATYPE(OPCITEMATTRIBUTES *)
Q_DECLARE_METATYPE(OPCITEMSTATE *)

typedef QList<OPCSERVERINFO *> ServerInfoList;
typedef QList<OPCGROUPSTATE *> GroupStateList;
typedef QList<OPCITEMBROWSE *> ItemBrowseList;
typedef QList<OPCITEMPROPERTY *> ItemPropertyList;
typedef QList<OPCITEMATTRIBUTES *> ItemAttributeList;
typedef QList<OPCITEMSTATE *> ItemStateList;
typedef QList<XYOPCServerList *> OPCServerListList;
typedef QList<XYOPCServer *> OPCServerList;
typedef QList<XYOPCGroup *> OPCGroupList;

template <class T>
T *ComStructAlloc(size_t s = 1)
{ return reinterpret_cast<T *>(CoTaskMemAlloc(sizeof(T) * s));}

inline void ComStructFree(void *p)
{ CoTaskMemFree(p); }

LPWSTR ComStringAlloc(LPCWSTR pzString);
LPWSTR ComStringRealloc(LPWSTR szAlloc, LPCWSTR pzString);
void ComStringFree(LPWSTR pzString);

QString BSTR_to_QString(const BSTR &bstr);
BSTR QString_to_BSTR(const QString &string);
QString BSTRARRAY_to_QString(SAFEARRAY *array);

QDateTime DATE_to_QDateTime(const DATE &date);
DATE QDateTime_to_DATE(const QDateTime &datetime);

QDateTime FILETIME_to_QDateTime(const FILETIME &filetime);
FILETIME QDateTime_to_FILETIME(const QDateTime &datetime);

VARIANT QVariant_to_VARIANT(const QVariant &qvar, VARTYPE type);
QVariant VARIANT_to_QVariant(const VARIANT &var);

QString VARTYPE_to_QString(VARTYPE type);
QString QUALITY_to_QString(WORD quality);
QString RIGHTS_to_QString(DWORD right);
QString EUTYPE_to_QString(OPCEUTYPE type);
QString SERVERSTATE_to_String(OPCSERVERSTATE state);

#endif // OPCDEFINE_H
