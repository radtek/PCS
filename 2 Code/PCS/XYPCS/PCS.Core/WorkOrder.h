#ifndef WORKORDER_H
#define WORKORDER_H

#include "WorkDefine.h"

class WorkManager;

class WorkOrder
{
public:
    explicit WorkOrder(WorkManager *manager);
    virtual ~WorkOrder();

    //初始化工单
    bool initialOrder(const QString &orderID);

    //保存工单
    bool saveOrderData();
public:
    //更新已完成数量
    void updateFinishQuantity() { ++orderData.finishQuantity; }
    //更新未完成数量
    void updateFailedQuantity() { ++orderData.failedQuantity; }
    //更新包装数量
    void updatePackageQuantity() { ++orderData.packageQuantity; }
    //更新抽检数量
    void updateSampleQuantity() { ++orderData.sampleQuantity; }
    //更新返修数量
    void updateRepairQuantity() { ++orderData.repairQuantity; }
    //更新一致性成功数
    void updateFirstPassQuantity() { ++orderData.firstPassQuantity; }
public:
    //工单是否完成
    inline bool isOrderFinished() const
    {
        return orderData.orderPlanQuantity <= orderData.finishQuantity;
    }
    //是否执行抽检n == int(a*(s+1)), a = N/(S+1)
    inline bool isExecuteSample() const
    {
        return (0.0 != orderData.inspectInterval) &&
               (orderData.sampleQuantity != orderData.inspectQuantity) &&
               (orderData.finishQuantity == int(orderData.inspectInterval * (orderData.sampleQuantity + 1.0)));
    }

    bool verifyFirstProductQA();            //校验首件检验是否执行
    bool verifyFirstPackageQA();            //校验首箱检验是否执行
    bool verifyFinalProductQA();            //校验末件质检是否执行
    bool verifyFinalPackageQA();            //校验末箱质检是否执行

    //首件检验
    void firstProductQA();
    //首箱检验
    void firstPackageQA();
    //末件质检
    void finalProductQA();
    //末箱质检
    void finalPackageQA();


    inline QString getOrderID() const { return orderData.orderID; }
    inline QString getCraftID() const { return orderData.craftID; }
    inline QString getProductBatch() const { return orderData.productBatch; }
    inline QDate getPlanDate() const { return orderData.orderPlanDate; }

    inline const OrderData &getOrderData() const { return orderData; }
private:
    WorkManager *manager;

    OrderData orderData;            //工单数据
};

#endif // WORKORDER_H
