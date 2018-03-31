#ifndef STATIONOPERATOR_H
#define STATIONOPERATOR_H

#include "WorkDefine.h"

class WorkManager;
class WorkStation;

class StationOperator
{
public:
    explicit StationOperator(WorkStation *station);
    virtual ~StationOperator();

    bool operatorLogin(const QString &barcode);
    bool operatorLogout();

public:
    inline QString getOperatorID() const { return operatorData.operatorID; }
    inline QString getOperatorCard() const { return operatorData.operatorCard; }
    inline QString getOperatorName() const { return operatorData.operatorName; }

    inline const OperatorData &getOperatorData() const { return operatorData; }
private:
    WorkManager *manager;
    WorkStation *station;

    OperatorData operatorData;
};

#endif // STATIONOPERATOR_H
