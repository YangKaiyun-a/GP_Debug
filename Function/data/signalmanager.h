#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H
#include "global.h"

#include <QObject>


class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    static SignalManager* instance();

signals:
    void updateSysTime();
    void backHome();
    void languageChanged(ENUM_CLASS::Current_Language type);
    void updatePipetteDeviation(ENUM_CLASS::Gantry gantry);
    void updateClawInfo(ENUM_CLASS::Gantry gantry);
    void updateRackDeviation(int rackId);
    void showLog(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, const LaserInfo &laserInfo, float deviationX, float deviationY, float deviationZ, float x, float y, float z, float resetZ0, const SingleChannelInfo &singleInfo, const QString &str);

private:
    static SignalManager* m_signalManager;

};

#endif // SIGNALMANAGER_H
