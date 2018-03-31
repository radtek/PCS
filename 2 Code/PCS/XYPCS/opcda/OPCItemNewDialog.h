#ifndef OPCITEMNEWDIALOG_H
#define OPCITEMNEWDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include "OPCDefine.h"

namespace Ui {
class OPCItemNewDialog;
}

class XYOPCServer;
class XYOPCGroup;

class OPCItemNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OPCItemNewDialog(XYOPCGroup *group, QWidget *parent = 0);
    virtual ~OPCItemNewDialog();

public slots:
    void treeItemExpanded(QTreeWidgetItem *item);
    void treeItemCollapsed(QTreeWidgetItem *item);
    void tableItemChanged(QTableWidgetItem *item);
    void tableItemClicked(QTableWidgetItem *item);
    void refreshItemList();

    void addItem();
    void addLeaves();
    void removeItem();

    void accept();
private:
    void initialWidget();
    void deleteAllBrowse();
private:
    Ui::OPCItemNewDialog *ui;

    XYOPCGroup *group;
    XYOPCServer *server;

    ItemBrowseList allBrowseList;
};

#endif // OPCITEMNEWDIALOG_H
