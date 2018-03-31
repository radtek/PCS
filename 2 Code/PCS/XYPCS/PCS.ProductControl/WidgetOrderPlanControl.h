#ifndef PCS_PRODUCTCONTROL_WIDGETWORKORDER_H_
#define PCS_PRODUCTCONTROL_WIDGETWORKORDER_H_

#include "WorkManager.h"

class WorkOrder;
class WorkCraft;
class WorkStation;
class StationOperator;
class StationMaterial;

namespace Ui
{
class WidgetOrderPlanControl;
}

class WidgetOrderPlanControl : public QWidget
{
    Q_OBJECT
public:
    enum class StationHeader
    {
        StationID,
        StationName,
        FinishQuantity,
        FailedQuantity,
        OperatorLogin,
        MaterialInput,
        MachineCheck,
        ProductCheck,
        FixtureCheck,
    };

    enum class MaterialHeader
    {
        StationID,
        MaterialID,
        MaterialName,
        MaterialBatch,
        BatchQuantity,
        UsageQuantity,
        AlarmQuantity,
        Operation,
    };

    enum class MeasureHeader
    {
        StationID,
        MeasureID,
        MeasureName,
        MeasureUnit,
        StandardValue,
        UpperLimit,
        LowerLimit,
    };

    enum class FixtureHeader
    {
        StationID,
        FixtureID,
        FixtureName,
        FixtureBarcode,
        FixtureClassify,
        TotalTimes,
        UsedTimes,
        RemindTimes,
    };

    enum class OperatorHeader
    {
        StationID,
        OperatorID,
        OperatorCard,
        OperatorName,
        OperatorTeam,
        OperatorDuty,
        Operation,
    };

    enum class QualityAssure
    {
        FirstProduct,
        FirstPackage,
        FinalProduct,
        FinalPackage,
    };

public:
    explicit WidgetOrderPlanControl(QWidget *parent = 0);
    virtual ~WidgetOrderPlanControl();

public slots:
    void slotOrderActivated();
    void slotOrderSuspended();

    void slotOrderPlanUpdate(const OrderData &data);
    void slotStationUpdate(const StationData &data);

    void slotMaterialInput(const QString &stationID, const MaterialData &data);
    void slotMaterialReturn(const QString &stationID, const MaterialData &data);
    void slotOperatorLogin(const QString &stationID, const OperatorData &data);
    void slotOperatorLogout(const QString &stationID, const OperatorData &data);
    void slotFixtureLoaded(const QString &stationID, const FixtureData &data);

    void slotMaterialUpdate(const QString &stationID, const MaterialData &data);
    void slotFixtureUpdate(const QString &stationID, const FixtureData &data);

    void slotOperatorLogin();
    void slotMaterialInput();
    void slotLogisticsFeed();

    void slotCustomLabelDblClicked(int row, int index);
    void slotOperatorButtonClicked(int row, int index);
    void slotMaterialButtonClicked(int row, int index);

    void slotPauseWork();
    void slotStartWork();

private:
    void initialWidget();

    void materialInput(const QString &stationID, const QString &materialID, const QString &batch, int quantity);
    void materialInput(const QString &stationID, const QString &barcode);
    void operatorLogin(const QString &stationID, const QString &barcode);
    void materialReturn(const QString &stationID, const QString &materialID);
    void operatorLogout(const QString &stationID, const QString &operatorID);

    void changeButtonState();

private:
    Ui::WidgetOrderPlanControl *ui;

    QMap<StationHeader, QString> stationHeaderMap;
    QMap<MaterialHeader, QString> materialHeaderMap;
    QMap<MeasureHeader, QString> measureHeaderMap;
    QMap<FixtureHeader, QString> fixtureHeaderMap;
    QMap<OperatorHeader, QString> operatorHeaderMap;

    QStringList materialButtonList;
    QStringList operatorButtonList;
};

#endif    // WIDGETWORKORDER_H
