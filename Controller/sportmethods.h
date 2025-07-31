#ifndef SPORTMETHODS_H
#define SPORTMETHODS_H

#include "Function/data/global.h"

#include <QObject>

class SportMethods : public QObject
{
    Q_OBJECT
public:
    explicit SportMethods(QObject *parent = nullptr);

    //单例
    static SportMethods* instance();


    /****************DJ1******************/

    //运动至移液器偏差校准位
    void moveToPipetteAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float deviationZ);
    //运动至载架校准位
    void moveToRackAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float deviationZ);
    //运动至夹爪校准位
    void moveToClawAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ);
    //运动至转运校准位
    void moveToTransportAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ);


    /****************DJ2******************/

    //运动至移液器偏差校准位
    void moveToPipetteAccuratingPositionDJ2(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float deviationZ);


    /****************DP1******************/

    //运动至原点
    void moveToOriginPosition(const LaserInfo &laserInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY);
    //运动至原点偏移位
    void moveToOriginPositionOffset(const LaserInfo &laserInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY);
    //运动至载架校准点
    void moveToRackAccuratingPosition(const LaserInfo &laserInfo, const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY);
    //运动至载架校准点偏移位
    void moveToRackAccuratingPositionOffset(const LaserInfo &laserInfo, const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY);
    //运动至移液器偏差校准位
    void moveToPipetteAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo);
    //运动至夹爪校准位
    void moveToClawAccuratingPositionDP1(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ);


    /****************DP8******************/

    //运动至原点
    void moveToOriginPosition(const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0);
    //运动至原点偏移位
    void moveToOriginPositionOffset(const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0);
    //运动至载架校准点
    void moveToRackAccuratingPositionDP8(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0);
    //运动至载架校准点偏移位
    void moveToRackAccuratingPositionOffset(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0);
    //运动至移液器偏差校准位
    void moveToPipetteAccuratingPositionDP8(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float resetZ0);
    //运动至夹爪校准位
    void moveToClawAccuratingPositionDP8(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ);
    //运动至PCR校准位
    void moveToPcrAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ);
    //运动至单通道校准位
    void moveToSingleChannelAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const SingleChannelInfo &singleInfo, float deviationX, float deviationY, float deviationZ);


    /****************QPCR夹爪******************/

    //运动至QPCR夹爪校准位
    void moveToQPcrAccuratingPosition(const Storage_Info &storageInfo, float deviationX, float deviationZ);



    /****************执行python脚本******************/
    void executePythonScript();


signals:

private:
    static SportMethods* m_sportMethods;
};

#endif // SPORTMETHODS_H
