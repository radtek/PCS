#ifndef DIALOGSTEPMEASCOMDATA_H
#define DIALOGSTEPMEASCOMDATA_H

#include "WorkManager.h"

namespace Ui
{
class DialogStepMeasureData;
}

class DialogStepMeasureData : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStepMeasureData(QWidget *parent = Q_NULLPTR);
    virtual ~DialogStepMeasureData();

public:
    void setMeasureIDList(const QStringList &list);
    void setDacItemIDList(const QStringList &list);
    void setOpcItemIDList(const QStringList &list);

    void setWidgetData(const MeasureDataItem &item);
    MeasureDataItem getWidgetData() const;

public slots:
    void slotCheckDisabled(bool check);
    void slotSelectMeasureType(const QString &text);
    virtual int exec();

private:
    void initialWidget();

private:
    Ui::DialogStepMeasureData *ui;

    QStringList dacItemIDList;
    QStringList opcItemIDList;
};

#endif    // DIALOGSTEPMEASCOMDATA_H
