#include "sportmethods.h"
#include "Controller/Amplification_Dj1/amplification_dj1.h"
#include "Controller/Extract_Dj1/extract_dj1.h"
#include "Controller/Dp1_xy/dp1_xy.h"
#include "Controller/Dp1/dp1.h"
#include "Controller/Dp8_xy/dp8_xy.h"
#include "Controller/Dp8/dp8.h"
#include "Controller/xzclaw/xzclaw_control.h"
#include "application.h"

#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QAxObject>


const float DJ1_XY_SPEED = 150.0;
const float DJ1_Z_SPEED = 80.0;

const float DP1_XY_SPEED = 150.0;
const float DP1_Z_SPEED = 80.0;

const float DP8_XY_SPEED = 150.0;
const float DP8_Z_SPEED = 80.0;

const float QPCR_CLAW_XY_SPEED = 50;
const float QPCR_CLAW_Z_SPEED = 50;


SportMethods* SportMethods::m_sportMethods = nullptr;

SportMethods::SportMethods(QObject *parent)
    : QObject{parent}
{

}

SportMethods *SportMethods::instance()
{
    if(nullptr == m_sportMethods)
    {
        m_sportMethods = new SportMethods();
    }

    return m_sportMethods;
}



/*-----------------------------------------------------------------
* @函数名称         moveToPipetteAccuratingPosition
* @创建时间         2024/5/16
* @创建人           yangkaiyun
* @函数说明         运动至移液器偏差校准位（DJ1）
* @输入参数
* @计算公式         X：-（校准点坐标 + 标记偏差坐标 - 移液器坐标）- 输入x偏差
*                  Y：校准点坐标 + 标记偏差坐标 - 移液器坐标 + 输入y偏差
*                  Z：校准点坐标 + 标记偏差坐标 - 移液器坐标 + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToPipetteAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = -(storageInfo.loc_x + storageInfo.crossing_x - pipetteInfo.distance_x_from_orig) - deviationX;
    float y = storageInfo.loc_y + storageInfo.crossing_y - pipetteInfo.distance_y_from_orig + deviationY;
    float z = storageInfo.loc_z + storageInfo.crossing_z - pipetteInfo.distance_z_from_orig + deviationZ;

    QString str1 = "X： -（校准点坐标 + 标记偏差坐标 - 移液器坐标）- 输入偏差\n";
    QString str2 = "Y、Z： 校准点坐标 + 标记偏差坐标 - 移液器坐标 + 输入偏差";

    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2);

    //移液器复位
    Amplification_Dj1::instance()->module_reset();

    //运动至指定X、Y位置
    Amplification_Dj1::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DJ1_XY_SPEED;
    param.positions << x << y;
    Amplification_Dj1::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.targetPosition = z;
    param2.speed = DJ1_Z_SPEED;
    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_DJ1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToRackAccuratingPosition
* @创建时间         2024/5/17
* @创建人           yangkaiyun
* @函数说明         运动至载架校准位
* @输入参数
* @计算公式         X：-((校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差)) - 输入x偏差
*                  Y：(校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToRackAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = -((storageInfo.loc_x + storageInfo.crossing_x) - (pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x)) - deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y) - (pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z) - (pipetteInfo.distance_z_from_orig - pipetteInfo.deviate_z) + deviationZ;

    QString str1 = "X： -((校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差)) - 输入x偏差\n";
    QString str2 = "Y、Z： (校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差) + 输入偏差";

    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2);

    //移液器复位
    Amplification_Dj1::instance()->module_reset();

    //运动至指定X、Y位置
    Amplification_Dj1::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DJ1_XY_SPEED;
    param.positions << x << y;
    Amplification_Dj1::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.targetPosition = z;
    param2.speed = DJ1_Z_SPEED;
    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_DJ1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToClawAccuratingPosition
* @创建时间         2024/5/17
* @创建人           yangkaiyun
* @函数说明         运动至夹爪校准位
* @输入参数
* @计算公式         X：-((校准点坐标 + 校准点标记偏差 + 校准点板位偏差) - (夹爪坐标 - 移液器偏差)) - 输入x偏差
*                  Y：(校准点坐标 + 校准点标记偏差 + 校准点板位偏差) - (夹爪坐标 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 校准点标记偏差 + 校准点板位偏差 + 4.9) - (夹爪坐标 - 移液器偏差) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToClawAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = -((storageInfo.loc_x + storageInfo.crossing_x + storageInfo.deviation_x) - (clawInfo.loc_x - pipetteInfo.deviate_x)) - deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y + storageInfo.deviation_y) - (clawInfo.loc_y - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z + storageInfo.deviation_z + 4.9) - (clawInfo.loc_z - pipetteInfo.deviate_z) + deviationZ;

    QString str1 = "X：-((校准点坐标 + 校准点标记偏差 + 校准点板位偏差) - (夹爪坐标 - 移液器偏差)) - 输入x偏差\n";
    QString str2 = "Y：(校准点坐标 + 校准点标记偏差 + 校准点板位偏差) - (夹爪坐标 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 校准点标记偏差 + 校准点板位偏差 + 4.9) - (夹爪坐标 - 移液器偏差) + 输入z偏差";

    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, clawInfo, LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器夹爪Z复位
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_CLAW_Z);

    //运动至指定X、Y位置
    Amplification_Dj1::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DJ1_XY_SPEED;
    param.positions << x << y;
    Amplification_Dj1::instance()->move(param);

    //运动至指定Z位置（夹爪Z）
    DevicePart::MotorMotionParams param2;
    param2.targetPosition = z;
    param2.speed = DJ1_Z_SPEED;
    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_CLAW_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToTransportAccuratingPosition
* @创建时间         2024/5/17
* @创建人           yangkaiyun
* @函数说明         运动至转运校准位
* @输入参数
* @计算公式         X：-((校准点坐标 + 校准点标记偏差) - (移液器坐标 - 移液器偏差 + 夹爪相对葫芦头位置的最终值) + 输入x偏差)
*                  Y：(校准点坐标 + 校准点标记偏差) - (移液器坐标 - 移液器偏差 + 夹爪相对葫芦头位置的最终值) + 输入y偏差
*                  Z：(校准点坐标 + 校准点标记偏差) - (移液器坐标 - 移液器偏差 + 夹爪相对葫芦头位置的最终值) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToTransportAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = -((storageInfo.loc_x + storageInfo.crossing_x) - (pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x + clawInfo.distance_end_x) + deviationX);
    float y = (storageInfo.loc_y + storageInfo.crossing_y) - (pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y + clawInfo.distance_end_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z) - (pipetteInfo.distance_z_from_orig - pipetteInfo.deviate_z + clawInfo.distance_end_z) + deviationZ;

    QString str1 = "X：-((校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差 + 夹爪相对葫芦头位置的最终值) + 输入x偏差)\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差 + 夹爪相对葫芦头位置的最终值) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差) - (移液器坐标 - 移液器偏差 + 夹爪相对葫芦头位置的最终值) + 输入z偏差";

    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, clawInfo, LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器夹爪Z复位
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_CLAW_Z);

    //运动至指定X、Y位置
    Amplification_Dj1::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DJ1_XY_SPEED;
    param.positions << x << y;
    Amplification_Dj1::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.targetPosition = z;
    param2.speed = DJ1_Z_SPEED;
    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_CLAW_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToPipetteAccuratingPositionDJ2
* @创建时间         2024/6/18
* @创建人           yangkaiyun
* @函数说明         运动至移液器偏差校准位
* @输入参数
* @计算公式         X：-（校准点坐标 + 机位偏差 + 标记偏差坐标 - 移液器复位位置）- 输入x偏差
*                  Y：校准点坐标 + 机位偏差 + 标记偏差坐标 - 移液器复位位置 + 输入y偏差
*                  Z：校准点坐标 + 机位偏差 + 标记偏差坐标 - 移液器复位位置 + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToPipetteAccuratingPositionDJ2(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = -(storageInfo.loc_x + storageInfo.deviation_x + storageInfo.crossing_x - pipetteInfo.distance_x_from_orig) - deviationX;
    float y = storageInfo.loc_y + storageInfo.deviation_y + storageInfo.crossing_y - pipetteInfo.distance_y_from_orig + deviationY;
    float z = storageInfo.loc_z + storageInfo.deviation_z + storageInfo.crossing_z - pipetteInfo.distance_z_from_orig + deviationZ;

    QString str1 = "X：-（校准点坐标 + 机位偏差 + 标记偏差坐标 - 移液器复位位置）- 输入x偏差\n";
    QString str2 = "Y：校准点坐标 + 机位偏差 + 标记偏差坐标 - 移液器复位位置 + 输入y偏差\n";
    QString str3 = "Z：校准点坐标 + 机位偏差 + 标记偏差坐标 - 移液器复位位置 + 输入z偏差";

    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器复位
    Extract_DJ1::instance()->module_reset();

    //运动至指定X、Y位置
    Extract_DJ1::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DJ1_XY_SPEED;
    param.positions << x << y;
    Extract_DJ1::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.targetPosition = z;
    param2.speed = DJ1_Z_SPEED;
    Extract_DJ1::instance()->motorMove(Extract_DJ1::MOTOR_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToOriginPosition
* @创建时间         2024/5/18
* @创建人           yangkaiyun
* @函数说明         运动至原点
* @输入参数
* @计算公式         X：原点坐标 + 原点标记偏差 - 激光坐标 + 输入x偏差
*                  Y：原点坐标 + 原点标记偏差 - 激光坐标 + 输入y偏差
*                  Z：原点坐标 + 原点标记偏差 - 激光坐标 + 初始移液器z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToOriginPosition(const LaserInfo &laserInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY)
{
    //计算目标点位
    float x = 0 + 0 - laserInfo.loc_x + deviationX;
    float y = 0 + 0 - laserInfo.loc_y + deviationY;
    float z = 0 + 0 - laserInfo.loc_z + pipetteInfo.deviate_z;

    QString str1 = "X：- 激光坐标 + 输入x偏差\n";
    QString str2 = "Y：- 激光坐标 + 输入y偏差\n";
    QString str3 = "Z：- 激光坐标 + 初始移液器z偏差";

    Q_EMIT hApp->m_sigmanager->showLog(Storage_Info(), pipetteInfo, ClawInfo(), laserInfo, deviationX, deviationY, -1, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);

    //运动至指定X、Y位置
    Dp1_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP1_XY_SPEED;
    param.positions << x << y;
    Dp1_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP1_Z_SPEED;
    param2.targetPosition = z;
    Dp1::instance()->motorMove(Dp1::MOTOR_Dp1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToOriginPosition
* @创建时间         2024/5/18
* @创建人           yangkaiyun
* @函数说明         运动至原点偏移位
* @输入参数
* @计算公式         X：原点坐标 + 原点标记偏差 - 激光坐标 + 输入x偏差 + 1
*                  Y：原点坐标 + 原点标记偏差 - 激光坐标 + 输入y偏差 + 1
*                  Z：原点坐标 + 原点标记偏差 - 激光坐标 + 初始移液器z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToOriginPositionOffset(const LaserInfo &laserInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY)
{
    //计算目标点位
    float x = 0 + 0 - laserInfo.loc_x + deviationX + 1;
    float y = 0 + 0 - laserInfo.loc_y + deviationY + 1;
    float z = 0 + 0 - laserInfo.loc_z + pipetteInfo.deviate_z;

    QString str1 = "X：- 激光坐标 + 输入x偏差 + 1\n";
    QString str2 = "Y：- 激光坐标 + 输入y偏差 + 1\n";
    QString str3 = "Z：- 激光坐标 + 初始移液器z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(Storage_Info(), PipetteInfo(), ClawInfo(), laserInfo, deviationX, deviationY, -1, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);

    //运动至指定X、Y位置
    Dp1_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP1_XY_SPEED;
    param.positions << x << y;
    Dp1_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP1_Z_SPEED;
    param2.targetPosition = z;
    Dp1::instance()->motorMove(Dp1::MOTOR_Dp1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToRackAccuratingPosition
* @创建时间         2024/5/20
* @创建人           yangkaiyun
* @函数说明         运动至载架校准点
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入x偏差
*                  Y：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 原始z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToRackAccuratingPosition(const LaserInfo &laserInfo, const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x) - (laserInfo.loc_x - pipetteInfo.deviate_x) + deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y) - (laserInfo.loc_y - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z) - (laserInfo.loc_z - pipetteInfo.deviate_z) + storageInfo.deviation_z;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入x偏差\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 机位偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), laserInfo, deviationX, deviationY, -1, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);

    //运动至指定X、Y位置
    Dp1_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP1_XY_SPEED;
    param.positions << x << y;
    Dp1_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP1_Z_SPEED;
    param2.targetPosition = z;
    Dp1::instance()->motorMove(Dp1::MOTOR_Dp1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToRackAccuratingPositionOffset
* @创建时间         2024/5/20
* @创建人           yangkaiyun
* @函数说明         运动至载架校准点偏移位
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入x偏差 + 1
*                  Y：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入y偏差 + 1
*                  Z：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 原始z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToRackAccuratingPositionOffset(const LaserInfo &laserInfo, const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x) - (laserInfo.loc_x - pipetteInfo.deviate_x) + deviationX + 1;
    float y = (storageInfo.loc_y + storageInfo.crossing_y) - (laserInfo.loc_y - pipetteInfo.deviate_y) + deviationY + 1;
    float z = (storageInfo.loc_z + storageInfo.crossing_z) - (laserInfo.loc_z - pipetteInfo.deviate_z) + storageInfo.deviation_z;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入x偏差 + 1\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 输入y偏差 + 1\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标) - (激光坐标 - 移液器偏差) + 机位偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), laserInfo, deviationX, deviationY, -1, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);

    //运动至指定X、Y位置
    Dp1_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP1_XY_SPEED;
    param.positions << x << y;
    Dp1_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP1_Z_SPEED;
    param2.targetPosition = z;
    Dp1::instance()->motorMove(Dp1::MOTOR_Dp1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToPipetteAccuratingPosition
* @创建时间         2024/5/20
* @创建人           yangkaiyun
* @函数说明         运动至移液器偏差校准位
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (移液器复位位置 - 移液器偏差) - 14 - 4.5
*                  Y：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (移液器复位位置 - 移液器偏差) + 247.85 - 4.5
*                  Z：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (移液器复位位置 - 移液器偏差) + 42 - 100
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToPipetteAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x + storageInfo.deviation_x) - (pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x) - 14 - 4.5;
    float y = (storageInfo.loc_y + storageInfo.crossing_y + storageInfo.deviation_y) - (pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y) + 247.85 - 4.5;
    float z = (storageInfo.loc_z + storageInfo.crossing_z + storageInfo.deviation_z) - (pipetteInfo.distance_z_from_orig - pipetteInfo.deviate_z) + 42 - 100;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (移液器复位位置 - 移液器偏差) - 14 - 4.5\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (移液器复位位置 - 移液器偏差) + 247.85 - 4.5\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (移液器复位位置 - 移液器偏差) + 42 - 100";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), -1, -1, -1, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);

    //运动至指定X、Y位置
    Dp1_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP1_XY_SPEED;
    param.positions << x << y;
    Dp1_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP1_Z_SPEED;
    param2.targetPosition = z;
    Dp1::instance()->motorMove(Dp1::MOTOR_Dp1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToClawAccuratingPositionDP1
* @创建时间         2024/5/20
* @创建人           yangkaiyun
* @函数说明         运动至夹爪校准位
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入x偏差
*                  Y：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 0.5 - 5) - (夹爪坐标 - 移液器偏差) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToClawAccuratingPositionDP1(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x + storageInfo.deviation_x) - (clawInfo.loc_x - pipetteInfo.deviate_x) + deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y + storageInfo.deviation_y) - (clawInfo.loc_y - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z + storageInfo.deviation_z - 5.5) - (clawInfo.loc_z - pipetteInfo.deviate_z) + deviationZ;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入x偏差\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 0.5 - 5) - (夹爪坐标 - 移液器偏差) + 输入z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, clawInfo, LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);
    //夹爪Z轴复位
    Dp1::instance()->motorReset(Dp1::MOTOR_CLAW_Z);

    //运动至指定X、Y位置
    Dp1_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP1_XY_SPEED;
    param.positions << x << y;
    Dp1_Xy::instance()->move(param);

    //运动至指定Z位置（夹爪Z）
    DevicePart::MotorMotionParams param2;
    param2.speed = DP1_Z_SPEED;
    param2.targetPosition = z;
    Dp1::instance()->motorMove(Dp1::MOTOR_CLAW_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToOriginPosition
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至原点校准位
* @输入参数
* @计算公式         X：原点坐标 + 原点标记偏差坐标 - 移液器复位坐标 + 输入x偏差
*                  Y：原点坐标 + 原点标记偏差坐标 - 移液器复位坐标 + 输入y偏差
*                  Z：原点坐标 + 原点标记偏差坐标 - 激光复位Z0（全局） + 原始移液器z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToOriginPosition(const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0)
{
    //计算目标点位
    float x = 0 + 0 - pipetteInfo.distance_x_from_orig + deviationX;
    float y = 0 + 0 - pipetteInfo.distance_y_from_orig + deviationY;
    float z = 0 + 0 - resetZ0 + pipetteInfo.deviate_z;

    QString str1 = "X：- 移液器复位位置 + 输入x偏差\n";
    QString str2 = "Y：- 移液器复位位置 + 输入y偏差\n";
    QString str3 = "Z：- 激光复位Z0（全局） + 原始移液器z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(Storage_Info(), pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, -1, x, y, z, resetZ0, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToOriginPositionOffset
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至原点偏移位
* @输入参数
* @计算公式         X：原点坐标 + 原点标记偏差坐标 - 移液器复位坐标 + 输入x偏差 + 1
*                  Y：原点坐标 + 原点标记偏差坐标 - 移液器复位坐标 + 输入y偏差 + 1
*                  Z：原点坐标 + 原点标记偏差坐标 - 激光复位Z0（全局） + 原始移液器z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToOriginPositionOffset(const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0)
{
    //计算目标点位
    float x = 0 + 0 - pipetteInfo.distance_x_from_orig + deviationX + 1;
    float y = 0 + 0 - pipetteInfo.distance_y_from_orig + deviationY + 1;
    float z = 0 + 0 - resetZ0 + pipetteInfo.deviate_z;

    QString str1 = "X：- 移液器复位位置 + 输入x偏差 + 1\n";
    QString str2 = "Y：- 移液器复位位置 + 输入y偏差 + 1\n";
    QString str3 = "Z：- 激光复位Z0（全局）+ 原始移液器z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(Storage_Info(), pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, -1, x, y, z, resetZ0, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToRackAccuratingPositionDP8
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至载架校准点
* @输入参数
* @计算公式         X：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入x偏差
*                  Y：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入y偏差
*                  Z：校准点坐标 + 标记偏差坐标 - (激光复位Z0（全局） - 移液器偏差) + 原始z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToRackAccuratingPositionDP8(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0)
{
    //计算目标点位
    float x = storageInfo.loc_x + storageInfo.crossing_x - (pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x) + deviationX;
    float y = storageInfo.loc_y + storageInfo.crossing_y - (pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y) + deviationY;
    float z = storageInfo.loc_z + storageInfo.crossing_z - (resetZ0 - pipetteInfo.deviate_z) + storageInfo.deviation_z;

    QString str1 = "X：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入x偏差\n";
    QString str2 = "Y：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：校准点坐标 + 标记偏差坐标 - (激光复位Z0（全局） - 移液器偏差) + 原始z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, -1, x, y, z, resetZ0, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToRackAccuratingPositionOffset
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至载架校准点偏移位
* @输入参数
* @计算公式         X：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入x偏差 + 1
*                  Y：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入y偏差 + 1
*                  Z：校准点坐标 + 标记偏差坐标 - (激光复位Z0（全局） - 移液器偏差) + 原始z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToRackAccuratingPositionOffset(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float deviationX, float deviationY, float resetZ0)
{
    //计算目标点位
    float x = storageInfo.loc_x + storageInfo.crossing_x - (pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x) + deviationX + 1;
    float y = storageInfo.loc_y + storageInfo.crossing_y - (pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y) + deviationY + 1;
    float z = storageInfo.loc_z + storageInfo.crossing_z - (resetZ0 - pipetteInfo.deviate_z) + storageInfo.deviation_z;

    QString str1 = "X：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入x偏差 + 1\n";
    QString str2 = "Y：校准点坐标 + 标记偏差坐标 - (移液器复位位置 - 移液器偏差) + 输入y偏差 + 1\n";
    QString str3 = "Z：校准点坐标 + 标记偏差坐标 - (激光复位Z0（全局） - 移液器偏差) + 原始z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, -1, x, y, z, resetZ0, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToPipetteAccuratingPositionDP8
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至移液器偏差校准位
* @输入参数
* @计算公式         X：校准点坐标 + 标记偏差坐标 + 机位偏差 - (移液器复位位置 - 移液器偏差) - 4.5
*                  Y：校准点坐标 + 标记偏差坐标 + 机位偏差 - (移液器复位位置 - 移液器偏差)
*                  Z：校准点坐标 + 标记偏差坐标 + 机位偏差 - (激光复位Z0（全局） - 移液器偏差) + 100
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToPipetteAccuratingPositionDP8(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, float resetZ0)
{
    //计算目标点位
    float x = storageInfo.loc_x + storageInfo.crossing_x + storageInfo.deviation_x - (pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x) - 4.5;
    float y = storageInfo.loc_y + storageInfo.crossing_y + storageInfo.deviation_y - (pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y);
    float z = storageInfo.loc_z + storageInfo.crossing_z + storageInfo.deviation_z - (resetZ0 - pipetteInfo.deviate_z) + 100;

    QString str1 = "X：校准点坐标 + 标记偏差坐标 + 机位偏差 - (移液器复位位置 - 移液器偏差) - 4.5\n";
    QString str2 = "Y：校准点坐标 + 标记偏差坐标 + 机位偏差 - (移液器复位位置 - 移液器偏差)\n";
    QString str3 = "Z：校准点坐标 + 标记偏差坐标 + 机位偏差 - (激光复位Z0（全局） - 移液器偏差) + 100";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), -1, -1, -1, x, y, z, resetZ0, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToClawAccuratingPositionDP8
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至夹爪校准位
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入x偏差
*                  Y：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 2.5 - 1.5) - (夹爪坐标 - 移液器偏差) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToClawAccuratingPositionDP8(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x + storageInfo.deviation_x) - (clawInfo.loc_x - pipetteInfo.deviate_x) + deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y + storageInfo.deviation_y) - (clawInfo.loc_y - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z + storageInfo.deviation_z - 2.5 - 1.5) - (clawInfo.loc_z - pipetteInfo.deviate_z) + deviationZ;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入x偏差\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标 + 机位偏差) - (夹爪坐标 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 2.5 - 1.5) - (夹爪坐标 - 移液器偏差) + 输入z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, clawInfo, LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);
    //夹爪Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置（夹爪z）
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_CLAW_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToClawAccuratingPositionDP8
* @创建时间         2024/5/21
* @创建人           yangkaiyun
* @函数说明         运动至PCR校准位
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标) - (夹爪相对位置终值 + 移液器复位位置 - 移液器偏差) + 输入x偏差
*                  Y：(校准点坐标 + 标记偏差坐标) - (夹爪相对位置终值 + 移液器复位位置 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 标记偏差坐标) - (夹爪相对位置终值 + 移液器复位位置 - 移液器偏差) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToPcrAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x) - (clawInfo.distance_end_x + pipetteInfo.distance_x_from_orig - pipetteInfo.deviate_x) + deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y) - (clawInfo.distance_end_y + pipetteInfo.distance_y_from_orig - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z) - (clawInfo.distance_end_z + pipetteInfo.distance_z_from_orig - pipetteInfo.deviate_z) + deviationZ;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标) - (夹爪相对位置终值 + 移液器复位位置 - 移液器偏差) + 输入x偏差\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标) - (夹爪相对位置终值 + 移液器复位位置 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标) - (夹爪相对位置终值 + 移液器复位位置 - 移液器偏差) + 输入z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, clawInfo, LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);
    //夹爪Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8::instance()->motorMove(Dp8::MOTOR_CLAW_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToSingleChannelAccuratingPosition
* @创建时间         2024/7/10
* @创建人           yangkaiyun
* @函数说明         运动至单通道校准位
* @输入参数
* @计算公式         X：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 4.5) - (单通道坐标 - 移液器偏差) + 输入x偏差
*                  Y：(校准点坐标 + 标记偏差坐标 + 机位偏差 + 4.5) - (单通道坐标 - 移液器偏差) + 输入y偏差
*                  Z：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 2.5 - 1.5) - (单通道坐标 - 移液器偏差) + 输入z偏差
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToSingleChannelAccuratingPosition(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const SingleChannelInfo &singleInfo, float deviationX, float deviationY, float deviationZ)
{
    //计算目标点位
    float x = (storageInfo.loc_x + storageInfo.crossing_x + storageInfo.deviation_x - 4.5) - (singleInfo.loc_x - pipetteInfo.deviate_x) + deviationX;
    float y = (storageInfo.loc_y + storageInfo.crossing_y + storageInfo.deviation_y + 4.5) - (singleInfo.loc_y - pipetteInfo.deviate_y) + deviationY;
    float z = (storageInfo.loc_z + storageInfo.crossing_z + storageInfo.deviation_z - 2.5 - 1.5) - (singleInfo.loc_z - pipetteInfo.deviate_z) + deviationZ;

    QString str1 = "X：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 4.5) - (单通道坐标 - 移液器偏差) + 输入x偏差\n";
    QString str2 = "Y：(校准点坐标 + 标记偏差坐标 + 机位偏差 + 4.5) - (单通道坐标 - 移液器偏差) + 输入y偏差\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标 + 机位偏差 - 2.5 - 1.5) - (单通道坐标 - 移液器偏差) + 输入z偏差";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, pipetteInfo, ClawInfo(), LaserInfo(), deviationX, deviationY, deviationZ, x, y, z, -1, singleInfo, str1 + str2 + str3);

    //移液器Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);
    //单通道Z轴复位
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_DP1_Z);
    //夹爪Z轴复位
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW_Z);

    //运动至指定X、Y位置
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = DP8_XY_SPEED;
    param.positions << x << y;
    Dp8_Xy::instance()->move(param);

    //运动至指定Z位置（单通道z）
    DevicePart::MotorMotionParams param2;
    param2.speed = DP8_Z_SPEED;
    param2.targetPosition = z;
    Dp8_Xy::instance()->motorMove(Dp8_Xy::MOTOR_DP1_Z, param2);
}

/*-----------------------------------------------------------------
* @函数名称         moveToQPcrAccuratingPosition
* @创建时间         2024/7/11
* @创建人           yangkaiyun
* @函数说明         运动至QPCR夹爪校准位
* @输入参数
* @计算公式         X：-(校准点坐标 + 标记偏差坐标 + 输入x偏差)
*                  Z：(校准点坐标 + 标记偏差坐标 + 输入z偏差) - 2.43
* @输出参数         无
* @返回值           无
------------------------------------------------------------------*/
void SportMethods::moveToQPcrAccuratingPosition(const Storage_Info &storageInfo, float deviationX, float deviationZ)
{
    //计算目标点位
    float x = -(storageInfo.loc_x + storageInfo.crossing_x + deviationX);
    float z = (storageInfo.loc_z + storageInfo.crossing_z + deviationZ) - 2.43;

    //Z轴复位
    xzclaw_control::instance()->motorReset(xzclaw_control::Z_comboBox);

    QString str1 = "X：-(校准点坐标 + 标记偏差坐标 + 输入x偏差)\n";
    QString str2 = "Y：0\n";
    QString str3 = "Z：(校准点坐标 + 标记偏差坐标 + 输入z偏差) - 2.43";
    Q_EMIT hApp->m_sigmanager->showLog(storageInfo, PipetteInfo(), ClawInfo(), LaserInfo(), deviationX, -1, deviationZ, x, 0, z, -1, SingleChannelInfo(), str1 + str2 + str3);

    //X轴运动
    DevicePart::MotorMotionParams param;
    param.speed = QPCR_CLAW_XY_SPEED;
    param.targetPosition = x;
    xzclaw_control::instance()->motorMove(xzclaw_control::X_comboBox, param);

    //Z轴运动
    DevicePart::MotorMotionParams param2;
    param2.speed = QPCR_CLAW_Z_SPEED;
    param2.targetPosition = z;
    xzclaw_control::instance()->motorMove(xzclaw_control::Z_comboBox, param2);
}

//执行python脚本
void SportMethods::executePythonScript()
{
    QProcess process;

    QString pythonInterpreter = "python";
    QString scriptPath = QCoreApplication::applicationDirPath() + "/script/update_urdf.py";

    process.start(pythonInterpreter, QStringList() << scriptPath);

    if(!process.waitForFinished())
    {
        qDebug() << "Failed to execute script:" << process.errorString();
        return;
    }

    QString output = process.readAllStandardOutput();
    qDebug() << "Script output:" << output;
}




