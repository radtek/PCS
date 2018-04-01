#ifndef DIALOGCRAFTMATERIAL_H
#define DIALOGCRAFTMATERIAL_H

#include <QDialog>
#include <QtSql>

namespace Ui
{
class DialogCraftMaterial;
}

class DialogCraftMaterial : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCraftMaterial(const QSqlRecord &record, QWidget *parent = 0);
    ~DialogCraftMaterial();

public:
    void setCraftID(const QString &CraftID)
    {
        craftID = CraftID;
    }
    void setStationID(const QString &StationID)
    {
        stationID = StationID;
    }

    QString getMaterialID()
    {
        return materialID;
    }
    QString getConsumeQuantity()
    {
        return singleConsumeQuantity;
    }
    QString getWarningQuantity()
    {
        return shortWarningQuantity;
    }

public slots:
    void slotSelectMaterial(const QString &);
private Q_SLOTS:
    void slotAccept();

private:
    void initialDialog();
    Ui::DialogCraftMaterial *ui;
    QString craftID;
    QString stationID;
    QString materialID;
    QString singleConsumeQuantity;
    QString shortWarningQuantity;

    QSqlRecord dbRecord;
};

#endif    // DIALOGCRAFTMATERIAL_H
