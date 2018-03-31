#ifndef DIALOGDATECODEDESIGN_H
#define DIALOGDATECODEDESIGN_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui
{
class DialogDateCodeDesign;
}

class DialogDateCodeDesign : public QDialog
{
    Q_OBJECT
public:
    enum class DateHeader
    {
        YearText,
        YearCode,
        MonthText,
        MonthCode,
        DayText,
        DayCode,
    };

public:
    explicit DialogDateCodeDesign(QWidget *parent = Q_NULLPTR);
    virtual ~DialogDateCodeDesign();

private slots:
    void slotTreeSelectionChanged();
    void slotDateItemDoubleClicked();

    void slotAddnew();
    void slotModify();
    void slotDelete();
    void slotCopy();
    void slotSave();

private:
    void initialWidget();
    void loadFormula();
    void clearUiData();

private:
    Ui::DialogDateCodeDesign *ui;

    QMap<DateHeader, QString> dateHeaderMap;
};

#endif    // DIALOGDATECODEDESIGN_H
