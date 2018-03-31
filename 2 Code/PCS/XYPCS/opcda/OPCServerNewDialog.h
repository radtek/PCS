#ifndef OPCSERVERNEWDIALOG_H
#define OPCSERVERNEWDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "OPCDefine.h"

namespace Ui {
class OPCServerNewDialog;
}

class XYOPCManager;
class XYOPCServer;

class OPCServerNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OPCServerNewDialog(XYOPCManager *manager, QWidget *parent = 0);
    virtual ~OPCServerNewDialog();

    XYOPCServer *getNewServer() { return server; }

public slots:
    void treeItemPressed(QTreeWidgetItem *item, int column);
    void accept();
private:
    void initialWidget();
    void browseLocalServer();
    void browseRemoteServer();
    void deleteAllInfo();
private:
    Ui::OPCServerNewDialog *ui;
    XYOPCManager *manager;
    XYOPCServer *server;

    ServerInfoList infoList;
};

#endif // OPCSERVERNEWDIALOG_H
