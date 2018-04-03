#include "DialogSelectStation.h"
#include "WorkManager.h"
#include "ui_DialogSelectStation.h"

#include "XYSqlQueryModel.h"
#include <QMessageBox>
#include <QtSql>
const QMap<QVariant, QString> Station_Type =
    {
        {Assembly, "装配"},
        {Package, "包装"},
};
DialogSelectStation::DialogSelectStation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSelectStation)
    , model(new XYSqlQueryModel)
{
    ui->setupUi(this);
    this->init();

    QObject::connect(ui->buttonAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
    QObject::connect(ui->buttonReject, SIGNAL(clicked()), this, SLOT(reject()));
}

DialogSelectStation::~DialogSelectStation()
{
    delete ui;
}

void DialogSelectStation::init()
{
    headlist << tr("工位编号") << tr("工位名称") << tr("工位代号") << tr("工位类型") << tr("备注");
    QTableView *view = ui->tableView;
    QHeaderView *header = view->horizontalHeader();
    header->setFixedHeight(30);
    header->setHighlightSections(false);
    view->setSelectionMode(QTableView::SingleSelection);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->horizontalHeader()->setMinimumSectionSize(120);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QSqlQuery query(LOCAL_DB);
    query.exec(R"(SELECT [WorkStationID]
                 ,[WorkStationName]
                 ,[WorkStationCode]
                 ,[IsPackage]
                 ,[Description]
             FROM [PCS_Base_Station] WHERE [State] != 2 )");    //////

    model->setQuery(query);

    //   ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //   ui->tableView->setModel(model);
    for (int col = 0; col < model->columnCount(); col++)
        model->setHeaderData(col, Qt::Horizontal, headlist.at(col));

    ui->tableView->setModel(model);

    model->setColumnEnum("IsPackage", Station_Type);
}

void DialogSelectStation::slotAccept()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "请选择一行");
        return;
    }

    stationID = model->record(index.row()).value("WorkStationID").toString();

    // check
    QSqlQuery query(LOCAL_DB);
    query.prepare(R"(SELECT [UID] FROM [PCS_Craft_Station] WHERE [CraftID] = :CraftID AND [WorkStationID] = :WorkStationID )");    //////
    query.bindValue(":WorkStationID", stationID);
    query.bindValue(":CraftID", craftID);
    if (!query.exec())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "数据库错误,请重试");
        return;
    }
    if (query.first())
    {
        QMessageBox::warning(Q_NULLPTR, "警告", "已存在工位绑定");
        return;
    }

    accept();
}
