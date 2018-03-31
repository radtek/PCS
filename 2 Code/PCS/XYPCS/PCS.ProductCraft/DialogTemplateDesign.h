#ifndef DIALOGTEMPLATEDESIGN_H
#define DIALOGTEMPLATEDESIGN_H

#include "WorkManager.h"

namespace Ui
{
class DialogTemplateDesign;
}

class DialogTemplateDesign : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTemplateDesign(QWidget *parent = Q_NULLPTR);
    virtual ~DialogTemplateDesign();

public:
    QString getItemType() const;
    QString getItemName() const;
    QString getItemText() const;

    void setItemType(const QString &type);
    void setItemName(const QString &name);
    void setItemText(const QString &text);

    void setItemTypeList(const QStringList &typeList);
    void setItemNameList(const QStringList &nameList);
public Q_SLOTS:
    void slotItemTypeChanged(const QString &type);

private:
    void initialWidget();

private:
    Ui::DialogTemplateDesign *ui;
};

#endif
