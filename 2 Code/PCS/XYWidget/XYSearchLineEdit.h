#ifndef XYTECH_XYSEARCHLINEEDIT_H
#define XYTECH_XYSEARCHLINEEDIT_H

#include "XYWidgetExport.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class XYWIDGET_EXPORT XYSearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    XYSearchLineEdit(QWidget *parent = Q_NULLPTR);
    virtual ~XYSearchLineEdit();

    void initForm();
    void initConnect();
    void translator();

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

private Q_SLOTS:
    void slotSendSearchContent();

Q_SIGNALS:
    void signalSearchContent(const QString &test);

private:
    QToolButton *m_tbnSearch;
    QHBoxLayout *m_mainLayout;
};

#endif
