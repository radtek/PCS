#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>

namespace Ui
{
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = Q_NULLPTR);
    ~DialogLogin();

private:
    void initUI();

private Q_SLOTS:
    void Slot_QPushButton_clicked_Login();

private:
    Ui::DialogLogin *ui;
};

#endif // DIALOGLOGIN_H
