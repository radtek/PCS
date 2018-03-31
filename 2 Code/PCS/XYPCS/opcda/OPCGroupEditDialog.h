#ifndef OPCGROUPEDITDIALOG_H
#define OPCGROUPEDITDIALOG_H

#include <QDialog>

namespace Ui {
class OPCGroupEditDialog;
}

class XYOPCGroup;

class OPCGroupEditDialog : public QDialog
{
    Q_OBJECT
public:
    enum GroupEditType{
        GroupClone,
        GroupModify,
        GroupProperties,
    };
public:
    explicit OPCGroupEditDialog(XYOPCGroup *group, QWidget *parent = 0);
    virtual ~OPCGroupEditDialog();

    void setEditType(GroupEditType type);
    XYOPCGroup *getGroupPtr() { return group; }

public slots:
    void accept();
private:
    void initialWidget();

    void setGroupState();
private:
    Ui::OPCGroupEditDialog *ui;

    GroupEditType type;
    XYOPCGroup *group;
};

#endif // OPCGROUPEDITDIALOG_H
