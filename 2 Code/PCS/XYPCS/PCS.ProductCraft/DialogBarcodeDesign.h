#ifndef DIALOGBARCODEDESIGN_H
#define DIALOGBARCODEDESIGN_H

#include "WorkManager.h"

namespace Ui
{
class DialogBarcodeDesign;
}

class DialogBarcodeDesign : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBarcodeDesign(QWidget *parent = Q_NULLPTR);
    virtual ~DialogBarcodeDesign();

public:
    QString getItemType() const;
    int getItemSize() const;
    QString getItemText() const;

    void setItemType(const QString &type);
    void setItemSize(int size);
    void setItemText(const QString &text);

    void setItemTypeList(const QStringList &typeList);
public slots:
    void slotItemTypeChanged(const QString &type);
    void slotItemTextChanged(const QString &text);

private:
    void initialWidget();

private:
    Ui::DialogBarcodeDesign *ui;
};

#endif
