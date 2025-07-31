#ifndef MQTTEXECUTOR_H
#define MQTTEXECUTOR_H
#include "Function/data/global.h"

#include <QObject>


class MqttExecutor : public QObject
{
    Q_OBJECT
public:
    explicit MqttExecutor(QObject *parent = nullptr);
    static MqttExecutor *instance();

    /*************************************PCR运动*************************************/

    //整个PCR模块复位
    bool moduleReset(ENUM_CLASS::PCR_Id id);
    //PCR电机复位
    bool motorReset(ENUM_CLASS::PCR_Id id, ENUM_CLASS::PCR_Motor motor);
    //PCR电机运动
    bool motorMove(ENUM_CLASS::PCR_Id id, ENUM_CLASS::PCR_Motor motor, float distance);
    //PCR托盘推入/推出     0：推入  1推出
    bool blockMove(ENUM_CLASS::PCR_Id id, int dir);
    //下发sample参数
    bool setSampleParams(ENUM_CLASS::PCR_Id id, const SampleParams &params);
    //下发PCR实验方案
    bool setCurrentScheme(ENUM_CLASS::PCR_Id id, const QString &currentPcrConfig, const QString &schemaName, bool enableSample);
    //执行/停止PCR方案     0：停止，1开始
    bool runScheme(ENUM_CLASS::PCR_Id id, int isRun);
    //开启/停止热盖        0：停止，1开始
    bool runCap(ENUM_CLASS::PCR_Id id, int isRun, int tarTemp);
    //切换通道
    bool changeChannel(ENUM_CLASS::PCR_Id id, ENUM_CLASS::Light_Channel channel);
    //通道复位
    bool resetChannel(ENUM_CLASS::PCR_Id id);
    //拍照
    bool getPhoto(ENUM_CLASS::PCR_Id id);
    //荧光校准方案
    bool setFluorescenceCalibrationParams(ENUM_CLASS::PCR_Id id, int capTem, int blockTem, int duration);
    //荧光校准   0：停止，1开始
    bool startFluorescenceCalibration(ENUM_CLASS::PCR_Id id, ENUM_CLASS::Light_Channel channel, int isStart);



    /*************************************PCR参数修改*************************************/

    //修改第一通道位置参数和通道间隔
    bool setFirstChannelPosition(ENUM_CLASS::PCR_Id id, const ChannelParams &params);
    //修改曝光时间
    bool setChannelExposureTime(ENUM_CLASS::PCR_Id id, const ChannelParams &params);
    //修改浓度系数
    bool setChannelPotency(ENUM_CLASS::PCR_Id id, const ChannelParams &params);
    //修改推入/推出位
    bool setBlockPosition(ENUM_CLASS::PCR_Id id, const BlockParams &params);
    //保存顶升位
    bool setCompactPosition(ENUM_CLASS::PCR_Id id, const QString &position);
    //保存热盖补偿值
    bool setCapTemOffset(ENUM_CLASS::PCR_Id id, const QString &value);
    //保存传感器补偿值
    bool setConsistencyOffset(ENUM_CLASS::PCR_Id id, const BlockTemOffset &blockTemOffset);
    //保存KB
    bool setAccuracyOffset(ENUM_CLASS::PCR_Id id, const BlockCompenSate &blockCompenSate);
    //修改ip
    bool setIpAddress(ENUM_CLASS::PCR_Id id, const QString &address);


    /*************************************整机*************************************/

    //整机复位
    bool machineRest(ENUM_CLASS::PCR_Id client_id);
    //发送载架
    bool publishCarrier(ENUM_CLASS::PCR_Id client_id, const QString &json);
    //发送流程
    bool publishTestProject(ENUM_CLASS::PCR_Id client_id, const QString &json);
    //发送台面
    bool publishProcessTable(ENUM_CLASS::PCR_Id client_id, int pipiet_id, const QString &workFlowName);
    //开启实验
    bool pubishRunProject(ENUM_CLASS::PCR_Id client_id);


signals:

private:
    static MqttExecutor *m_mqttExecutor;

};

#endif // MQTTEXECUTOR_H
