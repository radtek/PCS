#ifndef DIALOGSELECTSTATION_H
#define DIALOGSELECTSTATION_H

#include <QDialog>
enum Station_Type
{
    Assembly,
    Package,
};
extern const QMap<QVariant, QString> Station_Type;
namespace Ui
{
class DialogSelectStation;
}

class XYSqlQueryModel;
class DialogSelectStation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectStation(QWidget *parent = 0);
    ~DialogSelectStation();

public:
    void setCraftID(const QString &CraftID)
    {
        craftID = CraftID;
    }

    QString getStationID()
    {
        return stationID;
    }

private:
    void init();

private Q_SLOTS:
    void slotAccept();

private:
    Ui::DialogSelectStation *ui;
    XYSqlQueryModel *model;
    QString stationID;
    QString craftID;
    QStringList headlist;
};

#endif
