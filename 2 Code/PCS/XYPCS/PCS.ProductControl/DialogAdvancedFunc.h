#ifndef DIALOGADVANCEDFUNC_H
#define DIALOGADVANCEDFUNC_H

#include <QDialog>

namespace Ui
{
class DialogAdvancedFunc;
}

class DialogAdvancedFunc : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAdvancedFunc(QWidget *parent = 0);
    ~DialogAdvancedFunc();

private:
    Ui::DialogAdvancedFunc *ui;
};

#endif    // DIALOGADVANCEDFUNC_H
