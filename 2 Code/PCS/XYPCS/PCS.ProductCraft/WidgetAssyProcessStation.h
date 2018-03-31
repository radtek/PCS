#ifndef WIDGETASSYPROCESSSTATION_H
#define WIDGETASSYPROCESSSTATION_H

#include "WorkManager.h"

namespace Ui
{
class WidgetAssyProcessStation;
}

class WidgetAssyProcessStation : public QWidget
{
    Q_OBJECT

public:
    enum class StepHeader
    {
        IsDisabled,     //是否跳过
        Interaction,    //交互类型
        Description,    //步骤描述
        MaxMeasure,     //采集次数

        StartRequestItemID,    //PLC启动请求信号
        StartRequestValue,
        StartResponseItemID,    //PC启动反馈信号
        StartResponseValue,
        SkipResponseItemID,    //PC跳过反馈信号
        SkipResponseValue,
        MeasureRequestItemID,    //PLC采集请求信号
        MeasureRequestValue,
        SuccessResponseItemID,    //PC成功反馈信号
        SuccessResponseValue,
        FailedResponseItemID,    //PC失败反馈信号
        FailedResponseValue,
    };

    enum class BarcodeHeader
    {
        IsDisabled,      //是否屏蔽
        BarcodeType,     //条码类型
        BarcodeID,       //物料或工位编号
        ReadBarcode,     //采集设备或采集地址
        WriteDisplay,    //条码内容显示地址
        WriteResult,     //扫描结果显示地址
        ResultOK,        //结果OK
        ResultNG,        //结果NG
    };

    enum class MeasureHeader
    {
        IsDisabled,      //是否屏蔽
        MeasureID,       //采集编号
        OffsetValue,     //偏差值
        ReadMeasure,     //采集设备或采集地址
        WriteDisplay,    //采集数值写入地址
        WriteResult,     //判断结果写入地址
        ResultOK,        //结果OK
        ResultNG,        //结果NG
    };

public:
    explicit WidgetAssyProcessStation(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetAssyProcessStation();

public slots:
    void loadParameter(const QString &craftID, const QString &stationID);
    void saveParameter();

    void slotStepItemDoubleClicked();
    void slotStepItemSelectionChanged();
    void slotBarcodeItemDoubleClicked();
    void slotMeasureItemDoubleClicked();

    void slotSaveLoopStart();
    void slotSaveLoopEnd();
    void slotSaveAutoMotion();

    void slotAddnewStepItem();
    void slotDeleteStepItem();
    void slotMoveupStepItem();
    void slotMovedownStepItem();

    void slotAddnewBarcodeItem();
    void slotDeleteBarcodeItem();
    void slotMoveupBarcodeItem();
    void slotMovedownBarcodeItem();

    void slotAddnewMeasureItem();
    void slotDeleteMeasureItem();
    void slotMoveupMeasureItem();
    void slotMovedownMeasureItem();

private:
    void initialWidget();
    void setWidgetData();
    void getWidgetData();

private:
    Ui::WidgetAssyProcessStation *ui;

    QString craftID;
    QString stationID;

    QStringList dacItemIDList;
    QStringList opcItemIDList;
    QStringList stationIDList;
    QStringList materialIDList;
    QStringList measureIDList;

    QMap<StepHeader, QString> stepHeaderMap;
    QMap<BarcodeHeader, QString> barcodeHeaderMap;
    QMap<MeasureHeader, QString> measureHeaderMap;

    PreparePara preparePara;
    ProcessPara processPara;
    QList<StepPara> stepParaList;
};

#endif    // WIDGETASSYPROCESSSTATION_H
