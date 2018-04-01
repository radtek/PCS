#ifndef WIDGETBASEWORKER_H
#define WIDGETBASEWORKER_H

#include <QWidget>
#include <QtSql>

enum Duty
{
    Operator,
    Monitor,
    Chargeman,
    Director,
};

extern const QMap<QVariant, QString> Dict_Duty;

namespace Ui
{
class WidgetBaseWorker;
}

class XYSqlQueryModel;
class WidgetBaseWorker : public QWidget
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
    explicit WidgetBaseWorker(QWidget *parent = 0);
    ~WidgetBaseWorker();
public slots:
    void slotWorkerSelect(const QModelIndex &);
    void slotAddWorker();
    void slotDeleteWorker();
    void slotModifyWorker();
    void slotSaveWorker();
    void slotCancel();

private:
    void initialWidget();
    void initialWidgetWorkerList();
    void updateWidgetWorkerList();
    void updateWidget();
    Ui::WidgetBaseWorker *ui;
    QStringList headerList;
    QList<int> widthList;
    XYSqlQueryModel *model;
    StateMark saveStateMark;
    QModelIndex tableIndex;
};

#endif    // WIDGETBASEWORKER_H
