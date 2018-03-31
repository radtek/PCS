#include "MainWindow.h"
#include "CommManager.h"
#include "CommWidget.h"
#include "DialogPowerLogin.h"
#include "OPCWidget.h"
#include "WidgetAssemblyControl.h"
#include "WidgetAssyProcessDesign.h"
#include "WidgetBaseFixture.h"
#include "WidgetBaseMaterial.h"
#include "WidgetBaseMeasure.h"
#include "WidgetBaseStation.h"
#include "WidgetBaseWorker.h"
#include "WidgetCommDataMonitor.h"
#include "WidgetCraftEdit.h"
#include "WidgetCraftRouteDesign.h"
#include "WidgetLabelPrintDesign.h"
#include "WidgetOrderPlanControl.h"
#include "WidgetOrderPlanHistory.h"
#include "WidgetOrderPlanManage.h"
#include "WidgetWorkDataMonitor.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialManager();    //先加载管理器
    initialWindow();     //再加载界面
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{

    QMainWindow::showEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (qWorkManager->isWorkActivated())
    {
        MessageBox_Warning("关闭程序前，请先暂停或关闭工单！");
        event->ignore();
        return;
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::initialManager()
{
    qCommManager->readXmlFile(COMM_CONFIG_XML);

    qOPCManager->ReadXmlFile(OPC_CONFIG_XML);

    qWorkManager->loadSqlData(qCfgManager->getWorkShop(), qCfgManager->getWorkLine());

    connect(qOPCManager, SIGNAL(opcValueChanged(QString, DataDefine)), qWorkManager, SLOT(opcValueChanged(QString, DataDefine)));

    connect(qOPCManager, SIGNAL(opcReadComplete(QString, DataDefine)), qWorkManager, SLOT(opcReadComplete(QString, DataDefine)));

    connect(qWorkManager, SIGNAL(opcWriteValue(QString, DataDefine)), qOPCManager, SLOT(opcWriteValue(QString, DataDefine)));

    connect(qWorkManager, SIGNAL(opcReadValue(QString, DataDefine)), qOPCManager, SLOT(opcReadValue(QString, DataDefine)));

    connect(qCommManager, SIGNAL(commDataReceived(QString, DataDefine)), qWorkManager, SLOT(commDataReceived(QString, DataDefine)));

    connect(qWorkManager, SIGNAL(commWriteData(QString, DataDefine)), qCommManager, SLOT(commWriteData(QString, DataDefine)));

    qOPCManager->enableCallBack();

    qCommManager->startCommThread();
}

void MainWindow::initialWindow()
{
    this->setWindowTitle("PCS");
    this->setMinimumWidth(1000);
    this->setMinimumHeight(800);

    ui->navbarWidget->readData("./NavigationBar.xml");

    connect(ui->navbarWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(navbarClicked(QModelIndex)));
    connect(ui->tabbedWidget, SIGNAL(removeWidget(QWidget *)), this, SLOT(windowRemoved(QWidget *)));

    ui->headerWidget->setWorkshopName(qWorkManager->getWorkshopName());
    ui->headerWidget->setWorklineName(qWorkManager->getWorklineName());
}

void MainWindow::navbarClicked(const QModelIndex &index)
{
    auto node = index.data(Qt::UserRole).value<XYNavModel::TreeNode *>();

    if (widgetMap.contains(node->info))
    {
        ui->tabbedWidget->activeWidget(widgetMap.value(node->info));
    }
    else
    {
        QWidget *widget = createSubWindow(node->info);

        if (widget != nullptr)
        {
            widgetMap.insert(node->info, widget);
            ui->tabbedWidget->appendWidget(widget, node->label, node->image);
        }
    }
}

QWidget *MainWindow::createSubWindow(const QString &info)
{
    if (info == "OrderPlanManage")
    {
        auto widget = new WidgetOrderPlanManage(this);

        return widget;
    }

    if (info == "OrderPlanHistory")
    {
        auto widget = new WidgetOrderPlanHistory(this);

        return widget;
    }

    if (info == "CraftFormula")
    {
        auto widget = new WidgetCraftRouteDesign(this);

        return widget;
    }

    if (info == "AssemblyFlow")
    {
        auto widget = new WidgetAssyProcessDesign(this);

        return widget;
    }

    if (info == "LabelPrinting")
    {
        auto widget = new WidgetLabelPrintDesign(this);

        return widget;
    }

    if (info == "OPC_Config")
    {
        auto widget = new OPCWidget(this);

        return widget;
    }

    if (info == "Comm_Config")
    {
        auto widget = new CommWidget(this);

        return widget;
    }

    if (info == "OrderPlanControl")
    {
        auto *widget = new WidgetOrderPlanControl(this);

        connect(qWorkManager, SIGNAL(signalOrderActivated()), widget, SLOT(slotOrderActivated()), Qt::QueuedConnection);
        connect(qWorkManager, SIGNAL(signalOrderSuspended()), widget, SLOT(slotOrderSuspended()), Qt::QueuedConnection);

        connect(qWorkManager, SIGNAL(signalOrderPlanUpdate(OrderData)), widget, SLOT(slotOrderPlanUpdate(OrderData)));
        connect(qWorkManager, SIGNAL(signalStationUpdate(StationData)), widget, SLOT(slotStationUpdate(StationData)));

        connect(qWorkManager, SIGNAL(signalMaterialInput(QString, MaterialData)), widget, SLOT(slotMaterialInput(QString, MaterialData)));
        connect(qWorkManager, SIGNAL(signalMaterialReturn(QString, MaterialData)), widget, SLOT(slotMaterialReturn(QString, MaterialData)));
        connect(qWorkManager, SIGNAL(signalOperatorLogin(QString, OperatorData)), widget, SLOT(slotOperatorLogin(QString, OperatorData)));
        connect(qWorkManager, SIGNAL(signalOperatorLogout(QString, OperatorData)), widget, SLOT(slotOperatorLogout(QString, OperatorData)));
        connect(qWorkManager, SIGNAL(signalMaterialUpdate(QString, MaterialData)), widget, SLOT(slotMaterialUpdate(QString, MaterialData)));
        connect(qWorkManager, SIGNAL(signalFixtureUpdate(QString, FixtureData)), widget, SLOT(slotFixtureUpdate(QString, FixtureData)));

        return widget;
    }

    if (info == "AssemblyControl")
    {
        auto *widget = new WidgetAssemblyControl(this);

        connect(qWorkManager, SIGNAL(signalOrderActivated()), widget, SLOT(slotOrderActivated()), Qt::QueuedConnection);
        connect(qWorkManager, SIGNAL(signalOrderSuspended()), widget, SLOT(slotOrderSuspended()), Qt::QueuedConnection);

        connect(qWorkManager, SIGNAL(signalAssemblyFinish(AssemblyData)), widget, SLOT(slotAssemblyFinish(AssemblyData)));
        connect(qWorkManager, SIGNAL(signalPackageFinish(PackageData)), widget, SLOT(slotPackageFinish(PackageData)));
        connect(qWorkManager, SIGNAL(signalPackageUpdate(PackageData)), widget, SLOT(slotPackageUpdate(PackageData)));

        return widget;
    }

    if (info == "WorkDataMonitor")
    {
        auto *widget = new WidgetWorkDataMonitor(this);

        return widget;
    }

    if (info == "CommDataMonitor")
    {
        auto *widget = new WidgetCommDataMonitor(this);

        connect(qOPCManager, SIGNAL(opcValueChanged(QString, DataDefine)), widget, SLOT(slotOpcDataReceived(QString, DataDefine)));
        connect(qOPCManager, SIGNAL(opcReadComplete(QString, DataDefine)), widget, SLOT(slotOpcDataReceived(QString, DataDefine)));
        connect(widget, SIGNAL(signalOpcReadData(QString, DataDefine)), qOPCManager, SLOT(opcReadValue(QString, DataDefine)));
        connect(qCommManager, SIGNAL(commDataReceived(QString, DataDefine)), widget, SLOT(slotDacDataReceived(QString, DataDefine)));
        connect(widget, SIGNAL(signalDacReadData(QString, DataDefine)), qCommManager, SLOT(commWriteData(QString, DataDefine)));
        connect(qWorkManager, SIGNAL(opcWriteValue(QString, DataDefine)), widget, SLOT(slotOpcDataReceived(QString, DataDefine)));

        return widget;
    }
    if (info == "WorkerInfo")
    {
        QWidget *widget = new WidgetBaseWorker(this);

        return widget;
    }
    if (info == "WorkStationInfo")
    {
        QWidget *widget = new WidgetBaseStation(this);

        return widget;
    }
    if (info == "MaterialInfo")
    {
        QWidget *widget = new WidgetBaseMaterial(this);

        return widget;
    }
    if (info == "MeasureInfo")
    {
        QWidget *widget = new WidgetBaseMeasure(this);

        return widget;
    }
    if (info == "FixtureInfo")
    {
        QWidget *widget = new WidgetBaseFixture(this);

        return widget;
    }
    if (info == "CraftInfo")
    {
        QWidget *widget = new WidgetCraftEdit(this);

        return widget;
    }

    return Q_NULLPTR;
}

void MainWindow::windowRemoved(QWidget *widget)
{
    for (auto i = widgetMap.begin(); i != widgetMap.end(); ++i)
    {
        if (i.value() == widget)
        {
            widget->close();
            delete widget;
            widgetMap.remove(i.key());
            return;
        }
    }
}
