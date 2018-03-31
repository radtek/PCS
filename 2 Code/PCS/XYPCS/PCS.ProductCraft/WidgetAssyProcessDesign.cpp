#include "WidgetAssyProcessDesign.h"
#include "ui_WidgetAssyProcessDesign.h"
#include "WidgetAssyProcessProduce.h"
#include "WidgetAssyProcessStation.h"
#include "WidgetAssyProcessPackage.h"

WidgetAssyProcessDesign::WidgetAssyProcessDesign(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAssyProcessDesign)
{
    ui->setupUi(this);

    initialWidget();
    initialRecord();
}

WidgetAssyProcessDesign::~WidgetAssyProcessDesign()
{
    delete ui;
}


void WidgetAssyProcessDesign::initialWidget()
{
    do
    {
        QTreeWidget *tree = ui->treeWidget;

        tree->setFont(qApp->font());
        tree->header()->setFont(qApp->font());
        tree->header()->setFixedHeight(25);
        tree->setHeaderLabel("工艺列表");
        tree->setStyleSheet("QTreeWidget::item{height:21px}");

        connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(slotTreeSelectionChanged()));
    }
    while (0);

    do
    {
        QStackedWidget *stacked = ui->stackedWidget;

        stacked->addWidget(new WidgetAssyProcessProduce);
        stacked->addWidget(new WidgetAssyProcessStation);
        stacked->addWidget(new WidgetAssyProcessPackage);

        stacked->setCurrentIndex(0);
    }
    while (0);

}

void WidgetAssyProcessDesign::initialRecord()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = nullptr;

    do
    {
        for (auto craftID : qWorkManager->getCraftIDList())
        {
            item = new QTreeWidgetItem;
            item->setText(0, craftID);
            item->setIcon(0, QIcon(":/images/pngs/tree_craft.png"));
            tree->addTopLevelItem(item);
        }
    }
    while (0);

    for (int i = 0; i < tree->topLevelItemCount(); ++i)
    {
        for (auto stationID : qWorkManager->getStationIDList())
        {
            item = new QTreeWidgetItem;
            item->setText(0, stationID);
            item->setIcon(0, QIcon(":/images/pngs/tree_station.png"));
            tree->topLevelItem(i)->addChild(item);
        }
    }
}

void WidgetAssyProcessDesign::slotTreeSelectionChanged()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();
    QStackedWidget *stacked = ui->stackedWidget;

    if (item->parent() == nullptr)
    {
        QString craftID = item->text(0);

        auto widget = dynamic_cast<WidgetAssyProcessProduce *>(stacked->widget(0));
        widget->loadParameter(craftID);
        stacked->setCurrentIndex(0);
    }
    else
    {
        QString craftID = item->parent()->text(0);
        QString stationID = item->text(0);
        QString stationType = qWorkManager->getStationType(stationID);

        switch (stationTypeMap.key(stationType))
        {
        case StationType::Station:
        {
            auto widget = dynamic_cast<WidgetAssyProcessStation *>(stacked->widget(1));
            widget->loadParameter(craftID, stationID);
            stacked->setCurrentIndex(1);
            break;
        }
        case StationType::Package:
        {
            auto widget = dynamic_cast<WidgetAssyProcessPackage *>(stacked->widget(2));
            widget->loadParameter(craftID, stationID);
            stacked->setCurrentIndex(2);
            break;
        }
        }
    }
}
