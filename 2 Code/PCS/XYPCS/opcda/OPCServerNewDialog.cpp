#include "OPCServerNewDialog.h"
#include "ui_OPCServerNewDialog.h"
#include "OPCManager.h"
#include "OPCServerList.h"
#include <QMessageBox>

OPCServerNewDialog::OPCServerNewDialog(XYOPCManager *common, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OPCServerNewDialog)
    , manager(common)
    , server(nullptr)
{
    ui->setupUi(this);

    initialWidget();
    browseLocalServer();
}

OPCServerNewDialog::~OPCServerNewDialog()
{
    deleteAllInfo();

    delete ui;
}

void OPCServerNewDialog::initialWidget()
{
    this->setWindowTitle("New Server");
    this->setMinimumHeight(360);
    this->setMinimumHeight(480);

    ui->editCLSID->setReadOnly(true);
    ui->editProgID->setReadOnly(true);
    ui->editUserType->setReadOnly(true);
    ui->editMachine->setReadOnly(true);

    connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->buttonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = nullptr;

    tree->setSortingEnabled(true);
    tree->sortByColumn(0, Qt::AscendingOrder);
    tree->setColumnCount(1);
    tree->setHeaderLabel(tr("OPC Objects"));

    item = new QTreeWidgetItem();
    item->setText(0, "Local Machine");
    tree->addTopLevelItem(item);

    item = new QTreeWidgetItem();
    item->setText(0, "Remote Machine");
    tree->addTopLevelItem(item);

    connect(tree, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(treeItemPressed(QTreeWidgetItem*,int)));
}

void OPCServerNewDialog::browseLocalServer()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->topLevelItem(0);

    XYOPCServerList *pServerList = manager->CreateServerList(L"127.0.0.1");

    if (pServerList == Q_NULLPTR) return;

    ServerInfoList list = pServerList->BrowseServerList();

    QList<QTreeWidgetItem *> items;
    for (auto info : list)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, QString::fromWCharArray(info->szProgID));
        item->setData(0, Qt::UserRole, QVariant::fromValue(info));
        items.append(item);
    }

    infoList.append(list);
    item->addChildren(items);

    manager->ReleaseServerList(pServerList);
}

void OPCServerNewDialog::deleteAllInfo()
{
    for (auto info : infoList)
    {
        ComStringFree(info->szProgID);
        ComStringFree(info->szUserType);
        ComStringFree(info->szMachineName);
        ComStructFree(info);
    }
}

void OPCServerNewDialog::treeItemPressed(QTreeWidgetItem *item, int column)
{
    QTreeWidget *tree = ui->treeWidget;

    if (-1 != tree->indexOfTopLevelItem(item))
    {
        ui->editCLSID->clear();
        ui->editProgID->clear();
        ui->editUserType->clear();
        ui->editMachine->clear();
        return;
    }

    auto info = item->data(column,Qt::UserRole).value<OPCSERVERINFO *>();

    ui->editCLSID->setText(QUuid(info->clsid).toString());
    ui->editProgID->setText(QString::fromWCharArray(info->szProgID));
    ui->editUserType->setText(QString::fromWCharArray(info->szUserType));
    ui->editMachine->setText(QString::fromWCharArray(info->szMachineName));
}

void OPCServerNewDialog::accept()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();
    if (-1 != tree->indexOfTopLevelItem(item)) return;

    auto info = item->data(0,Qt::UserRole).value<OPCSERVERINFO *>();

    server = manager->CreateServer(info);

    if (server == nullptr)
    {
        QMessageBox::critical(this, "Error", "Add new Connection failed!");
        return;
    }

    QDialog::accept();
}
