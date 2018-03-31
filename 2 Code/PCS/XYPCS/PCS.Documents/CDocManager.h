#ifndef PCS_DOUCMENT_CDOCMANAGER_H
#define PCS_DOUCMENT_CDOCMANAGER_H

#include "GlobalData.h"

class CDocManager : public QObject
{
public:
    static CDocManager &instance();

private:
    CDocManager();
    virtual ~CDocManager();

public:
    static bool CreateDoc(SubmitDocType docType, const QString &workStation, const QString &workOrder = QString(), const QString &temCode = QString());
    static bool ModifyDoc(SubmitDocType docType, const QString &docCode);
    static bool DeleteDoc(SubmitDocType docType, const QString &docCode);

private:
    bool CheckWidgetExist(SubmitDocType docType);

    bool CreateMachineCheckDoc(const QString &workStation, const QString &workOrder, const QString &temCode);
    bool CreateProductCheckDoc(const QString &workStation, const QString &workOrder, const QString &temCode);
    bool CreateFixtureCheckDoc(const QString &workStation, const QString &workOrder, const QString &temCode);
    bool CreateQualityCheckDoc(SubmitDocType docType, const QString &workStation, const QString &workOrder, const QString &temCode);

    bool ModifyMachineCheckDoc(const QString &docCode);
    bool ModifyProductCheckDoc(const QString &docCode);
    bool ModifyFixtureCheckDoc(const QString &docCode);
    bool ModifyQualityCheckDoc(SubmitDocType docType, const QString &DocCode);

private:
    QMap<SubmitDocType, QSharedPointer<QWidget>> widgetList;
};

#endif
