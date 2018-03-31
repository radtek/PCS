#ifndef DIALOGPOWERLOGIN_H
#define DIALOGPOWERLOGIN_H

#include <GlobalData.h>

namespace Ui
{
class DialogPowerLogin;
}

class DialogPowerLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPowerLogin(QWidget *parent = Q_NULLPTR);
    virtual ~DialogPowerLogin();

    void setTitle(const QString &title);
    void hideClose(bool hide);
    QString getQAUser() const;
private Q_SLOTS:
    void Slot_QPushButton_clicked_Accept();

private:
    void initWidget();
private:
    Ui::DialogPowerLogin *ui;
};

#endif
