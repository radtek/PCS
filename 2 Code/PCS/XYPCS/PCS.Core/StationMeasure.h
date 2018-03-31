#ifndef STATIONMEASURE_H
#define STATIONMEASURE_H

#include "WorkDefine.h"

class WorkManager;
class WorkStation;

class StationMeasure
{
public:
    explicit StationMeasure(WorkStation *station);
    virtual ~StationMeasure();

    bool initialMeasure(const QString &measureID);      //初始化

    bool verifyValue(double value);         //判断结果
public:
    inline void setMeasureValue(double value) { measureData.measureValue = value; }
    inline void setMeasureState(int result) { measureData.measureState = result; }
    inline void setRetestCount(int count) { measureData.retestCount = count; }

    inline QString getMeasureID() const { return measureData.measureID; }
    inline const MeasureData &getMeasureData() const { return measureData; }
private:
    WorkManager *manager;
    WorkStation *station;

    MeasureData measureData;
};

#endif // STATIONMEASURE_H
