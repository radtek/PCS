#ifndef ASSEMBLYFLOWWIDGET_H
#define ASSEMBLYFLOWWIDGET_H

#include <QWidget>
#include <QMenu>

#include "GlobalData.h"
#include "APCDefine.h"
#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"

namespace Ui {
class AssemblyFlowWidget;
}

class AssemblyFlowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AssemblyFlowWidget(QWidget *parent = 0);
    virtual ~AssemblyFlowWidget();

public slots:
    void treeSelectChanged();
    void tableSelectChanged();


    void valueChanged(QtProperty *property, const QVariant &value);

    void slotReadXml();
    void slotSaveXml();
    void slotNewStep();
    void slotDelStep();
    void slotCopyStep();
    void slotSaveStep();

private:
    void initialTree();
    void initialTable();
    void initialWidget();
    void initialBrowser();

    void updateTable(APCProcess *process);
    void updateBrowser(APCStep *step);


    QtProperty *getGroupInBrowser(const QString &name);
    QtProperty *getGroupInManager(const QString &name);
    QtProperty *getSubPropInGroup(QtProperty *group, const QString &name);

    void removeGroupInBrowser(const QString &name);
    void insertGroupInBrowser(QtProperty *parent, const QString &name);
private:
    Ui::AssemblyFlowWidget *ui;

    QtTreePropertyBrowser *browser;
    QtVariantPropertyManager *manager;
    QtVariantEditorFactory *factory;

    QList<APCProcess *> processList;
};

#endif // ASSEMBLYFLOWWIDGET_H
