#include "OPCItemNewDialog.h"
#include "ui_OPCItemNewDialog.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include <QDebug>

OPCItemNewDialog::OPCItemNewDialog(XYOPCGroup *group, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OPCItemNewDialog)
    , group(group)
    , server(group->GetServer())
{
    ui->setupUi(this);

    initialWidget();
}

OPCItemNewDialog::~OPCItemNewDialog()
{
    deleteAllBrowse();

    delete ui;
}

void OPCItemNewDialog::initialWidget()
{
    this->setWindowTitle("New Items");
    this->setMinimumHeight(600);
    this->setMinimumHeight(600);

    QTableWidget *table = ui->tableWidget;
    table->setColumnCount(4);
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 100);
    table->setColumnWidth(2, 100);
    table->setColumnWidth(3, 100);
    QStringList headerLables;
    headerLables << "Item ID" << "Data Type" << "RW Rights" << "Active Status";
    table->setHorizontalHeaderLabels(headerLables);
    table->setSelectionBehavior(QTableWidget::SelectRows);
    table->horizontalHeader()->setStretchLastSection(false);
    table->horizontalHeader()->setHighlightSections(false);

    connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableItemChanged(QTableWidgetItem*)));
    connect(table, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(tableItemClicked(QTableWidgetItem*)));

    QListWidget *list = ui->listWidget;
    list->setSelectionMode(QListWidget::ExtendedSelection);

    QTreeWidget *tree = ui->treeWidget;
    tree->setHeaderHidden(true);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, QString::fromWCharArray(server->GetProgID()));
    item->setData(0, Qt::UserRole, QVariant::fromValue(server));
    item->addChild(new QTreeWidgetItem);
    tree->addTopLevelItem(item);
    tree->setCurrentItem(item);

    connect(tree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(treeItemExpanded(QTreeWidgetItem*)));
    connect(tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(treeItemCollapsed(QTreeWidgetItem*)));
    connect(tree, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this ,SLOT(refreshItemList()));

    connect(ui->checkBrowseFlat, SIGNAL(clicked(bool)), this ,SLOT(refreshItemList()));
    connect(ui->buttonAddItem, SIGNAL(clicked(bool)), this ,SLOT(addItem()));
    connect(ui->buttonAddLeaves, SIGNAL(clicked(bool)), this ,SLOT(addLeaves()));
    connect(ui->buttonRemoveItem, SIGNAL(clicked(bool)), this ,SLOT(removeItem()));

    connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->buttonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

void OPCItemNewDialog::deleteAllBrowse()
{
    for (auto data : allBrowseList)
    {
        ComStringFree(data->szItemID);
        ComStringFree(data->szItemDataID);
        ComStructFree(data);
    }
}

void OPCItemNewDialog::treeItemExpanded(QTreeWidgetItem *item)
{
    delete item->takeChild(0);

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *child = nullptr;
    ItemBrowseList browseList;

    if (-1 != tree->indexOfTopLevelItem(item))
    {
        browseList = server->BrowseItemIDs(L"", OPC_BRANCH);
    }
    else
    {
        auto browse = item->data(0, Qt::UserRole).value<OPCITEMBROWSE *>();
        browseList = server->BrowseItemIDs(browse->szItemID, OPC_BRANCH);
    }

    for (auto browse : browseList)
    {
        child = new QTreeWidgetItem;
        child->setText(0, QString::fromWCharArray(browse->szItemDataID));
        child->setData(0, Qt::UserRole, QVariant::fromValue(browse));
        child->addChild(new QTreeWidgetItem);
        item->addChild(child);
    }

    allBrowseList.append(browseList);
}

void OPCItemNewDialog::treeItemCollapsed(QTreeWidgetItem *item)
{
    for (auto child : item->takeChildren())
        delete child;

    item->addChild(new QTreeWidgetItem);
}

void OPCItemNewDialog::refreshItemList()
{
    QTreeWidget *treeWidget = ui->treeWidget;
    QTreeWidgetItem *treeItem = treeWidget->currentItem();
    QListWidget *listWidget = ui->listWidget;
    QListWidgetItem *listItem = nullptr;
    bool flat = ui->checkBrowseFlat->isChecked();
    ItemBrowseList browseList;
    listWidget->clear();

    if (-1 != treeWidget->indexOfTopLevelItem(treeItem))
    {
        browseList = server->BrowseItemIDs(L"", flat ? OPC_FLAT : OPC_LEAF);
    }
    else
    {
        auto browse = treeItem->data(0, Qt::UserRole).value<OPCITEMBROWSE *>();
        browseList = server->BrowseItemIDs(browse->szItemID, flat ? OPC_FLAT : OPC_LEAF);
    }

    for (auto browse : browseList)
    {
        listItem = new QListWidgetItem;
        listItem->setText(QString::fromWCharArray(browse->szItemDataID));
        listItem->setData(Qt::UserRole, QVariant::fromValue(browse));
        listWidget->addItem(listItem);
    }

    allBrowseList.append(browseList);
}

void OPCItemNewDialog::addItem()
{
    OPCITEMATTRIBUTES attribute;

    attribute.szAccessPath = (LPWSTR)L"";
    attribute.szItemID = (LPWSTR)ui->editItemID->text().unicode();
    attribute.bActive = FALSE;
    attribute.hClient = 0L;
    attribute.dwBlobSize = 0L;
    attribute.pBlob = NULL;
    attribute.vtRequestedDataType = VT_EMPTY;

    if (!group->ValidateItems(1, &attribute))
    {
        QMessageBox::critical(this, "Error", "Validate the items failed!");
        return;
    }

    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;
    int row = table->rowCount();
    table->insertRow(row);
    table->selectRow(row);
    table->setRowHeight(row, 21);

    item = new QTableWidgetItem;
    item->setText(QString::fromWCharArray(attribute.szItemID));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    table->setItem(row, 0, item);

    item = new QTableWidgetItem;
    item->setText(VARTYPE_to_QString(attribute.vtCanonicalDataType));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    table->setItem(row, 1, item);

    item = new QTableWidgetItem;
    item->setText(RIGHTS_to_QString(attribute.dwAccessRights));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    table->setItem(row, 2, item);

    item = new QTableWidgetItem;
    item->setCheckState(attribute.bActive ? Qt::Checked : Qt::Unchecked);
    item->setText(attribute.bActive ? "Activated" : "Inactivated");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    table->setItem(row, 3, item);
}


void OPCItemNewDialog::addLeaves()
{
    QListWidget *list = ui->listWidget;
    QList<QListWidgetItem *> items = list->selectedItems();
    const int count = items.size();
    OPCITEMATTRIBUTES attributes[count];

    for (int i = 0; i < count; ++i)
    {
        auto data = items.at(i)->data(Qt::UserRole).value<OPCITEMBROWSE *>();

        attributes[i].szAccessPath = (LPWSTR)L"";
        attributes[i].szItemID = data->szItemID;
        attributes[i].bActive = FALSE;
        attributes[i].hClient = 0L;
        attributes[i].hServer = 0L;
        attributes[i].dwBlobSize = 0L;
        attributes[i].pBlob = NULL;
        attributes[i].vtRequestedDataType = VT_EMPTY;
    }

    if (!group->ValidateItems(count, attributes))
    {
        QMessageBox::critical(this, "Error", "Validate the items failed!");
        return;
    }

    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;

    for (int i = 0; i < count; ++i)
    {
        int row = table->rowCount();
        table->insertRow(row);
        table->selectRow(row);
        table->setRowHeight(row, 21);

        item = new QTableWidgetItem;
        item->setText(QString::fromWCharArray(attributes[i].szItemID));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 0, item);

        item = new QTableWidgetItem;
        item->setText(VARTYPE_to_QString(attributes[i].vtCanonicalDataType));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 1, item);

        item = new QTableWidgetItem;
        item->setText(RIGHTS_to_QString(attributes[i].dwAccessRights));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 2, item);

        item = new QTableWidgetItem;
        item->setCheckState(attributes[i].bActive ? Qt::Checked : Qt::Unchecked);
        item->setText(attributes[i].bActive ? "Activated" : "Inactivated");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 3, item);
    }
}

void OPCItemNewDialog::removeItem()
{
    QTableWidget *table = ui->tableWidget;
    table->removeRow(table->currentRow());
    table->selectRow(table->currentRow());
}

void OPCItemNewDialog::tableItemChanged(QTableWidgetItem *item)
{
    if (item->column() != 3) return;

    item->setText(item->checkState() == Qt::Checked ? "Activated" : "Inactivated");
}

void OPCItemNewDialog::tableItemClicked(QTableWidgetItem *item)
{
//    if (item->column() != 0) return;

    ui->editItemID->setText(item->text());
}

void OPCItemNewDialog::accept()
{
    QTableWidget *table = ui->tableWidget;
    const int count = table->rowCount();
    OPCITEMATTRIBUTES attributes[count];

    for (int row = 0; row < count; ++row)
    {
        QString itemID = table->item(row, 0)->text();
        bool active = table->item(row, 3)->checkState() == Qt::Checked;

        attributes[row].szAccessPath = (LPWSTR)L"";
        attributes[row].szItemID = (LPWSTR)itemID.unicode();
        attributes[row].bActive = active ? TRUE : FALSE;
        attributes[row].hClient = 0L;
        attributes[row].hServer = 0L;
        attributes[row].dwBlobSize = 0L;
        attributes[row].pBlob = NULL;
        attributes[row].vtRequestedDataType = VT_EMPTY;
    }

    group->AddItems(count, attributes);

    QDialog::accept();
}
