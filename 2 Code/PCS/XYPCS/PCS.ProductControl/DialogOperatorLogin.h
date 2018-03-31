#ifndef DIALOGOPERATORLOGIN_H
#define DIALOGOPERATORLOGIN_H

#include <QDialog>

namespace Ui
{
class DialogOperatorLogin;
}

class DialogOperatorLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOperatorLogin(QWidget *parent = 0);
    virtual ~DialogOperatorLogin();

    void setStationID(const QString &stationID);
    QString getStationID() const;
    QString getOperatorCard() const;
private slots:
    void slotSelectStationID();

private:
    void initialWidget();

private:
    Ui::DialogOperatorLogin *ui;
};

#endif    // DIALOGOPERATORLOGIN_H
