#ifndef WIDGETWORKDATAMONITOR_H
#define WIDGETWORKDATAMONITOR_H

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>

namespace Ui
{
class WidgetWorkDataMonitor;
}

class WidgetWorkDataMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetWorkDataMonitor(QWidget *parent = 0);
    virtual ~WidgetWorkDataMonitor();

private slots:
    void slotRefreshView();

private:
    void initialWidget();

    void queryByCurrentOrder();
    void queryByLocalDatabase();
    void queryByAssemblyBarcode();
    void queryByPackageBarcode();
    void queryByMaterialBarcode();
    void queryByMaterialBatch();

private:
    Ui::WidgetWorkDataMonitor *ui;

    QSqlQueryModel *assemblyModel;
    QSqlQueryModel *packageModel;
    QSqlQueryModel *stationModel;
    QSqlQueryModel *MaterialModel;
    QSqlQueryModel *MeasureModel;
    QSqlQueryModel *TransferModel;

    QString assemblySQL;
    QString packageSQL;
    QString stationSQL;
    QString materialSQL;
    QString measureSQL;
    QString transferSQL;
};

#endif    // WIDGETWORKDATAMONITOR_H
