#ifndef DIALOGCRAFTMEASURE_H
#define DIALOGCRAFTMEASURE_H

#include <QDialog>
#include <QtSql>

namespace Ui
{
class DialogCraftMeasure;
}

class DialogCraftMeasure : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCraftMeasure(const QSqlRecord &record, QWidget *parent = 0);
    ~DialogCraftMeasure();

public:
    QString getMeasureType()
    {
        return measureType;
    }
    QString getMeasureSerial()
    {
        return measureSerial;
    }
    QString getStandardValue()
    {
        return standardValue;
    }
    QString getUpperLimit()
    {
        return upperLimit;
    }
    QString getLowerLimit()
    {
        return lowerLimit;
    }
public slots:
    void slotSelectMeasure(const QString &);
private Q_SLOTS:
    void slotAccept();

private:
    void initialDialog();
    Ui::DialogCraftMeasure *ui;
    QString measureType;
    QString measureSerial;
    QString standardValue;
    QString upperLimit;
    QString lowerLimit;

    QSqlRecord dbRecord;
};

#endif    // DIALOGCRAFTMEASURE_H
