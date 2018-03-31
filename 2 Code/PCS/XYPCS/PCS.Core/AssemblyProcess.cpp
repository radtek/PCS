#include "AssemblyProcess.h"
#include "AssemblyStation.h"
#include "ProcessStep.h"
#include "StepLoopStart.h"
#include "StepLoopEnd.h"
#include "StepAutoMotion.h"
#include "StepScanBarcode.h"
#include "StepMeasureData.h"

AssemblyProcess::AssemblyProcess(AssemblyStation *station)
    : station(station)
    , currentStep(nullptr)
    , processIndex(0)
{

}

AssemblyProcess::~AssemblyProcess()
{
    for (auto ptr : stepList)
    {
        delete ptr;
    }

    stepList.clear();
}

bool AssemblyProcess::loadProcess(const QList<StepPara> &stepParaList)
{
    for (auto &stepPara : stepParaList)
    {
        ProcessStep *step = nullptr;

        switch (stepPara.stepType)
        {
        case StepType::LoopStart:
        {
            step = new StepLoopStart(this);
            auto ptr = dynamic_cast<StepLoopStart *>(step);
            ptr->setStepItem(stepPara.startItem);
            break;
        }

        case StepType::LoopEnd:
        {
            step = new StepLoopEnd(this);
            auto ptr = dynamic_cast<StepLoopEnd *>(step);
            ptr->setStepItem(stepPara.endItem);
            break;
        }

        case StepType::AutoMotion:
        {
            step = new StepAutoMotion(this);
            auto ptr = dynamic_cast<StepAutoMotion *>(step);
            ptr->setStepItem(stepPara.motionItem);
            break;
        }

        case StepType::ScanBarcode:
        {
            step = new StepScanBarcode(this);
            auto ptr = dynamic_cast<StepScanBarcode *>(step);
            ptr->setStepItemList(stepPara.barcodeItemList);
            break;
        }

        case StepType::MeasureData:
        {
            step = new StepMeasureData(this);
            auto ptr = dynamic_cast<StepMeasureData *>(step);
            ptr->setStepItemList(stepPara.measureItemList);
            break;
        }
        }

        if (step == nullptr)
        {
            return false;
        }

        step->setStepItem(stepPara.stepItem);
        step->setStepType(stepPara.stepType);

        stepList.append(step);
    }

    initStepData();

    return true;
}

bool AssemblyProcess::execValueChanged(const DataDefine &data)
{
    return (currentStep == nullptr) ? false : currentStep->opcValueChanged(data);
}

bool AssemblyProcess::execReadComplete(const DataDefine &data)
{
    return (currentStep == nullptr) ? false : currentStep->opcReadComplete(data);
}

bool AssemblyProcess::execDataReceived(const DataDefine &data)
{
    return (currentStep == nullptr) ? false : currentStep->dacDataReceived(data);
}

//重置步骤
void AssemblyProcess::initStepData()
{
    for (auto step : stepList)
    {
        step->initStepData();
    }

    processData.initial();
    materialList.clear();
    assemblyList.clear();

    currentStep = stepList.first();
}

void AssemblyProcess::saveStepData()
{
    for (auto step : stepList)
    {
        step->saveStepData();
    }

    if (1 == processData.subAssemblyIDs.size())
    {
        processData.assemblyID = processData.subAssemblyIDs.takeFirst();
    }
    else
    {
        processData.assemblyID = QUuid::createUuid().toString().remove('{').remove('}');
    }

    station->saveStationData(processData);
}

//初始化工位数据
void AssemblyProcess::finishStep(ProcessStep *step)
{
    int index = stepList.indexOf(step);

    if (++index == stepList.size())
    {
        station->finishProcess(this);
    }
    else
    {
        currentStep = stepList.at(index);
    }
}

EventState AssemblyProcess::verifyProcessResult()
{
    if (processData.assemblyState == 1)
    {
        return EventState::E_WorkFinished_Failed;
    }

    if (processData.repairState == 1)
    {
        return EventState::E_WorkFinished_Repair;
    }

    if (processData.retestState == 1)
    {
        return EventState::E_WorkFinished_Retest;
    }

    return EventState::E_WorkFinished_Normal;
}
