#ifndef WIDGETBASEMATERIAL_H
#define WIDGETBASEMATERIAL_H

#include <QWidget>
#include <QtSql>

namespace Ui
{
class WidgetBaseMaterial;
}
class XYSqlQueryModel;
class WidgetBaseMaterial : public QWidget
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
    explicit WidgetBaseMaterial(QWidget *parent = 0);
    ~WidgetBaseMaterial();
public slots:
    void slotMaterialSelect(const QModelIndex &);
    void slotAddMaterial();
    void slotDeleteMaterial();
    void slotModifyMaterial();
    void slotSaveMaterial();
    void slotCancel();

private:
    void initialWidget();
    void initialWidgetMaterialList();
    void updateWidgetMaterialList();
    void updateWidget();
    Ui::WidgetBaseMaterial *ui;
    QStringList headerList;
    QList<int> widthList;
    XYSqlQueryModel *model;
    StateMark saveStateMark;
    QModelIndex tableIndex;
};

#endif    // WIDGETBASEMATERIAL_H
