#ifndef PCS_MAIN_GLOBALDATA_H
#define PCS_MAIN_GLOBALDATA_H

#include "GCfgManager.h"
#include "PCSApplication.h"
#include "XYLogManager.h"
#include "XYSqlManager.h"
#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtSerialPort>
#include <QtSql>
#include <QtWidgets>
#include <QtXml>

struct DataDefine
{
    QString itemID;
    QVariant itemValue;

    bool operator==(const DataDefine &data) const
    {
        return itemID == data.itemID && itemValue == data.itemValue;
    }
};

struct ErrorDefine
{
    QString deviceID;
    QString errorText;
};

//提交单据类型
enum class SubmitDocType
{
    MachineCheckDoc,      /// 设备点检单
    ProductCheckDoc,      /// 产品点检
    FixtureCheckDoc,      /// 工装模具指导书点检
    FirstProductQADoc,    /// 首件检验
    FinalProductQADoc,    /// 末件检验
    RouteProductQADoc,    /// 巡检
    FirstPackageQADoc,    /// 首箱检验
    FinalPackageQADoc,    /// 末箱检验
};

//提交单据结果
enum class SubmitDocResult
{
    ResultOK,
    ResultNG,
};

void MessageBox_Information(const QString &text);
void MessageBox_Warning(const QString &text);
void MessageBox_Critical(const QString &text);
bool MessageBox_Question(const QString &text);
int MessageBox_Question3(const QString &text);
void MessageBox_Critical1(const QString &text);

#endif
