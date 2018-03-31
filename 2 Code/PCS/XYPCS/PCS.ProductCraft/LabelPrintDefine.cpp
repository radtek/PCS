#include "LabelPrintDefine.h"

bool labelPrint_readXmlFile(const QString &filename, LabelPrintItems *ptr)
{
    if (ptr == nullptr)
        return false;
    else
        ptr->initial();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "readXmlFile() file open() failed! filename:" << filename;
        return false;
    }

    QDomDocument document;
    QString errMsg;
    int errLine, errCol;
    if (!document.setContent(&file, &errMsg, &errLine, &errCol))
    {
        qDebug() << QString("Parse xml failed! filename: %1, err msg: %2, err line: %3, err col: %4")
                 .arg(filename).arg(errMsg).arg(errLine).arg(errCol);

        file.close();
        return false;
    }
    file.close();

    QDomElement rootNode = document.documentElement();

    do
    {
        QDomElement bartenderNode = rootNode.firstChildElement("bartender");

        if (bartenderNode.isNull())
        {
            qDebug() << "readXmlFile() cannot find bartender node!";
            return false;
        }

        ptr->templateFile = bartenderNode.firstChildElement("templateFile").text();
        ptr->printerName = bartenderNode.firstChildElement("printerName").text();
        ptr->dateCode = bartenderNode.firstChildElement("dateCode").text();
        ptr->checkCode = bartenderNode.firstChildElement("checkCode").text();
    }
    while (0);

    do
    {
        QDomElement templateNode = rootNode.firstChildElement("template");

        if (templateNode.isNull())
        {
            qDebug() << "readXmlFile() cannot find template node!";
            return false;
        }

        QDomElement itemNode = templateNode.firstChildElement("item");

        while (!itemNode.isNull())
        {
            TemplateItem item =
            {
                itemNode.attribute("type"),
                itemNode.attribute("name"),
                itemNode.attribute("text"),
            };

            ptr->templateItems.append(item);

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    do
    {
        QDomElement barcodeNode = rootNode.firstChildElement("barcode");

        if (barcodeNode.isNull())
        {
            qDebug() << "readXmlFile() cannot find barcode node!";
            return false;
        }

        QDomElement itemNode = barcodeNode.firstChildElement("item");

        while (!itemNode.isNull())
        {
            BarcodeItem item =
            {
                itemNode.attribute("type"),
                itemNode.attribute("size").toInt(),
                itemNode.attribute("text"),
            };

            ptr->barcodeItems.append(item);

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    return true;
}

bool labelPrint_saveXmlFile(const QString &filename, LabelPrintItems *ptr)
{
    if (ptr == nullptr)
        return false;

    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement rootNode = document.createElement("root");
    document.appendChild(rootNode);

    do
    {
        QDomElement bartenderNode = document.createElement("bartender");
        rootNode.appendChild(bartenderNode);

        QDomElement elementNode; QDomText textNode;

        elementNode = document.createElement("templateFile");
        textNode = document.createTextNode(ptr->templateFile);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);

        elementNode = document.createElement("printerName");
        textNode = document.createTextNode(ptr->printerName);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);

        elementNode = document.createElement("dateCode");
        textNode = document.createTextNode(ptr->dateCode);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);

        elementNode = document.createElement("checkCode");
        textNode = document.createTextNode(ptr->checkCode);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);
    }
    while (0);

    do
    {
        QDomElement templateNode = document.createElement("template");
        rootNode.appendChild(templateNode);

        for (auto item : ptr->templateItems)
        {
            QDomElement itemNode = document.createElement("item");
            templateNode.appendChild(itemNode);

            itemNode.setAttribute("type", item.type);
            itemNode.setAttribute("name", item.name);
            itemNode.setAttribute("text", item.text);
        }
    }
    while (0);

    do
    {
        QDomElement barcodeNode = document.createElement("barcode");
        rootNode.appendChild(barcodeNode);

        for (auto item : ptr->barcodeItems)
        {
            QDomElement itemNode = document.createElement("item");
            barcodeNode.appendChild(itemNode);

            itemNode.setAttribute("type", item.type);
            itemNode.setAttribute("size", item.size);
            itemNode.setAttribute("text", item.text);
        }
    }
    while (0);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    document.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

bool dateCode_readXmlFile(const QString &filename, DateCodeItems *ptr)
{
    if (ptr == nullptr)
        return false;
    else
        ptr->initial();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "readXmlFile() file open() failed! filename:" << filename;
        return false;
    }

    QDomDocument document;
    QString errMsg;
    int errLine, errCol;
    if (!document.setContent(&file, &errMsg, &errLine, &errCol))
    {
        qDebug() << QString("Parse xml failed! filename: %1, err msg: %2, err line: %3, err col: %4")
                 .arg(filename).arg(errMsg).arg(errLine).arg(errCol);

        file.close();
        return false;
    }
    file.close();

    QDomElement rootNode = document.documentElement();

    do
    {
        QDomElement yearNode = rootNode.firstChildElement("year");

        if (yearNode.isNull())
        {
            qDebug() << "readXmlFile() cannot find year node!";
            return false;
        }

        QDomElement itemNode = yearNode.firstChildElement("item");

        while (!itemNode.isNull())
        {
            ptr->yearMap.insert(itemNode.attribute("key").toInt(),
                                itemNode.attribute("value"));

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    do
    {
        QDomElement monthNode = rootNode.firstChildElement("month");

        if (monthNode.isNull())
        {
            qDebug() << "readXmlFile() cannot find month node!";
            return false;
        }

        QDomElement itemNode = monthNode.firstChildElement("item");

        while (!itemNode.isNull())
        {
            ptr->monthMap.insert(itemNode.attribute("key").toInt(),
                                 itemNode.attribute("value"));

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    do
    {
        QDomElement dayNode = rootNode.firstChildElement("day");

        if (dayNode.isNull())
        {
            qDebug() << "readXmlFile() cannot find day node!";
            return false;
        }

        QDomElement itemNode = dayNode.firstChildElement("item");

        while (!itemNode.isNull())
        {
            ptr->dayMap.insert(itemNode.attribute("key").toInt(),
                               itemNode.attribute("value"));

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    return true;
}

bool dateCode_saveXmlFile(const QString &filename, DateCodeItems *ptr)
{
    if (ptr == nullptr)
        return false;

    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement rootNode = document.createElement("root");
    document.appendChild(rootNode);

    do
    {
        QDomElement yearNode = document.createElement("year");
        rootNode.appendChild(yearNode);

        for (auto iter = ptr->yearMap.begin(); iter != ptr->yearMap.end(); ++iter)
        {
            QDomElement itemNode = document.createElement("item");
            yearNode.appendChild(itemNode);

            itemNode.setAttribute("key", iter.key());
            itemNode.setAttribute("value", iter.value());
        }
    }
    while (0);

    do
    {
        QDomElement monthNode = document.createElement("month");
        rootNode.appendChild(monthNode);

        for (auto iter = ptr->monthMap.begin(); iter != ptr->monthMap.end(); ++iter)
        {
            QDomElement itemNode = document.createElement("item");
            monthNode.appendChild(itemNode);


            itemNode.setAttribute("key", iter.key());
            itemNode.setAttribute("value", iter.value());
        }
    }
    while (0);

    do
    {
        QDomElement dayNode = document.createElement("day");
        rootNode.appendChild(dayNode);

        for (auto iter = ptr->dayMap.begin(); iter != ptr->dayMap.end(); ++iter)
        {
            QDomElement itemNode = document.createElement("item");
            dayNode.appendChild(itemNode);

            itemNode.setAttribute("key", iter.key());
            itemNode.setAttribute("value", iter.value());
        }
    }
    while (0);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    document.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}
//生成大众校验码
char checkCode_generateVW(const QString &string)
{
    static const QVector<char> vectorVW =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z', '-', '.', ' ', '$',
        '/', '+', '%'
    };

    int sum = 0, size = vectorVW.size();

    for (auto c : string)
    {
        sum += vectorVW.indexOf(c.toLatin1());
    }

    int mod = sum % size;

    return vectorVW.at(mod);
}
//生产神龙校验码
char checkCode_generateSL(const QString &string)
{
    static const QVector<char> vectorSL =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K',
        'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z'
    };

    int size = vectorSL.size();
    QString value;

    for (auto c : string)
    {
        int index = vectorSL.indexOf(c.toLatin1());
        value.append(QString::number(index));
    }

    int mod = 0;
    while (value.length() > 8)
    {
        mod = value.left(8).toInt() % size;
        value = QString::number(mod) + value.remove(0, 8);
    }

    mod = value.toInt() % size;

    return vectorSL.at(mod);
}
//生产奇瑞校验码
char checkCode_generateQR(const QString &string)
{
    static const QVector<char> vectorQR =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'
    };

    int sum = 0, size = vectorQR.size();

    for (auto c : string)
    {
        sum += vectorQR.indexOf(c.toLatin1());
    }

    int mod = sum % size;

    return vectorQR.at(mod);
}
