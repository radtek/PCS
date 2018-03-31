/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    QtBarTender.cpp
 *
 * @example
 *
 * @brief
 *
 * @date 2018-04-01 @author weiheng             @note create this file
 * *****************************************************************************
 */

#include "QtBarTender.h"

namespace BarTender
{

////////////////////////////////////////////////////////////////////////////////
// Application wrapper class

BarTender::Format *Application::ActiveFormat()
{
    return reinterpret_cast<BarTender::Format *>(querySubObject("ActiveFormat"));
}

void Application::SetActiveFormat(BarTender::Format *ActiveFormat)
{
    dynamicCall("SetActiveFormat(QVariant& ActiveFormat)", ActiveFormat->asVariant());
}

int Application::BuildNumber()
{
    return dynamicCall("BuildNumber").toInt();
}

void Application::SetBuildNumber(int BuildNumber)
{
    // Read Only
    dynamicCall("SetBuildNumber(QVariant& BuildNumber)", QVariant(BuildNumber));
}

BarTender::Formats *Application::Formats()
{
    return reinterpret_cast<BarTender::Formats *>(querySubObject("Formats"));
}

void Application::SetFormats(BarTender::Formats *Formats)
{
    dynamicCall("SetFormats(QVariant& Formats)", Formats->asVariant());
}

bool Application::IsPrinting()
{
    return dynamicCall("IsPrinting").toBool();
}

void Application::SetIsPrinting(bool IsPrinting)
{
    // Read Only
    dynamicCall("SetIsPrinting(QVariant& IsPrinting)", QVariant(IsPrinting));
}

bool Application::IsProcessingCommandLines()
{
    return dynamicCall("BuildNumber").toBool();
    ;
}

void Application::SetIsProcessingCommandLines(bool IsProcessingCommandLines)
{
    // Read Only
    dynamicCall("SetIsProcessingCommandLines(QVariant& IsProcessingCommandLines)", QVariant(IsProcessingCommandLines));
}

QAxObject *Application::Window()
{
    return querySubObject("Window");
}

void Application::SetWindow(QAxObject *Window)
{
    dynamicCall("SetWindow(QVariant& Window)", Window->asVariant());
}

QString Application::SAPIDocDefinitionFile()
{
    return dynamicCall("SAPIDocDefinitionFile").toString();
}

void Application::SetSAPIDocDefinitionFile(const QString &SAPIDocDefinitionFile)
{
    dynamicCall("SetSAPIDocDefinitionFile(QVariant& SAPIDocDefinitionFile)", QVariant(SAPIDocDefinitionFile));
}

QAxObject *Application::LicenseServer()
{
    return querySubObject("LicenseServer");
}

void Application::SetLicenseServer(QAxObject *LicenseServer)
{
    dynamicCall("SetLicenseServer(QVariant& LicenseServer)", LicenseServer->asVariant());
}

QString Application::Edition()
{
    return dynamicCall("Edition").toString();
}

void Application::SetEdition(const QString &Edition)
{
    // Read Only
    dynamicCall("SetEdition(QVariant& Edition)", QVariant(Edition));
}

BtVisibleWindows Application::VisibleWindows()
{
    return static_cast<BtVisibleWindows>(dynamicCall("VisibleWindows").toInt());
}

void Application::SetVisibleWindows(BtVisibleWindows VisibleWindows)
{
    dynamicCall("SetVisibleWindows(QVariant& VisibleWindows)", QVariant(static_cast<int>(VisibleWindows)));
}

bool Application::TopMost()
{
    return dynamicCall("TopMost").toBool();
}

void Application::SetTopMost(bool TopMost)
{
    dynamicCall("SetTopMost(QVariant& TopMost)", QVariant(TopMost));
}

int Application::ProcessId()
{
    return dynamicCall("ProcessId").toInt();
}

void Application::SetProcessId(int ProcessId)
{
    // Read Only
    dynamicCall("SetProcessId(QVariant& ProcessId)", QVariant(ProcessId));
}

QString Application::Version()
{
    return dynamicCall("Version").toString();
}

void Application::SetVersion(const QString &Version)
{
    // Read Only
    dynamicCall("SetVersion(QVariant& Version)", QVariant(Version));
}

bool Application::Visible()
{
    return dynamicCall("Visible").toBool();
}

void Application::SetVisible(bool Visible)
{
    dynamicCall("SetVisible(QVariant& Visible)", QVariant(Visible));
}

QString Application::FullVersion()
{
    return dynamicCall("FullVersion").toString();
}

void Application::SetFullVersion(const QString &FullVersion)
{
    // Read Only
    dynamicCall("SetFullVersion(QVariant& FullVersion)", QVariant(FullVersion));
}

void Application::Quit(BtSaveOptions SaveChanges)
{
    dynamicCall("Quit(QVariant& SaveChanges)", QVariant(static_cast<int>(SaveChanges)));
}

void Application::Save(bool DoPrompt)
{
    dynamicCall("Save(QVariant& DoPrompt)", QVariant(DoPrompt));
}

void Application::CommandLine(const QString &CommandLine)
{
    dynamicCall("CommandLine(QVariant& CommandLine)", QVariant(CommandLine));
}

////////////////////////////////////////////////////////////////////////////////
// Formats wrapper class

int Formats::Count()
{
    return dynamicCall("Count").toInt();
}

void Formats::SetCount(int Count)
{
    dynamicCall("SetCount(QVariant& Count)", QVariant(Count));
}

BarTender::Format *Formats::Add()
{
    return reinterpret_cast<BarTender::Format *>(querySubObject("Add()"));
}

BarTender::Format *Formats::Open(const QString &FileName, bool CloseOutFirstFormat, const QString &UsePrinter)
{
    return reinterpret_cast<BarTender::Format *>(querySubObject("Open(QVariant& FileName, QVariant& CloseOutFirstFormat, QVariant& UsePrinter)", QVariant(FileName), QVariant(CloseOutFirstFormat), QVariant(UsePrinter)));
}

QAxObject *Formats::Item(const QVariant &Index)
{
    return querySubObject("Item(QVariant& Index)", Index);
}

QAxObject *Formats::Format(const QVariant &Index)
{
    return querySubObject("GetFormat(QVariant& Index)", Index);
}

////////////////////////////////////////////////////////////////////////////////
// Format wrapper class

bool Format::AutoPrintAgain()
{
    return dynamicCall("AutoPrintAgain").toBool();
}

void Format::SetAutoPrintAgain(bool AutoPrintAgain)
{
    dynamicCall("SetAutoPrintAgain(QVariant& AutoPrintAgain)", QVariant(AutoPrintAgain));
}

QString Format::BaseName()
{
    return dynamicCall("BaseName").toString();
}

void Format::SetBaseName(const QString &BaseName)
{
    dynamicCall("SetBaseName(QVariant& BaseName)", QVariant(BaseName));
}

QString Format::Directory()
{
    return dynamicCall("Directory").toString();
}

void Format::SetDirectory(const QString &Directory)
{
    dynamicCall("SetDirectory(QVariant& Directory)", QVariant(Directory));
}

QString Format::FileName()
{
    return dynamicCall("FileName").toString();
}

void Format::SetFileName(const QString &FileName)
{
    dynamicCall("SetFileName(QVariant& FileName)", QVariant(FileName));
}

int Format::IdenticalCopiesOfLabel()
{
    return dynamicCall("IdenticalCopiesOfLabel").toInt();
}

void Format::SetIdenticalCopiesOfLabel(int IdenticalCopiesOfLabel)
{
    dynamicCall("SetIdenticalCopiesOfLabel(QVariant& IdenticalCopiesOfLabel)", QVariant(IdenticalCopiesOfLabel));
}

QAxObject *Format::InputDataFileSetup()
{
    return querySubObject("InputDataFileSetup");
}

void Format::SetInputDataFileSetup(QAxObject *InputDataFileSetup)
{
    dynamicCall("SetFileName(QVariant& FileName)", InputDataFileSetup->asVariant());
}

int Format::NumberSerializedLabels()
{
    return dynamicCall("NumberSerializedLabels").toInt();
}

void Format::SetNumberSerializedLabels(int NumberSerializedLabels)
{
    dynamicCall("SetNumberSerializedLabels(QVariant& NumberSerializedLabels)", QVariant(NumberSerializedLabels));
}

QString Format::Printer()
{
    return dynamicCall("Printer").toString();
}

void Format::SetPrinter(const QString &Printer)
{
    dynamicCall("SetPrinter(QVariant& Printer)", QVariant(Printer));
}

bool Format::PrintToFile()
{
    return dynamicCall("PrintToFile").toBool();
}

void Format::SetPrintToFile(bool PrintToFile)
{
    dynamicCall("SetPrintToFile(QVariant& PrintToFile)", QVariant(PrintToFile));
}

QString Format::Title()
{
    return dynamicCall("Title").toString();
}

void Format::SetTitle(const QString &Title)
{
    dynamicCall("SetTitle(QVariant& Title)", QVariant(Title));
}

bool Format::UseInputDataFile()
{
    return dynamicCall("UseInputDataFile").toBool();
}

void Format::SetUseInputDataFile(bool UseInputDataFile)
{
    dynamicCall("SetUseInputDataFile(QVariant& UseInputDataFile)", QVariant(UseInputDataFile));
}

BtVersion Format::SaveAsVersion()
{
    return static_cast<BtVersion>(dynamicCall("SaveAsVersion").toInt());
}

void Format::SetSaveAsVersion(BtVersion SaveAsVersion)
{
    dynamicCall("SetSaveAsVersion(QVariant& SaveAsVersion)", QVariant(static_cast<int>(SaveAsVersion)));
}

BarTender::NamedSubStrings *Format::NamedSubStrings()
{
    return reinterpret_cast<BarTender::NamedSubStrings *>(querySubObject("NamedSubStrings"));
}

void Format::SetNamedSubStrings(BarTender::NamedSubStrings *NamedSubStrings)
{
    dynamicCall("SetNamedSubStrings(QVariant& NamedSubStrings)", NamedSubStrings->asVariant());
}

BarTender::DesignObjects *Format::Objects()
{
    return reinterpret_cast<BarTender::DesignObjects *>(querySubObject("Objects"));
}

void Format::SetObjects(BarTender::DesignObjects *Objects)
{
    dynamicCall("SetObjects(QVariant& Objects)", Objects->asVariant());
}

void Format::Activate()
{
    dynamicCall("Activate()");
}

void Format::Close(BtSaveOptions SaveChanges)
{
    dynamicCall("Close(QVariant& SaveChanges)", QVariant(static_cast<int>(SaveChanges)));
}

void Format::PrintOut(bool ShowStatusWindow, bool ShowPrintDialog)
{
    dynamicCall("PrintOut(QVariant& ShowStatusWindow, QVariant& ShowPrintDialog)", QVariant(ShowStatusWindow), QVariant(ShowPrintDialog));
}

void Format::Save()
{
    dynamicCall("Save()");
}

void Format::SaveAs(const QString &FileName, bool OverwriteIfExists)
{
    dynamicCall("SaveAs(QVariant& FileName, QVariant& OverwriteIfExists)", QVariant(FileName), QVariant(OverwriteIfExists));
}

void Format::ExportToClipboard(BtColors Colors, BtResolution Resolution)
{
    dynamicCall("ExportToClipboard(QVariant& Colors, QVariant& Resolution)", QVariant(static_cast<int>(Colors)), QVariant(static_cast<int>(Resolution)));
}

void Format::ExportToFile(const QString &FileName, const QString &FileType, BtColors Colors, BtResolution Resolution, BtSaveOptions SaveOption)
{
    dynamicCall("ExportToFile(QVariant& FileName, QVariant& FileType, QVariant& Colors, QVariant& Resolution, QVariant& SaveOption)", QVariant(FileName), QVariant(FileType), QVariant(static_cast<int>(Colors)), QVariant(static_cast<int>(Resolution)), QVariant(static_cast<int>(SaveOption)));
}

QString Format::NamedSubStringValue(const QString &SubStringName)
{
    return dynamicCall("GetNamedSubStringValue(QVariant& Colors)", QVariant(SubStringName)).toString();
}

void Format::SetNamedSubStringValue(const QString &SubstringName, const QString &Value)
{
    dynamicCall("SetNamedSubStringValue(QVariant& SubstringName, QVariant& Value)", QVariant(SubstringName), QVariant(Value));
}

////////////////////////////////////////////////////////////////////////////////
// NamedSubStrings wrapper class
int NamedSubStrings::Count()
{
    return dynamicCall("Count").toInt();
}

void NamedSubStrings::SetCount(int Count)
{
    dynamicCall("SetCount(QVariant& Count)", QVariant(Count));
}

QAxObject *NamedSubStrings::Item(const QVariant &Index)
{
    return querySubObject("Item(QVariant& Index)", QVariant(Index));
}

QString NamedSubStrings::All(const QString &nameValueDelimiter, const QString &recordDelimiter)
{
    return dynamicCall("GetAll(QVariant& nameValueDelimiter, QVariant& recordDelimiter)", QVariant(nameValueDelimiter), QVariant(recordDelimiter)).toString();
}

void NamedSubStrings::SetAll(const QString &subStringsAndValues, const QString &delimiter)
{
    dynamicCall("SetAll(QVariant& subStringsAndValues, QVariant& delimiter)", QVariant(subStringsAndValues), QVariant(delimiter));
}

QAxObject *NamedSubStrings::SubString(const QVariant &Index)
{
    return querySubObject("GetSubString(QVariant& Index)", QVariant(Index));
}

////////////////////////////////////////////////////////////////////////////////
// SubString wrapper class
QString SubString::Name()
{
    return dynamicCall("Name").toString();
}

void SubString::SetName(const QString &Name)
{
    dynamicCall("SetName(QVariant& Name)", QVariant(Name));
}

QString SubString::Value()
{
    return dynamicCall("Value").toString();
}

void SubString::SetValue(const QString &Value)
{
    dynamicCall("SetValue(QVariant& Value)", QVariant(Value));
}

BtSubStringType SubString::Type()
{
    return static_cast<BtSubStringType>(dynamicCall("Type").toInt());
}

void SubString::SetType(BtSubStringType Type)
{
    dynamicCall("SetType(QVariant& Type)", QVariant(static_cast<int>(Type)));
}

QString SubString::SerializeBy()
{
    return dynamicCall("SerializeBy").toString();
}

void SubString::SetSerializeBy(const QString &SerializeBy)
{
    dynamicCall("SetSerializeBy(QVariant& SerializeBy)", QVariant(SerializeBy));
}

int SubString::SerializeEvery()
{
    return dynamicCall("SerializeEvery").toInt();
}

void SubString::SetSerializeEvery(int SerializeEvery)
{
    dynamicCall("SetSerializeEvery(QVariant& SerializeEvery)", QVariant(SerializeEvery));
}

bool SubString::Rollover()
{
    return dynamicCall("Rollover").toBool();
}

void SubString::SetRollover(bool Rollover)
{
    dynamicCall("SetRollover(QVariant& Rollover)", QVariant(Rollover));
}

QString SubString::RolloverLimit()
{
    return dynamicCall("RolloverLimit").toString();
}

void SubString::SetRolloverLimit(const QString &RolloverLimit)
{
    dynamicCall("SetRolloverLimit(QVariant& RolloverLimit)", QVariant(RolloverLimit));
}

QString SubString::RolloverResetValue()
{
    return dynamicCall("RolloverResetValue").toString();
}

void SubString::SetRolloverResetValue(const QString &RolloverResetValue)
{
    dynamicCall("SetRolloverResetValue(QVariant& RolloverResetValue)", QVariant(RolloverResetValue));
}

////////////////////////////////////////////////////////////////////////////////
// DesignObjects wrapper class
int DesignObjects::Count()
{
    return dynamicCall("Count").toInt();
}

void DesignObjects::SetCount(int Count)
{
    dynamicCall("SetCount(QVariant& Count)", QVariant(Count));
}

QAxObject *DesignObjects::Item(const QVariant &Index)
{
    return querySubObject("Item(QVariant& Index)", QVariant(Index));
}

QAxObject *DesignObjects::Find(const QString &Name)
{
    return querySubObject("Find(QVariant& Name)", QVariant(Name));
}

////////////////////////////////////////////////////////////////////////////////
// DesignObject wrapper class
QString DesignObject::Name()
{
    return dynamicCall("Name").toString();
}

void DesignObject::SetName(const QString &Name)
{
    dynamicCall("SetName(QVariant& Name)", QVariant(Name));
}

QString DesignObject::Value()
{
    return dynamicCall("Value").toString();
}

void DesignObject::SetValue(const QString &Value)
{
    dynamicCall("SetValue(QVariant& Value)", QVariant(Value));
}

}    // namespace BarTender
