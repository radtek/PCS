#include "DialogDateCodeDesign.h"
#include "DialogDateCodeEdit.h"
#include "WorkManager.h"
#include "ui_DialogDateCodeDesign.h"

DialogDateCodeDesign::DialogDateCodeDesign(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDateCodeDesign)
{
    ui->setupUi(this);

    initialWidget();
    loadFormula();
}

DialogDateCodeDesign::~DialogDateCodeDesign()
{
    delete ui;
}

void DialogDateCodeDesign::initialWidget()
{
    do
    {
        connect(ui->buttonAddnew, SIGNAL(clicked()), this, SLOT(slotAddnew()));
        connect(ui->buttonModify, SIGNAL(clicked()), this, SLOT(slotModify()));
        connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(slotDelete()));
        connect(ui->buttonCopy, SIGNAL(clicked()), this, SLOT(slotCopy()));
        connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSave()));
        connect(ui->buttonExit, SIGNAL(clicked()), this, SLOT(accept()));
    } while (0);

    do
    {
        QTreeWidget *tree = ui->treeWidget;

        tree->setFont(qApp->font());
        tree->header()->setFont(qApp->font());
        tree->header()->setFixedHeight(25);
        tree->setHeaderLabel("配方列表");
        tree->setStyleSheet("QTreeWidget::item{height:21px}");

        connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(slotTreeSelectionChanged()));
    } while (0);

    do
    {
        dateHeaderMap.insert(DateHeader::YearText, "年份");
        dateHeaderMap.insert(DateHeader::YearCode, "日代码");
        dateHeaderMap.insert(DateHeader::MonthText, "月份");
        dateHeaderMap.insert(DateHeader::MonthCode, "月代码");
        dateHeaderMap.insert(DateHeader::DayText, "日期");
        dateHeaderMap.insert(DateHeader::DayCode, "日代码");

        QTableWidget *table = ui->tableWidget;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(dateHeaderMap.size());
        table->setHorizontalHeaderLabels(dateHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectItems);    //选取一行
        table->setEditTriggers(QTableWidget::AllEditTriggers);     //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setVisible(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(80);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotDateItemDoubleClicked()));
    } while (0);
}

void DialogDateCodeDesign::loadFormula()
{
    do
    {
        QDir dir(DATE_CODE_PATH);
        QFileInfoList fileList = dir.entryInfoList(QStringList("*.xml"), QDir::Files);

        QTreeWidget *tree = ui->treeWidget;
        QTreeWidgetItem *item = nullptr;

        for (auto file : fileList)
        {
            item = new QTreeWidgetItem;
            item->setText(0, file.completeBaseName());
            tree->addTopLevelItem(item);
        }
    } while (0);

    do
    {
        QTableWidget *table = ui->tableWidget;
        QTableWidgetItem *item = nullptr;
        table->setRowCount(31);

        for (int row = 0; row < 31; ++row)
        {
            item = new QTableWidgetItem(QString::number(row + 2011));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(DateHeader::YearText), item);

            item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(DateHeader::YearCode), item);

            item = new QTableWidgetItem(QString::number(row + 1));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(DateHeader::DayText), item);

            item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, static_cast<int>(DateHeader::DayCode), item);

            if (row < 12)
            {
                item = new QTableWidgetItem(QString::number(row + 1));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setTextAlignment(Qt::AlignCenter);
                table->setItem(row, static_cast<int>(DateHeader::MonthText), item);

                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignCenter);
                table->setItem(row, static_cast<int>(DateHeader::MonthCode), item);
            }
            else
            {
                item = new QTableWidgetItem;
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                table->setItem(row, static_cast<int>(DateHeader::MonthText), item);

                item = new QTableWidgetItem;
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                table->setItem(row, static_cast<int>(DateHeader::MonthCode), item);
            }
        }

    } while (0);
}

void DialogDateCodeDesign::clearUiData()
{
    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;

    for (int row = 0; row < table->rowCount(); ++row)
    {
        item = table->item(row, static_cast<int>(DateHeader::YearCode));
        item->setText(QString());
        item = table->item(row, static_cast<int>(DateHeader::MonthCode));
        item->setText(QString());
        item = table->item(row, static_cast<int>(DateHeader::DayCode));
        item->setText(QString());
    }
}

void DialogDateCodeDesign::slotTreeSelectionChanged()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
    {
        clearUiData();
        return;
    }

    QString filename = QString("%1/%2.xml")
                           .arg(DATE_CODE_PATH)
                           .arg(item->text(0));

    DateCodeItems dateItems;

    if (!DateCode_ReadXmlFile(filename, dateItems))
    {
        clearUiData();
        return;
    }

    do
    {
        QTableWidget *table = ui->tableWidget;
        QTableWidgetItem *item = nullptr;

        for (int row = 0; row < dateItems.yearMap.size(); ++row)
        {
            item = table->item(row, static_cast<int>(DateHeader::YearCode));
            item->setText(dateItems.yearMap.value(row + 2011));
        }

        for (int row = 0; row < dateItems.monthMap.size(); ++row)
        {
            item = table->item(row, static_cast<int>(DateHeader::MonthCode));
            item->setText(dateItems.monthMap.value(row + 1));
        }

        for (int row = 0; row < dateItems.dayMap.size(); ++row)
        {
            item = table->item(row, static_cast<int>(DateHeader::DayCode));
            item->setText(dateItems.dayMap.value(row + 1));
        }
    } while (0);
}

void DialogDateCodeDesign::slotDateItemDoubleClicked()
{
}

void DialogDateCodeDesign::slotAddnew()
{
    DialogDateCodeEdit dialog(this);
    dialog.setWindowTitle(tr("新增配方"));

    if (QDialog::Accepted != dialog.exec())
        return;

    QString newFormula = dialog.getDateFormula();

    if (newFormula.isEmpty())
    {
        MessageBox_Warning("配方名称不能为空！");
        return;
    }

    QString newFile = QString("%1/%2.xml")
                          .arg(DATE_CODE_PATH)
                          .arg(newFormula);

    if (QFile::exists(newFile))
    {
        MessageBox_Warning("配方名称已经存在！");
        return;
    }

    QFile file(newFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        MessageBox_Critical("配方添加失败！");
        return;
    }
    file.close();

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, dialog.getDateFormula());
    tree->addTopLevelItem(item);
    tree->setCurrentItem(item);
}

void DialogDateCodeDesign::slotCopy()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    DialogDateCodeEdit dialog(this);
    dialog.setWindowTitle(tr("复制配方"));

    QString oldFormula = item->text(0);

    if (QDialog::Accepted != dialog.exec())
        return;

    QString newFormula = dialog.getDateFormula();

    if (newFormula.isEmpty())
    {
        MessageBox_Warning("配方名称不能为空！");
        return;
    }

    QString oldFile = QString("%1/%2.xml")
                          .arg(DATE_CODE_PATH)
                          .arg(oldFormula);
    QString newFile = QString("%1/%2.xml")
                          .arg(DATE_CODE_PATH)
                          .arg(newFormula);

    if (!QFile::copy(oldFile, newFile))
    {
        MessageBox_Critical("配方复制失败！");
        return;
    }

    item = new QTreeWidgetItem;
    item->setText(0, newFormula);
    tree->addTopLevelItem(item);

    tree->setCurrentItem(item);
}

void DialogDateCodeDesign::slotModify()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    DialogDateCodeEdit dialog(this);
    dialog.setWindowTitle(tr("修改配方"));

    QString oldFormula = item->text(0);
    dialog.setDateFormula(oldFormula);

    if (QDialog::Accepted != dialog.exec())
        return;

    QString newFormula = dialog.getDateFormula();

    if (newFormula.isEmpty())
    {
        MessageBox_Warning("配方名称不能为空！");
        return;
    }

    QString oldFile = QString("%1/%2.xml")
                          .arg(DATE_CODE_PATH)
                          .arg(oldFormula);
    QString newFile = QString("%1/%2.xml")
                          .arg(DATE_CODE_PATH)
                          .arg(newFormula);

    if (!QFile::rename(oldFile, newFile))
    {
        MessageBox_Critical("配方修改失败！");
        return;
    }

    item->setText(0, dialog.getDateFormula());
}

void DialogDateCodeDesign::slotDelete()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    QString filename = QString("%1/%2.xml")
                           .arg(DATE_CODE_PATH)
                           .arg(item->text(0));

    if (QFile::remove(filename))
    {
        int index = tree->indexOfTopLevelItem(item);
        delete tree->takeTopLevelItem(index);

        item = tree->currentItem();

        if (item == nullptr)
            clearUiData();
        else
            tree->setCurrentItem(item);

        MessageBox_Information("配方删除成功。");
    }
    else
    {
        MessageBox_Critical("配方删除失败！");
    }
}

void DialogDateCodeDesign::slotSave()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr)
        return;

    DateCodeItems dateItems;

    do
    {
        QTableWidget *table = ui->tableWidget;
        QTableWidgetItem *item = nullptr;

        for (int row = 0; row < 31; ++row)
        {
            item = table->item(row, static_cast<int>(DateHeader::YearText));
            int key = item->text().toInt();
            item = table->item(row, static_cast<int>(DateHeader::YearCode));
            QString value = item->text();

            dateItems.yearMap.insert(key, value);
        }

        for (int row = 0; row < 12; ++row)
        {
            item = table->item(row, static_cast<int>(DateHeader::MonthText));
            int key = item->text().toInt();
            item = table->item(row, static_cast<int>(DateHeader::MonthCode));
            QString value = item->text();

            dateItems.monthMap.insert(key, value);
        }

        for (int row = 0; row < 31; ++row)
        {
            item = table->item(row, static_cast<int>(DateHeader::DayText));
            int key = item->text().toInt();
            item = table->item(row, static_cast<int>(DateHeader::DayCode));
            QString value = item->text();

            dateItems.dayMap.insert(key, value);
        }
    } while (0);

    QString filename = QString("%1/%2.xml")
                           .arg(DATE_CODE_PATH)
                           .arg(item->text(0));

    if (DateCode_SaveXmlFile(filename, dateItems))
    {
        MessageBox_Information("配方保存成功。");
    }
    else
    {
        MessageBox_Critical("配方保存失败！");
    }
}
