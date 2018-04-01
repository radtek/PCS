#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui
{
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT
public:
    enum class GroupState
    {
        Unselect,
        Select,
        Edit,

    };

public:
    explicit ButtonGroup(QWidget *parent = 0);
    ~ButtonGroup();

    void setGroupState(GroupState state);

signals:
    void signalAdd();
    void signalModify();
    void signalDelete();
    void signalSave();
    void signalCancel();

private:
    Ui::ButtonGroup *ui;
};

#endif    // BUTTONGROUP_H
