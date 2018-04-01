#ifndef WIDGETBASEMEASURE_H
#define WIDGETBASEMEASURE_H

#include <QWidget>
#include <QtSql>

namespace Ui
{
class WidgetBaseMeasure;
}
class XYSqlQueryModel;
class WidgetBaseMeasure : public QWidget
{
    Q_OBJECT
public:
    enum class StateMark
    {
        NoMove,
        AddNew,
        ModifyOld,
    };

public:
    explicit WidgetBaseMeasure(QWidget *parent = 0);
    ~WidgetBaseMeasure();
public slots:
    void slotMeasureSelect(const QModelIndex &);
    void slotAddMeasure();
    void slotDeleteMeasure();
    void slotModifyMeasure();
    void slotSaveMeasure();
    void slotCancel();

private:
    void initialWidget();
    void initialWidgetMeasureList();
    void updateWidgetMeasureList();
    void updateWidget();
    Ui::WidgetBaseMeasure *ui;
    QStringList headerList;
    QList<int> widthList;
    XYSqlQueryModel *model;
    StateMark saveStateMark;
    QModelIndex tableIndex;
};

#endif    // WIDGETBASEMEASURE_H
