#include "OPCDefine.h"
#include <QDebug>

const QString OPC_CONFIG_XML = "./OPC.Configuration.xml";

LPWSTR ComStringAlloc(LPCWSTR pzString)
{
    SIZE_T cb = wcslen(pzString) * sizeof(pzString);
    LPWSTR sz = (LPWSTR)CoTaskMemAlloc(cb);
    return wcscpy(sz, pzString);
}

LPWSTR ComStringRealloc(LPWSTR szAlloc, LPCWSTR pzString)
{
    SIZE_T cb = wcslen(pzString) * sizeof(pzString);
    LPWSTR sz = (LPWSTR)CoTaskMemRealloc(szAlloc, cb);
    return wcscpy(sz, pzString);
}

void ComStringFree(LPWSTR pzString)
{
    CoTaskMemFree(pzString);
}

QString BSTR_to_QString(const BSTR &bstr)
{
    return QString((QChar *)bstr, wcslen(bstr));
}

BSTR QString_to_BSTR(const QString &string)
{
    return ::SysAllocString((LPCOLESTR)string.unicode());
}

QString BSTRARRAY_to_QString(SAFEARRAY *array)
{
    QString string;
    BSTR *buff;
    LONG LBound, UBound;
    SafeArrayGetLBound(array, 1, &LBound);
    SafeArrayGetUBound(array, 1, &UBound);
    SafeArrayAccessData(array, (void **)&buff);

    for (LONG i = 0; i <= UBound - LBound; ++i)
    {
        QString temp = BSTR_to_QString(buff[i]);
        string += temp + " | ";
    }

    SafeArrayUnaccessData(array);

    return string;
}

QDateTime DATE_to_QDateTime(const DATE &date)
{
    SYSTEMTIME sysTime;
    QDateTime datetime;

    ::VariantTimeToSystemTime(date, &sysTime);

    datetime.setDate(QDate(sysTime.wYear, sysTime.wMonth, sysTime.wDay));
    datetime.setTime(QTime(sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds));

    return datetime;
}

DATE QDateTime_to_DATE(const QDateTime &datetime)
{
    SYSTEMTIME sysTime;
    DATE date;

    sysTime.wYear = (WORD)datetime.date().year();
    sysTime.wMonth = (WORD)datetime.date().month();
    sysTime.wDay = (WORD)datetime.date().day();
    sysTime.wHour = (WORD)datetime.time().hour();
    sysTime.wMinute = (WORD)datetime.time().minute();
    sysTime.wSecond = (WORD)datetime.time().second();
    sysTime.wMilliseconds = (WORD)datetime.time().msec();

    ::SystemTimeToVariantTime(&sysTime, &date);

    return date;
}

QDateTime FILETIME_to_QDateTime(const FILETIME &filetime)
{
    SYSTEMTIME sysTime;
    QDateTime datetime;

    ::FileTimeToSystemTime(&filetime, &sysTime);

    datetime.setDate(QDate(sysTime.wYear, sysTime.wMonth, sysTime.wDay));
    datetime.setTime(QTime(sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds));

    return datetime;
}

FILETIME QDateTime_to_FILETIME(const QDateTime &datetime)
{
    SYSTEMTIME sysTime;
    FILETIME filetime;

    sysTime.wYear = (WORD)datetime.date().year();
    sysTime.wMonth = (WORD)datetime.date().month();
    sysTime.wDay = (WORD)datetime.date().day();
    sysTime.wHour = (WORD)datetime.time().hour();
    sysTime.wMinute = (WORD)datetime.time().minute();
    sysTime.wSecond = (WORD)datetime.time().second();
    sysTime.wMilliseconds = (WORD)datetime.time().msec();

    ::SystemTimeToFileTime(&sysTime, &filetime);

    return filetime;
}

VARIANT QVariant_to_VARIANT(const QVariant &qvar, VARTYPE type)
{
    VARIANT var;
    var.vt = type;

    switch (var.vt)
    {
    case VT_BOOL: var.boolVal = qvar.value<bool>() ? -1 : 0; break;
    case VT_I1: var.cVal = qvar.value<char>(); break;
    case VT_UI1: var.bVal = qvar.value<unsigned char>(); break;
    case VT_I2: var.iVal = qvar.value<short>(); break;
    case VT_UI2: var.uiVal = qvar.value<unsigned short>(); break;
    case VT_I4: var.intVal = qvar.value<int>(); break;
    case VT_UI4: var.uintVal = qvar.value<unsigned int>(); break;
    case VT_I8: var.lVal = qvar.value<long>(); break;
    case VT_UI8: var.ulVal = qvar.value<unsigned long>(); break;
    case VT_R4: var.fltVal = qvar.value<float>(); break;
    case VT_R8: var.dblVal = qvar.value<double>(); break;
    case VT_BSTR: var.bstrVal = QString_to_BSTR(qvar.toString()); break;
    case VT_DATE: var.date = QDateTime_to_DATE(qvar.toDateTime()); break;
    default: break;
    }

    return var;
}

QVariant VARIANT_to_QVariant(const VARIANT &var)
{
    QVariant qvar;

    switch (var.vt)
    {
    case VT_BOOL: qvar.setValue(var.boolVal == -1); break;
    case VT_I1: qvar.setValue(var.cVal); break;
    case VT_UI1: qvar.setValue(var.bVal); break;
    case VT_I2: qvar.setValue(var.iVal); break;
    case VT_UI2: qvar.setValue(var.uiVal); break;
    case VT_I4: qvar.setValue(var.lVal); break;
    case VT_UI4: qvar.setValue(var.ulVal); break;
    case VT_I8: qvar.setValue(var.llVal); break;
    case VT_UI8: qvar.setValue(var.ullVal); break;
    case VT_R4: qvar.setValue(var.fltVal); break;
    case VT_R8: qvar.setValue(var.dblVal); break;
    case VT_BSTR: qvar.setValue(BSTR_to_QString(var.bstrVal)); break;
    case VT_DATE: qvar.setValue(DATE_to_QDateTime(var.date)); break;
    case VT_ARRAY | VT_BSTR: qvar.setValue(BSTRARRAY_to_QString(var.parray)); break;
    default: break;
    }

    return qvar;
}

QString VARTYPE_to_QString(VARTYPE type)
{
    switch (type)
    {
    case VT_BOOL: return "BOOL";
    case VT_I1: return "CHAR";
    case VT_UI1: return "BYTE";
    case VT_I2: return "INT";
    case VT_UI2: return "WORD";
    case VT_I4: return "DINT";
    case VT_UI4: return "DWORD";
    case VT_I8: return "LONGLONG";
    case VT_UI8: return "ULONGLONG";
    case VT_R4: return "FLOAT";
    case VT_R8: return "DOUBLE";
    case VT_BSTR: return "STRING";
    case VT_DATE: return "DATE";
    default: return "UNKNOWN";
    }
}

QString QUALITY_to_QString(WORD quality)
{
    switch (quality)
    {
    case OPC_QUALITY_BAD: return "BAD";
    case OPC_QUALITY_UNCERTAIN: return "UNCERTAIN";
    case OPC_QUALITY_GOOD: return "GOOD";
    case OPC_QUALITY_CONFIG_ERROR: return "CONFIG ERROR";
    case OPC_QUALITY_NOT_CONNECTED: return "NOT CONNECTED";
    case OPC_QUALITY_DEVICE_FAILURE: return "DEVICE FAILURE";
    case OPC_QUALITY_SENSOR_FAILURE: return "SENSOR FAILURE";
    case OPC_QUALITY_LAST_KNOWN: return "LAST KNOWN";
    case OPC_QUALITY_COMM_FAILURE: return "COMM FAILURE";
    case OPC_QUALITY_OUT_OF_SERVICE: return "OUT OF SERVICE";
    case OPC_QUALITY_WAITING_FOR_INITIAL_DATA: return "WAITING FOR INITIAL DATA";
    case OPC_QUALITY_LAST_USABLE: return "LAST USABLE";
    case OPC_QUALITY_SENSOR_CAL: return "SENSOR CAL";
    case OPC_QUALITY_EGU_EXCEEDED: return "EGU EXCEEDED";
    case OPC_QUALITY_SUB_NORMAL: return "SUB NORMAL";
    case OPC_QUALITY_LOCAL_OVERRIDE: return "LOCAL OVERRIDE";
    default: return "UNKNOWN";
    }
}

QString RIGHTS_to_QString(DWORD right)
{
    switch (right)
    {
    case OPC_READABLE: return "READ ONLY";
    case OPC_WRITEABLE: return "WRITE ONLY";
    case OPC_READABLE | OPC_WRITEABLE: return "READ & WRITE";
    default: return "UNKNOWN";
    }
}

QString EUTYPE_to_QString(OPCEUTYPE type)
{
    switch (type)
    {
    case OPC_NOENUM: return "NOENUM";
    case OPC_ANALOG: return "ANALOG";
    case OPC_ENUMERATED: return "ENUMERATED";
    default: return "UNKNOWN";
    }
}

QString SERVERSTATE_to_String(OPCSERVERSTATE state)
{
    switch (state)
    {
    case OPC_STATUS_RUNNING: return "RUNNING";
    case OPC_STATUS_FAILED: return "FAILED";
    case OPC_STATUS_NOCONFIG: return "NOCONFIG";
    case OPC_STATUS_SUSPENDED: return "SUSPENDED";
    case OPC_STATUS_TEST: return "TEST";
    case OPC_STATUS_COMM_FAULT: return "COMM FAULT";
    default: return "UNKNOWN";
    }
}
