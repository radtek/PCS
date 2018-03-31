#ifndef ASSEMBLYPROCESS_H
#define ASSEMBLYPROCESS_H

#include "WorkDefine.h"

class AssemblyStation;
class ProcessStep;

class AssemblyProcess
{
public:
    explicit AssemblyProcess(AssemblyStation *station);
    virtual ~AssemblyProcess();

    bool loadProcess(const QList<StepPara> &stepParaList);
public:
    void initStepData();
    void saveStepData();

    void finishStep(ProcessStep *currentStep);
    EventState verifyProcessResult();

    inline void appendMaterial(const QString &id) { materialList.append(id); }
    inline void appendAssemlby(const QString &id) { assemblyList.append(id); }
    inline void updateAssemblyID(const QString &id) { processData.subAssemblyIDs.append(id); }

    bool findMaterial(const QString &material) { return materialList.contains(material); }
    bool findAssembly(const QString &assembly) { return assemblyList.contains(assembly); }
public:
    bool execValueChanged(const DataDefine &data);
    bool execReadComplete(const DataDefine &data);
    bool execDataReceived(const DataDefine &data);
public:
    inline AssemblyStation *getStation() const { return station; }
    inline ProcessStep *getCurrentStep() const { return currentStep; }

    inline bool verifyAssemblyState() const { return processData.assemblyState == 0; }
    inline bool verifyRepairState() const { return processData.repairState == 0; }
    inline bool verifyRetestState() const { return processData.retestState == 0; }
    inline void updateAssemblyState() { processData.assemblyState = 1; }
    inline void updateRepairState() { processData.repairState = 1; }
    inline void updateRetestState() { processData.retestState = 1; }
    inline void updateCreateTime() { processData.createTime = QDateTime::currentDateTime(); }
    inline void updateFinishTime() { processData.finishTime = QDateTime::currentDateTime(); }

    inline void setProcessIndex(int index) { processIndex = index; }
    inline int getProcessIndex() const { return processIndex; }

private:
    AssemblyStation *station;
    ProcessStep *currentStep;

    QList<ProcessStep *> stepList;

    ProcessData processData;

    QStringList materialList;
    QStringList assemblyList;

    int processIndex;
};

#endif // ASSEMBLYPROCESS_H
