#ifndef OPCITEMPROPDIALOG_H
#define OPCITEMPROPDIALOG_H

#include <QWidget>

namespace Ui {
class OPCItemPropDialog;
}

class OPCItemPropDialog : public QWidget
{
    Q_OBJECT

public:
    explicit OPCItemPropDialog(QWidget *parent = 0);
    ~OPCItemPropDialog();

private:
    Ui::OPCItemPropDialog *ui;
};

#endif // OPCITEMPROPDIALOG_H
