#include "CommDefine.h"

const QString COMM_CONFIG_XML = "./DAC.Configuration.xml";

const QMap<DeviceModel, QString> deviceModelMap =
{
    { DeviceModel::Scanner_HONEYWELL,   "条码枪(HONEYWELL)" },
    { DeviceModel::Scanner_DATALOGIC,   "条码枪(DATALOGIC)" },
    { DeviceModel::Tighten_DDK,         "拧紧枪(DDK)" },
    { DeviceModel::Tighten_ATLAS,       "拧紧枪(ATLAS)" },
    { DeviceModel::Tighten_DEPRAG,      "拧紧枪(DEPRAG)" },
    { DeviceModel::Clamp_OETIKER,       "大力钳(OETIKER)" },
    { DeviceModel::Ohmmeter_KEITHLY,    "电阻表(KEITHLY)" },
    { DeviceModel::Ohmmeter_CHROMA,     "欧姆表(CHROMA)" },
    { DeviceModel::Ohmmeter_KEYSIGHT,   "欧姆表(KEYSIGHT)" },
    { DeviceModel::Balance_TOLEDO,      "电子天平(TOLEDO)" },
    { DeviceModel::Weigher_TOLEDO,      "电子台秤(TOLEDO)" },
};
