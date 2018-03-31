#include "OPCWidget.h"
#include "ui_OPCWidget.h"
#include "OPCServerNewDialog.h"
#include "OPCServerPropDialog.h"
#include "OPCGroupNewDialog.h"
#include "OPCGroupEditDialog.h"
#include "OPCItemNewDialog.h"

OPCWidget::OPCWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OPCWidget)
{
    ui->setupUi(this);

    initialWidget();
    initialMenu();
    initialData();
}

OPCWidget::~OPCWidget()
{
    delete ui;
}

void OPCWidget::initialWidget()
{
    do
    {
        QTreeWidget *tree = ui->treeWidget;
        tree->setColumnCount(1);
        tree->setHeaderLabel(tr("OPC Objects"));
        tree->setSortingEnabled(true);
        tree->sortByColumn(0, Qt::AscendingOrder);
        tree->setContextMenuPolicy(Qt::CustomContextMenu);
        tree->setStyleSheet("QTreeWidget::item{height:21px}");

        connect(tree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(treeContextMenuRequest(QPoint)));
        connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(browseGroupItems()));
    }
    while (0);
    do
    {
        tableHeaderMap.insert(TableHeader::ItemID,      "Item ID");
        tableHeaderMap.insert(TableHeader::DataType,    "Data Type");
        tableHeaderMap.insert(TableHeader::RWrights,    "RW Rights");
        tableHeaderMap.insert(TableHeader::IsActive,    "Active Status");
        tableHeaderMap.insert(TableHeader::ItemValue,   "Value");
        tableHeaderMap.insert(TableHeader::Timestamp,   "Timestamp");
        tableHeaderMap.insert(TableHeader::Quality,     "Quality");

        QTableWidget *table = ui->tableWidget;

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(tableHeaderMap.size());
        table->setHorizontalHeaderLabels(tableHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);     //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);      //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);       //禁用编辑
        table->setContextMenuPolicy(Qt::CustomContextMenu);         //启用菜单

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(60);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tableContextMenuRequest(QPoint)));
        connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(changeItemState(int, int)));
    }
    while (0);
}

void OPCWidget::initialMenu()
{
    menuTree = new QMenu(this);
    menuTree->addAction("New Server Connection", this, SLOT(activateNewConnection()));
    menuTree->addSeparator();
    menuTree->addAction("Open Xml File", this, SLOT(activateOpenXmlFile()));
    menuTree->addAction("Save Xml File", this, SLOT(activateSaveXmlFile()));

    menuTable = new QMenu(this);
    menuTable->addAction("New Items", this, SLOT(activateGroupNewItems()));
    menuTable->addSeparator();
    menuTable->addAction("Remove All", this, SLOT(activateItemRemoveAll()));

    menuServer = new QMenu(this);
    menuServer->addAction("New Group", this, SLOT(activateServerNewGroup()));
    menuServer->addSeparator();
    menuServer->addAction("Remove", this, SLOT(activateServerRemove()));
    menuServer->addSeparator();
    menuServer->addAction("Properties", this, SLOT(activateServerProperties()));

    menuGroup = new QMenu(this);
    menuGroup->addAction("New Items", this, SLOT(activateGroupNewItems()));
    menuGroup->addSeparator();
    menuGroup->addSeparator();
    menuGroup->addSeparator();
    menuGroup->addAction("Clone Group", this, SLOT(activateGroupCloneGroup()));
    menuGroup->addSeparator();
    menuGroup->addAction("Modify", this, SLOT(activateGroupModify()));
    menuGroup->addAction("Remove", this, SLOT(activateGroupRemove()));
    menuGroup->addSeparator();
    menuGroup->addAction("Properties", this, SLOT(activateGroupProperties()));

    menuItem = new QMenu(this);
    menuItem->addAction("New Items", this, SLOT(activateGroupNewItems()));
    menuItem->addSeparator();
    menuItem->addAction("Remove", this, SLOT(activateItemRemove()));
    menuItem->addSeparator();
    menuItem->addAction("Properties", this, SLOT(activateItemProperties()));
}

void OPCWidget::initialData()
{
    QTreeWidget *tree = ui->treeWidget;

    for (auto pServer : qOPCManager->GetServerList())
    {
        QTreeWidgetItem *serverItem = new QTreeWidgetItem;
        serverItem->setText(0, QString::fromWCharArray(pServer->GetProgID()));
        serverItem->setData(0, Qt::UserRole, QVariant::fromValue(pServer));
        tree->addTopLevelItem(serverItem);

        for (auto pGroup : pServer->GetGroupList())
        {
            QTreeWidgetItem *groupItem = new QTreeWidgetItem;
            groupItem->setText(0, QString::fromWCharArray(pGroup->GetName()));
            groupItem->setData(0, Qt::UserRole, QVariant::fromValue(pGroup));
            serverItem->addChild(groupItem);

//            pGroup->AddDataChangeFunc(OPCWidget::OnValueChange, this);
//            pGroup->AddReadCompleteFunc(OPCWidget::OnReadComplete, this);
//            pGroup->AddWriteCompleteFunc(OPCWidget::OnWriteComplete, this);
//            pGroup->AddCancelCompleteFunc(OPCWidget::OnCancelComplete, this);
        }
    }
}

void OPCWidget::activateNewConnection()
{
    OPCServerNewDialog dialog(qOPCManager, this);

    if (QDialog::Accepted != dialog.exec())
        return;

    XYOPCServer *pServer = dialog.getNewServer();

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *root = new QTreeWidgetItem;

    root->setText(0, QString::fromWCharArray(pServer->GetProgID()));
    root->setData(0, Qt::UserRole, QVariant::fromValue(pServer));

    tree->addTopLevelItem(root);
    tree->setCurrentItem(root);
}

void OPCWidget::activateOpenXmlFile()
{
    ui->treeWidget->clear();

    qOPCManager->ReadXmlFile(OPC_CONFIG_XML);

    QTreeWidget *tree = ui->treeWidget;

    for (auto pServer : qOPCManager->GetServerList())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem;
        root->setText(0, QString::fromWCharArray(pServer->GetProgID()));
        root->setData(0, Qt::UserRole, QVariant::fromValue(pServer));
        tree->addTopLevelItem(root);

        for (auto pGroup : pServer->GetGroupList())
        {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, QString::fromWCharArray(pGroup->GetName()));
            child->setData(0, Qt::UserRole, QVariant::fromValue(pGroup));

//            pGroup->AddDataChangeFunc(OPCWidget::OnValueChange, this);
//            pGroup->AddReadCompleteFunc(OPCWidget::OnReadComplete, this);
//            pGroup->AddWriteCompleteFunc(OPCWidget::OnWriteComplete, this);
//            pGroup->AddCancelCompleteFunc(OPCWidget::OnCancelComplete, this);

            root->addChild(child);
        }
    }

    if (tree->topLevelItemCount() > 0)
    {
        QTreeWidgetItem *root = tree->topLevelItem(0);
        tree->setCurrentItem(root);
    }
}

void OPCWidget::activateSaveXmlFile()
{
    qOPCManager->SaveXmlFile(OPC_CONFIG_XML);
}

void OPCWidget::activateServerNewGroup()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *root = tree->currentItem();

    if (root == nullptr)
        return;

    XYOPCServer *pServer = root->data(0, Qt::UserRole).value<XYOPCServer *>();

    OPCGroupNewDialog dialog(pServer, this);

    if (QDialog::Accepted != dialog.exec())
        return;

    XYOPCGroup *pGroup = dialog.getNewGroup();

//    pGroup->AddDataChangeFunc(OPCWidget::OnValueChange, this);
//    pGroup->AddReadCompleteFunc(OPCWidget::OnReadComplete, this);
//    pGroup->AddWriteCompleteFunc(OPCWidget::OnWriteComplete, this);
//    pGroup->AddCancelCompleteFunc(OPCWidget::OnCancelComplete, this);

    QTreeWidgetItem *child = new QTreeWidgetItem;
    child->setText(0, QString::fromWCharArray(pGroup->GetName()));
    child->setData(0, Qt::UserRole, QVariant::fromValue(pGroup));

    root->addChild(child);
    tree->setCurrentItem(child);
}

void OPCWidget::activateServerRemove()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *root = tree->currentItem();

    if (root == nullptr)
        return;

    XYOPCServer *pServer = root->data(0, Qt::UserRole).value<XYOPCServer *>();

    qOPCManager->ReleaseServer(pServer);

    int index = tree->indexOfTopLevelItem(root);
    delete tree->takeTopLevelItem(index);
}

void OPCWidget::activateServerProperties()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *root = tree->currentItem();

    if (root == nullptr)
        return;

    auto pServer = root->data(0, Qt::UserRole).value<XYOPCServer *>();

    OPCServerPropDialog dialog(pServer, this);

    dialog.exec();
}

void OPCWidget::activateGroupNewItems()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();

    OPCItemNewDialog dialog(pGroup, this);

    if (QDialog::Accepted != dialog.exec())
        return;

    browseGroupItems();
}

void OPCWidget::activateGroupCloneGroup()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();

    OPCGroupEditDialog dialog(pGroup, this);
    dialog.setEditType(OPCGroupEditDialog::GroupClone);

    if (QDialog::Accepted != dialog.exec())
        return;

    pGroup = dialog.getGroupPtr();

//    pGroup->AddDataChangeFunc(OPCWidget::OnValueChange, this);
//    pGroup->AddReadCompleteFunc(OPCWidget::OnReadComplete, this);
//    pGroup->AddWriteCompleteFunc(OPCWidget::OnWriteComplete, this);
//    pGroup->AddCancelCompleteFunc(OPCWidget::OnCancelComplete, this);

    QTreeWidgetItem *root = item->parent();
    QTreeWidgetItem *child = new QTreeWidgetItem;
    child->setText(0, QString::fromWCharArray(pGroup->GetName()));
    child->setData(0, Qt::UserRole, QVariant::fromValue(pGroup));

    root->addChild(child);
    tree->setCurrentItem(child);
}

void OPCWidget::activateGroupModify()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();

    OPCGroupEditDialog dialog(pGroup, this);
    dialog.setEditType(OPCGroupEditDialog::GroupModify);

    if (QDialog::Accepted != dialog.exec())
        return;

    item->setText(0, QString::fromWCharArray(pGroup->GetName()));
}

void OPCWidget::activateGroupRemove()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();
    QTreeWidgetItem *root = item->parent();

    if (item == nullptr || root == nullptr)
        return;

    auto pServer = root->data(0, Qt::UserRole).value<XYOPCServer *>();
    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();

    pServer->RemoveGroup(pGroup);

    root->removeChild(item);
}

void OPCWidget::activateGroupProperties()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();

    OPCGroupEditDialog dialog(pGroup, this);

    dialog.exec();
}

void OPCWidget::activateItemRemoveAll()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();

    pGroup->RemoveAllItems();

    browseGroupItems();
}

void OPCWidget::activateItemRemove()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *treeItem = tree->currentItem();
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *tableItem = nullptr;
    int row = table->currentRow();

    if (treeItem == nullptr || row == -1)
        return;

    auto pGroup = treeItem->data(0, Qt::UserRole).value<XYOPCGroup *>();
    tableItem = table->item(row, static_cast<int>(TableHeader::ItemID));
    auto pAttribute = tableItem->data(Qt::UserRole).value<OPCITEMATTRIBUTES *>();
    pGroup->RemoveItems(1, &pAttribute->hServer);

    browseGroupItems();
}

void OPCWidget::activateItemProperties()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();
    QTableWidget *table = ui->tableWidget;
    int row = table->currentRow();

    if (item == nullptr || row == -1)
        return;

}

void OPCWidget::treeContextMenuRequest(const QPoint &pos)
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->itemAt(pos);

    if (item == nullptr)
        menuTree->exec(QCursor::pos());
    else if (-1 != tree->indexOfTopLevelItem(item))
        menuServer->exec(QCursor::pos());
    else
        menuGroup->exec(QCursor::pos());
}

void OPCWidget::tableContextMenuRequest(const QPoint &pos)
{
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = table->itemAt(pos);

    if (item == nullptr)
        menuTable->exec(QCursor::pos());
    else
        menuItem->exec(QCursor::pos());
}

void OPCWidget::browseGroupItems()
{
    QTreeWidget *treeWidget = ui->treeWidget;
    QTreeWidgetItem *treeItem = treeWidget->currentItem();
    QTableWidget *tableWidget = ui->tableWidget;
    QTableWidgetItem *tableItem = nullptr;

    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);

    if (treeItem == nullptr)
        return;
    if (-1 != treeWidget->indexOfTopLevelItem(treeItem))
        return;

    auto pGroup = treeItem->data(0, Qt::UserRole).value<XYOPCGroup *>();

    ItemAttributeList attributeList = pGroup->BrowseItemAttributes();
    ItemStateList stateList = pGroup->BrowseItemStates();

    int itemCount = attributeList.size();
    tableWidget->setRowCount(itemCount);

    for (int row = 0; row < itemCount; ++row)
    {
        tableWidget->setRowHeight(row, 21);

        OPCITEMATTRIBUTES *pAttribute = attributeList.at(row);
        OPCITEMSTATE *pState = stateList.at(row);

        //Item ID
        tableItem = new QTableWidgetItem;
        tableItem->setData(Qt::UserRole, QVariant::fromValue(pAttribute));
        tableItem->setText(QString::fromWCharArray(pAttribute->szItemID));
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::ItemID), tableItem);

        //Data Type
        tableItem = new QTableWidgetItem;
        tableItem->setText(VARTYPE_to_QString(pAttribute->vtCanonicalDataType));
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::DataType), tableItem);

        //RW Rights
        tableItem = new QTableWidgetItem;
        tableItem->setText(RIGHTS_to_QString(pAttribute->dwAccessRights));
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::RWrights), tableItem);

        //Active Status
        tableItem = new QTableWidgetItem;
        tableItem->setCheckState(pAttribute->bActive ? Qt::Checked : Qt::Unchecked);
        tableItem->setText(pAttribute->bActive ? "Activated" : "Inactivated");
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::IsActive), tableItem);

        //Value
        tableItem = new QTableWidgetItem;
        tableItem->setText(VARIANT_to_QVariant(pState->vDataValue).toString());
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::ItemValue), tableItem);

        //Timestamp
        tableItem = new QTableWidgetItem;
        tableItem->setText(FILETIME_to_QDateTime(pState->ftTimeStamp).toString());
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::Timestamp), tableItem);

        //Quality"
        tableItem = new QTableWidgetItem;
        tableItem->setText(QUALITY_to_QString(pState->wQuality));
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, static_cast<int>(TableHeader::Quality), tableItem);
    }

    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(0);
}

void OPCWidget::changeItemState(int row, int column)
{
    if (column != static_cast<int>(TableHeader::IsActive))
        return;

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *treeItem = tree->currentItem();
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *tableItem = nullptr;

    if (-1 != tree->indexOfTopLevelItem(treeItem))
        return;

    auto pGroup = treeItem->data(0, Qt::UserRole).value<XYOPCGroup *>();
    tableItem = table->item(row, static_cast<int>(TableHeader::ItemID));
    auto pAttribute = tableItem->data(Qt::UserRole).value<OPCITEMATTRIBUTES *>();

    tableItem = table->item(row, static_cast<int>(TableHeader::IsActive));
    bool active = (tableItem->checkState() == Qt::Checked);
    pGroup->SetActiveState(1, &pAttribute->hServer, active ? TRUE : FALSE);

    tableItem->setText(active ? "Activated" : "Inactivated");
}

//void OPCWidget::OnValueChange(void *pWidget, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState)
//{
//    OPCWidget *widget = (OPCWidget *)pWidget;
//    QTreeWidget *tree = widget->ui->treeWidget;
//    QTreeWidgetItem *item = tree->currentItem();
//    if (item == nullptr || -1 != tree->indexOfTopLevelItem(item))
//        return;

//    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();
//    if (pGroup->GetClientHandle() != pHeader->hClientGroup)
//        return;

//    QTableWidget *table = widget->ui->tableWidget;
//    int count = table->rowCount();

//    for (DWORD i = 0; i < pHeader->dwItemCount; ++i)
//    {
//        for (int row = 0; row < count; ++row)
//        {
//            if (table->item(row, 0) == nullptr) continue;

//            auto pAttribute = table->item(row, 0)->data(Qt::UserRole).value<OPCITEMATTRIBUTES *>();

//            if (pState[i].hClient == pAttribute->hClient)
//            {
//                table->item(row, 4)->setText(VARIANT_to_QVariant(pState[i].vDataValue).toString());
//                table->item(row, 5)->setText(FILETIME_to_QDateTime(pState->ftTimeStamp).toString());
//                table->item(row, 6)->setText(QUALITY_to_QString(pState->wQuality));
//            }
//        }
//    }
//}

//void OPCWidget::OnReadComplete(void *pWidget, OPCGROUPHEADER *pHeader, OPCITEMSTATE *pState)
//{
//    OPCWidget *widget = (OPCWidget *)pWidget;
//    QTreeWidget *tree = widget->ui->treeWidget;
//    QTreeWidgetItem *item = tree->currentItem();
//    if (item == nullptr || -1 != tree->indexOfTopLevelItem(item))
//        return;

//    auto pGroup = item->data(0, Qt::UserRole).value<XYOPCGroup *>();
//    if (pGroup->GetClientHandle() != pHeader->hClientGroup)
//        return;

//    QTableWidget *table = widget->ui->tableWidget;
//    int count = table->rowCount();

//    for (DWORD i = 0; i < pHeader->dwItemCount; ++i)
//    {
//        for (int row = 0; row < count; ++row)
//        {
//            auto pAttribute = table->item(row, 0)->data(Qt::UserRole).value<OPCITEMATTRIBUTES *>();

//            if (pState[i].hClient == pAttribute->hClient)
//            {
//                table->item(row, 4)->setText(VARIANT_to_QVariant(pState[i].vDataValue).toString());
//                table->item(row, 5)->setText(FILETIME_to_QDateTime(pState->ftTimeStamp).toString());
//                table->item(row, 6)->setText(QUALITY_to_QString(pState->wQuality));
//            }
//        }
//    }
//}

//void OPCWidget::OnWriteComplete(void *pWidget, OPCGROUPHEADER *pHeader)
//{

//}

//void OPCWidget::OnCancelComplete(void *pWidget, OPCGROUPHEADER *pHeader)
//{

//}








