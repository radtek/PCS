#ifndef OPCWIDGET_H
#define OPCWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSplitter>
#include <QMenu>

#include "OPCDefine.h"
#include "OPCManager.h"
#include "OPCServer.h"
#include "OPCGroup.h"

namespace Ui
{
class OPCWidget;
}

class OPCWidget : public QWidget
{
    Q_OBJECT
public:
    enum class TableHeader
    {
        ItemID,
        DataType,
        RWrights,
        IsActive,
        ItemValue,
        Timestamp,
        Quality,
    };
public:
    explicit OPCWidget(QWidget *parent = 0);
    virtual ~OPCWidget();

//    static void __stdcall OnValueChange(void *pWidget, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
//    static void __stdcall OnReadComplete(void *pWidget, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState);
//    static void __stdcall OnWriteComplete(void *pWidget, OPCGROUPHEADER *pHeader);
//    static void __stdcall OnCancelComplete(void *pWidget, OPCGROUPHEADER *pHeader);
public slots:
    void treeContextMenuRequest(const QPoint &pos);
    void browseGroupItems();
    void tableContextMenuRequest(const QPoint &pos);
    void changeItemState(int row, int column);

    void activateNewConnection();
    void activateOpenXmlFile();
    void activateSaveXmlFile();

    void activateServerNewGroup();
    void activateServerRemove();
    void activateServerProperties();

    void activateGroupNewItems();
    void activateGroupCloneGroup();
    void activateGroupModify();
    void activateGroupRemove();
    void activateGroupProperties();

    void activateItemRemoveAll();
    void activateItemRemove();
    void activateItemProperties();
private:
    void initialWidget();
    void initialMenu();
    void initialData();
private:
    Ui::OPCWidget *ui;

    QMenu *menuTree;
    QMenu *menuTable;
    QMenu *menuServer;
    QMenu *menuGroup;
    QMenu *menuItem;

    QMap<TableHeader, QString> tableHeaderMap;
};

#endif // OPCWIDGET_H
