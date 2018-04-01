#ifndef WIDGETCRAFTEDIT_H
#define WIDGETCRAFTEDIT_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QtSql>

namespace Ui
{
class WidgetCraftEdit;
}

class XYSqlQueryModel;
class WidgetCraftEdit : public QWidget
{
    Q_OBJECT
public:
    enum class StateMark
    {
        NoMove,
        AddNew,
        ModifyOld,
    };

    enum CraftTree
    {
        Craft,
        Station,
    };

public:
    explicit WidgetCraftEdit(QWidget *parent = 0);
    ~WidgetCraftEdit();
public slots:
    void slotTreeSelect(QTreeWidgetItem *, int);
    void slotMaterialSelect(const QModelIndex &);
    void slotMeasureSelect(const QModelIndex &);
    void slotFixtureSelect(const QModelIndex &);
    void showCraftStationDialog();
    void showCraftMeasureDialog();
    void slotModifyCraftMeasure();
    void slotDeleteCraftMeasure();
    void showCraftMaterialDialog();
    void slotModifyCraftMaterial();
    void slotDeleteCraftMaterial();
    void showCraftFixtureDialog();
    void slotModifyCraftFixture();
    void slotDeleteCraftFixture();

    void slotAddCraft();
    void slotDeleteCraft();
    void slotModifyCraft();
    void slotSaveCraft();
    void slotCancel();
    void slotDeleteStation();

private:
    Ui::WidgetCraftEdit *ui;
    void initialTreeWidget();
    void initialFrameCraft();
    void lockFrameCraft();
    void openFrameCraft();
    void initialFrameStation();
    void lockFrameStation();
    void openFrameStation();
    void initialTableViewMaterial();
    void updateTableViewMaterial();
    void initialTableViewMeasure();
    void updateTableViewMeasure();
    void initialTableViewFixture();
    void updateTableViewFixture();

    QStringList headerList;
    QList<int> widthList;
    XYSqlQueryModel *model;
    StateMark saveStateMark;
    QModelIndex tableIndex;
    QStringList headerListMeasure;
    XYSqlQueryModel *modelMeasure;
    QModelIndex tableIndexMeasure;
    QStringList headerListMaterial;
    XYSqlQueryModel *modelMaterial;
    QModelIndex tableIndexMaterial;
    QStringList headerListFixture;
    XYSqlQueryModel *modelFixture;
    QModelIndex tableIndexFixture;

    QString materialID;
};

#endif    // WIDGETCRAFTEDIT_H
