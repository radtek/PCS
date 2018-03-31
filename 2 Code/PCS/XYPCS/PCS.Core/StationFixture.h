#ifndef STATIONFIXTURE_H
#define STATIONFIXTURE_H

#include "WorkDefine.h"

class WorkManager;
class WorkStation;

class StationFixture
{
public:
    explicit StationFixture(WorkStation *station);
    virtual ~StationFixture();

    bool loadFixture(const QString &fixtureID);
    void updateUsedTimes();                            //扣减使用次数

public:
    //是否报废
    inline bool isTimesUseup() const { return fixtureData.usedTimes >= fixtureData.totalTimes; }
    //是否提醒
    inline bool isTimesAlert() const { return fixtureData.usedTimes >= fixtureData.remindTimes; }

    inline void setFixutreClassify(const QString &classify) { fixtureData.fixtureClassify = classify; }
    inline QString getFixtureID() const { return fixtureData.fixtureID; }
    inline const FixtureData &getFixtureData() const { return fixtureData; }
private:
    WorkManager *manager;
    WorkStation *station;

    FixtureData fixtureData;
};

#endif // STATIONFIXTURE_H
