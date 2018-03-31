#ifndef WORKMANAGER_H
#define WORKMANAGER_H

#include "WorkDefine.h"

class WorkOrder;
class WorkCraft;
class WorkStation;
class AssemblyStation;
class PackageStation;
class StationOperator;
class StationMaterial;

class WorkManager : public QObject
{
    Q_OBJECT
private:
    explicit WorkManager();
    virtual ~WorkManager();
public:
    //!Singleton
    static WorkManager *instance()
    {
        static WorkManager manager;
        return &manager;
    }

public:
    //数据库通用操作类
    bool loadSqlData(const QString &workshopID, const QString &worklineID);

    QStringList getCraftIDList() const { return craftIDList; }
    QStringList getStationIDList() const { return stationIDList; }
    QString getProductID(const QString &craftID) const { return craftProductMap.value(craftID); }
    QString getStationType(const QString &stationID) const { return station_TypeMap.value(stationID); }

    QStringList getMaterialIDList(const QString &craftID, const QString &stationID);
    QStringList getMeasureIDList(const QString &craftID, const QString &stationID);
    QStringList getFixtureClsList(const QString &craftID, const QString &stationID);
public:
    //加载生产数据
    bool initialManager(const QString &orderID);
    void releaseManager();

    WorkOrder *getOrder() const { return order; }
    WorkCraft *getCraft() const { return craft; }
    QList<WorkStation *> getStationList() const { return stationList; }

    WorkStation *getStation(const QString &id) const;
    PackageStation *getPackageStation() const;
    QList<AssemblyStation *> getProcessStation() const;

    QString getWorkshopID() const { return workshopData.id; }
    QString getWorklineID() const { return worklineData.id; }
    QString getWorkshopName() const { return workshopData.name; }
    QString getWorklineName() const { return worklineData.name; }

    QString getOrderID() const;
    QString getCraftID() const;
    QString getProductID() const;
    QString getOperatorID(const QString &stationID) const;
public:
    bool isWorkActivated() const { return activated; }
    bool isWorkRunning() const { return running; }

    //激活工单
    bool activateOrder(const QString &orderID);
    //停止工单
    void suspendOrder(const QString &orderID);
    //完结工单

    //开始生产
    void startWork();
    //暂停生产
    void pauseWork();

    void submitDocument(SubmitDocType type, const QString &stationID);
    void createDocument(SubmitDocType type, const QString &stationID);
    void browseDocument(SubmitDocType type, const QString &documentID);
public slots:
    void opcValueChanged(const QString &stationID, const DataDefine &data);
    void opcReadComplete(const QString &stationID, const DataDefine &data);
    void commDataReceived(const QString &stationID, const DataDefine &data);
    void commErrorOccurred(const QString &stationID, const ErrorDefine &error);

signals:
    void opcWriteValue(const QString &stationID, const DataDefine &data);
    void opcReadValue(const QString &stationID, const DataDefine &data);
    void commWriteData(const QString &stationID, const DataDefine &data);

    ///工单状态信号
    //工单已激活
    void signalOrderActivated();
    //工单已暂停
    void signalOrderSuspended();
    //生产已暂停
    void signalWorkPaused();
    //生产已启动
    void signalWorkStarted();

    ///工单更新
    void signalOrderPlanUpdate(const OrderData &data);
    ///工位更新
    void signalStationUpdate(const StationData &data);
    //员工登陆
    void signalOperatorLogin(const QString &stationID, const OperatorData &data);
    //员工登出
    void signalOperatorLogout(const QString &stationID, const OperatorData &data);
    //物料投料
    void signalMaterialInput(const QString &stationID, const MaterialData &data);
    //物料投料
    void signalMaterialReturn(const QString &stationID, const MaterialData &data);
    //物料剩余数量更新
    void signalMaterialUpdate(const QString &stationID, const MaterialData &data);
    //载入模具
    void signalFixtureLoaded(const QString &stationID, const FixtureData &data);
    //模具使用次数更新
    void signalFixtureUpdate(const QString &stationID, const FixtureData &data);

    //总成组装完成
    void signalAssemblyFinish(const AssemblyData &assyData);
    //包装打印完成
    void signalPackageFinish(const PackageData &packData);
    //包装重量变化
    void signalPackageUpdate(const PackageData &packData);
private:
    bool createOrder(const QString &orderID);
    void deleteOrder();

    bool createCraft(const QString &craftID);
    void deleteCraft();

    bool appendStation(const QString &stationID, const QString &stationType);
    void removeStation();
private:
    WorkOrder *order;
    WorkCraft *craft;
    QList<WorkStation *> stationList;

    WorkshopData workshopData;
    WorklineData worklineData;

    bool activated;
    bool running;

    //工艺、工位对照表
    QStringList craftIDList;
    QStringList stationIDList;

    QMap<QString, QString> craftProductMap;
    QMap<QString, QString> station_CodeMap;
    QMap<QString, QString> station_TypeMap;
};

#define qWorkManager (WorkManager::instance())

#endif // WORKMANAGER_H
