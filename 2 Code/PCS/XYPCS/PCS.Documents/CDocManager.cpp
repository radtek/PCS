#include "CDocManager.h"

CDocManager &CDocManager::instance()
{
    static CDocManager inst;
    return inst;
}

CDocManager::CDocManager()
{
}

CDocManager::~CDocManager()
{
}

bool CDocManager::CreateDoc(SubmitDocType docType, const QString &workStation, const QString &workOrder, const QString &temCode)
{
    switch (docType)
    {
    case SubmitDocType::MachineCheckDoc:
    {
        return CDocManager::instance().CreateMachineCheckDoc(workStation, workOrder, temCode);
    }

    case SubmitDocType::ProductCheckDoc:
    {
        return CDocManager::instance().CreateProductCheckDoc(workStation, workOrder, temCode);
    }

    case SubmitDocType::FixtureCheckDoc:
    {
        return CDocManager::instance().CreateFixtureCheckDoc(workStation, workOrder, temCode);
    }

    case SubmitDocType::FirstProductQADoc:
    case SubmitDocType::FinalProductQADoc:
    case SubmitDocType::RouteProductQADoc:
    case SubmitDocType::FirstPackageQADoc:
    case SubmitDocType::FinalPackageQADoc:
    {
        return CDocManager::instance().CreateQualityCheckDoc(docType, workStation, workOrder, temCode);
    }
    }

    return true;
}

bool CDocManager::ModifyDoc(SubmitDocType docType, const QString &docCode)
{
    qInfo() << "Modify Document, DocType:" << static_cast<int>(docType) << "DocCode:" << docCode;
    switch (docType)
    {
    case SubmitDocType::MachineCheckDoc:
    {
        return CDocManager::instance().ModifyMachineCheckDoc(docCode);
    }

    case SubmitDocType::ProductCheckDoc:
    {
        return CDocManager::instance().ModifyProductCheckDoc(docCode);
    }

    case SubmitDocType::FixtureCheckDoc:
    {
        return CDocManager::instance().ModifyFixtureCheckDoc(docCode);
    }

    case SubmitDocType::FirstProductQADoc:
    case SubmitDocType::FinalProductQADoc:
    case SubmitDocType::RouteProductQADoc:
    case SubmitDocType::FirstPackageQADoc:
    case SubmitDocType::FinalPackageQADoc:
    {
        return CDocManager::instance().ModifyQualityCheckDoc(docType, docCode);
    }
    }

    return true;
}

bool CDocManager::DeleteDoc(SubmitDocType docType, const QString &docCode)
{
    return true;
}

bool CDocManager::CheckWidgetExist(SubmitDocType docType)
{
    QSharedPointer<QWidget> widget = widgetList[docType];

    if (!widget.isNull())
    {
        if (widget->isVisible())
        {
            if (widget->isMaximized())
            {
                widget->showNormal();
            }
            else
            {
                widget->showMaximized();
            }
        }

        if (!widget->close())
        {
            return true;
        }
    }

    return false;
}

bool CDocManager::CreateMachineCheckDoc(const QString &workStation, const QString &workOrder, const QString &temCode)
{
    return true;
}

bool CDocManager::CreateProductCheckDoc(const QString &workStation, const QString &workOrder, const QString &temCode)
{
    return true;
}

bool CDocManager::CreateFixtureCheckDoc(const QString &workStation, const QString &workOrder, const QString &temCode)
{
    return true;
}

bool CDocManager::CreateQualityCheckDoc(SubmitDocType docType, const QString &workStation, const QString &workOrder, const QString &temCode)
{
    return true;
}

bool CDocManager::ModifyMachineCheckDoc(const QString &docCode)
{

    return true;
}

bool CDocManager::ModifyProductCheckDoc(const QString &docCode)
{

    return true;
}

bool CDocManager::ModifyFixtureCheckDoc(const QString &docCode)
{

    return true;
}

bool CDocManager::ModifyQualityCheckDoc(SubmitDocType docType, const QString &DocCode)
{

    return true;
}
