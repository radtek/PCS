#ifndef DIALOGMATERIALINPUT_H
#define DIALOGMATERIALINPUT_H

#include <QDialog>

namespace Ui
{
class DialogMaterialInput;
}

class DialogMaterialInput : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMaterialInput(QWidget *parent = 0);
    virtual ~DialogMaterialInput();

    void setStationID(const QString &stationID);
    void setMaterialID(const QString &materialID);
    QString getStationID() const;
    QString getMaterialID() const;
    QString getMaterialBatch() const;
    int getBatchQuantity() const;
    QString getPackageBatch() const;
private slots:
    void slotSelectStationID();
    void slotSelectMaterialID();

private:
    void initialWidget();

private:
    Ui::DialogMaterialInput *ui;
};

#endif    // DIALOGMATERIALINPUT_H
