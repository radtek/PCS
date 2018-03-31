#ifndef DIALOGSTEPSCANBARCODE_H
#define DIALOGSTEPSCANBARCODE_H

#include "WorkManager.h"

namespace Ui
{
class DialogStepScanBarcode;
}

class DialogStepScanBarcode : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStepScanBarcode(QWidget *parent = Q_NULLPTR);
    virtual ~DialogStepScanBarcode();

public:
    void setMaterialIDList(const QStringList &list);
    void setStationIDList(const QStringList &list);
    void setDacItemIDList(const QStringList &list);
    void setOpcItemIDList(const QStringList &list);

    void setWidgetData(const ScanBarcodeItem &item);
    ScanBarcodeItem getWidgetData() const;
public slots:
    void slotCheckDisabled(bool check);
    void slotSelectBarcodeType(const QString &text);
    void slotSelectMeasureType(const QString &text);
    virtual int exec();

private:
    void initialWidget();

private:
    Ui::DialogStepScanBarcode *ui;

    QStringList materialIDList;
    QStringList stationIDList;
    QStringList dacItemIDList;
    QStringList opcItemIDList;
};

#endif    // DIALOGSTEPSCANBARCODE_H
