#ifndef PCS_PRODUCTCONTROL_WIDGETASSEMBLY_H_
#define PCS_PRODUCTCONTROL_WIDGETASSEMBLY_H_

#include "WorkManager.h"

namespace Ui
{
class WidgetAssemblyControl;
}

class PackageStation;

class WidgetAssemblyControl : public QWidget
{
    Q_OBJECT
public:
    enum class AssemblyHeader
    {
        AssemblyBarcode,
        PackageBarcode,
        RepairFlag,
        SampleFlag,
        CreateTime,
        FinishTime,
    };

    enum class PackageHeader
    {
        PackageBarcode,
        PackageWeight,
        PackageCount,
        CreateTime,
        FinishTime,
    };

    enum class PackageStep
    {
        LockPackageStation,
        PutAssemblyInPackage,

        PrepareScanTransfer,
        ScanTransferBarcode,

        PrintAssemblyBarcode,

        PrepareRescanAssembly,
        RescanAssemblyBarcode,

        SaveAssemblyData,

        PrepareScanMarker,
        ScanPackageMarker,

        PrintPackageBarcode,

        PrepareRescanPackage,
        RescanPackageBarcode,

        SavePackageData,
    };

public:
    explicit WidgetAssemblyControl(QWidget *parent = 0);
    virtual ~WidgetAssemblyControl();

public slots:
    void slotScanTransferBarcode();
    void slotRescanAssmelbyBarcode();

    void slotScanPackageMarker();
    void slotRescanPackageBarcode();

    void slotReprintAssemblyBarcode();
    void slotReprintPackageBarcode();

    void slotScaleReset();
    void slotScaleRead();
    void slotScaleZero();

    void slotStartProcess();
    void slotForcePackage();

    void slotOrderActivated();
    void slotOrderSuspended();

    void slotAssemblyFinish(const AssemblyData &data);
    void slotPackageFinish(const PackageData &data);
    void slotPackageUpdate(const PackageData &data);

private:
    void initialWidget();

    void setPackageState(int state);
    void setScanResult(const QString &text, bool ok);
    void setScanRemind(const QString &text);

    void packageProcess(PackageStep step, const QString &barcode = QString());

private:
    Ui::WidgetAssemblyControl *ui;
    PackageStation *station;

    QMap<AssemblyHeader, QString> assemblyHeaderMap;
    QMap<PackageHeader, QString> packageHeaderMap;

    QPixmap checkedPixmap;
    QPixmap uncheckPixmap;
};

#endif
