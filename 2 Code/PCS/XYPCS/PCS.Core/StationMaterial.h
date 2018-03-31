#ifndef STATIONMATERIAL_H
#define STATIONMATERIAL_H

#include "WorkDefine.h"

class WorkManager;
class WorkStation;

class StationMaterial
{
public:
    explicit StationMaterial(WorkStation *station);
    virtual ~StationMaterial();

    bool initialMaterial(const QString &materialID);    //初始化物料

    //!条码验证程序
    bool verifyBarcodeFormat(const QString &barcode);    //校验条码格式是否正确
    bool verifyBarcodeUnique(const QString &barcode);    //校验条码是否唯一
    bool verifyBarcodeRepair(const QString &barcode);    //校验条码是否返修

    bool inputMaterialBatch(const QString &batch, int quantity);
    bool inputMaterialBatch(const QString &barcode);    //投入物料批次
    bool returnMaterialBatch();                         //退回物料批次
    void adjustBatchQuantity(int quantity);             //校准批次数量
    void updateBatchQuantity();                         //更新批次数量
public:
    inline bool isBatchInput() const { return !materialData.materialBatch.isEmpty() /* && !materialData.packageBatch.isEmpty()*/; }
    //批次数量是否发出警告
    inline bool isBatchAlarm() const { return materialData.batchQuantity < materialData.alarmQuantity; }
    //批次数量是否已经耗尽
    inline bool isBatchUseup() const { return materialData.batchQuantity < materialData.usageQuantity; }

    inline void setMaterialBarcode(const QString &barcode) { materialData.materialBarcode = barcode; }
    inline void setRepairCount(int count) { materialData.repairCount = count; }

    inline QString getMaterialID() const { return materialData.materialID; }
    inline const MaterialData &getMaterialData() const { return materialData; }

private:
    WorkManager *manager;
    WorkStation *station;

    MaterialData materialData;
};

#endif    // STATIONMATERIAL_H
