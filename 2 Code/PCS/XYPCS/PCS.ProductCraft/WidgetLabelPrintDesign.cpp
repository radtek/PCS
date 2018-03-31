#include "WidgetLabelPrintDesign.h"
#include "DialogBarcodeDesign.h"
#include "DialogBarcodePreview.h"
#include "DialogDateCodeDesign.h"
#include "DialogDateFormatInfo.h"
#include "DialogTemplateDesign.h"
#include "XYBarTender.h"
#include "ui_WidgetLabelPrintDesign.h"

WidgetLabelPrintDesign::WidgetLabelPrintDesign(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetLabelPrintDesign)
    , bartender(new XYBarTender)
{
    ui->setupUi(this);

    initialWidget();
    initialRecord();
}

WidgetLabelPrintDesign::~WidgetLabelPrintDesign()
{
    delete bartender;
    delete ui;
}

void WidgetLabelPrintDesign::initialWidget()
{
    do
    {
        ui->comboTemplateFile->setPopupEnable(false);
        ui->comboTemplateFile->lineEdit()->setReadOnly(true);
        ui->comboPrinterName->lineEdit()->setReadOnly(true);
        ui->comboDateCode->lineEdit()->setReadOnly(false);
        ui->comboCheckCode->lineEdit()->setReadOnly(false);

        ui->datePlanDate->setDate(QDate::currentDate());

        QSettings settings(PRINTER_REGEDIT, QSettings::NativeFormat);
        QStringList printerNames = settings.childGroups();
        ui->comboPrinterName->addItems(printerNames);
        ui->comboPrinterName->setCurrentIndex(-1);

        ui->comboCheckCode->addItems(checkCodeTypeMap.values());
        ui->comboCheckCode->setCurrentIndex(-1);
    } while (0);

    do
    {
        QTreeWidget *tree = ui->treeWidget;

        tree->setFont(qApp->font());
        tree->header()->setFont(qApp->font());
        tree->header()->setFixedHeight(25);
        tree->setHeaderLabel(tr("工艺列表"));
        tree->setStyleSheet("QTreeWidget::item{height:21px}");

    } while (0);

    do
    {
        templateHeaderMap.insert(TemplateHeader::ItemType, "元素类型");
        templateHeaderMap.insert(TemplateHeader::ItemName, "元素命名");
        templateHeaderMap.insert(TemplateHeader::ItemText, "元素内容");

        QTableWidget *table = ui->tableTemplate;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(templateHeaderMap.size());
        table->setHorizontalHeaderLabels(templateHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(200);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    } while (0);

    do
    {
        barcodeHeaderMap.insert(BarcodeHeader::ItemType, "字符类型");
        barcodeHeaderMap.insert(BarcodeHeader::ItemSize, "字符长度");
        barcodeHeaderMap.insert(BarcodeHeader::ItemText, "字符内容");

        QTableWidget *table = ui->tableBarcode;
        table->setFont(qApp->font());
        table->horizontalHeader()->setFont(qApp->font());
        table->verticalHeader()->setFont(qApp->font());

        table->setColumnCount(barcodeHeaderMap.size());
        table->setHorizontalHeaderLabels(barcodeHeaderMap.values());
        table->setSelectionMode(QTableWidget::SingleSelection);    //单选
        table->setSelectionBehavior(QTableWidget::SelectRows);     //选取一行
        table->setEditTriggers(QTableWidget::NoEditTriggers);      //禁用编辑

        table->horizontalHeader()->setHighlightSections(false);
        table->verticalHeader()->setHighlightSections(false);
        table->horizontalHeader()->setFixedHeight(25);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->horizontalHeader()->setMinimumSectionSize(200);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    } while (0);

    do
    {
        connect(ui->comboTemplateFile, SIGNAL(clickPopup()), this, SLOT(slotPopupTemplateFile()));
        connect(ui->comboDateCode, SIGNAL(clickPopup()), this, SLOT(slotPopupDateCode()));

        connect(ui->comboTemplateFile, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectTemplateFile(QString)));
        connect(ui->comboPrinterName, SIGNAL(activated(QString)), this, SLOT(slotSelectPrinterName(QString)));
        connect(ui->comboDateCode, SIGNAL(activated(QString)), this, SLOT(slotSelectDateCode(QString)));
        connect(ui->comboCheckCode, SIGNAL(activated(QString)), this, SLOT(slotSelectCheckCode(QString)));

        connect(ui->buttonTemplateAddnew, SIGNAL(clicked()), this, SLOT(slotTemplateItemAddnew()));
        connect(ui->buttonTemplateModify, SIGNAL(clicked()), this, SLOT(slotTemplateItemModify()));
        connect(ui->buttonTemplateDelete, SIGNAL(clicked()), this, SLOT(slotTemplateItemDelete()));
        connect(ui->buttonTemplateMoveup, SIGNAL(clicked()), this, SLOT(slotTemplateItemMoveup()));
        connect(ui->buttonTemplateMovedn, SIGNAL(clicked()), this, SLOT(slotTemplateItemMovedn()));

        connect(ui->buttonBarcodeAddnew, SIGNAL(clicked()), this, SLOT(slotBarcodeItemAddnew()));
        connect(ui->buttonBarcodeModify, SIGNAL(clicked()), this, SLOT(slotBarcodeItemModify()));
        connect(ui->buttonBarcodeDelete, SIGNAL(clicked()), this, SLOT(slotBarcodeItemDelete()));
        connect(ui->buttonBarcodeMoveup, SIGNAL(clicked()), this, SLOT(slotBarcodeItemMoveup()));
        connect(ui->buttonBarcodeMovedn, SIGNAL(clicked()), this, SLOT(slotBarcodeItemMovedn()));

        connect(ui->buttonDateDesign, SIGNAL(clicked()), this, SLOT(slotDateCodeDesign()));
        connect(ui->buttonDateFormat, SIGNAL(clicked()), this, SLOT(slotDateFormatInfo()));
        connect(ui->buttonLabelPreview, SIGNAL(clicked()), this, SLOT(slotLabelPreview()));
        connect(ui->buttonLabelPrint, SIGNAL(clicked()), this, SLOT(slotLabelPrint()));
        connect(ui->buttonLabelSave, SIGNAL(clicked()), this, SLOT(slotLabelSave()));

        connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectLabelItem()));
        connect(ui->tableTemplate, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotTemplateItemModify()));
        connect(ui->tableBarcode, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotBarcodeItemModify()));

        connect(bartender, SIGNAL(hasExportToClipboard()), this, SLOT(slotExportToClipboard()));
        connect(bartender, SIGNAL(hasPrintOut()), this, SLOT(slotPrintOut()));
    } while (0);
}

void WidgetLabelPrintDesign::initialRecord()
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
    } while (0);

    for (int i = 0; i < tree->topLevelItemCount(); ++i)
    {
        for (auto labelType : labelTypeStringMap.values())
        {
            item = new QTreeWidgetItem;
            item->setText(0, labelType);
            item->setIcon(0, QIcon(":/images/pngs/tree_label.png"));
            tree->topLevelItem(i)->addChild(item);
        }
    }
}

void WidgetLabelPrintDesign::generateTemplate(const PrintVariants &vars)
{
    QString value;

    for (auto item : labelItems.templateItems)
    {
        switch (templateItemTypeMap.key(item.type))
        {
        case TemplateItemType::FixedText:
            value = item.text;
            break;
        case TemplateItemType::Barcode:
            value = generateBarcode(vars);
            break;
        case TemplateItemType::DateFormat:
            value = generateDateText(item.text, vars.planDate);
            break;
        case TemplateItemType::SerialNo10:
            value = QString::asprintf(item.text.toLatin1().data(), vars.serialNo);
            break;
        case TemplateItemType::SerialNo16:
            value = QString::asprintf(item.text.toLatin1().data(), vars.serialNo);
            break;
        case TemplateItemType::PackWeight:
            value = QString::asprintf(item.text.toLatin1().data(), vars.packWeight);
            break;
        case TemplateItemType::PackNumber:
            value = QString::asprintf(item.text.toLatin1().data(), vars.packNumber);
            break;
        }

        bartender->setNamedSubStringValue(item.name, value);
    }
}

QString WidgetLabelPrintDesign::generateBarcode(const PrintVariants &vars)
{
    QString value, barcode;

    for (auto item : labelItems.barcodeItems)
    {
        switch (barcodeItemTypeMap.key(item.type))
        {
        case BarcodeItemType::FixedText:
            value = item.text;
            break;
        case BarcodeItemType::DateFormat:
            value = generateDateText(item.text, vars.planDate);
            break;
        case BarcodeItemType::DayOfYear:
            value = QString::number(vars.planDate.dayOfYear());
            value = value.rightJustified(item.size, '0', true);
            break;
        case BarcodeItemType::WeekOfYear:
            value = QString::number(vars.planDate.dayOfYear());
            value = value.rightJustified(item.size, '0', true);
            break;
        case BarcodeItemType::YearCode:
            value = dateItems.yearMap.value(vars.planDate.year());
            break;
        case BarcodeItemType::MonthCode:
            value = dateItems.monthMap.value(vars.planDate.month());
            break;
        case BarcodeItemType::DayCode:
            value = dateItems.dayMap.value(vars.planDate.day());
            break;
        case BarcodeItemType::SerialNo10:
            value = QString::number(vars.serialNo, 10);
            value = value.rightJustified(item.size, '0', true);
            break;
        case BarcodeItemType::SerialNo16:
            value = QString::number(vars.serialNo, 16);
            value = value.rightJustified(item.size, '0', true);
            break;
        case BarcodeItemType::SerialNo36:
            value = QString::number(vars.serialNo, 36);
            value = value.rightJustified(item.size, '0', true);
            break;
        case BarcodeItemType::CheckCode:
            value = generateCheckCode(barcode);
            break;
        case BarcodeItemType::BatchCode:
            value.clear();
            break;
        case BarcodeItemType::RepairCode:
            value.clear();
            break;
        }

        barcode.append(value.toUpper());
    }

    return barcode;
}

QString WidgetLabelPrintDesign::generateDateText(const QString &format, const QDate &date)
{
    QString text = format;

    text.replace("YYYY", QString::number(date.year()).right(4));
    text.replace("YY", QString::number(date.year()).right(2));
    text.replace("MM", QString::number(date.month()).rightJustified(2, '0', true));
    text.replace("DD", QString::number(date.day()).rightJustified(2, '0', true));
    text.replace("T", QString::number(date.dayOfWeek()).rightJustified(1, '0', true));
    text.replace("WW", QString::number(date.weekNumber()).rightJustified(2, '0', true));
    text.replace("ZZZ", QString::number(date.dayOfYear()).rightJustified(3, '0', true));

    return text;
}

char WidgetLabelPrintDesign::generateCheckCode(const QString &string)
{
    if (checkCodeTypeMap.values().contains(labelItems.checkCode))
    {
        switch (checkCodeTypeMap.key(labelItems.checkCode))
        {
        case CheckCodeType::VW:
            return CheckCode_GenerateVW(string);
        case CheckCodeType::SL:
            return CheckCode_GenerateSL(string);
        case CheckCodeType::QR:
            return CheckCode_GenerateQR(string);
        }
    }

    return '\0';
}

void WidgetLabelPrintDesign::slotPopupTemplateFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("打开"), QString(), "*.btw");

    if (filename.isEmpty())
        return;

    ui->comboTemplateFile->setCurrentText(filename);
    labelItems.templateFile = filename;
}

void WidgetLabelPrintDesign::slotPopupDateCode()
{
    ui->comboDateCode->clear();

    QDir dir(DATE_CODE_PATH);
    QFileInfoList fileList = dir.entryInfoList(QStringList("*.xml"), QDir::Files);

    for (auto file : fileList)
        ui->comboDateCode->addItem(file.completeBaseName());

    ui->comboDateCode->setCurrentText(labelItems.dateCode);
}

void WidgetLabelPrintDesign::slotSelectLabelItem()
{
    ui->comboTemplateFile->clearEditText();
    ui->comboPrinterName->clearEditText();
    ui->comboDateCode->clearEditText();
    ui->comboCheckCode->clearEditText();

    ui->tableTemplate->clearContents();
    ui->tableTemplate->setRowCount(0);
    ui->tableBarcode->clearContents();
    ui->tableBarcode->setRowCount(0);

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr || item->parent() == nullptr)
    {
        return;
    }

    QString craftID = item->parent()->text(0);
    LabelType type = labelTypeStringMap.key(item->text(0));
    QString labelType = labelTypeMap.value(type);

    QString filename = QString("%1/%2.%3.xml")
                           .arg(LABEL_PRINT_PATH)
                           .arg(craftID)
                           .arg(labelType);

    if (!LabelPrint_ReadXmlFile(filename, labelItems))
    {
        MessageBox_Critical("标签模板配置文件读取失败！");
        return;
    }

    do
    {
        ui->comboTemplateFile->setCurrentText(labelItems.templateFile);
        ui->comboPrinterName->setCurrentText(labelItems.printerName);
        ui->comboDateCode->setCurrentText(labelItems.dateCode);
        ui->comboCheckCode->setCurrentText(labelItems.checkCode);
    } while (0);

    do
    {
        QTableWidget *table = ui->tableTemplate;
        QTableWidgetItem *item = nullptr;

        for (int row = 0; row < labelItems.templateItems.size(); ++row)
        {
            table->insertRow(row);

            item = new QTableWidgetItem(labelItems.templateItems.at(row).type);
            table->setItem(row, static_cast<int>(TemplateHeader::ItemType), item);

            item = new QTableWidgetItem(labelItems.templateItems.at(row).name);
            table->setItem(row, static_cast<int>(TemplateHeader::ItemName), item);

            item = new QTableWidgetItem(labelItems.templateItems.at(row).text);
            table->setItem(row, static_cast<int>(TemplateHeader::ItemText), item);
        }
    } while (0);

    do
    {
        QTableWidget *table = ui->tableBarcode;
        QTableWidgetItem *item = nullptr;

        for (int row = 0; row < labelItems.barcodeItems.size(); ++row)
        {
            table->insertRow(row);

            item = new QTableWidgetItem(labelItems.barcodeItems.at(row).type);
            table->setItem(row, static_cast<int>(TemplateHeader::ItemType), item);

            item = new QTableWidgetItem(QString::number(labelItems.barcodeItems.at(row).size));
            table->setItem(row, static_cast<int>(TemplateHeader::ItemName), item);

            item = new QTableWidgetItem(labelItems.barcodeItems.at(row).text);
            table->setItem(row, static_cast<int>(TemplateHeader::ItemText), item);
        }
    } while (0);

    do
    {
        QString filename = QString("%1/%2.xml")
                               .arg(DATE_CODE_PATH)
                               .arg(labelItems.dateCode);

        DateCode_ReadXmlFile(filename, dateItems);
    } while (0);
}

void WidgetLabelPrintDesign::slotSelectTemplateFile(const QString &filename)
{
    labelItems.templateFile = filename;

    if (labelItems.templateFile.isEmpty())
    {
        return;
    }

    if (!bartender->createApplication())
    {
        MessageBox_Critical("Bartender应用程序打开失败！");
        return;
    }

    if (!bartender->openFormat(labelItems.templateFile))
    {
        MessageBox_Critical("标签模板文件打开失败！");
        return;
    }

    if (!labelItems.printerName.isEmpty())
    {
        bartender->setPrinter(labelItems.printerName);
    }
}

void WidgetLabelPrintDesign::slotSelectPrinterName(const QString &printer)
{
    labelItems.printerName = printer;

    bartender->setPrinter(labelItems.printerName);
}

void WidgetLabelPrintDesign::slotSelectDateCode(const QString &code)
{
    labelItems.dateCode = code;

    QString filename = QString("%1/%2.xml")
                           .arg(DATE_CODE_PATH)
                           .arg(labelItems.dateCode);

    DateCode_ReadXmlFile(filename, dateItems);
}

void WidgetLabelPrintDesign::slotSelectCheckCode(const QString &code)
{
    labelItems.checkCode = code;
}

void WidgetLabelPrintDesign::slotLabelPreview()
{
    PrintVariants vars =
        {
            ui->datePlanDate->date(),
            ui->spinSerialNo->value(),
            ui->dspinPackWeight->value(),
            ui->spinPackNumber->value(),
        };

    generateTemplate(vars);

    bartender->exportToClipboard(ui->spinResolution->value());
}

void WidgetLabelPrintDesign::slotLabelPrint()
{
    PrintVariants vars =
        {
            ui->datePlanDate->date(),
            ui->spinSerialNo->value(),
            ui->dspinPackWeight->value(),
            ui->spinPackNumber->value(),
        };

    generateTemplate(vars);

    bartender->printOut();
}

void WidgetLabelPrintDesign::slotExportToClipboard()
{
    DialogBarcodePreview dialog(this);

    dialog.exec();
}

void WidgetLabelPrintDesign::slotPrintOut()
{
    MessageBox_Information("标签打印成功。");
}

void WidgetLabelPrintDesign::slotLabelSave()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item = tree->currentItem();

    if (item == nullptr || item->parent() == nullptr)
    {
        return;
    }

    QString craftID = item->parent()->text(0);
    LabelType type = labelTypeStringMap.key(item->text(0));
    QString labelType = labelTypeMap.value(type);

    QString filename = QString("%1/%2.%3.xml")
                           .arg(LABEL_PRINT_PATH)
                           .arg(craftID)
                           .arg(labelType);

    if (LabelPrint_SaveXmlFile(filename, labelItems))
    {
        MessageBox_Information("标签模板保存成功。");
    }
    else
    {
        MessageBox_Critical("标签模板保存失败！");
    }
}

void WidgetLabelPrintDesign::slotDateCodeDesign()
{
    DialogDateCodeDesign dialog(this);

    dialog.exec();
}

void WidgetLabelPrintDesign::slotDateFormatInfo()
{
    DialogDateFormatInfo dialog(this);

    dialog.exec();
}

void WidgetLabelPrintDesign::slotTemplateItemAddnew()
{
    DialogTemplateDesign dialog(this);
    dialog.setItemTypeList(templateItemTypeMap.values());
    dialog.setItemNameList(bartender->getNamedSubStrings());

    if (QDialog::Accepted != dialog.exec())
        return;

    TemplateItem templateItem =
        {
            dialog.getItemType(),
            dialog.getItemName(),
            dialog.getItemText(),
        };

    labelItems.templateItems.append(templateItem);

    QTableWidget *table = ui->tableTemplate;
    QTableWidgetItem *item = nullptr;
    int row = table->rowCount();
    table->insertRow(row);

    item = new QTableWidgetItem(labelItems.templateItems.at(row).type);
    table->setItem(row, static_cast<int>(TemplateHeader::ItemType), item);

    item = new QTableWidgetItem(labelItems.templateItems.at(row).name);
    table->setItem(row, static_cast<int>(TemplateHeader::ItemName), item);

    item = new QTableWidgetItem(labelItems.templateItems.at(row).text);
    table->setItem(row, static_cast<int>(TemplateHeader::ItemText), item);

    table->selectRow(row);
}

void WidgetLabelPrintDesign::slotTemplateItemModify()
{
    QTableWidget *table = ui->tableTemplate;
    QTableWidgetItem *item = nullptr;
    int row = table->currentRow();
    if (row == -1)
        return;

    DialogTemplateDesign dialog(this);
    dialog.setItemTypeList(templateItemTypeMap.values());
    dialog.setItemNameList(bartender->getNamedSubStrings());

    dialog.setItemType(labelItems.templateItems.at(row).type);
    dialog.setItemName(labelItems.templateItems.at(row).name);
    dialog.setItemText(labelItems.templateItems.at(row).text);

    if (QDialog::Accepted != dialog.exec())
        return;

    labelItems.templateItems[row].type = dialog.getItemType();
    labelItems.templateItems[row].name = dialog.getItemName();
    labelItems.templateItems[row].text = dialog.getItemText();

    item = table->item(row, static_cast<int>(TemplateHeader::ItemType));
    item->setText(labelItems.templateItems.at(row).type);

    item = table->item(row, static_cast<int>(TemplateHeader::ItemName));
    item->setText(labelItems.templateItems.at(row).name);

    item = table->item(row, static_cast<int>(TemplateHeader::ItemText));
    item->setText(labelItems.templateItems.at(row).text);
}

void WidgetLabelPrintDesign::slotTemplateItemDelete()
{
    QTableWidget *table = ui->tableTemplate;
    int row = table->currentRow();
    if (row == -1)
        return;

    labelItems.templateItems.removeAt(row);

    table->removeRow(row);
}

void WidgetLabelPrintDesign::slotTemplateItemMoveup()
{
    QTableWidget *table = ui->tableTemplate;
    int row = table->currentRow();
    if (row == -1 || row == 0)
        return;

    labelItems.templateItems.swap(row, row - 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row - 1, col);
        table->setItem(row, col, item2);
        table->setItem(row - 1, col, item1);
    }

    table->selectRow(row - 1);
}

void WidgetLabelPrintDesign::slotTemplateItemMovedn()
{
    QTableWidget *table = ui->tableTemplate;
    int row = table->currentRow();
    int count = table->rowCount();
    if (row == -1 || row == count - 1)
        return;

    labelItems.templateItems.swap(row, row + 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row + 1, col);
        table->setItem(row, col, item2);
        table->setItem(row + 1, col, item1);
    }

    table->selectRow(row + 1);
}

void WidgetLabelPrintDesign::slotBarcodeItemAddnew()
{
    DialogBarcodeDesign dialog(this);
    dialog.setItemTypeList(barcodeItemTypeMap.values());

    if (QDialog::Accepted != dialog.exec())
        return;

    BarcodeItem barcodeItem =
        {
            dialog.getItemType(),
            dialog.getItemSize(),
            dialog.getItemText(),
        };

    labelItems.barcodeItems.append(barcodeItem);

    QTableWidget *table = ui->tableBarcode;
    QTableWidgetItem *item = nullptr;
    int row = table->rowCount();
    table->insertRow(row);

    item = new QTableWidgetItem(labelItems.barcodeItems.at(row).type);
    table->setItem(row, static_cast<int>(BarcodeHeader::ItemType), item);

    item = new QTableWidgetItem(QString::number(labelItems.barcodeItems.at(row).size));
    table->setItem(row, static_cast<int>(BarcodeHeader::ItemSize), item);

    item = new QTableWidgetItem(labelItems.barcodeItems.at(row).text);
    table->setItem(row, static_cast<int>(BarcodeHeader::ItemText), item);

    table->selectRow(row);
}

void WidgetLabelPrintDesign::slotBarcodeItemModify()
{
    QTableWidget *table = ui->tableBarcode;
    QTableWidgetItem *item = nullptr;
    int row = table->currentRow();
    if (row == -1)
        return;

    DialogBarcodeDesign dialog(this);
    dialog.setItemTypeList(barcodeItemTypeMap.values());

    dialog.setItemType(labelItems.barcodeItems.at(row).type);
    dialog.setItemSize(labelItems.barcodeItems.at(row).size);
    dialog.setItemText(labelItems.barcodeItems.at(row).text);

    if (QDialog::Accepted != dialog.exec())
        return;

    labelItems.barcodeItems[row].type = dialog.getItemType();
    labelItems.barcodeItems[row].size = dialog.getItemSize();
    labelItems.barcodeItems[row].text = dialog.getItemText();

    item = table->item(row, static_cast<int>(BarcodeHeader::ItemType));
    item->setText(labelItems.barcodeItems.at(row).type);

    item = table->item(row, static_cast<int>(BarcodeHeader::ItemSize));
    item->setText(QString::number(labelItems.barcodeItems.at(row).size));

    item = table->item(row, static_cast<int>(BarcodeHeader::ItemText));
    item->setText(labelItems.barcodeItems.at(row).text);
}

void WidgetLabelPrintDesign::slotBarcodeItemDelete()
{
    QTableWidget *table = ui->tableBarcode;
    int row = table->currentRow();
    if (row == -1)
        return;

    labelItems.barcodeItems.removeAt(row);

    table->removeRow(row);
}

void WidgetLabelPrintDesign::slotBarcodeItemMoveup()
{
    QTableWidget *table = ui->tableBarcode;
    int row = table->currentRow();
    if (row == -1 || row == 0)
        return;

    labelItems.barcodeItems.swap(row, row - 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row - 1, col);
        table->setItem(row, col, item2);
        table->setItem(row - 1, col, item1);
    }

    table->selectRow(row - 1);
}

void WidgetLabelPrintDesign::slotBarcodeItemMovedn()
{
    QTableWidget *table = ui->tableBarcode;
    int row = table->currentRow();
    int count = table->rowCount();
    if (row == -1 || row == count - 1)
        return;

    labelItems.barcodeItems.swap(row, row + 1);

    for (int col = 0; col < table->columnCount(); ++col)
    {
        QTableWidgetItem *item1 = table->takeItem(row, col);
        QTableWidgetItem *item2 = table->takeItem(row + 1, col);
        table->setItem(row, col, item2);
        table->setItem(row + 1, col, item1);
    }

    table->selectRow(row + 1);
}
