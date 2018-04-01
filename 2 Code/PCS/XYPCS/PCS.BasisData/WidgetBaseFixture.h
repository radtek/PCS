#ifndef WIDGETBASEFIXTURE_H
#define WIDGETBASEFIXTURE_H

#include <QWidget>
#include <QtSql>
enum Scrap
{
    Useing,
    Scraped,
};

extern const QMap<QVariant, QString> Dict_Scrap;
namespace Ui
{
class WidgetBaseFixture;
}
class XYSqlQueryModel;
class WidgetBaseFixture : public QWidget
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
    explicit WidgetBaseFixture(QWidget *parent = 0);
    ~WidgetBaseFixture();
public slots:
    void slotFixtureSelect(const QModelIndex &);
    void slotAddFixture();
    void slotDeleteFixture();
    void slotModifyFixture();
    void slotSaveFixture();
    void slotCancel();

private:
    void initialWidget();
    void initialWidgetFixtureList();
    void updateWidgetFixtureList();
    void updateWidget();
    Ui::WidgetBaseFixture *ui;
    QStringList headerList;
    QList<int> widthList;
    XYSqlQueryModel *model;
    StateMark saveStateMark;
    QModelIndex tableIndex;
};

#endif    // WIDGETBASEFIXTURE_H
