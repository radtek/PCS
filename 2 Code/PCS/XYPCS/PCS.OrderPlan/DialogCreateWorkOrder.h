#ifndef DIALOGCREATEWORKORDER_H
#define DIALOGCREATEWORKORDER_H

#include <QDialog>
#include <QtSql>

namespace Ui
{
class DialogCreateWorkOrder;
}

class DialogCreateWorkOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreateWorkOrder(QWidget *parent = 0);
    ~DialogCreateWorkOrder();
public slots:
    void slotSelectCraft(const QString &);
private Q_SLOTS:
    void slotAccept();

private:
    void initialDialog();
    Ui::DialogCreateWorkOrder *ui;
};

#endif    // DIALOGCREATEWORKORDER_H
