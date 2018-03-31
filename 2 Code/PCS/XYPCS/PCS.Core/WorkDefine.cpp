#include "WorkDefine.h"

const QString PRINTER_REGEDIT = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Print\\Printers";
const QString PROCESS_PRODUCE_PATH = "./Process/Produce";
const QString PROCESS_ASSEMBLY_PATH = "./Process/Assembly";
const QString PROCESS_PACKAGE_PATH = "./Process/Package";
const QString LABEL_PRINT_PATH = "./Print/LabelPrint";
const QString DATE_CODE_PATH = "./Print/DateCode";

QSqlDatabase LOCAL_DB;
QSqlDatabase REMOTE_DB;

const int RECORD_CREATE = static_cast<int>(RecordState::Create);
const int RECORD_UPDATE = static_cast<int>(RecordState::Update);
const int RECORD_DELETE = static_cast<int>(RecordState::Delete);

const QMap<OrderState, QString> orderStateMap =
{
    { OrderState::Create,      "新建" },
    { OrderState::Issued,      "下发" },
    { OrderState::Closed,      "关闭" },
    { OrderState::ForceClose,  "强制关闭" },
    { OrderState::Delete,      "作废" },
    { OrderState::Return,      "退回" },
    { OrderState::PlanReturn,  "计划退回" },
    { OrderState::Activated,   "激活" },
    { OrderState::Suspended,   "暂停" },
};

const QMap<LogState, QString> logStateMap =
{
    { LogState::Login,      "登陆" },
    { LogState::Logout,     "登出" },
};

const QMap<BatchState, QString> batchStateMap =
{
    { BatchState::Feed,         "投料" },
    { BatchState::Input,        "上料" },
    { BatchState::Return,       "退料" },
};

const QMap<OperatorDuty, QString> OperatorDutyMap =
{
    { OperatorDuty::LineWorker,     "操作员" },
    { OperatorDuty::LineLeader,     "线长" },
    { OperatorDuty::GroupLeader,    "组长" },
};

const QMap<SerialRule, QString> SerialRuleMap =
{
    { SerialRule::UpdateByBatch,    "按批更新" },
    { SerialRule::UpdateByDay,      "按天更新" },
    { SerialRule::UpdateByMonth,    "按月更新" },
    { SerialRule::UpdateByYear,     "按年更新" },
    { SerialRule::KeepContinuous,   "保持连续" },
};

const QMap<StationType, QString> stationTypeMap =
{
    {StationType::Station,  "Station"},
    {StationType::Package,  "Package"},
};

const QMap<StationType, QString> stationTypeStringMap =
{
    {StationType::Station,  "装配工位"},
    {StationType::Package,  "包装工位"},
};

const QMap<LabelType, QString> labelTypeMap =
{
    {LabelType::Assembly, "Assembly"},
    {LabelType::Package,  "Package"},
};

const QMap<LabelType, QString> labelTypeStringMap =
{
    {LabelType::Assembly, "总成标签"},
    {LabelType::Package,  "包装标签"},
};

const QMap<StepType, QString> stepTypeMap =
{
    {StepType::LoopStart,       "loopStart"},
    {StepType::LoopEnd,         "loopEnd"},
    {StepType::AutoMotion,      "autoMotion"},
    {StepType::ScanBarcode,     "scanBarcode"},
    {StepType::MeasureData,     "measureData"},
};

const QMap<StepType, QString> stepTypeStringMap =
{
    {StepType::LoopStart,       "循环开始"},
    {StepType::LoopEnd,         "循环结束"},
    {StepType::AutoMotion,      "设备动作"},
    {StepType::ScanBarcode,     "扫描条码"},
    {StepType::MeasureData,     "采集数据"},
};

const QMap<BarcodeType, QString> barcodeTypeMap =
{
    {BarcodeType::Material,     "Material"},
    {BarcodeType::Transfer,     "Transfer"},
};

const QMap<BarcodeType, QString> barcodeTypeStringMap =
{
    {BarcodeType::Material,     "物料条码"},
    {BarcodeType::Transfer,     "过渡条码"},
};

const QMap<MeasureType, QString> measureTypeMap =
{
    {MeasureType::ByDAC,     "ByDAC"},
    {MeasureType::ByOPC,     "ByOPC"},
};

const QMap<MeasureType, QString> measureTypeStringMap =
{
    {MeasureType::ByDAC,     "COM读取"},
    {MeasureType::ByOPC,     "PLC读取"},
};

bool AssyProduce_ReadXmlFile(const QString &filename, ProducePara &producePara)
{
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
        QDomElement qualityNode = rootNode.firstChildElement("QualityControl");

        if (!qualityNode.isNull())
        {
            producePara.enableFirstProductQA = qualityNode.firstChildElement("enableFirstProductQA").text().toInt();
            producePara.enableFinalProductQA = qualityNode.firstChildElement("enableFinalProductQA").text().toInt();
        }
    }
    while (0);

    do
    {
        QDomElement mistakeNode = rootNode.firstChildElement("MistakeProof");

        if (!mistakeNode.isNull())
        {
            producePara.enableMistakeProof01 = mistakeNode.firstChildElement("enableMistakeProof01").text().toInt();
            producePara.enableMistakeProof02 = mistakeNode.firstChildElement("enableMistakeProof02").text().toInt();
            producePara.enableMistakeProof03 = mistakeNode.firstChildElement("enableMistakeProof03").text().toInt();
            producePara.enableMistakeProof04 = mistakeNode.firstChildElement("enableMistakeProof04").text().toInt();
            producePara.enableMistakeProof05 = mistakeNode.firstChildElement("enableMistakeProof05").text().toInt();
            producePara.enableMistakeProof06 = mistakeNode.firstChildElement("enableMistakeProof06").text().toInt();
            producePara.enableMistakeProof07 = mistakeNode.firstChildElement("enableMistakeProof07").text().toInt();
            producePara.enableMistakeProof08 = mistakeNode.firstChildElement("enableMistakeProof08").text().toInt();
            producePara.enableMistakeProof09 = mistakeNode.firstChildElement("enableMistakeProof09").text().toInt();
            producePara.enableMistakeProof10 = mistakeNode.firstChildElement("enableMistakeProof10").text().toInt();
            producePara.enableMistakeProof11 = mistakeNode.firstChildElement("enableMistakeProof11").text().toInt();
        }
    }
    while (0);

    return true;
}

bool AssyProduce_SaveXmlFile(const QString &filename, ProducePara &producePara)
{
    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement rootNode = document.createElement("root");
    document.appendChild(rootNode);

    do
    {
        QDomElement qualityNode = document.createElement("QualityControl");
        rootNode.appendChild(qualityNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("enableFirstProductQA");
        textNode = document.createTextNode(QString::number(producePara.enableFirstProductQA));
        elememtNode.appendChild(textNode);
        qualityNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableFinalProductQA");
        textNode = document.createTextNode(QString::number(producePara.enableFinalProductQA));
        elememtNode.appendChild(textNode);
        qualityNode.appendChild(elememtNode);
    }
    while (0);

    do
    {
        QDomElement mistakeNode = document.createElement("MistakeProof");
        rootNode.appendChild(mistakeNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("enableMistakeProof01");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof01));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof02");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof02));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof03");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof03));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof04");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof04));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof05");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof05));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof06");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof06));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof07");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof07));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof08");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof08));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof09");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof09));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof10");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof10));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMistakeProof11");
        textNode = document.createTextNode(QString::number(producePara.enableMistakeProof11));
        elememtNode.appendChild(textNode);
        mistakeNode.appendChild(elememtNode);
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

bool AssyStation_ReadXmlFile(const QString &filename, PreparePara &preparePara, ProcessPara &processPara, QList<StepPara> &stepParaList)
{
    stepParaList.clear();

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
        QDomElement prepareNode = rootNode.firstChildElement("prepare");

        if (!prepareNode.isNull())
        {
            preparePara.enableOperatorLogin = prepareNode.firstChildElement("enableOperatorLogin").text().toInt();
            preparePara.enableMaterialInput = prepareNode.firstChildElement("enableMaterialInput").text().toInt();
            preparePara.enableFixtureCheck = prepareNode.firstChildElement("enableFixtureCheck").text().toInt();
            preparePara.enableMachineCheck = prepareNode.firstChildElement("enableMachineCheck").text().toInt();
            preparePara.enableProductCheck = prepareNode.firstChildElement("enableProductCheck").text().toInt();
        }
    }
    while (0);

    do
    {
        QDomElement stationNode = rootNode.firstChildElement("station");

        if (!stationNode.isNull())
        {
            processPara.formulaNumber = stationNode.firstChildElement("formulaNumber").text().toInt();
            processPara.processNumber = stationNode.firstChildElement("processNumber").text().toInt();

            processPara.writeProductID.itemID = stationNode.firstChildElement("writeProductID").attribute("itemID");
            processPara.writeProductBatch.itemID = stationNode.firstChildElement("writeProductBatch").attribute("itemID");
            processPara.writeFinishQuantity.itemID = stationNode.firstChildElement("writeFinishQuantity").attribute("itemID");
            processPara.writeFailedQuantity.itemID = stationNode.firstChildElement("writeFailedQuantity").attribute("itemID");
            processPara.writeFormulaNumber.itemID = stationNode.firstChildElement("writeFormulaNumber").attribute("itemID");
            processPara.writeProcessPrompt.itemID = stationNode.firstChildElement("writeProcessPrompt").attribute("itemID");

            processPara.resetSignal.itemID = stationNode.firstChildElement("resetSignal").attribute("itemID");
            processPara.resetSignal.itemValue = stationNode.firstChildElement("resetSignal").attribute("value");
            processPara.alarmSignal.itemID = stationNode.firstChildElement("alarmSignal").attribute("itemID");
            processPara.alarmSignal.itemValue = stationNode.firstChildElement("alarmSignal").attribute("value");

        }
    }
    while (0);

    do
    {
        QDomElement stepNode = rootNode.firstChildElement("step");

        while (!stepNode.isNull())
        {
            stepParaList.push_back(StepPara());
            StepPara &stepPara = stepParaList.last();
            stepPara.stepType = stepTypeMap.key(stepNode.attribute("type"));

            StepItem &stepItem = stepPara.stepItem;

            stepItem.description = stepNode.firstChildElement("description").text();
            stepItem.interaction = stepNode.firstChildElement("interaction").text();
            stepItem.isDisabled = stepNode.firstChildElement("isDisabled").text().toInt();
            stepItem.maxMeasure = stepNode.firstChildElement("maxMeasure").text().toInt();

            stepItem.startRequest.itemID = stepNode.firstChildElement("startRequest").attribute("itemID");
            stepItem.startRequest.itemValue = stepNode.firstChildElement("startRequest").attribute("value");

            stepItem.startResponse.itemID = stepNode.firstChildElement("startResponse").attribute("itemID");
            stepItem.startResponse.itemValue = stepNode.firstChildElement("startResponse").attribute("value");

            stepItem.skipResponse.itemID = stepNode.firstChildElement("skipResponse").attribute("itemID");
            stepItem.skipResponse.itemValue = stepNode.firstChildElement("skipResponse").attribute("value");

            stepItem.measureRequest.itemID = stepNode.firstChildElement("measureRequest").attribute("itemID");
            stepItem.measureRequest.itemValue = stepNode.firstChildElement("measureRequest").attribute("value");

            stepItem.successResponse.itemID = stepNode.firstChildElement("successResponse").attribute("itemID");
            stepItem.successResponse.itemValue = stepNode.firstChildElement("successResponse").attribute("value");

            stepItem.failedResponse.itemID = stepNode.firstChildElement("failedResponse").attribute("itemID");
            stepItem.failedResponse.itemValue = stepNode.firstChildElement("failedResponse").attribute("value");

            QDomElement typeNode = stepNode.firstChildElement(stepTypeMap.value(stepPara.stepType));

            if (!typeNode.isNull())
            {
                switch (stepPara.stepType)
                {
                case StepType::LoopStart:
                {
                    break;
                }

                case StepType::LoopEnd:
                {
                    break;
                }

                case StepType::AutoMotion:
                {
                    break;
                }

                case StepType::ScanBarcode:
                {
                    QDomElement itemNode = typeNode.firstChildElement("item");

                    while (!itemNode.isNull())
                    {
                        stepPara.barcodeItemList.push_back(ScanBarcodeItem());
                        ScanBarcodeItem &item = stepPara.barcodeItemList.last();

                        item.isDisabled = itemNode.firstChildElement("isDisabled").text().toInt();
                        item.barcodeType = barcodeTypeStringMap.key(itemNode.firstChildElement("barcodeType").text());
                        item.barcodeID = itemNode.firstChildElement("barcodeID").text();
                        item.measureType = measureTypeStringMap.key(itemNode.firstChildElement("measureType").text());
                        item.readBarcode.itemID = itemNode.firstChildElement("readBarcode").attribute("itemID");
                        item.writeDisplay.itemID = itemNode.firstChildElement("writeDisplay").attribute("itemID");
                        item.writeResult.itemID = itemNode.firstChildElement("writeResult").attribute("itemID");
                        item.resultOK = itemNode.firstChildElement("resultValue").attribute("OK");
                        item.resultNG = itemNode.firstChildElement("resultValue").attribute("NG");

                        itemNode = itemNode.nextSiblingElement("item");
                    }

                    break;
                }

                case StepType::MeasureData:
                {
                    QDomElement itemNode = typeNode.firstChildElement("item");

                    while (!itemNode.isNull())
                    {
                        stepPara.measureItemList.push_back(MeasureDataItem());
                        MeasureDataItem &item = stepPara.measureItemList.last();

                        item.isDisabled = itemNode.firstChildElement("isDisabled").text().toInt();
                        item.measureID = itemNode.firstChildElement("measureID").text();
                        item.offsetValue = itemNode.firstChildElement("offsetValue").text();
                        item.measureType = measureTypeStringMap.key(itemNode.firstChildElement("measureType").text());
                        item.readMeasure.itemID = itemNode.firstChildElement("readMeasure").attribute("itemID");
                        item.writeDisplay.itemID = itemNode.firstChildElement("writeDisplay").attribute("itemID");
                        item.writeResult.itemID = itemNode.firstChildElement("writeResult").attribute("itemID");
                        item.resultOK = itemNode.firstChildElement("resultValue").attribute("OK");
                        item.resultNG = itemNode.firstChildElement("resultValue").attribute("NG");

                        itemNode = itemNode.nextSiblingElement("item");
                    }

                    break;
                }
                }
            }

            stepNode = stepNode.nextSiblingElement("step");
        }
    }
    while (0);

    return true;
}

bool AssyStation_SaveXmlFile(const QString &filename, PreparePara &preparePara, ProcessPara &processPara, QList<StepPara> &stepParaList)
{
    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement rootNode = document.createElement("root");
    document.appendChild(rootNode);

    do
    {
        QDomElement prepareNode = document.createElement("prepare");
        rootNode.appendChild(prepareNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("enableOperatorLogin");
        textNode = document.createTextNode(QString::number(preparePara.enableOperatorLogin));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMaterialInput");
        textNode = document.createTextNode(QString::number(preparePara.enableMaterialInput));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableFixtureCheck");
        textNode = document.createTextNode(QString::number(preparePara.enableFixtureCheck));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMachineCheck");
        textNode = document.createTextNode(QString::number(preparePara.enableMachineCheck));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableProductCheck");
        textNode = document.createTextNode(QString::number(preparePara.enableProductCheck));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);
    }
    while (0);

    do
    {
        QDomElement stationNode = document.createElement("station");
        rootNode.appendChild(stationNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("formulaNumber");
        textNode = document.createTextNode(QString::number(processPara.formulaNumber));
        elememtNode.appendChild(textNode);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("processNumber");
        textNode = document.createTextNode(QString::number(processPara.processNumber));
        elememtNode.appendChild(textNode);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("writeProductID");
        elememtNode.setAttribute("itemID", processPara.writeProductID.itemID);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("writeProductBatch");
        elememtNode.setAttribute("itemID", processPara.writeProductBatch.itemID);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("writeFinishQuantity");
        elememtNode.setAttribute("itemID", processPara.writeFinishQuantity.itemID);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("writeFailedQuantity");
        elememtNode.setAttribute("itemID", processPara.writeFailedQuantity.itemID);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("writeFormulaNumber");
        elememtNode.setAttribute("itemID", processPara.writeFormulaNumber.itemID);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("writeProcessPrompt");
        elememtNode.setAttribute("itemID", processPara.writeProcessPrompt.itemID);
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("resetSignal");
        elememtNode.setAttribute("itemID", processPara.resetSignal.itemID);
        elememtNode.setAttribute("value", processPara.resetSignal.itemValue.toString());
        stationNode.appendChild(elememtNode);

        elememtNode = document.createElement("alarmSignal");
        elememtNode.setAttribute("itemID", processPara.alarmSignal.itemID);
        elememtNode.setAttribute("value", processPara.alarmSignal.itemValue.toString());
        stationNode.appendChild(elememtNode);

    }
    while (0);

    for (auto &stepPara : stepParaList)
    {
        QDomElement stepNode = document.createElement("step");
        stepNode.setAttribute("type", stepTypeMap.value(stepPara.stepType));
        rootNode.appendChild(stepNode);

        do
        {
            auto &stepItem = stepPara.stepItem;

            QDomElement elememtNode; QDomText textNode;

            elememtNode = document.createElement("interaction");
            textNode = document.createTextNode(stepItem.interaction);
            elememtNode.appendChild(textNode);
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("description");
            textNode = document.createTextNode(stepItem.description);
            elememtNode.appendChild(textNode);
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("isDisabled");
            textNode = document.createTextNode(QString::number(stepItem.isDisabled));
            elememtNode.appendChild(textNode);
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("maxMeasure");
            textNode = document.createTextNode(QString::number(stepItem.maxMeasure));
            elememtNode.appendChild(textNode);
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("startRequest");
            elememtNode.setAttribute("itemID", stepItem.startRequest.itemID);
            elememtNode.setAttribute("value", stepItem.startRequest.itemValue.toString());
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("startResponse");
            elememtNode.setAttribute("itemID", stepItem.startResponse.itemID);
            elememtNode.setAttribute("value", stepItem.startResponse.itemValue.toString());
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("skipResponse");
            elememtNode.setAttribute("itemID", stepItem.skipResponse.itemID);
            elememtNode.setAttribute("value", stepItem.skipResponse.itemValue.toString());
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("measureRequest");
            elememtNode.setAttribute("itemID", stepItem.measureRequest.itemID);
            elememtNode.setAttribute("value", stepItem.measureRequest.itemValue.toString());
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("successResponse");
            elememtNode.setAttribute("itemID", stepItem.successResponse.itemID);
            elememtNode.setAttribute("value", stepItem.successResponse.itemValue.toString());
            stepNode.appendChild(elememtNode);

            elememtNode = document.createElement("failedResponse");
            elememtNode.setAttribute("itemID", stepItem.failedResponse.itemID);
            elememtNode.setAttribute("value", stepItem.failedResponse.itemValue.toString());
            stepNode.appendChild(elememtNode);

        }
        while (0);

        QDomElement typeNode = document.createElement(stepTypeMap.value(stepPara.stepType));
        stepNode.appendChild(typeNode);

        switch (stepPara.stepType)
        {
        case StepType::LoopStart:
        {
            break;
        }

        case StepType::LoopEnd:
        {
            break;
        }

        case StepType::AutoMotion:
        {
            break;
        }

        case StepType::ScanBarcode:
        {
            for (auto &item : stepPara.barcodeItemList)
            {
                QDomElement itemNode = document.createElement("item");
                typeNode.appendChild(itemNode);

                QDomElement elememtNode; QDomText textNode;

                elememtNode = document.createElement("isDisabled");
                textNode = document.createTextNode(QString::number(item.isDisabled));
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("barcodeType");
                textNode = document.createTextNode(barcodeTypeStringMap.value(item.barcodeType));
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("barcodeID");
                textNode = document.createTextNode(item.barcodeID);
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("measureType");
                textNode = document.createTextNode(measureTypeStringMap.value(item.measureType));
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("readBarcode");
                elememtNode.setAttribute("itemID", item.readBarcode.itemID);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("writeDisplay");
                elememtNode.setAttribute("itemID", item.writeDisplay.itemID);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("writeResult");
                elememtNode.setAttribute("itemID", item.writeResult.itemID);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("resultValue");
                elememtNode.setAttribute("OK", item.resultOK);
                elememtNode.setAttribute("NG", item.resultNG);
                itemNode.appendChild(elememtNode);
            }

            break;
        }

        case StepType::MeasureData:
        {
            for (auto &item : stepPara.measureItemList)
            {
                QDomElement itemNode = document.createElement("item");
                typeNode.appendChild(itemNode);

                QDomElement elememtNode; QDomText textNode;

                elememtNode = document.createElement("isDisabled");
                textNode = document.createTextNode(QString::number(item.isDisabled));
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("measureID");
                textNode = document.createTextNode(item.measureID);
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("offsetValue");
                textNode = document.createTextNode(item.offsetValue);
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("measureType");
                textNode = document.createTextNode(measureTypeStringMap.value(item.measureType));
                elememtNode.appendChild(textNode);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("readMeasure");
                elememtNode.setAttribute("itemID", item.readMeasure.itemID);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("writeDisplay");
                elememtNode.setAttribute("itemID", item.writeDisplay.itemID);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("writeResult");
                elememtNode.setAttribute("itemID", item.writeResult.itemID);
                itemNode.appendChild(elememtNode);

                elememtNode = document.createElement("resultValue");
                elememtNode.setAttribute("OK", item.resultOK);
                elememtNode.setAttribute("NG", item.resultNG);
                itemNode.appendChild(elememtNode);
            }

            break;
        }
        }
    }

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    document.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

bool AssyPackage_ReadXmlFile(const QString &filename, PreparePara &preparePara, AssemblyPara &assemblyPara, PackagePara &packagePara)
{
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
        QDomElement prepareNode = rootNode.firstChildElement("prepare");

        if (!prepareNode.isNull())
        {
            preparePara.enableOperatorLogin = prepareNode.firstChildElement("enableOperatorLogin").text().toInt();
            preparePara.enableMaterialInput = prepareNode.firstChildElement("enableMaterialInput").text().toInt();
            preparePara.enableFixtureCheck = prepareNode.firstChildElement("enableFixtureCheck").text().toInt();
            preparePara.enableMachineCheck = prepareNode.firstChildElement("enableMachineCheck").text().toInt();
            preparePara.enableProductCheck = prepareNode.firstChildElement("enableProductCheck").text().toInt();
        }
    }
    while (0);

    do
    {
        QDomElement assemblyNode = rootNode.firstChildElement("assembly");

        if (!assemblyNode.isNull())
        {
            assemblyPara.enable = assemblyNode.attribute("enable").toInt();
            assemblyPara.rescanBarcode = assemblyNode.firstChildElement("rescanBarcode").text().toInt();
            assemblyPara.verifyBarcode = assemblyNode.firstChildElement("verifyBarcode").text().toInt();
            assemblyPara.printCount = assemblyNode.firstChildElement("printCount").text().toInt();
            assemblyPara.rescanCount = assemblyNode.firstChildElement("rescanCount").text().toInt();
            assemblyPara.prevStationID = assemblyNode.firstChildElement("prevStationID").text();
        }
    }
    while (0);

    do
    {
        QDomElement packageNode = rootNode.firstChildElement("package");

        if (!packageNode.isNull())
        {
            packagePara.enable = packageNode.attribute("enable").toInt();
            packagePara.rescanBarcode = packageNode.firstChildElement("rescanBarcode").text().toInt();
            packagePara.verifyBarcode = packageNode.firstChildElement("verifyBarcode").text().toInt();
            packagePara.printCount = packageNode.firstChildElement("printCount").text().toInt();
            packagePara.rescanCount = packageNode.firstChildElement("rescanCount").text().toInt();
            packagePara.packageMarker = packageNode.firstChildElement("packageMarker").text();
            packagePara.scaleDeviceID = packageNode.firstChildElement("scaleDeviceID").text();
            packagePara.packageNumber = packageNode.firstChildElement("packageNumber").text().toInt();
            packagePara.singleWeight = packageNode.firstChildElement("singleWeight").text().toDouble();
            packagePara.offsetWeight = packageNode.firstChildElement("offsetWeight").text().toDouble();
        }
    }
    while (0);

    return true;
}

bool AssyPackage_SaveXmlFile(const QString &filename, PreparePara &preparePara, AssemblyPara &assemblyPara, PackagePara &packagePara)
{
    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement rootNode = document.createElement("root");
    document.appendChild(rootNode);

    do
    {
        QDomElement prepareNode = document.createElement("prepare");
        rootNode.appendChild(prepareNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("enableOperatorLogin");
        textNode = document.createTextNode(QString::number(preparePara.enableOperatorLogin));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMaterialInput");
        textNode = document.createTextNode(QString::number(preparePara.enableMaterialInput));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableFixtureCheck");
        textNode = document.createTextNode(QString::number(preparePara.enableFixtureCheck));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableMachineCheck");
        textNode = document.createTextNode(QString::number(preparePara.enableMachineCheck));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);

        elememtNode = document.createElement("enableProductCheck");
        textNode = document.createTextNode(QString::number(preparePara.enableProductCheck));
        elememtNode.appendChild(textNode);
        prepareNode.appendChild(elememtNode);
    }
    while (0);

    do
    {
        QDomElement assemblyNode = document.createElement("assembly");
        assemblyNode.setAttribute("enable", QString::number(assemblyPara.enable));
        rootNode.appendChild(assemblyNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("rescanBarcode");
        textNode = document.createTextNode(QString::number(assemblyPara.rescanBarcode));
        elememtNode.appendChild(textNode);
        assemblyNode.appendChild(elememtNode);

        elememtNode = document.createElement("verifyBarcode");
        textNode = document.createTextNode(QString::number(assemblyPara.verifyBarcode));
        elememtNode.appendChild(textNode);
        assemblyNode.appendChild(elememtNode);

        elememtNode = document.createElement("printCount");
        textNode = document.createTextNode(QString::number(assemblyPara.printCount));
        elememtNode.appendChild(textNode);
        assemblyNode.appendChild(elememtNode);

        elememtNode = document.createElement("rescanCount");
        textNode = document.createTextNode(QString::number(assemblyPara.rescanCount));
        elememtNode.appendChild(textNode);
        assemblyNode.appendChild(elememtNode);

        elememtNode = document.createElement("prevStationID");
        textNode = document.createTextNode(assemblyPara.prevStationID);
        elememtNode.appendChild(textNode);
        assemblyNode.appendChild(elememtNode);
    }
    while (0);

    do
    {
        QDomElement packageNode = document.createElement("package");
        packageNode.setAttribute("enable", QString::number(packagePara.enable));
        rootNode.appendChild(packageNode);

        QDomElement elememtNode; QDomText textNode;

        elememtNode = document.createElement("rescanBarcode");
        textNode = document.createTextNode(QString::number(packagePara.rescanBarcode));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("verifyBarcode");
        textNode = document.createTextNode(QString::number(packagePara.verifyBarcode));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("printCount");
        textNode = document.createTextNode(QString::number(packagePara.printCount));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("rescanCount");
        textNode = document.createTextNode(QString::number(packagePara.rescanCount));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("packageMarker");
        textNode = document.createTextNode(packagePara.packageMarker);
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("scaleDeviceID");
        textNode = document.createTextNode(packagePara.scaleDeviceID);
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("packageNumber");
        textNode = document.createTextNode(QString::number(packagePara.packageNumber));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("singleWeight");
        textNode = document.createTextNode(QString::number(packagePara.singleWeight));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);

        elememtNode = document.createElement("offsetWeight");
        textNode = document.createTextNode(QString::number(packagePara.offsetWeight));
        elememtNode.appendChild(textNode);
        packageNode.appendChild(elememtNode);
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

const QMap<TemplateItemType, QString> templateItemTypeMap =
{
    {TemplateItemType::FixedText,   "固定内容"},
    {TemplateItemType::Barcode,     "追溯条码"},
    {TemplateItemType::DateFormat,  "格式化日期"},
    {TemplateItemType::SerialNo10,  "流水码(10)"},
    {TemplateItemType::SerialNo16,  "流水码(16)"},
    {TemplateItemType::PackWeight,  "包装箱重量"},
    {TemplateItemType::PackNumber,  "包装箱数量"},
};

const QMap<BarcodeItemType, QString> barcodeItemTypeMap =
{
    {BarcodeItemType::FixedText,    "固定内容"},
    {BarcodeItemType::DateFormat,   "格式化日期"},
    {BarcodeItemType::YearCode,     "年代码"},
    {BarcodeItemType::MonthCode,    "月代码"},
    {BarcodeItemType::DayCode,      "日代码"},
    {BarcodeItemType::DayOfYear,    "年天数"},
    {BarcodeItemType::WeekOfYear,   "年星期数"},
    {BarcodeItemType::SerialNo10,   "流水码(10)"},
    {BarcodeItemType::SerialNo16,   "流水码(16)"},
    {BarcodeItemType::SerialNo36,   "流水码(36)"},
    {BarcodeItemType::CheckCode,    "校验码"},
    {BarcodeItemType::BatchCode,    "批次码"},
    {BarcodeItemType::RepairCode,   "返修码"},
};

const QMap<CheckCodeType, QString> checkCodeTypeMap =
{
    {CheckCodeType::VW,     "大众抽检"},
    {CheckCodeType::SL,     "神龙抽检"},
    {CheckCodeType::QR,     "奇瑞抽检"},
};


bool LabelPrint_ReadXmlFile(const QString &filename, LabelPrintItems &items)
{
    items.initial();

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

        items.templateFile = bartenderNode.firstChildElement("templateFile").text();
        items.printerName = bartenderNode.firstChildElement("printerName").text();
        items.dateCode = bartenderNode.firstChildElement("dateCode").text();
        items.checkCode = bartenderNode.firstChildElement("checkCode").text();
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

            items.templateItems.append(item);

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

            items.barcodeItems.append(item);

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    return true;
}

bool LabelPrint_SaveXmlFile(const QString &filename, LabelPrintItems &items)
{
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
        textNode = document.createTextNode(items.templateFile);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);

        elementNode = document.createElement("printerName");
        textNode = document.createTextNode(items.printerName);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);

        elementNode = document.createElement("dateCode");
        textNode = document.createTextNode(items.dateCode);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);

        elementNode = document.createElement("checkCode");
        textNode = document.createTextNode(items.checkCode);
        elementNode.appendChild(textNode);
        bartenderNode.appendChild(elementNode);
    }
    while (0);

    do
    {
        QDomElement templateNode = document.createElement("template");
        rootNode.appendChild(templateNode);

        for (auto item : items.templateItems)
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

        for (auto item : items.barcodeItems)
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

bool DateCode_ReadXmlFile(const QString &filename, DateCodeItems &items)
{
    items.initial();

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
            items.yearMap.insert(itemNode.attribute("key").toInt(),
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
            items.monthMap.insert(itemNode.attribute("key").toInt(),
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
            items.dayMap.insert(itemNode.attribute("key").toInt(),
                                itemNode.attribute("value"));

            itemNode = itemNode.nextSiblingElement("item");
        }
    }
    while (0);

    return true;
}

bool DateCode_SaveXmlFile(const QString &filename, DateCodeItems &ptr)
{
    QDomDocument document;
    QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.appendChild(instruction);
    QDomElement rootNode = document.createElement("root");
    document.appendChild(rootNode);

    do
    {
        QDomElement yearNode = document.createElement("year");
        rootNode.appendChild(yearNode);

        for (auto iter = ptr.yearMap.begin(); iter != ptr.yearMap.end(); ++iter)
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

        for (auto iter = ptr.monthMap.begin(); iter != ptr.monthMap.end(); ++iter)
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

        for (auto iter = ptr.dayMap.begin(); iter != ptr.dayMap.end(); ++iter)
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
char CheckCode_GenerateVW(const QString &string)
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
char CheckCode_GenerateSL(const QString &string)
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
char CheckCode_GenerateQR(const QString &string)
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


