#include "AssemblyFlowWidget.h"
#include "ui_AssemblyFlowWidget.h"
#include <QDomDocument>
#include <QFile>
#include <QDebug>

AssemblyFlowWidget::AssemblyFlowWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AssemblyFlowWidget)
{
    ui->setupUi(this);

    initialWidget();
    initialTree();
    initialBrowser();
}

AssemblyFlowWidget::~AssemblyFlowWidget()
{
    for (auto item : processList)
    {
        delete item;
    }

    delete ui;
}


void AssemblyFlowWidget::initialWidget()
{
    QTreeWidget *tree = ui->treeWidget;

    tree->setFont(qApp->font());
    tree->header()->setFont(qApp->font());
    tree->header()->setFixedHeight(25);
    tree->setHeaderLabel("路由列表");

    connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(treeSelectChanged()));


    QTableWidget *table = ui->tableWidget;
    table->setSelectionBehavior(QTableWidget::SelectRows);
    table->horizontalHeader()->setHighlightSections(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setFont(qApp->font());
    table->horizontalHeader()->setFixedHeight(25);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"当前步号", "OK跳转", "NG跳转", "启用禁用", "交互类型", "步骤描述"});

    connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(tableSelectChanged()));


    browser = ui->propertyBrowser;
    browser->setFont(qApp->font());
    browser->header()->setFixedHeight(25);
    browser->header()->setFont(qApp->font());
    browser->setHeaderLables({"属性", "参数"});
    browser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

    manager = new QtVariantPropertyManager(browser);
    factory = new QtVariantEditorFactory(browser);

    browser->setFactoryForManager(manager, factory);

    connect(manager, SIGNAL(valueChanged(QtProperty *, QVariant)), this, SLOT(valueChanged(QtProperty *, QVariant)));


    ui->toolReadXml->setEnabled(false);
    ui->toolSaveXml->setEnabled(false);
    ui->toolNewStep->setEnabled(false);
    ui->toolDelStep->setEnabled(false);
    ui->toolCopyStep->setEnabled(false);
    ui->toolSaveStep->setEnabled(false);

    connect(ui->toolReadXml, SIGNAL(clicked(bool)), this, SLOT(slotReadXml()));
    connect(ui->toolSaveXml, SIGNAL(clicked(bool)), this, SLOT(slotSaveXml()));
    connect(ui->toolNewStep, SIGNAL(clicked(bool)), this, SLOT(slotNewStep()));
    connect(ui->toolDelStep, SIGNAL(clicked(bool)), this, SLOT(slotDelStep()));
    connect(ui->toolCopyStep, SIGNAL(clicked(bool)), this, SLOT(slotCopyStep()));
    connect(ui->toolSaveStep, SIGNAL(clicked(bool)), this, SLOT(slotSaveStep()));
}

void AssemblyFlowWidget::initialTree()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *item, *root;

    root = new QTreeWidgetItem;
    root->setText(0, "产品路由一");
    tree->addTopLevelItem(root);

    APCProcess *process = new APCProcess;
    processList.append(process);
    process->readXmlFile("C:/Users/Zhuang Ming/Desktop/test.xml");

    item = new QTreeWidgetItem;
    item->setText(0, "ST10");
    item->setData(0, Qt::UserRole, QVariant::fromValue(process));
    root->addChild(item);

//    process = new APCProcess;
//    processList.append(process);
    item = new QTreeWidgetItem;
    item->setText(0, "ST20");
//    item->setData(0, Qt::UserRole, QVariant::fromValue(process));
    root->addChild(item);


    root->setExpanded(true);

}

void AssemblyFlowWidget::initialBrowser()
{
    QFile file("./ProcessTemplate.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;

    QDomDocument document;
    if (!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = document.documentElement();
    QDomElement step = root.firstChildElement();
    QtVariantProperty *g = nullptr, *t = nullptr;

    manager->blockSignals(true);

    while (!step.isNull())
    {
        QDomElement group = step.firstChildElement();

        while (!group.isNull())
        {
            QString name = group.attribute("name");
            QString parentName = group.attribute("parentName");

            g = manager->addProperty(QtVariantPropertyManager::groupTypeId(), name);
            g->setAttribute("parentName", parentName);

            if (g == nullptr)
                continue;

            QDomElement tag = group.firstChildElement();

            while (!tag.isNull())
            {
                QString name = tag.attribute("name");
                QString type = tag.attribute("type");
                QString value = tag.text();
                int valueType = static_cast<int>(QVariant::nameToType(type.toLocal8Bit().data()));
                int enumType = tag.attribute("enumType").toInt();
                if (enumType)
                {
                    QStringList enumList = GlobalData::getEnumStringList(enumType);
                    t = manager->addProperty(QtVariantPropertyManager::enumTypeId(), name);
                    t->setAttribute("enumNames", enumList);
                    t->setValue(enumList.indexOf(value));
                }
                else
                {
                    t = manager->addProperty(valueType, name);
                    t->setValue(value);
                }

                if (t == nullptr)
                    continue;

                g->addSubProperty(t);

                tag = tag.nextSiblingElement();
            }

            group = group.nextSiblingElement();
        }

        step = step.nextSiblingElement();
    }

    manager->blockSignals(false);
}

void AssemblyFlowWidget::valueChanged(QtProperty *property, const QVariant &value)
{
    Q_UNUSED(value);

    QtVariantProperty *p = reinterpret_cast<QtVariantProperty *>(property);

    if (p->propertyType() == QtVariantPropertyManager::enumTypeId())
    {
        removeGroupInBrowser(p->propertyName());
        insertGroupInBrowser(p->parentProperty(), p->valueText());
    }
}




void AssemblyFlowWidget::treeSelectChanged()
{
    ui->toolReadXml->setEnabled(false);
    ui->toolSaveXml->setEnabled(false);
    ui->toolNewStep->setEnabled(false);
    ui->toolDelStep->setEnabled(false);
    ui->toolCopyStep->setEnabled(false);
    ui->toolSaveStep->setEnabled(false);

    ui->tableWidget->clearContents();

    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *select = tree->currentItem();
    if (select == nullptr)
        return;
    if (-1 != tree->indexOfTopLevelItem(select))
        return;

    ui->toolReadXml->setEnabled(true);
    ui->toolSaveXml->setEnabled(true);
    ui->toolNewStep->setEnabled(true);

    auto process = select->data(0, Qt::UserRole).value<APCProcess *>();

    updateTable(process);
}

void AssemblyFlowWidget::updateTable(APCProcess *process)
{
    if (process == nullptr)
        return;

    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;

    for (auto step : process->getStepList())
    {
        int row = table->rowCount();
        table->insertRow(row);

        item = new QTableWidgetItem;
        item->setText(QString::number(step->getStepID()));
        item->setData(Qt::UserRole, QVariant::fromValue(step));
        table->setItem(row, 0, item);

        item = new QTableWidgetItem;
        item->setText(QString::number(step->getSkipOK()));
        table->setItem(row, 1, item);

        item = new QTableWidgetItem;
        item->setText(QString::number(step->getSkipNG()));
        table->setItem(row, 2, item);

        item = new QTableWidgetItem;
        item->setText(step->getEnable() ? "true" : "fasle");
        table->setItem(row, 3, item);

        item = new QTableWidgetItem;
        item->setText(step->getStepType());
        table->setItem(row, 4, item);

        item = new QTableWidgetItem;
        item->setText(step->getStepText());
        table->setItem(row, 5, item);
    }
}

void AssemblyFlowWidget::tableSelectChanged()
{
    ui->toolDelStep->setEnabled(false);
    ui->toolCopyStep->setEnabled(false);
    ui->toolSaveStep->setEnabled(false);

    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *select = table->currentItem();
    if (select == nullptr)
        return;

    ui->toolDelStep->setEnabled(true);
    ui->toolCopyStep->setEnabled(true);
    ui->toolSaveStep->setEnabled(true);

    QTableWidgetItem *item = table->item(select->row(), 0);
    auto step = item->data(Qt::UserRole).value<APCStep *>();

    updateBrowser(step);
}

void AssemblyFlowWidget::updateBrowser(APCStep *step)
{
    if (step == nullptr)
        return;

    browser->clear();
    manager->blockSignals(true);

    QtVariantProperty *g = nullptr, *t = nullptr;
    for (auto group : step->getGroupList())
    {
        g = reinterpret_cast<QtVariantProperty *>(getGroupInManager(group->getName()));

        if (g == nullptr)
            continue;

        browser->addProperty(g);

        for (auto tag : group->getTagList())
        {
            t = reinterpret_cast<QtVariantProperty *>(getSubPropInGroup(g, tag->getName()));

            if (t == nullptr)
                continue;

            if (tag->getEnumType())
            {
                QStringList enumList = t->attributeValue("enumNames").toStringList();
                t->setValue(enumList.indexOf(tag->getValue().toString()));
            }
            else
            {
                t->setValue(tag->getValue());
            }
        }
    }

    manager->blockSignals(false);
}


void AssemblyFlowWidget::slotReadXml()
{

}

void AssemblyFlowWidget::slotSaveXml()
{

}

void AssemblyFlowWidget::slotNewStep()
{
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *select = tree->currentItem();
    if (select == nullptr)
        return;
    if (-1 != tree->indexOfTopLevelItem(select))
        return;

    auto process = select->data(0, Qt::UserRole).value<APCProcess *>();

    if (process == nullptr)
        return;

    QTableWidget *table = ui->tableWidget;
    QTableWidgetItem *item = nullptr;
    int row = table->rowCount();
    table->insertRow(row);

    for (int col = 0; col < 6; ++col)
    {
        item = new QTableWidgetItem;
        table->setItem(row, col, item);
    }

    APCStep *step = process->addStep();
    step->loadDefault();

    item = table->item(row, 0);
    item->setData(Qt::UserRole, QVariant::fromValue(step));

    table->selectRow(row);
}

void AssemblyFlowWidget::slotDelStep()
{

}

void AssemblyFlowWidget::slotCopyStep()
{

}


void AssemblyFlowWidget::slotSaveStep()
{

}

QtProperty *AssemblyFlowWidget::getGroupInBrowser(const QString &name)
{
    for (auto p : browser->properties())
    {
        if (p->propertyName() == name)
        {
            return p;
        }
    }

    return nullptr;
}

QtProperty *AssemblyFlowWidget::getGroupInManager(const QString &name)
{
    for (auto p : manager->properties())
    {
        if (p->propertyName() == name)
        {
            return p;
        }
    }

    return nullptr;
}

QtProperty *AssemblyFlowWidget::getSubPropInGroup(QtProperty *group, const QString &name)
{
    for (auto p : group->subProperties())
    {
        if (p->propertyName() == name)
        {
            return p;
        }
    }

    return nullptr;
}

void AssemblyFlowWidget::removeGroupInBrowser(const QString &name)
{
    for (auto group : browser->properties())
    {
        QtVariantProperty *g = reinterpret_cast<QtVariantProperty *>(group);
        QString parentName = g->attributeValue("parentName").toString();

        if (parentName == name)
        {
            for (auto prop : group->subProperties())
            {
                QtVariantProperty *p = reinterpret_cast<QtVariantProperty *>(prop);

                if (p->propertyType() == QtVariantPropertyManager::enumTypeId())
                {
                    removeGroupInBrowser(p->propertyName());
                }
            }

            browser->removeProperty(g);
        }
    }
}

void AssemblyFlowWidget::insertGroupInBrowser(QtProperty *parent, const QString &name)
{
    QtProperty *group = getGroupInManager(name);

    if (group == nullptr || parent == nullptr)
        return;

    qDebug() << "group name:" << parent->propertyName() << "parent name:" << parent->valueText();

    browser->insertProperty(group, parent);

    for (auto prop : group->subProperties())
    {
        QtVariantProperty *p = reinterpret_cast<QtVariantProperty *>(prop);
        if (p->propertyType() == QtVariantPropertyManager::enumTypeId())
        {
            qDebug() << "property name:" << p->propertyName() << "property value:" << p->valueText();

            insertGroupInBrowser(group->parentProperty(), p->valueText());
        }
    }

}
