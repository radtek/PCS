#ifndef WORKCRAFT_H
#define WORKCRAFT_H

#include "WorkDefine.h"

class WorkManager;

class WorkCraft
{
public:
    explicit WorkCraft(WorkManager *manager);
    virtual ~WorkCraft();

public:
    bool initialCraft(const QString &craftID);
public:
    inline QString getCraftID() const { return craftData.craftID; }
    inline QString getProductID() const { return craftData.productID; }

    const CraftData &getCraftData() const { return craftData; }
private:
    WorkManager *manager;

    CraftData craftData;   //工艺属性
};

#endif // WORKCRAFT_H
