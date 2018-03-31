#ifndef BARCODEITEMDEFINE_H
#define BARCODEITEMDEFINE_H

#include "GlobalInc.h"

enum class TemplateItemType
{
    FixedText,
    Barcode,
    DateFormat,
    SerialNo10,
    SerialNo16,
    PackWeight,
    PackNumber,
};

enum class BarcodeItemType
{
    FixedText,
    DateFormat,
    DayOfYear,
    WeekOfYear,
    YearCode,
    MonthCode,
    DayCode,
    SerialNo10,
    SerialNo16,
    SerialNo36,
    CheckCode,
    BatchCode,
    RepairCode,
};

enum class CheckCodeType
{
    VW,
    SL,
    QR,
};

struct TemplateItem
{
    QString type;
    QString name;
    QString text;
};

struct BarcodeItem
{
    QString type;
    int size;
    QString text;
};

struct LabelPrintItems
{
    QString templateFile;
    QString printerName;
    QString dateCode;
    QString checkCode;

    QList<TemplateItem> templateItems;
    QList<BarcodeItem> barcodeItems;

    void initial()
    {
        templateFile.clear();
        printerName.clear();
        dateCode.clear();
        checkCode.clear();

        templateItems.clear();
        barcodeItems.clear();
    }
};

struct DateCodeItems
{
    QMap<int, QString> yearMap;
    QMap<int, QString> monthMap;
    QMap<int, QString> dayMap;

    void initial()
    {
        yearMap.clear();
        monthMap.clear();
        dayMap.clear();
    }
};

struct PrintVariants
{
    QDate planDate;
    int serialNo;
    double packWeight;
    int packNumber;
};

const static QMap<TemplateItemType, QString> templateItemTypeMap =
{
    {TemplateItemType::FixedText,   "固定内容"},
    {TemplateItemType::Barcode,     "追溯条码"},
    {TemplateItemType::DateFormat,  "格式化日期"},
    {TemplateItemType::SerialNo10,  "流水码(10)"},
    {TemplateItemType::SerialNo16,  "流水码(16)"},
    {TemplateItemType::PackWeight,  "包装箱重量"},
    {TemplateItemType::PackNumber,  "包装箱数量"},
};

const static QMap<BarcodeItemType, QString> barcodeItemTypeMap =
{
    {BarcodeItemType::FixedText,    "固定内容"},
    {BarcodeItemType::DateFormat,   "格式化日期"},
    {BarcodeItemType::YearCode,     "年代码"},
    {BarcodeItemType::MonthCode,    "月代码"},
    {BarcodeItemType::DayCode,      "日代码"},
    {BarcodeItemType::DayOfYear,    "年天数"},
    {BarcodeItemType::WeekOfYear,   "年星期数"},
    {BarcodeItemType::SerialNo10,   "流水码(10)"},
    {BarcodeItemType::SerialNo16,   "流水码(16)"},
    {BarcodeItemType::SerialNo36,   "流水码(36)"},
    {BarcodeItemType::CheckCode,    "校验码"},
    {BarcodeItemType::BatchCode,    "批次码"},
    {BarcodeItemType::RepairCode,   "返修码"},
};

const static QMap<CheckCodeType, QString> checkCodeTypeMap =
{
    {CheckCodeType::VW,     "大众抽检"},
    {CheckCodeType::SL,     "神龙抽检"},
    {CheckCodeType::QR,     "奇瑞抽检"},
};

bool labelPrint_readXmlFile(const QString &filename, LabelPrintItems *ptr);
bool labelPrint_saveXmlFile(const QString &filename, LabelPrintItems *ptr);
bool dateCode_readXmlFile(const QString &filename, DateCodeItems *ptr);
bool dateCode_saveXmlFile(const QString &filename, DateCodeItems *ptr);
char checkCode_generateVW(const QString &string);
char checkCode_generateSL(const QString &string);
char checkCode_generateQR(const QString &string);

#endif // BARCODEITEMDEFINE_H
