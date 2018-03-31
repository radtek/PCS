#ifndef DIALOGPROCESSSTEPDATA_H
#define DIALOGPROCESSSTEPDATA_H

#include "WorkManager.h"

namespace Ui
{
class DialogProcessStepData;
}

class DialogProcessStepData : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProcessStepData(QWidget *parent = Q_NULLPTR);
    virtual ~DialogProcessStepData();

public:
    void setOpcItemIDList(const QStringList &list);

    void setWidgetData(const StepItem &item);
    StepItem getWidgetData() const;

private:
    void initialWidget();

private:
    Ui::DialogProcessStepData *ui;
};

#endif    // DIALOGPROCESSSTEPDATA_H
