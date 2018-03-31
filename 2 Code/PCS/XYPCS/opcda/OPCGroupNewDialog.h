#ifndef OPCGROUPNEWDIALOG_H
#define OPCGROUPNEWDIALOG_H

#include <QDialog>

namespace Ui {
class OPCGroupNewDialog;
}

class XYOPCServer;
class XYOPCGroup;

class OPCGroupNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OPCGroupNewDialog(XYOPCServer *server, QWidget *parent = 0);
    virtual ~OPCGroupNewDialog();

    XYOPCGroup *getNewGroup() { return group; }

public slots:
    void accept();
private:
    void initialWidget();
private:
    Ui::OPCGroupNewDialog *ui;

    XYOPCServer *server;
    XYOPCGroup *group;
};

#endif // OPCGROUPNEWDIALOG_H
