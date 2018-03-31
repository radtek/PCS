#include "DialogBarcodePreview.h"
#include "ui_DialogBarcodePreview.h"

DialogBarcodePreview::DialogBarcodePreview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogBarcodePreview)
{
    ui->setupUi(this);

    ui->labelPixmap->setPixmap(qApp->clipboard()->pixmap());
}

DialogBarcodePreview::~DialogBarcodePreview()
{
    delete ui;
}

