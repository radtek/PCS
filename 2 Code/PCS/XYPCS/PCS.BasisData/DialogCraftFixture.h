#ifndef DIALOGCRAFTFIXTURE_H
#define DIALOGCRAFTFIXTURE_H

#include <QDialog>
#include <QtSql>

namespace Ui
{
class DialogCraftFixture;
}

class DialogCraftFixture : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCraftFixture(const QSqlRecord &record, QWidget *parent = 0);
    ~DialogCraftFixture();
private Q_SLOTS:
    void slotAccept();

public:
    QString getFixtureType()
    {
        return fixtureType;
    }
    QString getFixtureSerial()
    {
        return fixtureSerial;
    }
public slots:
    void slotSelectFixture(const QString &);

private:
    void initialDialog();
    Ui::DialogCraftFixture *ui;
    QString fixtureType;
    QString fixtureSerial;

    QSqlRecord dbRecord;
};

#endif    // DIALOGCRAFTFIXTURE_H
