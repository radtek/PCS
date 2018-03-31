#ifndef XYSERIALMANAGER_H
#define XYSERIALMANAGER_H

#include "CommDefine.h"
#include "CommDevice.h"
#include "GlobalData.h"

class CommManager : public QObject
{
    Q_OBJECT
private:
    explicit CommManager();
    virtual ~CommManager();

    CommDevice *createDevice(const QString &stationID, const QString &deviceID, const QString &deviceType);
    void removeDevice(CommDevice *device);
    void removeAllDevice();

public:
    static CommManager *instance()
    {
        static CommManager manager;
        return &manager;
    }

    bool readXmlFile(const QString &filename);
    bool saveXmlFile(const QString &filename);

    QStringList getStationIDList() const;
    QStringList getDeviceIDList(const QString &stationID) const;
    QStringList getItemTypeList(const QString &deviceID) const;
    QStringList getItemIDList(const QString &stationID) const;

    CommDeviceList getDeviceList() const;
    CommDeviceList getDeviceList(const QString &stationID) const;

    void startCommThread();
    void endCommThread();

public slots:
    void commWriteData(const QString &stationID, const DataDefine &data);
signals:
    void commDataReceived(const QString &stationID, const DataDefine &data);
    void commErrorOccurred(const QString &stationID, const ErrorDefine &error);

private:
    CommDeviceList deviceList;
};

#define qCommManager (CommManager::instance())

#endif    // XYSERIALMANAGER_H
