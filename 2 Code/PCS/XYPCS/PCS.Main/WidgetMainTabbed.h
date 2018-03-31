#ifndef PCS_MAIN_WIDGETMAINTABBED_H
#define PCS_MAIN_WIDGETMAINTABBED_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui
{
class WidgetMainTabbed;
}

class WidgetMainTabbed : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMainTabbed(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetMainTabbed();

    void appendWidget(QWidget *widget, const QString &title, const QString &image);
    void activeWidget(QWidget *widget);
public slots:
    void activeWidget(int index);
    void removeWidget(int index);
signals:
    void removeWidget(QWidget *widget);

private:
    void initialWidget();

private:
    Ui::WidgetMainTabbed *ui;

    QTabBar *tabBar;
    QStackedWidget *stackedWidget;
};

#endif    // TABBEDWIDGET_H
