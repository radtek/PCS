#ifndef LABELPRINTINGWIDGET_H
#define LABELPRINTINGWIDGET_H

#include <QWidget>

namespace Ui {
class LabelPrintingWidget;
}

class LabelPrintingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LabelPrintingWidget(QWidget *parent = 0);
    virtual ~LabelPrintingWidget();

private:
    Ui::LabelPrintingWidget *ui;
};

#endif // LABELPRINTINGWIDGET_H
