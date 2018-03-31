#ifndef CRAFTFORMULAWIDGET_H
#define CRAFTFORMULAWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui
{
class WidgetCraftRouteDesign;
}

class WidgetCraftRouteDesign : public QWidget
{
    Q_OBJECT
public:
    enum class MaterialHeader
    {
        MaterialID,
        MaterialName,
        BarcodeRule,
        UsageQuantity,
        AlarmQuantity,
    };

    enum class MeasureHeader
    {
        MeasureID,
        MeasureName,
        MeasureUnit,
        StandardValue,
        UpperLimit,
        LowerLimit,
    };

    enum class FixtureHeader
    {
        FixtureID,
        FixtureName,
        FixtureBarcode,
        FixtureClassify,
        TotalTimes,
        UsedTimes,
        RemindTimes,
    };

public:
    explicit WidgetCraftRouteDesign(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetCraftRouteDesign();

public slots:
    void slotTreeSelectionChanged();

private:
    void initialWidget();
    void initialRecord();

private:
    Ui::WidgetCraftRouteDesign *ui;

    QMap<MaterialHeader, QString> materialHeaderMap;
    QMap<MeasureHeader, QString> measureHeaderMap;
    QMap<FixtureHeader, QString> fixtureHeaderMap;
};

#endif    // CRAFTFORMULAWIDGET_H
