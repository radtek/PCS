#ifndef PACKAGESTATION_H
#define PACKAGESTATION_H

#include "WorkStation.h"

class LabelPrint;

class PackageStation : public WorkStation
{
public:
    explicit PackageStation(WorkManager *manager);
    virtual ~PackageStation();

    virtual bool initialStation(const QString &station);
public:
    //过渡条码在工位是否有相关数据
    bool isTransferBarcodeValidInPrevStation(const QString &barcode);
    //过渡条码在工位是否可用
    bool isTransferBarcodeExistInThisStation(const QString &barcode);
    //总成条码是否存在重复记录
    bool isAssemblyBarcodeExistInHistoryData(const QString &barcode);
    //包装箱码是否存在重复记录
    bool isPackageBarcodeExistInHistoryData(const QString &barcode);
    //通过过渡条码获取总成信息
    void getAssemblyDataByTransferBarcode(const QString &barcode);

    ///总成打印方法
    //判断上工位过渡条码是否完成
    EventState verifyTransferBarcode(const QString &barcode);
    //判断总成条码是否存在历史记录
    EventState verifyAssemblyBarcode(const QString &barcode);

    //打印总成条码
    QString printAssemblyBarcode();
    //打印总成条码
    void reprintAssemblyBarcode();
    void reprintAssemblyBarcode(const QString &barcode);
    //保存总成数据
    void saveAssemblyData();

    ////包装及打印方法
    //判断包装箱防错标识
    EventState verifyPackageMarker(const QString &barcode);
    //判断包装条码是否存在历史记录
    EventState verifyPackageBarcode(const QString &barcode);

    //打印包装箱码
    QString printPackageBarcode();
    //打印包装箱码
    void reprintPackageBarcode();
    void reprintPackageBarcode(const QString &barcode);
    //保存总成与包装箱信息
    void savePackageData();

    //电子秤采集
    void scaleRead();
    //电子秤清零
    void scaleZero();
    //电子秤复位
    void scaleReset();
public:
    virtual void opcValueChanged(const DataDefine &data) { Q_UNUSED(data); }
    virtual void opcReadComplete(const DataDefine &data) { Q_UNUSED(data); }
    virtual void dacDataReceived(const DataDefine &data);

    virtual void readOpcData(const DataDefine &data) { Q_UNUSED(data); }
    virtual void writeOpcData(const DataDefine &data) { Q_UNUSED(data); }
    virtual void writeDacData(const DataDefine &data);

    virtual EventState verifyStationStatus();
    virtual void updateStationStatus();
public:
    inline bool isAssyEnable() const { return assemblyPara.enable; }
    inline bool isAssyEnableRescan() const { return assemblyPara.rescanBarcode; }
    inline bool isAssyEnableVerify() const { return assemblyPara.verifyBarcode; }
    inline bool isPackEnable() const { return packagePara.enable; }
    inline bool isPackEnableRescan() const { return packagePara.rescanBarcode; }
    inline bool isPackEnableVerify() const { return packagePara.verifyBarcode; }

    inline int getAssemblyCount() const { return packageData.assemblyIDList.size(); }
    inline int getPackageCount() const { return packageData.packageCount; }
    inline int getPackageNumber() const { return packagePara.packageNumber; }
    inline bool isAssemblySample() const { return assemblyData.sampleState == 1; }

    inline QString getAssemblyBarcode() const { return assemblyBarcode; }
    inline QString getPackageBarcode() const { return packageBarcode; }
private:
    LabelPrint *assemblyPrint;
    LabelPrint *packagePrint;

    PreparePara preparePara;    //生产准备参数
    AssemblyPara assemblyPara;  //总成打印参数
    PackagePara packagePara;    //包装打印参数

    AssemblyData assemblyData;  //总成打印数据
    PackageData packageData;    //包装打印数据

    QString assemblyBarcode;
    QString packageBarcode;
};

#endif // PACKAGESTATION_H
