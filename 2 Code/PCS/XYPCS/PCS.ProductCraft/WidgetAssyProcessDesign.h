#ifndef ASSEMBLYFLOWWIDGET_H
#define ASSEMBLYFLOWWIDGET_H

#include <QWidget>

namespace Ui
{
class WidgetAssyProcessDesign;
}

class WidgetAssyProcessDesign : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAssyProcessDesign(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetAssyProcessDesign();

public slots:
    void slotTreeSelectionChanged();

private:
    void initialWidget();
    void initialRecord();

private:
    Ui::WidgetAssyProcessDesign *ui;
};

#endif    // ASSEMBLYFLOWWIDGET_H
