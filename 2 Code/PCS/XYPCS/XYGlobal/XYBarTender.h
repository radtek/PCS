#ifndef PCS_EXCOM_XYBARTENDER_H_
#define PCS_EXCOM_XYBARTENDER_H_

#include "QtBarTender.h"
#include <QVariant>

class XYBarTender : public QObject
{
    Q_OBJECT

public:
    explicit XYBarTender(QObject *parent = Q_NULLPTR);
    virtual ~XYBarTender();

public:
    bool createApplication();
    bool createFormat();
    bool openFormat(const QString &filename);
    void saveFormat();
    void saveAsFormat(const QString &filename);
    void closeFormat();
    bool isApplicationCreated();
    bool isFormatOpened();

    void setPrinter(const QString &printer);
    void exportToClipboard(int resolution = 0);
    void exportToFile(const QString &filename);
    void printOut();

    QStringList getDesignObjects();
    QStringList getNamedSubStrings();
    QString getNamedSubStringValue(const QString &substring);
    void setNamedSubStringValue(const QString &substring, const QString &value);

signals:
    void hasExportToClipboard();
    void hasPrintOut();

public Q_SLOTS:
    void showApplication();
    void hideApplication();
    void quitApplication();

public:
    BarTender::Application  *m_pApplication;
    BarTender::Format       *m_pFormat;
};

#endif
