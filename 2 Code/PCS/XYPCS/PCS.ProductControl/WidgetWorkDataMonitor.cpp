#include "WidgetWorkDataMonitor.h"
#include "WorkManager.h"
#include "ui_WidgetWorkDataMonitor.h"

WidgetWorkDataMonitor::WidgetWorkDataMonitor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetWorkDataMonitor)
    , assemblyModel(new QSqlQueryModel)
    , packageModel(new QSqlQueryModel)
    , stationModel(new QSqlQueryModel)
    , MaterialModel(new QSqlQueryModel)
    , MeasureModel(new QSqlQueryModel)
    , TransferModel(new QSqlQueryModel)
{
    ui->setupUi(this);

    initialWidget();
}

WidgetWorkDataMonitor::~WidgetWorkDataMonitor()
{
    delete assemblyModel;
    delete packageModel;
    delete stationModel;
    delete MaterialModel;
    delete MeasureModel;
    delete TransferModel;
    delete ui;
}

void WidgetWorkDataMonitor::initialWidget()
{
    do
    {
        ui->toolAssembly->setChecked(true);
        ui->radioCurrentOrder->setChecked(true);
        ui->stackedWidget->setCurrentIndex(0);

        QButtonGroup *buttonGroup = new QButtonGroup(this);

        buttonGroup->addButton(ui->toolAssembly, 0);
        buttonGroup->addButton(ui->toolPackage, 1);
        buttonGroup->addButton(ui->toolStation, 2);
        buttonGroup->addButton(ui->toolMaterial, 3);
        buttonGroup->addButton(ui->toolMeasure, 4);
        buttonGroup->addButton(ui->toolTransfer, 5);

        connect(buttonGroup, SIGNAL(buttonClicked(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

        connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(slotRefreshView()));
    } while (0);

    do
    {
        QTableView *table = ui->tableAssembly;
        table->setModel(assemblyModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        QTableView *table = ui->tablePackage;
        table->setModel(packageModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        QTableView *table = ui->tableStation;
        table->setModel(stationModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        QTableView *table = ui->tableMaterial;
        table->setModel(MaterialModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        QTableView *table = ui->tableMeasure;
        table->setModel(MeasureModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        QTableView *table = ui->tableTransfer;
        table->setModel(TransferModel);

        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    } while (0);

    do
    {
        assemblySQL = R"(SELECT [UID] AS [序号]
                      ,[OrderID] AS [工单编号]
                      ,[ProductID] AS [产品编号]
                      ,[OperatorID] AS [操作员编号]
                      ,[AssemblyBarcode] AS [总成条码]
                      ,CASE [AssemblyState] WHEN 0 THEN '合格' WHEN 1 THEN '不合格' END AS [组装状态]
                      ,CASE [RepairState] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [抽检状态]
                      ,CASE [InspectionMarker] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [返修状态]
                      ,CASE [RetestState] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [复检状态]
                      ,CONVERT(NVARCHAR, [CreateTime], 20) AS [开始时间]
                      ,CONVERT(NVARCHAR, [FinishTime], 20) AS [完成时间]
                      FROM [PCS_Data_Assembly])";

        packageSQL = R"(SELECT [UID] AS [序号]
                     ,[OrderID] AS [工单编号]
                     ,[ProductID] AS [产品编号]
                     ,[OperatorID] AS [操作员编号]
                     ,CAST([PackageBarcode] AS NVARCHAR) AS [包装箱条码]
                     ,CAST([PackageWeight] AS NVARCHAR) AS [装箱重量]
                     ,CAST([PackageQuantity] AS NVARCHAR) AS [装箱数量]
                     ,CASE [PackageState] WHEN 0 THEN '满箱' WHEN 1 THEN '不足' WHEN 2 THEN '过量' END AS [装箱状态]
                     ,CONVERT(NVARCHAR, [CreateTime], 20) AS [开始时间]
                     ,CONVERT(NVARCHAR, [FinishTime], 20) AS [完成时间]
                     FROM [PCS_Data_Package])";

        stationSQL = R"(SELECT [UID] AS [序号]
                     ,[OrderID] AS [工单编号]
                     ,[StationID] AS [工位编号]
                     ,[OperatorID] AS [操作员编号]
                     ,CASE [AssemblyState] WHEN 0 THEN '合格' WHEN 1 THEN '不合格' END AS [组装状态]
                     ,CASE [RepairState] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [返修状态]
                     ,CASE [RetestState] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [复检状态]
                     ,CONVERT(NVARCHAR, [CreateTime], 20) AS [开始时间]
                     ,CONVERT(NVARCHAR, [FinishTime], 20) AS [完成时间]
                     FROM [PCS_Data_Station])";

        materialSQL = R"(SELECT [UID] AS [序号]
                      ,[OrderID] AS [工单编号]
                      ,[StationID] AS [工位编号]
                      ,[MaterialID] AS [物料编号]
                      ,[MaterialBarcode] AS [物料条码]
                      ,[MaterialBatch] AS [物料批次]
                      ,CAST([RepairCount] AS NVARCHAR) AS [返修次数]
                      FROM [PCS_Data_Station_Material])";

        measureSQL = R"(SELECT [UID] AS [序号]
                     ,[OrderID] AS [工单编号]
                     ,[StationID] AS [工位编号]
                     ,[MeasureType] + '#' + CAST([MeasureOrder] AS NVARCHAR) AS [测试编号]
                     ,CAST([MeasureValue] AS NVARCHAR) AS [测试数值]
                     ,CASE [MeasureState] WHEN 0 THEN '合格' WHEN 1 THEN '不合格' END AS [测试结果]
                     ,CAST([RetestCount] AS NVARCHAR) AS [复检次数]
                     FROM [PCS_Data_Station_Measure])";

        transferSQL = R"(SELECT [UID] AS [序号]
                      ,[OrderID] AS [工单编号]
                      ,[StationID] AS [工位编号]
                      ,[TransferBarcode] AS [过渡条码]
                      ,CASE [AssemblyState] WHEN 0 THEN '合格' WHEN 1 THEN '不合格' END AS [组装状态]
                      ,CASE [RepairState] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [返修状态]
                      ,CASE [RetestState] WHEN 0 THEN 'N' WHEN 1 THEN 'Y' END AS [复检状态]
                      ,CONVERT(NVARCHAR, [CreateTime], 20) AS [开始时间]
                      ,CONVERT(NVARCHAR, [FinishTime], 20) AS [完成时间]
                      FROM [PCS_Data_Transfer])";
    } while (0);
}

void WidgetWorkDataMonitor::slotRefreshView()
{
    if (ui->radioCurrentOrder->isChecked())
        queryByCurrentOrder();
    else if (ui->radioLocalDataBase->isChecked())
        queryByLocalDatabase();
    else if (ui->radioAssemblyBarcode->isChecked())
        queryByAssemblyBarcode();
    else if (ui->radioPackageBarcode->isChecked())
        queryByPackageBarcode();
    else if (ui->radioMaterialBarcode->isChecked())
        queryByMaterialBarcode();
    else if (ui->radioMaterialBatch->isChecked())
        queryByMaterialBatch();
    else
        queryByCurrentOrder();
}

void WidgetWorkDataMonitor::queryByLocalDatabase()
{
    assemblyModel->setQuery(assemblySQL, LOCAL_DB);
    packageModel->setQuery(packageSQL, LOCAL_DB);
    stationModel->setQuery(stationSQL, LOCAL_DB);
    MaterialModel->setQuery(materialSQL, LOCAL_DB);
    MeasureModel->setQuery(measureSQL, LOCAL_DB);
    TransferModel->setQuery(transferSQL, LOCAL_DB);
}

void WidgetWorkDataMonitor::queryByCurrentOrder()
{
    QString orderID = qWorkManager->getOrderID();
    QString filter = QString("WHERE [OrderID] = '%1'").arg(orderID);

    assemblyModel->setQuery(assemblySQL + filter, LOCAL_DB);
    packageModel->setQuery(packageSQL + filter, LOCAL_DB);
    stationModel->setQuery(stationSQL + filter, LOCAL_DB);
    MaterialModel->setQuery(materialSQL + filter, LOCAL_DB);
    MeasureModel->setQuery(measureSQL + filter, LOCAL_DB);
    TransferModel->setQuery(transferSQL + filter, LOCAL_DB);
}

void WidgetWorkDataMonitor::queryByAssemblyBarcode()
{
    QString barcode = ui->editAssemblyBarcode->text();
    QString filter = QString("WHERE [AssemblyBarcode] = '%1'").arg(barcode);
    assemblyModel->setQuery(assemblySQL + filter, LOCAL_DB);

    filter = QString("WHERE [PackageID] IN "
                     "(SELECT [PackageID] FROM [PCS_Data_Package_Assembly] "
                     "WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Assembly]"
                     "WHERE [AssemblyBarcode] = '%1'))")
                 .arg(barcode);
    packageModel->setQuery(packageSQL + filter, LOCAL_DB);

    filter = QString("WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Assembly] "
                     "WHERE [AssemblyBarcode] = '%1')")
                 .arg(barcode);
    stationModel->setQuery(stationSQL + filter, LOCAL_DB);
    MaterialModel->setQuery(materialSQL + filter, LOCAL_DB);
    MeasureModel->setQuery(measureSQL + filter, LOCAL_DB);
    TransferModel->setQuery(transferSQL + filter, LOCAL_DB);
}

void WidgetWorkDataMonitor::queryByPackageBarcode()
{
    QString barcode = ui->editPackageBarcode->text();
    QString filter = QString("WHERE [PackageBarcode] = '%1'").arg(barcode);
    packageModel->setQuery(packageSQL + filter, LOCAL_DB);

    filter = QString("WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Package_Assembly] "
                     "WHERE [PackageID] IN "
                     "(SELECT [PackageID] FROM [PCS_Data_Package]"
                     "WHERE [PackageBarcode] = '%1'))")
                 .arg(barcode);
    assemblyModel->setQuery(assemblySQL + filter, LOCAL_DB);

    stationModel->setQuery(stationSQL + filter, LOCAL_DB);
    MaterialModel->setQuery(materialSQL + filter, LOCAL_DB);
    MeasureModel->setQuery(measureSQL + filter, LOCAL_DB);
    TransferModel->setQuery(transferSQL + filter, LOCAL_DB);
}

void WidgetWorkDataMonitor::queryByMaterialBarcode()
{
    QString barcode = ui->editMaterialBarcode->text();
    QString filter = QString("WHERE [MaterialBarcode] = '%1'").arg(barcode);
    MaterialModel->setQuery(materialSQL + filter, LOCAL_DB);

    filter = QString("WHERE [PackageID] IN "
                     "(SELECT [PackageID] FROM [PCS_Data_Package_Assembly] "
                     "WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Station_Material]"
                     "WHERE [MaterialBarcode] = '%1'))")
                 .arg(barcode);
    packageModel->setQuery(packageSQL + filter, LOCAL_DB);

    filter = QString("WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Station_Material] "
                     "WHERE [MaterialBarcode] = '%1')")
                 .arg(barcode);
    assemblyModel->setQuery(assemblySQL + filter, LOCAL_DB);
    stationModel->setQuery(stationSQL + filter, LOCAL_DB);
    MeasureModel->setQuery(measureSQL + filter, LOCAL_DB);
    TransferModel->setQuery(transferSQL + filter, LOCAL_DB);
}

void WidgetWorkDataMonitor::queryByMaterialBatch()
{
    QString batch = ui->editMaterialBatch->text();
    QString filter = QString("WHERE [MaterialBatch] = '%1'").arg(batch);
    MaterialModel->setQuery(materialSQL + filter, LOCAL_DB);

    filter = QString("WHERE [PackageID] IN "
                     "(SELECT [PackageID] FROM [PCS_Data_Package_Assembly] "
                     "WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Station_Material]"
                     "WHERE [MaterialBatch] = '%1'))")
                 .arg(batch);
    packageModel->setQuery(packageSQL + filter, LOCAL_DB);

    filter = QString("WHERE [AssemblyID] IN "
                     "(SELECT [AssemblyID] FROM [PCS_Data_Station_Material] "
                     "WHERE [MaterialBatch] = '%1')")
                 .arg(batch);
    assemblyModel->setQuery(assemblySQL + filter, LOCAL_DB);
    stationModel->setQuery(stationSQL + filter, LOCAL_DB);
    MeasureModel->setQuery(measureSQL + filter, LOCAL_DB);
    TransferModel->setQuery(transferSQL + filter, LOCAL_DB);
}
