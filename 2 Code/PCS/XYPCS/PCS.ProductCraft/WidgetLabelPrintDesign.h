#ifndef PCS_PRODUCTCRAFT_LABELPRINTINGWIDGET_H_
#define PCS_PRODUCTCRAFT_LABELPRINTINGWIDGET_H_

#include "WorkManager.h"

namespace Ui
{
class WidgetLabelPrintDesign;
}

class XYBarTender;
class WidgetLabelPrintDesign : public QWidget
{
    Q_OBJECT
public:
    enum class TemplateHeader
    {
        ItemType,
        ItemName,
        ItemText,
    };

    enum class BarcodeHeader
    {
        ItemType,
        ItemSize,
        ItemText,
    };

public:
    explicit WidgetLabelPrintDesign(QWidget *parent = Q_NULLPTR);
    virtual ~WidgetLabelPrintDesign();

public slots:
    void slotPopupTemplateFile();
    void slotPopupDateCode();

    void slotSelectLabelItem();
    void slotSelectTemplateFile(const QString &filename);
    void slotSelectPrinterName(const QString &filename);
    void slotSelectDateCode(const QString &code);
    void slotSelectCheckCode(const QString &code);

    void slotTemplateItemAddnew();
    void slotTemplateItemModify();
    void slotTemplateItemDelete();
    void slotTemplateItemMoveup();
    void slotTemplateItemMovedn();

    void slotBarcodeItemAddnew();
    void slotBarcodeItemModify();
    void slotBarcodeItemDelete();
    void slotBarcodeItemMoveup();
    void slotBarcodeItemMovedn();

    void slotDateCodeDesign();
    void slotDateFormatInfo();
    void slotLabelPreview();
    void slotLabelPrint();
    void slotLabelSave();

    void slotExportToClipboard();
    void slotPrintOut();

private:
    void initialWidget();
    void initialRecord();

    void generateTemplate(const PrintVariants &vars);
    QString generateBarcode(const PrintVariants &vars);
    QString generateDateText(const QString &format, const QDate &date);
    char generateCheckCode(const QString &string);

private:
    Ui::WidgetLabelPrintDesign *ui;
    XYBarTender *bartender;

    QMap<TemplateHeader, QString> templateHeaderMap;
    QMap<BarcodeHeader, QString> barcodeHeaderMap;

    LabelPrintItems labelItems;
    DateCodeItems dateItems;
};

#endif
