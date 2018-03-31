#ifndef PCS_ORDERPLAN_WIDGETORDERPLANHISTORY_H
#define PCS_ORDERPLAN_WIDGETORDERPLANHISTORY_H

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>

namespace Ui
{
class WidgetOrderPlanHistory;
}

class WidgetOrderPlanHistory : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetOrderPlanHistory(QWidget *parent = 0);
    virtual ~WidgetOrderPlanHistory();

public slots:
    void slotRefreshView();

private:
    void initialWidget();

    void queryByAllOrder();
    void queryByProductID();
    void queryByCraftID();
    void queryByProductBatch();
    void queryByOrderState();
    void queryByPlanDate();
    void queryByDeliveryDate();

private:
    Ui::WidgetOrderPlanHistory *ui;

    QSqlQueryModel *orderModel;

    QString orderSQL;
};

#endif
