/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    QtBarTender.h
 * @brief   BarTender条码打印库
 * @version 1.0
 * @note
 *  部分功能待完善
 * *****************************************************************************
 */

#ifndef XYTECH_QTBARTENDER_H
#define XYTECH_QTBARTENDER_H

#include "XYComExport.h"

#ifndef BARTENDER_EXPORT
#define BARTENDER_EXPORT XYCOM_EXPORT
#endif

#include <QAxObject>

namespace BarTender
{

enum class BtBarCodePrintMethod
{
    btBarCodeRasterGraphics = 0,
    btBarCodeDeviceLines = 1,
    btBarCodeAuto = 2
};

enum class BtBoxPrintMethod
{
    btBoxRasterGraphics = 0,
    btBoxDeviceLines = 1,
    btBoxDeviceBox = 2,
    btBoxAuto = 3
};

enum class BtCacheFlushInterval
{
    btCacheFlushPerSession = 0,
    btCacheFlushDaily = 1,
    btCacheFlushWeekly = 2,
    btCacheFlushMonthly = 3,
    btCacheFlushNever = 4
};

enum class BtColors
{
    btColorsMono = 0,
    btColors16 = 1,
    btColors256 = 2,
    btColors24Bit = 4
};

enum class BtEllipsePrintMethod
{
    btEllipseRasterGraphics = 0,
    btEllipseDevice = 1,
    btEllipseAuto = 2
};

enum class BtFontScript
{
    btFontScriptArabic = 178,
    btFontScriptBaltic = 186,
    btFontScriptCentralEurope = 238,
    btFontScriptChineseGB2312 = 134,
    btFontScriptChineseBig5 = 136,
    btFontScriptCyrillic = 204,
    btFontScriptGreek = 161,
    btFontScriptHebrew = 177,
    btFontScriptJapanese = 128,
    btFontScriptKoreanHangul = 129,
    btFontScriptKoreanJohab = 130,
    btFontScriptMac = 77,
    btFontScriptOEM = 255,
    btFontScriptSymbol = 2,
    btFontScriptThai = 222,
    btFontScriptTurkish = 162,
    btFontScriptVietnamese = 163,
    btFontScriptWestern = 0
};

enum class BtFontWeight
{
    btFontWeightThin = 100,
    btFontWeightExtraLight = 200,
    btFontWeightLight = 300,
    btFontWeightNormal = 400,
    btFontWeightMedium = 500,
    btFontWeightSemiBold = 600,
    btFontWeightBold = 700,
    btFontWeightExtraBold = 800,
    btFontWeightHeavy = 900
};

enum class BtFormatIdValueType
{
    btFormatIdConstant = 0,
    btFormatIdFromTemplateField = 1,
    btFormatIdSetByDriver = 2,
    btFormatIdNotSupported = 3
};

enum class BtHorizontalAlignment
{
    btHorizontalAlignmentLeft = 0,
    btHorizontalAlignmentCenter = 1,
    btHorizontalAlignmentRight = 2
};

enum class BtInputDataFile
{
    btTextFile = 0,
    btODBC = 1,
    btSAPIDoc = 2,
    btOLEDB = 3
};

enum class BtLinePrintMethod
{
    btLineRasterGraphics = 0,
    btLineDeviceLines = 1,
    btLineAuto = 2
};

enum class BtMediaHandlingAction
{
    btMHDriverSettings = 0,
    btMHPageBreak = 1,
    btMHPause = 2,
    btMHWaitForLabelTakenSensor = 3,
    btMHCut = 4,
    btMHCutAndPause = 5,
    btMHPartialCut = 6,
    btMHPartialCutAndPause = 7
};

enum class BtMediaHandlingDataItemType
{
    btMHDatabaseField = 0,
    btMHNamedSubstring = 1,
    btMHNamedObject = 2
};

enum class BtMediaHandlingOccurrence
{
    btMHAtEndOfPrintJob = 0,
    btMHAfterEveryPage = 1,
    btMHAfterEveryRecord = 2,
    btMHAfterIdenticalCopies = 3,
    btMHAfterDataChange = 4,
    btMHAfterNumberOfLabels = 5
};

enum class BtMsgSeverity
{
    btMsgSeverityInformation = 0,
    btMsgSeverityWarning = 1,
    btMsgSeverityError = 2
};

enum class BtMsgType
{
    btMsgTypeMiscellaneous = 0,
    btMsgTypeVBScripting = 1,
    btMsgTypePrinter = 2,
    btMsgTypeDatabase = 3,
    btMsgTypeFile = 4,
    btMsgTypeCommandLine = 5,
    btMsgTypeNetworkTrace = 6,
    btMsgTypeLicensing = 7,
    btMsgTypeEmail = 8,
    btMsgTypeService = 9,
    btMsgTypeVerification = 10,
    btMsgTypeAutomation = 11,
    btMsgTypeXMLScript = 12,
    btMsgTypeCluster = 13,
    btMsgTypeLogToDatabase = 14,
    btMsgTypePrinting = 15
};

enum class BtNameType
{
    btUsedNamedSubStrings = 0,
    btUsedDatabaseFields = 1,
    btUsedTemplateFields = 2
};

enum class BtObjectType
{
    btObjectError = -1,
    btObjectLine = 2,
    btObjectBox = 3,
    btObjectBarcode = 4,
    btObjectText = 5,
    btObjectPicture = 6,
    btObjectRFID = 7,
    btObjectEllipse = 8,
    btObjectRichText = 9,
    btObjectShape = 10,
    btObjectMagneticStripe = 11
};

enum class BtOrientation
{
    btPortrait = 0,
    btLandscape = 1,
    btPortrait180 = 2,
    btLandscape180 = 3
};

enum class BtPctExportType
{
    btPctExportSeparate = 0,
    btPctExportCombined = 1,
    btPctExportToPort = 2
};

enum class BtPctExportVersion
{
    btPctExportVersion773AndEarlier = 0,
    btPctExportVersionCurrent = 1
};

enum class BtPrintPreviewResult
{
    btError = 0,
    btCanceled = 1,
    btClosed = 2,
    btPrinted = 3
};

enum class BtPrintResult
{
    btSuccess = 0,
    btTimeout = 1,
    btFailure = 2
};

enum class BtPrinterBatchCopiesValueType
{
    btPrinterBatchCopiesConstant = 0,
    btPrinterBatchCopiesFromTemplateField = 1,
    btPrinterBatchCopiesFromPrintDialogCopies = 2,
    btPrinterBatchCopiesNotSupported = 3
};

enum class BtPrinterCopiesValueType
{
    btPrinterCopiesConstant = 0,
    btPrinterCopiesFromTemplateField = 1,
    btPrinterCopiesFromPrintDialogCopies = 2,
    btPrinterCopiesNotSupported = 3
};

enum class BtPrinterSerializedCopiesValueType
{
    btPrinterSerializedCopiesConstant = 0,
    btPrinterSerializedCopiesFromTemplateField = 1,
    btPrinterSerializedCopiesFromPrintDialogCopies = 2,
    btPrinterSerializedCopiesNotSupported = 3
};

enum class BtResolution
{
    btResolutionScreen = 0,
    btResolutionPrinter = 1
};

enum class BtSaveOptions
{
    btPromptSave = 0,
    btDoNotSaveChanges = 1,
    btSaveChanges = 2
};

enum class BtShape
{
    btSquare = 1,
    btRectangle = 2,
    btRound = 3,
    btEllipse = 4
};

enum class BtStartingCorner
{
    btTopLeft = 0,
    btTopRight = 1,
    btBottomLeft = 2,
    btBottomRight = 3
};

enum class BtStartingPositionDirection
{
    btHorizontal = 0,
    btVertical = 1
};

enum class BtSubStringType
{
    btSubStringScreenData = 0,
    btSubStringDate = 1,
    btSubStringTime = 2,
    btSubStringVBScript = 3,
    btSubStringDatabase = 4,
    btSubStringDiskWizard = 5,
    btSubStringTemplateField = 6,
    btSubStringLabelObject = 7,
    btSubStringPrintJobField = 8,
    btSubStringGlobalDataField = 9
};

enum class BtTextFileDelimitation
{
    btDelimMixedQuoteAndComma = 0,
    btDelimQuoteAndComma = 1,
    btDelimComma = 2,
    btDelimTab = 3,
    btDelimCustom = 4
};

enum class BtTextPrintMethod
{
    btTextRasterGraphics = 0,
    btTextOutput = 1
};

enum class BtToolbarIconSize
{
    btToolbarIconSizeSmall = 0,
    btToolbarIconSizeLarge = 1
};

enum class BtToolbarPosition
{
    btToolbarPositionTop = 0,
    btToolbarPositionBottom = 1
};

enum class BtUnits
{
    btUnitsCurrent = 0,
    btUnitsInches = 1,
    btUnitsCentimeters = 2,
    btUnitsMillimeters = 3
};

enum class BtUsedNamesSampleDataType
{
    btUsedDatabaseFieldsSampleData = 0
};

enum class BtVersion
{
    btLatest = 0,
    bt610 = 610,
    bt620 = 620,
    bt700 = 700,
    bt710 = 710,
    bt750 = 750,
    bt770 = 770,
    bt772 = 772,
    bt773 = 773,
    bt774 = 774,
    bt775 = 775,
    bt800 = 800,
    bt801 = 801,
    bt901 = 901,
    bt910 = 910,
    bt920 = 920,
    bt921 = 921,
    bt930 = 930,
    bt940 = 940,
    bt1000 = 1000
};

enum class BtVisibleWindows
{
    btNone = 0,
    btAll = 1,
    btInteractiveDialogs = 2,
    btStartingLabelDialogOnly = 3
};

enum class BtWindowState
{
    btNormal = 0,
    btMaximize = 1,
    btMinimize = 2
};

enum class BtXMLSourceType
{
    btXMLScriptString = 0,
    btXMLScriptFile = 1
};

class Application;
class Formats;
class Format;
class NamedSubStrings;
class SubString;
class DesignObjects;
class DesignObject;

////////////////////////////////////////////////////////////////////////////////
// Application wrapper class
class BARTENDER_EXPORT Application : public QAxObject
{
public:
    explicit Application(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }
    explicit Application(const QString &c, QObject *parent = Q_NULLPTR)
        : QAxObject(c, parent)
    {
    }

    virtual ~Application() {}
    // Attributes
public:
    BarTender::Format *ActiveFormat();
    void SetActiveFormat(BarTender::Format *ActiveFormat);

    int BuildNumber();
    void SetBuildNumber(int BuildNumber);

    BarTender::Formats *Formats();
    void SetFormats(BarTender::Formats *Formats);

    bool IsPrinting();
    void SetIsPrinting(bool IsPrinting);

    bool IsProcessingCommandLines();
    void SetIsProcessingCommandLines(bool IsProcessingCommandLines);

    QString Version();
    void SetVersion(const QString &Version);

    bool Visible();
    void SetVisible(bool Visible);

    QAxObject *Window();
    void SetWindow(QAxObject *Window);

    QString SAPIDocDefinitionFile();
    void SetSAPIDocDefinitionFile(const QString &SAPIDocDefinitionFile);

    QAxObject *LicenseServer();
    void SetLicenseServer(QAxObject *LicenseServer);

    QString Edition();
    void SetEdition(const QString &Edition);

    BtVisibleWindows VisibleWindows();
    void SetVisibleWindows(BtVisibleWindows VisibleWindows);

    bool TopMost();
    void SetTopMost(bool TopMost);

    int ProcessId();
    void SetProcessId(int ProcessId);

    QString FullVersion();
    void SetFullVersion(const QString &FullVersion);

    // Operations
public:
    void Quit(BtSaveOptions SaveChanges = BtSaveOptions::btPromptSave);
    void Save(bool DoPrompt = false);
    void CommandLine(const QString &CommandLine);
    //    QString XMLScript(const QString& XMLScript, long SourceType, QAxObject** Messages);
};

////////////////////////////////////////////////////////////////////////////////
// Formats wrapper class
class BARTENDER_EXPORT Formats : public QAxObject
{
public:
    explicit Formats(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }

    // Attributes
public:
    int Count();
    void SetCount(int Count);

    // Operations
public:
    BarTender::Format *Add();
    BarTender::Format *Open(const QString &FileName, bool CloseOutFirstFormat, const QString &UsePrinter);
    QAxObject *Item(const QVariant &Index);
    QAxObject *Format(const QVariant &Index);
};

////////////////////////////////////////////////////////////////////////////////
// Format wrapper class
class BARTENDER_EXPORT Format : public QAxObject
{
public:
    explicit Format(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }

    // Attributes
public:
    bool AutoPrintAgain();
    void SetAutoPrintAgain(bool AutoPrintAgain);

    QString BaseName();
    void SetBaseName(const QString &BaseName);

    QString Directory();
    void SetDirectory(const QString &Directory);

    QString FileName();
    void SetFileName(const QString &FileName);

    int IdenticalCopiesOfLabel();
    void SetIdenticalCopiesOfLabel(int IdenticalCopiesOfLabel);

    QAxObject *InputDataFileSetup();
    void SetInputDataFileSetup(QAxObject *InputDataFileSetup);

    int NumberSerializedLabels();
    void SetNumberSerializedLabels(int NumberSerializedLabels);

    QString Printer();
    void SetPrinter(const QString &Printer);

    bool PrintToFile();
    void SetPrintToFile(bool PrintToFile);

    QString Title();
    void SetTitle(const QString &Title);

    bool UseInputDataFile();
    void SetUseInputDataFile(bool UseInputDataFile);

    BtVersion SaveAsVersion();
    void SetSaveAsVersion(BtVersion SaveAsVersion);

    BarTender::NamedSubStrings *NamedSubStrings();
    void SetNamedSubStrings(BarTender::NamedSubStrings *NamedSubStrings);

    //    QAxObject* Databases();
    //    void SetDatabases(QAxObject*);
    //    bool EnablePrompting();
    //    void SetEnablePrompting(bool);
    //    QString RecordRange();
    //    void SetRecordRange(const QString&);
    //    bool SelectRecordsAtPrint();
    //    void SetSelectRecordsAtPrint(bool);
    //    bool UseDatabase();
    //    void SetUseDatabase(bool);
    //    QString PrinterFile();
    //    void SetPrinterFile(const QString&);
    //    QAxObject* PageSetup();
    //    void SetPageSetup(QAxObject*);
    //    QAxObject* PrintSetup();
    //    void SetPrintSetup(QAxObject*);
    //    QAxObject* PrinterCodeTemplate();
    //    void SetPrinterCodeTemplate(QAxObject*);
    //    QString UsedNames();
    //    void SetUsedNames(const QString&);
    //    QString UsedNamesSampleData();
    //    void SetUsedNamesSampleData(const QString&);
    //    QString JobName();
    //    void SetJobName(const QString&);
    //    QString PrintToFileLicense();
    //    void SetPrintToFileLicense(const QString&);
    //    QString PaperTray();
    //    void SetPaperTray(const QString&);
    //    QString Comment();
    //    void SetComment(const QString&);
    //    long Encryption();
    //    void SetEncryption(long);
    //    bool SupportsSetSerializedCopies();
    //    void SetSupportsSetSerializedCopies(bool);
    //    bool SupportsSetIdenticalCopies();
    //    void SetSupportsSetIdenticalCopies(bool);
    //    long MeasurementUnits();
    //    void SetMeasurementUnits(long);
    BarTender::DesignObjects *Objects();
    void SetObjects(BarTender::DesignObjects *Objects);
    //    long LatestSaveNumber();
    //    void SetLatestSaveNumber(long);
    //    bool IsModified();
    //    void SetIsModified(bool);
    //    QAxObject* ViewRecordNavigator();
    //    void SetViewRecordNavigator(QAxObject*);
    //    QString PromptValues();
    //    void SetPromptValues(const QString&);
    //    long StartingPosition();
    //    void SetStartingPosition(long);

    // Operations
public:
    void Activate();
    void Close(BtSaveOptions SaveChanges);
    void PrintOut(bool ShowStatusWindow, bool ShowPrintDialog);
    void Save();
    void SaveAs(const QString &FileName, bool OverwriteIfExists);
    void ExportToClipboard(BtColors Colors, BtResolution Resolution);
    void ExportToFile(const QString &FileName, const QString &FileType, BtColors Colors, BtResolution Resolution, BtSaveOptions SaveOption);
    //    void SetAvailableTemplateFieldNames(const QString& FieldnameSampleDataList, const QString& delimiter);
    //    QString ExportPromptToXML(const QString& XSLFileName);
    //    void SetPrompt(const QString& PromptName, const QString& PromptData);
    //    long Print(const QString& PrintJobName, bool WaitForSpoolJobToComplete, long TimeoutMs, QAxObject** Messages);
    //    long ExportPrintPreviewToImage(const QString& Directory, const QString& FileNameTemplate, const QString& FileType, long Colors, long dpi, long backgroundColor, long SaveOpts, bool IncludeMargins, bool IncludeBorders, QAxObject** Messages);
    //    long ExportPrintPreviewRangeToImage(const QString& PageRange, const QString& Directory, const QString& FileNameTemplate, const QString& FileType, long Colors, long dpi, long backgroundColor, long SaveOpts, bool IncludeMargins, bool IncludeBorders, QAxObject** Messages);
    //    QString Prompt(const QString& PromptData);
    //    void SetPrompts(const QString& PromptNameValuePairs, const QString& delimiter);
    QString NamedSubStringValue(const QString &SubStringName);
    void SetNamedSubStringValue(const QString &SubstringName, const QString &Value);
};

////////////////////////////////////////////////////////////////////////////////
// NamedSubStrings wrapper class
class BARTENDER_EXPORT NamedSubStrings : public QAxObject
{
public:
    explicit NamedSubStrings(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }

    // Attributes
public:
    int Count();
    void SetCount(int Count);

    // Operations
public:
    QAxObject *Item(const QVariant &Index);
    QString All(const QString &nameValueDelimiter, const QString &recordDelimiter);
    void SetAll(const QString &subStringsAndValues, const QString &delimiter);
    QAxObject *SubString(const QVariant &Index);
};

////////////////////////////////////////////////////////////////////////////////
// SubString wrapper class
class BARTENDER_EXPORT SubString : public QAxObject
{
public:
    explicit SubString(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }

    // Attributes
public:
    QString Name();
    void SetName(const QString &Name);
    QString Value();
    void SetValue(const QString &Value);
    BtSubStringType Type();
    void SetType(BtSubStringType Type);
    QString SerializeBy();
    void SetSerializeBy(const QString &SerializeBy);
    int SerializeEvery();
    void SetSerializeEvery(int SerializeEvery);
    bool Rollover();
    void SetRollover(bool Rollover);
    QString RolloverLimit();
    void SetRolloverLimit(const QString &RolloverLimit);
    QString RolloverResetValue();
    void SetRolloverResetValue(const QString &RolloverResetValue);

    // Operations
public:
};

////////////////////////////////////////////////////////////////////////////////
// DesignObjects wrapper class
class BARTENDER_EXPORT DesignObjects : public QAxObject
{
public:
    explicit DesignObjects(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }

    // Attributes
public:
    int Count();
    void SetCount(int Count);

    // Operations
public:
    QAxObject *Item(const QVariant &Index);
    QAxObject *Find(const QString &Name);
    //    QString ExportDataSourceValuesToXML();
    //    long ImportDataSourceValuesFromXML(const QString& dataSourceXML);
};

////////////////////////////////////////////////////////////////////////////////
// DesignObject wrapper class
class BARTENDER_EXPORT DesignObject : public QAxObject
{
public:
    explicit DesignObject(QObject *parent = Q_NULLPTR)
        : QAxObject(parent)
    {
    }

    // Attributes
public:
    QString Name();
    void SetName(const QString &Name);
    QString Value();
    void SetValue(const QString &Value);
    //    long Type();
    //    void SetType(long);
    //    double X();
    //    void SetX(double);
    //    double Y();
    //    void SetY(double);
    //    double RotationAngle();
    //    void SetRotationAngle(double);
    //    bool DoNotPrint();
    //    void SetDoNotPrint(bool);
    //    QString Comment();
    //    void SetComment(const QString&);
    //    double Width();
    //    void SetWidth(double);
    //    double Height();
    //    void SetHeight(double);
    //    double Length();
    //    void SetLength(double);
    //    double LineThickness();
    //    void SetLineThickness(double);
    //    double LineStartX();
    //    void SetLineStartX(double);
    //    double LineStartY();
    //    void SetLineStartY(double);
    //    double LineEndX();
    //    void SetLineEndX(double);
    //    double LineEndY();
    //    void SetLineEndY(double);
    //    double CornerRadius();
    //    void SetCornerRadius(double);
    //    unsigned long LineColor();
    //    void SetLineColor(unsigned long);
    //    unsigned long FillColor();
    //    void SetFillColor(unsigned long);
    //    unsigned long BarCodeColor();
    //    void SetBarCodeColor(unsigned long);
    //    unsigned long TextColor();
    //    void SetTextColor(unsigned long);
    //    unsigned long TextBackgroundColor();
    //    void SetTextBackgroundColor(unsigned long);
    //    QString FontName();
    //    void SetFontName(const QString&);
    //    double FontSize();
    //    void SetFontSize(double);
    //    double FontScale();
    //    void SetFontScale(double);
    //    long FontWeight();
    //    void SetFontWeight(long);
    //    bool FontBold();
    //    void SetFontBold(bool);
    //    bool FontItalic();
    //    void SetFontItalic(bool);
    //    bool FontUnderline();
    //    void SetFontUnderline(bool);
    //    bool FontStrikeout();
    //    void SetFontStrikeout(bool);
    //    long FontScript();
    //    void SetFontScript(long);
    //    QString HumanReadableValue();
    //    void SetHumanReadableValue(const QString&);
    //    QString EncodedValue();
    //    void SetEncodedValue(const QString&);
    //    bool MirrorHorizontal();
    //    void SetMirrorHorizontal(bool);
    //    bool MirrorVertical();
    //    void SetMirrorVertical(bool);
    //    double PercentWidth();
    //    void SetPercentWidth(double);
    //    double PercentHeight();
    //    void SetPercentHeight(double);
    //    bool PreserveAspectRatio();
    //    void SetPreserveAspectRatio(bool);
    //    QString PicturePath();
    //    void SetPicturePath(const QString&);

    // Operations
public:
    void ExportToClipboard(long Colors, long Resolution);
    void ExportToFile(const QString &FileName, const QString &FileType, long Colors, long Resolution, long SaveOption);
};

}    // namespace BarTender

#endif
