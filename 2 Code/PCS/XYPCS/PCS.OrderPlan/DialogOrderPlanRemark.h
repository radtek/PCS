#ifndef PCS_ORDERPLAN_DIALOGORDERPLANREMARK_H
#define PCS_ORDERPLAN_DIALOGORDERPLANREMARK_H

#include <QDialog>

namespace Ui
{
class DialogOrderPlanRemark;
}

class DialogOrderPlanRemark : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrderPlanRemark(QWidget *parent = 0);
    virtual ~DialogOrderPlanRemark();

    QString getRemark() const;
    void setReason(const QString &reason);
    void setNotice(const QString &notice);

private:
    void initialWidget();

private:
    Ui::DialogOrderPlanRemark *ui;
};

#endif    // DIALOGORDERPLANREMARK_H
