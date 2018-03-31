#include "LabelPrint.h"

LabelPrint::LabelPrint()
{
    bartender = new XYBarTender;
}

LabelPrint::~LabelPrint()
{
    delete bartender;
}

bool LabelPrint::loadBarcode(const QString &craftID, const QString &labelType)
{
    do
    {
        QString filename = QString("%1/%2.%3.xml")
                           .arg(LABEL_PRINT_PATH)
                           .arg(craftID)
                           .arg(labelType);

        if (!LabelPrint_ReadXmlFile(filename, labelItems))
        {
            qDebug() << "initialBarcode() labelPrint_readXmlFile() failed!";
            return false;
        }
    }
    while (0);

    do
    {
        QString filename = QString("%1/%2.xml")
                           .arg(DATE_CODE_PATH)
                           .arg(labelItems.dateCode);

        if (!DateCode_ReadXmlFile(filename, dateItems))
        {
            qDebug() << "initialBarcode() dateCode_readXmlFile() failed!";
        }
    }
    while (0);

    do
    {
        if (labelItems.templateFile.isEmpty())
        {
            qDebug() << "initialBarcode() templateFile is empty!";
            return false;
        }

        if (!bartender->createApplication())
        {
            qDebug() << "initialBarcode() create application failed!";
            return false;
        }

        if (!bartender->openFormat(labelItems.templateFile))
        {
            qDebug() << "initialBarcode() open format failed!";
            return false;
        }

        if (!labelItems.printerName.isEmpty())
        {
            bartender->setPrinter(labelItems.printerName);
        }
    }
    while (0);

    return true;
}

void LabelPrint::generateTemplate(const PrintVariants &vars)
{
    for (auto item : labelItems.templateItems)
    {
        QString value;

        switch (templateItemTypeMap.key(item.type))
        {
        case TemplateItemType::FixedText:
            value = item.text;
            break;

        case TemplateItemType::Barcode:
            value = generateBarcode(vars);
            barcodeName = item.name;
            barcodeText = value;
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

QString LabelPrint::generateBarcode(const PrintVariants &vars)
{
    QString barcode;

    for (auto item : labelItems.barcodeItems)
    {
        QString value;

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

QString LabelPrint::generateDateText(const QString &format, const QDate &date)
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

char LabelPrint::generateCheckCode(const QString &string)
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

void LabelPrint::printOut()
{
    bartender->printOut();
}

void LabelPrint::printOut(const QString &barcode)
{
    bartender->setNamedSubStringValue(barcodeName, barcode);

    bartender->printOut();
}





































