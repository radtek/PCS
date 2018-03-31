#ifndef OPCSERVERPROPDIALOG_H
#define OPCSERVERPROPDIALOG_H

#include <QDialog>

namespace Ui {
class OPCServerPropDialog;
}

class XYOPCServer;

class OPCServerPropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OPCServerPropDialog(XYOPCServer *pServer, QWidget *parent = 0);
    virtual ~OPCServerPropDialog();

private:
    void initialWidget();
    void setServerProp();
private:
    Ui::OPCServerPropDialog *ui;

    XYOPCServer *server;
};

#endif // OPCSERVERPROPDIALOG_H
