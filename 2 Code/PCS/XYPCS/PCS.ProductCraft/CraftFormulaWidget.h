#ifndef CRAFTFORMULAWIDGET_H
#define CRAFTFORMULAWIDGET_H

#include <QWidget>

namespace Ui {
class CraftFormulaWidget;
}

class CraftFormulaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CraftFormulaWidget(QWidget *parent = 0);
    virtual ~CraftFormulaWidget();

private:
    Ui::CraftFormulaWidget *ui;
};

#endif // CRAFTFORMULAWIDGET_H
