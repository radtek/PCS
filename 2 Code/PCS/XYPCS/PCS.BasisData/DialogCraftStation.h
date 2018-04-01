#ifndef DIALOGCRAFTSTATION_H
#define DIALOGCRAFTSTATION_H

#include <QDialog>

namespace Ui
{
class DialogCraftStation;
}

class DialogCraftStation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCraftStation(QWidget *parent = 0);
    ~DialogCraftStation();

private Q_SLOTS:
    void slotAccept();

private:
    Ui::DialogCraftStation *ui;
};

#endif    // DIALOGCRAFTSTATION_H
