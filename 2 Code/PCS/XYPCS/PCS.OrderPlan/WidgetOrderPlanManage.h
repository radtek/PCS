#ifndef PCS_ORDERPLAN_WIDGETORDERPLANMANAGE_H
#define PCS_ORDERPLAN_WIDGETORDERPLANMANAGE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "WorkManager.h"

namespace Ui
{
class WidgetOrderPlanManage;
}

class WidgetOrderPlanManage : public QWidget
{
    Q_OBJECT

public:
    enum class TableHeader
    {
        OrderID,
        CraftID,
        ProductID,
        ProductName,
        ProductBatch,
        OrderPlanDate,
        OrderPlanQuantity,
        InspectQuantity,
        OrderState,
    };

public:
    explicit WidgetOrderPlanManage(QWidget *parent = 0);
    virtual ~WidgetOrderPlanManage();

public slots:
    void slotSelectOrder();      //选择
    void slotRefreshOrder();     //刷新
    void slotSyncOnline();       //在线同步工单
    void slotLoadOffline();      //离线导入工单
    void slotReturnOrder();      //退回工单
    void slotForceClose();       //强制完结工单
    void slotActivateOrder();    //激活工单
    void slotSuspendOrder();     //暂停工单
    void slotCloseOrder();       //关闭工单
signals:
    void signalActivateOrder(const QString &order);
    void signalSuspendOrder(const QString &order);

private:
    void initialWidget();
    void initialOrder();
    void initialButton();

    void displayOrderInfo(const QString &orderID);
    void changeButtonState(OrderState state);
    void changeOrderState(int row, OrderState state, const QString &remark = QString());

    void uploadZKserver(const QString &orderID);

private:
    Ui::WidgetOrderPlanManage *ui;

    QMap<TableHeader, QString> tableHeaderMap;
};

#endif
