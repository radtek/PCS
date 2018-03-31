#ifndef PCS_PRODUCTCRAFT_DIALOGBARCODEPREVIEW_H_
#define PCS_PRODUCTCRAFT_DIALOGBARCODEPREVIEW_H_

#include <QDialog>
#include <QClipboard>
#include <QPixmap>

namespace Ui
{
class DialogBarcodePreview;
}

class DialogBarcodePreview : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBarcodePreview(QWidget *parent = Q_NULLPTR);
    virtual ~DialogBarcodePreview();

private:
    Ui::DialogBarcodePreview *ui;
};

#endif
