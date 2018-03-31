#ifndef COMMWIDGET_H
#define COMMWIDGET_H

#include "CommDefine.h"

namespace Ui
{
class CommWidget;
}

class CommWidget : public QWidget
{
    Q_OBJECT
public:
    enum class TableHeader
    {
        DeviceID,
        StationID,
        DeviceType,
        IsEnable,
        CommType,
        CommPara,
    };
public:
    explicit CommWidget(QWidget *parent = 0);
    virtual ~CommWidget();

private:
    void initialWidget();
    void initialMenu();
    void initialData();
private:
    Ui::CommWidget *ui;

    QMap<TableHeader, QString> tableHeaderMap;
};

#endif // COMMWIDGET_H
