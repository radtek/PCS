#ifndef WORKSTATION_H
#define WORKSTATION_H

#include "WorkManager.h"
#include "WorkOrder.h"
#include "WorkCraft.h"
#include "StationMaterial.h"
#include "StationMeasure.h"
#include "StationFixture.h"
#include "StationOperator.h"

class WorkStation
{
public:
    explicit WorkStation(WorkManager *manager);
    virtual ~WorkStation();

    //载入工位信息
    virtual bool initialStation(const QString &stationID);
public:
    StationMaterial *getMaterial(const QString &id) const;
    StationMeasure *getMeasure(const QString &id) const;
    StationFixture *getFixture(const QString &id) const;
    StationOperator *getOperator(const QString &id) const;

    QList<StationMaterial *> getMaterialList() const { return materialList; }
    QList<StationMeasure *> getMeasureList() const { return measureList; }
    QList<StationFixture *> getFixtureList() const { return fixtureList; }
    QList<StationOperator *> getOperatorList() const { return operatorList; }

    StationOperator *getOperatorByBarcode(const QString &barcode) const;
    StationMaterial *getMaterialByBarcode(const QString &barcode) const;
private:
    //载入物料信息
    bool appendMaterial(const QString &materialID);
    void removeMaterial();
    //载入测试信息
    bool appendMeasure(const QString &measureID);
    void removeMeasure();
    //载入工装信息
    bool appendFixture(const QString &fixtureID);
    void removeFixture();
    //加载操作员信息
    bool appendOperator(const QString &barcode);
    bool removeOperator(StationOperator *ptr);
    void removeOperator();
public:
    ///跟工单有关的函数
    //获取完成数量，保存数据后触发
    void updateFinishQuantity() { ++stationData.finishQuantity; }
    //获取不合格数量，保存数据后触发
    void updateFailedQuantity() { ++stationData.failedQuantity; }

    bool verifyOrderStatus();       //校验工单状态
    bool verifyWorkStatus();        //校验生产状态
    bool verifyOrderFinished();     //校验工单是否完成
    bool verifyOperatorLogin();     //校验员工是否登陆
    bool verifyMaterialInput();     //校验物料是否投料
    bool verifyMaterialUseup();     //校验物料是否用完
    bool verifyMaterialAlarm();     //校验物料是否提醒
    bool verifyFixtureUseup();      //校验模具是否用完
    bool verifyMachineCheck();      //校验设备是否点检
    bool verifyProductCheck();      //校验产品是否点检
    bool verifyFixtureCheck();      //校验工装是否校验

    //设备点检
    void machineCheck();
    //产品点检
    void productCheck();
    //工装模具点检
    void fixtureCheck();

    //工装模具载入
    bool fixtureLoad(const QString &documentID);
    //操作员登陆
    bool operatorLogin(const QString &barcode);
    //操作员登出
    bool operatorLogout(const QString &operatorID);
    //物料投料
    bool materialInput(const QString &materialID, const QString &batch, int quantity);
    //物料投料
    bool materialInput(const QString &barcode);
    //物料退料
    bool materialReturn(const QString &materialID);

public:
    virtual void opcValueChanged(const DataDefine &) = 0;
    virtual void opcReadComplete(const DataDefine &) = 0;
    virtual void dacDataReceived(const DataDefine &) = 0;

    virtual void readOpcData(const DataDefine &) = 0;
    virtual void writeOpcData(const DataDefine &) = 0;
    virtual void writeDacData(const DataDefine &) = 0;

    virtual EventState verifyStationStatus() = 0;
    virtual void updateStationStatus() = 0;
public:
    inline WorkManager *getManager() const { return manager; }
    inline WorkOrder *getOrder() const { return order; }
    inline WorkCraft *getCraft() const { return craft; }

    inline QString getStationID() const { return stationData.stationID; }
    inline QString getStationType() const { return stationData.stationType; }
    inline const StationData &getStationData() const { return stationData; }

    inline QString getOperatorID() const { return operatorList.isEmpty() ? QString() : operatorList.at(0)->getOperatorCard(); }
    inline QVariant getUploadTime() const { return qApplication->isOnline() ? QDateTime::currentDateTime() : QVariant(); }
protected:
    WorkManager *manager;
    WorkOrder *order;
    WorkCraft *craft;

    StationData stationData;    //工位属性

    QList<StationMaterial *> materialList;       //物料列表
    QList<StationMeasure *> measureList;         //测试列表
    QList<StationFixture *> fixtureList;         //工装列表
    QList<StationOperator *> operatorList;       //操作员列表
};

#endif // WORKSTATION_H
