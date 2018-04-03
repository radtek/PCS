/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLogManager.cpp
 *
 * @example
 * @code
 * int main()
 * {
 *      while (true)
 *          qDebug() << XYAutoSerialNumber::createSerialNumber(AutoSerialType::WorkShopCode);
 * }
 * @endcode
 *
 * @brief
 *  10线程下，单次调用约10毫秒
 *  需要表PCS_Base_SerialNumber辅助
 *  日期，例如当前时间为2018-04-01 12:13:14，则l为20180401，s为180401，t为201804
 *
 * @date 2018-04-03 @author weiheng             @note create this file
 * *****************************************************************************
 */

#include "XYAutoSerialNumber.h"

bool XYAutoSerialNumber::EncodingRule(SerialRule &rule)
{
    QString serial;

    rule.currDateTime = QDateTime::currentDateTime();

    QStringList sections = rule.pattern.split('|', QString::SkipEmptyParts);

    for (int i = 0; i < sections.length(); ++i)
    {
        QString section = sections[i];
        if (i != 0)
        {
            serial.append(rule.seq);
        }

        switch (section[0].toLatin1())
        {
        case 'P':
        {
            if (section.length() != 1)
            {
                return false;
            }
            serial.append(rule.prefix);
            break;
        }
        case 'N':
        {
            // 拆分
            QStringList sub = section.split(':', QString::SkipEmptyParts);
            if (sub.length() != 2)
            {
                return false;
            }
            // 数字转换
            bool ok = false;
            int count = sub[1].toInt(&ok);
            if (!ok)
            {
                return false;
            }

            serial.append(QString("%1").arg(rule.lastNumber, count, rule.numBase, rule.numFill[0]));

            // 成功后+1
            rule.currNumber = rule.lastNumber + 1;
            break;
        }
        case 'D':
        {
            // 拆分
            QStringList sub = section.split(':', QString::SkipEmptyParts);
            if (sub.length() != 2)
            {
                return false;
            }

            // 流水号在日期前面，不重置流水号
            switch (sub[1].toLatin1()[0])
            {
            case 'l':
            {
                serial.append(rule.lastDateTime.toString("yyyyMMdd"));
                if (rule.currDateTime.date() != rule.lastDateTime.date())
                {
                    rule.lastNumber = 1;
                }
                break;
            }
            case 's':
            {
                serial.append(rule.lastDateTime.toString("yyMMdd"));
                if (rule.currDateTime.date() != rule.lastDateTime.date())
                {
                    rule.lastNumber = 1;
                }
                break;
            }
            case 't':
            {
                serial.append(rule.lastDateTime.toString("yyMM"));
                if (rule.currDateTime.date().year() != rule.lastDateTime.date().year() ||
                    rule.currDateTime.date().month() != rule.lastDateTime.date().month())
                {
                    rule.lastNumber = 1;
                }
                break;
            }
            default:
                return false;
            }
            break;
        }
        }
    }

    rule.lastSerialNumber = serial;
    return true;
}

QString XYAutoSerialNumber::createSerialNumber(AutoSerialType type, QSqlDatabase db)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString SerialNo;

    do
    {
        QSqlQuery dbQuery(db);

        dbQuery.prepare(R"( SELECT
            [Prefix],[SerialRule],[LastNumber],[LastDateTime],[LastSerialNumber],[Sep],[NumFill],[NumBase]
            FROM [PCS_Base_SerialNumber]
            WHERE [Type] = :Type AND [State] = :State          )");
        dbQuery.bindValue(":Type", static_cast<int>(type));
        dbQuery.bindValue(":State", static_cast<int>(RecordState::Enable));
        if (!dbQuery.exec())
        {
            qWarning() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            break;
        }
        if (!dbQuery.first())
        {
            break;
        }

        SerialRule rule;
        rule.pattern = dbQuery.value("SerialRule").toString();
        rule.prefix = dbQuery.value("Prefix").toString();
        rule.seq = dbQuery.value("Sep").toString();
        rule.numFill = dbQuery.value("NumFill").toString();
        rule.numBase = dbQuery.value("NumBase").toInt();
        rule.lastNumber = dbQuery.value("LastNumber").toInt();
        rule.lastDateTime = dbQuery.value("LastDateTime").toDateTime();

        if (!EncodingRule(rule))
        {
            break;
        }

        // 回写
        dbQuery.prepare(R"( UPDATE [PCS_Base_SerialNumber] SET
                        [LastNumber] = :LastNumber,
                        [LastDateTime] = :LastDateTime,
                        [LastSerialNumber] = :LastSerialNumber
                        WHERE [Type] = :Type AND [State] = :State          )");
        dbQuery.bindValue(":LastNumber", rule.currNumber);
        dbQuery.bindValue(":LastDateTime", rule.currDateTime);
        dbQuery.bindValue(":LastSerialNumber", rule.lastSerialNumber);
        dbQuery.bindValue(":Type", static_cast<int>(type));
        dbQuery.bindValue(":State", static_cast<int>(RecordState::Enable));
        if (!dbQuery.exec())
        {
            qWarning() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            break;
        }

        SerialNo = rule.lastSerialNumber;
    } while (0);

    return SerialNo;
}
