#ifndef LABELPRINT_H
#define LABELPRINT_H

#include "WorkDefine.h"
#include "XYBarTender.h"

class LabelPrint
{
public:
    explicit LabelPrint();
    virtual ~LabelPrint();

    bool loadBarcode(const QString &craftID, const QString &labelType);
public:
    void printOut();
    void printOut(const QString &barcode);

    void generateTemplate(const PrintVariants &vars);
    QString generateBarcode(const PrintVariants &vars);
    QString generateDateText(const QString &format, const QDate &date);
    char generateCheckCode(const QString &string);

    QString getBarcodeText() const { return barcodeText; }
private:
    XYBarTender *bartender;

    QString barcodeName;        //条码命名
    QString barcodeText;        //条码内容

    LabelPrintItems labelItems;
    DateCodeItems dateItems;
};

#endif // LABELPRINT_H
