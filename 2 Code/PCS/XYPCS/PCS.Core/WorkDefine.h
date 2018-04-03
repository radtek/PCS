#ifndef WORKDEFINE_H
#define WORKDEFINE_H

#include "GlobalData.h"
#include "PCSApplication.h"
#include "CommManager.h"
#include "OPCManager.h"

extern const QString PRINTER_REGEDIT;
extern const QString PROCESS_PRODUCE_PATH;
extern const QString PROCESS_ASSEMBLY_PATH;
extern const QString PROCESS_PACKAGE_PATH;
extern const QString LABEL_PRINT_PATH;
extern const QString DATE_CODE_PATH;

extern QSqlDatabase LOCAL_DB;
extern QSqlDatabase REMOTE_DB;

enum class RecordState
{
    Create,
    Update,
    Delete,
};

extern const int RECORD_CREATE;
extern const int RECORD_UPDATE;
extern const int RECORD_DELETE;

//工单计划状态
enum class OrderState
{
    Create = 0,            //新建
    Issued = 1,            //下发
    Closed = 2,            //关闭
    ForceClose = 3,        //强制关闭
    Delete = 4,            //作废
    Return = 5,            //退回
    PlanReturn = 6,        //计划退回
    Activated = 7,         //已激活
    Suspended = 8,         //已暂停
};

extern const QMap<OrderState, QString> orderStateMap;

enum class LogState
{
    Login = 0,              //登陆
    Logout = 1,             //登出
};

extern const QMap<LogState, QString> logStateMap;

enum class BatchState
{
    Feed = 0,               //投料
    Input = 1,              //上料
    Return = 2,             //退料
};

extern const QMap<BatchState, QString> batchStateMap;

//员工职务
enum class OperatorDuty
{
    LineWorker = 0,         //操作员
    LineLeader = 1,         //线长
    GroupLeader = 2,        //组长
};

extern const QMap<OperatorDuty, QString> OperatorDutyMap;

//条码更新规则
enum class SerialRule
{
    UpdateByBatch,          //按批更新
    UpdateByDay,            //按天更新
    UpdateByMonth,          //按月更新
    UpdateByYear,           //按年更新
    KeepContinuous,         //保持连续
};

extern const QMap<SerialRule, QString> SerialRuleMap;

//交互事件状态
enum class EventState
{
    E_General_Successfully = 0,
    E_General_Failed = 1,
    E_General_ResetMachine = 2,             //请复位设备
    E_General_ScanBarcode = 3,              //请扫描条码

    E_General_OrderClosed = 100,            //工单已完成
    E_General_WorkPaused = 101,             //生产已暂停

    E_OperatorNotLogin = 110,               //操作员未登录
    E_MaterialNotInput = 111,               //物料未投料
    E_MaterialisUesup = 112,                //物料已用完
    E_FixtureisUesup = 113,                 //模具已到限
    E_FixtureNotVerify = 114,               //模具校验未执行
    E_MachineNotCheck = 115,                //设备点检未执行
    E_ProductNotCheck = 116,                //产品点检未执行
    E_FirstProductNotQA = 117,              //首件检验未执行
    E_FinalProductNotQA = 118,              //末件检验未执行

    E_WorkFinished_Normal = 200,            //工作循环结束
    E_WorkFinished_Failed = 201,            //工作循环结束（不合格品）
    E_WorkFinished_Repair = 202,            //工作循环结束（返修件）
    E_WorkFinished_Retest = 203,            //工作循环结束（非一次通过）

    E_ScanMbar_Successfully = 310,          //物料条码扫描成功
    E_ScanMbar_VerifyFormatFailed = 311,    //物料条码扫描失败，未通过格式校验
    E_ScanMbar_VerifyUniqueFailed = 312,    //物料条码扫描失败，未通过唯一性校验
    E_ScanMbar_AlreadyInUse = 313,          //物料条码扫描失败，条码正在使用中

    E_ScanTbar_Successfully = 320,          //过渡条码扫描成功
    E_ScanTbar_PrevStationNonData = 321,    //过渡条码扫描失败，上工位无相关数据
    E_ScanTbar_ThisStationHasData = 322,    //过渡条码扫描失败，本工位已有相关数据
    E_ScanTbar_AlreadyInUse = 323,          //过渡条码扫描失败，条码正在使用中

    E_ScanAbar_Successfully = 330,          //总成条码扫描成功
    E_ScanAbar_ContentMismatch = 331,       //总成条码扫描失败，扫描内容不匹配
    E_ScanAbar_HistoryHasData = 332,        //总成条码扫描失败，数据库存在历史数据

    E_ScanPbar_Successfully = 340,          //包装条码扫描成功
    E_ScanPbar_ContentMismatch = 341,       //包装条码扫描失败，扫描内容不匹配
    E_ScanPbar_HistoryHasData = 342,        //包装条码扫描失败，数据库存在历史数据

    E_ScanMark_Successfully = 350,          //包装标识扫描成功
    E_ScanMark_ContentMismatch = 351,       //包装条码扫描失败，标识内容不匹配
};

//工位类型
enum class StationType
{
    Station,
    Package,
};

extern const QMap<StationType, QString> stationTypeMap;
extern const QMap<StationType, QString> stationTypeStringMap;

//标签类型
enum class LabelType
{
    Assembly,
    Package,
};

extern const QMap<LabelType, QString> labelTypeMap;
extern const QMap<LabelType, QString> labelTypeStringMap;

//////////////////////////////////////////////////////////////////////////////
/// 生产数据
///////////////////////////////////////////////////////////////////////////////
struct WorkshopData
{
    QString id;
    QString name;
};

struct WorklineData
{
    QString id;
    QString name;
};

//工单计划数据
struct OrderData
{
    QString orderID;                //工单编号
    QString craftID;                //工艺路由编号
    QString productBatch;           //生产批次
    QDate orderPlanDate;            //计划生产时间

    int orderPlanQuantity;          //工单数量
    int inspectQuantity;            //抽检数量
    double inspectInterval;         //抽检间隔

    int finishQuantity;             //已完成数量
    int failedQuantity;             //不合格数量
    int packageQuantity;            //包装箱数量
    int repairQuantity;             //返修件数量
    int sampleQuantity;             //已抽检数量
    int firstPassQuantity;          //一次通过数量

    bool isFirstProductQA;          //首件检验
    bool isFirstPackageQA;          //首箱检验
    bool isFinalProductQA;          //末件检验
    bool isFinalPackageQA;          //末箱检验

    QString firstProductQADocID;    //首件质检单据
    QString firstPackageQADocID;    //首箱质检单据
    QString finalProductQADocID;    //末件质检单据
    QString finalPackageQADocID;    //末箱质检单据
};
//工艺路由属性
struct CraftData
{
    QString craftID;            //工艺路由ID
    QString craftName;          //工艺路由名称
    QString productID;          //产品ID
    QString productName;        //产品名称

    SerialRule assySerialRule;         //总成流水号更新规则
    SerialRule packSerialRule;         //包装流水号更新规则
    int assySerialInit;         //总成流水号起始值
    int packSerialInit;         //包装流水号起始值
};
//工位数据
struct StationData
{
    QString stationID;          //工位ID
    QString stationCode;        //工位代号
    QString stationName;        //工位名称
    QString stationType;        //工位类型

    int finishQuantity;         //完成数量
    int failedQuantity;         //不合格数量

    bool isOperatorLogin;         //操作员登陆
    bool isMaterialInput;         //物料投料
    bool isFixtureCheck;          //模具确认
    bool isMachineCheck;          //设备点检
    bool isProductCheck;          //产品点检

    QString machineCheckDocID;       //设备点检单据编号
    QString productCheckDocID;       //产品点检单据编号
    QString fixtureCheckDocID;       //工装模具单据编号
};
//物料数据
struct MaterialData
{
    QString materialID;         //物料编号
    QString materialName;       //物料名称
    QString barcodeRule;        //条码规则
    int usageQuantity;          //单件使用数量
    int alarmQuantity;          //补料提醒数量

    QString materialBarcode;    //当前物料条码
    int repairCount;            //物料返修次数

    QString materialBatch;       //当前物料批次
    QString packageBatch;       //物料包装批次
    int batchQuantity;          //批次剩余数量
};
//测试数据
struct MeasureData
{
    QString measureID;          //测试编号
    QString measureName;        //测试名称
    QString measureUnit;        //测试单位

    double standardValue;       //标准值
    double offsetValue;         //偏差值
    double upperLimit;          //上限
    double lowerLimit;          //下限

    double measureValue;        //测试值
    int measureState;          //测试结果
    int retestCount;            //复测次数
};
//工装模具数据
struct FixtureData
{
    QString fixtureID;          //工装模具ID
    QString fixtureName;        //工装模具名称
    QString fixtureBarcode;     //工装模具条码
    QString fixtureClassify;    //工装模具类型

    int totalTimes;             //额定使用次数
    int usedTimes;              //已使用次数
    int remindTimes;            //提醒次数
};
//员工数据
struct OperatorData
{
    QString operatorID;         //操作员ID
    QString operatorCard;       //操作员卡号
    QString operatorName;       //操作员姓名
    QString operatorTeam;       //操作员班组
    QString operatorDuty;       //操作员类型

    OperatorDuty dutyLevel;     //职务等级
    QString logMarker;          //登陆标识
};
//总成打印数据
struct AssemblyData
{
    QString assemblyID;
    QString assemblyBarcode;
    int assemblySerial;
    int assemblyState;
    int sampleState;
    int repairState;
    int retestState;
    QDateTime createTime;
    QDateTime finishTime;

    void initial()
    {
        assemblyID.clear();
        assemblyBarcode.clear();
    }

    void serialInc()
    {
        ++assemblySerial;
    }
};
//总成包装数据
struct PackageData
{
    QString packageID;
    QString packageBarcode;
    QStringList assemblyIDList;
    int packageSerial;
    double packageWeight;
    int packageCount;
    int packageState;
    QDateTime createTime;
    QDateTime finishTime;

    void initial()
    {
        packageID.clear();
        packageBarcode.clear();
        assemblyIDList.clear();
    }

    void serialInc()
    {
        ++packageSerial;
    }
};

struct ProcessData
{
    QString assemblyID;             //总成ID
    QStringList subAssemblyIDs;     //子总成ID

    int assemblyState;          //装配状态
    int repairState;            //返修状态
    int retestState;            //重新检测状态

    QDateTime createTime;       //开始时间
    QDateTime finishTime;       //结束时间

    void initial()
    {
        assemblyID.clear();
        subAssemblyIDs.clear();
        assemblyState = 0;
        repairState = 0;
        retestState = 0;
    }
};
//////////////////////////////////////////////////////////////////////////////
/// 装配流程
///////////////////////////////////////////////////////////////////////////////


//步骤类型
enum class StepType
{
    LoopStart,
    LoopEnd,
    AutoMotion,
    ScanBarcode,
    MeasureData,
};

extern const QMap<StepType, QString> stepTypeMap;
extern const QMap<StepType, QString> stepTypeStringMap;

//条码扫描类型
enum class BarcodeType
{
    Material,
    Transfer,
};

extern const QMap<BarcodeType, QString> barcodeTypeMap;
extern const QMap<BarcodeType, QString> barcodeTypeStringMap;

//数据采集类型
enum class MeasureType
{
    ByDAC,
    ByOPC,
};

extern const QMap<MeasureType, QString> measureTypeMap;
extern const QMap<MeasureType, QString> measureTypeStringMap;

//////////////////////////////////////////////////////////////////////////////
/// 步骤参数
///////////////////////////////////////////////////////////////////////////////


//循环开始
struct LoopStartItem
{
};

//循环结束
struct LoopEndItem
{
};

//自动动作
struct AutoMotionItem
{
};

//扫描条码
struct ScanBarcodeItem
{
    bool isDisabled;

    BarcodeType barcodeType;        //条码类型
    QString barcodeID;             //物料或工位编号
    MeasureType measureType;        //采集类型
    DataDefine readBarcode;         //采集设备或采集地址
    DataDefine writeDisplay;        //写入条码内容
    DataDefine writeResult;         //写入扫描结果
    QString resultOK;               //扫描成功值
    QString resultNG;               //扫描失败值

    QString assemblyID;             //总成编号
    QString barcodeText;            //当前条码
    int repairCount;                //条码返修次数
    bool isFinished;                //是否扫描完成

    void initial()
    {
        assemblyID.clear();
        barcodeText.clear();
        repairCount = 0;
        isFinished = false;
    }
};

//采集数据参数
struct MeasureDataItem
{
    bool isDisabled;

    QString measureID;              //采集编号
    QString offsetValue;            //偏差值
    MeasureType measureType;        //采集类型
    DataDefine readMeasure;         //采集设备或采集地址
    DataDefine writeDisplay;        //写入采集数值
    DataDefine writeResult;         //写入判断结果
    QString resultOK;               //采集成功值
    QString resultNG;               //采集失败值

    double measureValue;            //采集数值
    int measureState;               //判断结果
    int retestCount;                //重复检测次数
    bool isFinished;                //是否检测完成

    void initial()
    {
        measureValue = 0.0;
        measureState = 0;
        retestCount = 0;
        isFinished = false;
    }
};

//生产过程参数
struct ProducePara
{
    bool enableFirstProductQA;              //启用首件检验
    bool enableFinalProductQA;              //启用末件检验

    bool enableMistakeProof01;
    bool enableMistakeProof02;
    bool enableMistakeProof03;
    bool enableMistakeProof04;
    bool enableMistakeProof05;
    bool enableMistakeProof06;
    bool enableMistakeProof07;
    bool enableMistakeProof08;
    bool enableMistakeProof09;
    bool enableMistakeProof10;
    bool enableMistakeProof11;
};

//生产准备参数
struct PreparePara
{
    bool enableOperatorLogin;               //启用员工登陆
    bool enableMaterialInput;               //启用物料投料
    bool enableFixtureCheck;                //启用模具检查
    bool enableMachineCheck;                //启用设备点检
    bool enableProductCheck;                //启用产品点检
};

//流程参数
struct ProcessPara
{
    int formulaNumber;
    int processNumber;

    DataDefine writeProductID;
    DataDefine writeProductBatch;
    DataDefine writeFinishQuantity;
    DataDefine writeFailedQuantity;
    DataDefine writeFormulaNumber;
    DataDefine writeProcessPrompt;

    DataDefine resetSignal;
    DataDefine alarmSignal;
};

//步骤基础参数
struct StepItem
{
    QString interaction;
    QString description;

    bool    isDisabled;
    int     maxMeasure;

    DataDefine startRequest;
    DataDefine startResponse;
    DataDefine skipResponse;

    DataDefine measureRequest;
    DataDefine successResponse;
    DataDefine failedResponse;
};

//步骤参数列表
struct StepPara
{
    StepType                stepType;
    StepItem                stepItem;

    LoopStartItem           startItem;
    LoopEndItem             endItem;
    AutoMotionItem          motionItem;
    QList<ScanBarcodeItem>  barcodeItemList;
    QList<MeasureDataItem>  measureItemList;
};

//总成装配参数
struct AssemblyPara
{
    bool enable;

    bool rescanBarcode;
    bool verifyBarcode;

    int printCount;
    int rescanCount;

    QString prevStationID;
};

//总成包装参数
struct PackagePara
{
    bool enable;

    bool rescanBarcode;
    bool verifyBarcode;

    int printCount;
    int rescanCount;

    QString packageMarker;
    QString scaleDeviceID;
    int packageNumber;
    double singleWeight;
    double offsetWeight;
};

bool AssyProduce_ReadXmlFile(const QString &filename, ProducePara &producePara);
bool AssyProduce_SaveXmlFile(const QString &filename, ProducePara &producePara);

bool AssyStation_ReadXmlFile(const QString &filename, PreparePara &preparePara, ProcessPara &processPara, QList<StepPara> &stepParaList);
bool AssyStation_SaveXmlFile(const QString &filename, PreparePara &preparePara, ProcessPara &processPara, QList<StepPara> &stepParaList);

bool AssyPackage_ReadXmlFile(const QString &filename, PreparePara &preparePara, AssemblyPara &assemblyPara, PackagePara &packagePara);
bool AssyPackage_SaveXmlFile(const QString &filename, PreparePara &preparePara, AssemblyPara &assemblyPara, PackagePara &packagePara);
//////////////////////////////////////////////////////////////////////////////
/// 标签打印
///////////////////////////////////////////////////////////////////////////////

//模板元素类型
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

//模板元素对应文本
extern const QMap<TemplateItemType, QString> templateItemTypeMap;

//条码字符类型
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

//条码字符对应文本
extern const QMap<BarcodeItemType, QString> barcodeItemTypeMap;

//校验码类型
enum class CheckCodeType
{
    VW,
    SL,
    QR,
};

//校验码对应文本
extern const QMap<CheckCodeType, QString> checkCodeTypeMap;

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

bool LabelPrint_ReadXmlFile(const QString &filename, LabelPrintItems &items);
bool LabelPrint_SaveXmlFile(const QString &filename, LabelPrintItems &items);
bool DateCode_ReadXmlFile(const QString &filename, DateCodeItems &items);
bool DateCode_SaveXmlFile(const QString &filename, DateCodeItems &items);
char CheckCode_GenerateVW(const QString &string);
char CheckCode_GenerateSL(const QString &string);
char CheckCode_GenerateQR(const QString &string);

#endif // WORKDEFINE_H
