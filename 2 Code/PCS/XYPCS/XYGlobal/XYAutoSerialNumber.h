/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYAutoSerialNumber.h
 * @brief   自动编号
 * @version 1.0
 * @note
 *  需要数据库辅助使用
 * *****************************************************************************
 */

#ifndef XYTECH_XYAUTOSERIALNUMBER_H
#define XYTECH_XYAUTOSERIALNUMBER_H

#include <QtCore>
#include <QtSql>

enum class AutoSerialType
{
    // 车间编号
    WorkShopCode = 100,
    // 产线编号
    WorkLineCode = 101,
    // 工位编号
    WorkStationCode = 102,
    // 工艺编号
    CraftCode = 103,
    // 测试编号
    MeasureCode = 104,
    // 物料编号
    MaterialCode = 105,
    // 工单编号
    OrderCode = 106,
    // 工装编号
    FixtureCode = 107,
    // 员工编号
    WorkerCode = 108,
    // 产品编号
    ProductCode = 109,

};

enum class RecordState
{
    Disable = 0,
    Enable = 1
};

class XYAutoSerialNumber
{
public:
    struct SerialRule
    {
        QString pattern;
        QString prefix;
        QString seq;
        QString numFill;
        int numBase;
        int lastNumber;
        QDateTime lastDateTime;
        QString lastSerialNumber;
        int currNumber;
        QDateTime currDateTime;
    };

public:
    static QString createSerialNumber(AutoSerialType type, QSqlDatabase db = QSqlDatabase::database());

private:
    static bool EncodingRule(SerialRule &rule);
};

#endif
