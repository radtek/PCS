#ifndef XYTABLEBUTTON_H
#define XYTABLEBUTTON_H

#include "XYWidgetExport.h"

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>

class XYWIDGET_EXPORT ChildButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ChildButton(QWidget *parent = Q_NULLPTR);
    virtual ~ChildButton();

    inline void setIndex(int index) { m_index = index; }
    void setButtonText(const QString &text);
public slots:
    void slotClicked();
signals:
    void signalClicked(int index);
private:
    int m_index;
};

class XYWIDGET_EXPORT XYCustomButton : public QWidget
{
    Q_OBJECT
public:
    explicit XYCustomButton(QWidget *parent = Q_NULLPTR);
    virtual ~XYCustomButton();

    inline void setRow(int row) { m_row = row; }
    void addButtons(const QStringList &items);
public slots:
    void slotButtonClicked(int index);
signals:
    void signalButtonClicked(int row, int index);
private:
    int m_row;
    int m_count;
};

#endif // XYTABLEBUTTON_H
