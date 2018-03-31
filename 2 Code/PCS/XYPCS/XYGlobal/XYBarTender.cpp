#include "XYBarTender.h"

#include <QDebug>

using namespace BarTender;

XYBarTender::XYBarTender(QObject *parent)
    : QObject(parent)
    , m_pApplication(Q_NULLPTR)
    , m_pFormat(Q_NULLPTR)
{

}

XYBarTender::~XYBarTender()
{
    quitApplication();
}


bool XYBarTender::createApplication()
{
    if (m_pApplication)
    {
        return true;
    }

    m_pApplication = new Application("BarTender.Application");

    return m_pApplication != Q_NULLPTR;
}

bool XYBarTender::createFormat()
{
    closeFormat();

    if (!m_pApplication)
    {
        return false;
    }

    Formats *pFormats = m_pApplication->Formats();

    if (!pFormats)
    {
        return false;
    }

    m_pFormat = pFormats->Add();

    return m_pFormat != Q_NULLPTR;
}

bool XYBarTender::openFormat(const QString &filename)
{
    closeFormat();

    if (filename.isEmpty())
    {
        return false;
    }

    Formats *pFormats = m_pApplication->Formats();

    if (!pFormats)
    {
        return false;
    }

    m_pFormat = pFormats->Open(filename, false, "");

    return m_pFormat != Q_NULLPTR;
}

void XYBarTender::saveFormat()
{
    if (m_pFormat)
    {
        m_pFormat->Save();
    }
}

void XYBarTender::saveAsFormat(const QString &filename)
{
    if (m_pFormat)
    {
        m_pFormat->SaveAs(filename, true);
    }
}

void XYBarTender::closeFormat()
{
    if (m_pFormat)
    {
        m_pFormat->Close(BtSaveOptions::btSaveChanges);

        m_pFormat->clear();

        m_pFormat = Q_NULLPTR;
    }
}

bool XYBarTender::isApplicationCreated()
{
    return m_pApplication != Q_NULLPTR;
}

bool XYBarTender::isFormatOpened()
{
    return m_pFormat != Q_NULLPTR;
}

void XYBarTender::showApplication()
{
    if (m_pApplication)
    {
        m_pApplication->SetVisible(true);
    }
}

void XYBarTender::hideApplication()
{
    if (m_pApplication)
    {
        m_pApplication->SetVisible(false);
    }
}

void XYBarTender::quitApplication()
{
    closeFormat();

    if (m_pApplication)
    {
        m_pApplication->Quit(BtSaveOptions::btSaveChanges);

        m_pApplication->clear();
    }
}

void XYBarTender::setPrinter(const QString &printer)
{
    if (m_pFormat)
    {
        m_pFormat->SetPrinter(printer);
    }
}

void XYBarTender::printOut()
{
    if (m_pFormat)
    {
        m_pFormat->PrintOut(false, false);

        emit hasPrintOut();
    }
}


void XYBarTender::exportToClipboard(int resolution)
{
    if (m_pFormat)
    {
        m_pFormat->ExportToClipboard(BtColors::btColors24Bit, static_cast<BtResolution>(resolution));

        emit hasExportToClipboard();
    }
}

void XYBarTender::exportToFile(const QString &filename)
{
    if (m_pFormat)
    {
        m_pFormat->ExportToFile(filename, "bmp", BtColors::btColors24Bit, BtResolution::btResolutionPrinter, BtSaveOptions::btSaveChanges);
    }
}

QStringList XYBarTender::getNamedSubStrings()
{
    QStringList subStringList;
    if (m_pFormat)
    {
        NamedSubStrings *pNamedSubStrings = m_pFormat->NamedSubStrings();

        int count = pNamedSubStrings->Count();
        for (int i = 1; i <= count; ++i)
        {
            SubString *pSubString = reinterpret_cast<SubString *>(pNamedSubStrings->Item(i)) ;

            subStringList << pSubString->Name();
        }
    }
    return subStringList;
}

QStringList XYBarTender::getDesignObjects()
{
    QStringList objectList;
    if (m_pFormat)
    {
        DesignObjects *pObjects = m_pFormat->Objects();

        int count = pObjects->Count();
        for (int i = 1; i <= count; ++i)
        {
            DesignObject *pObject = reinterpret_cast<DesignObject *>(pObjects->Item(i));

            objectList << pObject->Name();
        }
    }
    return objectList;
}

QString XYBarTender::getNamedSubStringValue(const QString &substring)
{
    QString strValue;
    if (m_pFormat)
    {
        return m_pFormat->NamedSubStringValue(substring);
    }
    return strValue;
}

void XYBarTender::setNamedSubStringValue(const QString &substring, const QString &value)
{
    if (substring.isEmpty())
    {
        return;
    }
    if (m_pFormat)
    {
        m_pFormat->SetNamedSubStringValue(substring, value);
    }
}

