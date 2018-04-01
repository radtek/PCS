#ifndef WIDGETBASESTATION_H
#define WIDGETBASESTATION_H

#include <QWidget>
#include <QtSql>

enum Type
{
    Assembly,
    Package,
};

extern const QMap<QVariant, QString> Dict_Type;

namespace Ui
{
class WidgetBaseStation;
}

class XYSqlQueryModel;
class WidgetBaseStation : public QWidget
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
    explicit WidgetBaseStation(QWidget *parent = 0);
    ~WidgetBaseStation();
public slots:
    void slotStationSelect(const QModelIndex &);
    void slotAddStation();
    void slotDeleteStation();
    void slotModifyStation();
    void slotSaveStation();
    void slotCancel();

private:
    void initialWidget();
    void initialWidgetStationList();
    void updateWidgetStationList();
    void updateWidget();
    Ui::WidgetBaseStation *ui;
    QStringList headerList;
    QList<int> widthList;
    XYSqlQueryModel *model;
    StateMark saveStateMark;
    QModelIndex tableIndex;
};

#endif    // WIDGETBASESTATION_H
