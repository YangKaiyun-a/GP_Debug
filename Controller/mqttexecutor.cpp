#include "mqttexecutor.h"
#include "Function/Data/mqttcommunication.h"
#include "Function/Data/parsemessageinfo.h"
#include "Function/data/global.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <cmath>
#include <QCoreApplication>
#include <QThread>

//校准位Z运动时间
const QString CAP_Z_MOVE_TIME = "1500";

//托盘Y轴偏移量运动时间
const QString BLOCK_Y_MOVE_TIME = "1500";



MqttExecutor* MqttExecutor::m_mqttExecutor = nullptr;

MqttExecutor::MqttExecutor(QObject *parent)
    : QObject{parent}
{

}

MqttExecutor *MqttExecutor::instance()
{
    if(!m_mqttExecutor)
    {
        m_mqttExecutor = new MqttExecutor();
    }

    return m_mqttExecutor;
}

//整个模块复位
bool MqttExecutor::moduleReset(ENUM_CLASS::PCR_Id id)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QString command = "init_dev";

    //构建 JSON 对象
    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = command;
    jsonMessage["params"] = QJsonObject();

    //将 JSON 对象转换为 QByteArray
    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//电机复位
bool MqttExecutor::motorReset(ENUM_CLASS::PCR_Id id, ENUM_CLASS::PCR_Motor motor)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QString command = "reset_mechanism";
    QString resetObject = "none";

    switch(motor)
    {
        case ENUM_CLASS::Cap:
            resetObject = "cap";
            break;
        case ENUM_CLASS::Block:
            resetObject = "block";
            break;
        case ENUM_CLASS::Filter:
            resetObject = "filter";
            break;
        default:
            break;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = command;

    QJsonObject params;
    params["reset_object"] = resetObject;
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//电机运动
bool MqttExecutor::motorMove(ENUM_CLASS::PCR_Id id, ENUM_CLASS::PCR_Motor motor, float distance)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QString command = "none";
    QString workingTime = "none";

    switch(motor)
    {
        case ENUM_CLASS::Cap:
            command = "move_cap";
            workingTime = CAP_Z_MOVE_TIME;
            break;
        case ENUM_CLASS::Block:
            command = "move_block";
            workingTime = BLOCK_Y_MOVE_TIME;
            break;
        default:
            break;
    }

    //确定运动方向   0：推入/向上运动  1推出/向下运动
    QString dir = (distance > 0 ? "1" : "0");
    //取绝对值
    distance = std::fabs(distance);

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = command;

    QJsonObject params;
    params["dir"] = dir;
    params["distance"] = QString::number(distance, 'f', 3);
    params["working_time"] = workingTime;
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//托盘推入/推出     0：推入  1推出
bool MqttExecutor::blockMove(ENUM_CLASS::PCR_Id id, int dir)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "move_block";

    QJsonObject params;
    params["dir"] = QString::number(dir);
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//下发sample参数
bool MqttExecutor::setSampleParams(ENUM_CLASS::PCR_Id id, const SampleParams &params)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QString command = "set_sample_param";

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = command;
    jsonMessage["command"] = command;

    QJsonObject params2;
    params2["k"] = params.k;
    params2["b"] = params.b;
    params2["duration"] = params.duration;
    jsonMessage["params"] = params2;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    // 发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//下发PCR实验方案
bool MqttExecutor::setCurrentScheme(ENUM_CLASS::PCR_Id id, const QString &currentPcrConfig, const QString &schemaName, bool enableSample)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QString command = "set_schema";

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = command;

    QJsonObject params;
    QString schemaFileObj = ParseMessageInfo::createCompleteJson(JSON_FILE_PATH("completeJson.json"), currentPcrConfig);

    if (schemaFileObj.isEmpty())
    {
        return false;
    }

    params["schema_file"] = schemaFileObj;
    params["schema_name"] = schemaName;
    params["is_sample"] = enableSample;
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    // 发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//执行/停止PCR方案     0：停止，1开始
bool MqttExecutor::runScheme(ENUM_CLASS::PCR_Id id, int isRun)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QString command = "run_status";

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = command;

    QJsonObject params;
    params["status"] = QString::number(isRun);   //0：停止   1：开始
    params["ignore"] = "0";                      //0：忽略   1：不忽略
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//开启/停止热盖        0：停止，1开始
bool MqttExecutor::runCap(ENUM_CLASS::PCR_Id id, int isRun, int tarTemp)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "cap_status";

    QJsonObject params;
    params["status"] = QString::number(isRun);   //0：停止   1：开始
    params["tar_temp"] = QString::number(tarTemp);
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//切换通道
bool MqttExecutor::changeChannel(ENUM_CLASS::PCR_Id id, ENUM_CLASS::Light_Channel channel)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "move_filter";

    QJsonObject params;
    params["channel"] = QString::number(channel);
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//通道复位
bool MqttExecutor::resetChannel(ENUM_CLASS::PCR_Id id)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "reset_mechanism";

    QJsonObject params;
    params["reset_object"] = "filter";
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//拍照（暂未使用）
bool MqttExecutor::getPhoto(ENUM_CLASS::PCR_Id id)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "get_photo";
    jsonMessage["params"] = QJsonObject();

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//荧光校准方案
bool MqttExecutor::setFluorescenceCalibrationParams(ENUM_CLASS::PCR_Id id, int capTem, int blockTem, int duration)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "set_calibration_params";

    QJsonObject params;
    params["lidTemperature"] = QString::number(capTem);
    params["blockTemperature"] = QString::number(blockTem);
    params["duration"] = QString::number(duration);
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//荧光校准   0：停止，1开始
bool MqttExecutor::startFluorescenceCalibration(ENUM_CLASS::PCR_Id id, ENUM_CLASS::Light_Channel channel, int isStart)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "asdfgh";
    jsonMessage["command"] = "calibration";

    QJsonObject params;
    params["status"] = QString::number(isStart);
    params["board"] = ParseMessageInfo::enumToString(channel);
    jsonMessage["params"] = params;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    //发布消息
    MqttCommunication::instance()->publishMessage(id, TC384_CONTROL_DOWN, message);

    return true;
}

//修改第一通道位置参数和通道间隔
bool MqttExecutor::setFirstChannelPosition(ENUM_CLASS::PCR_Id id, const ChannelParams &params)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_channel_position";
    jsonMessage["command"] = "set_channel_position";

    QJsonObject params2;
    params2["channel1_position"] = params.channel1_position;
    params2["channel_gap"] = params.channel_gap;
    jsonMessage["params"] = params2;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//修改曝光时间
bool MqttExecutor::setChannelExposureTime(ENUM_CLASS::PCR_Id id, const ChannelParams &params)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_channel_exposure";
    jsonMessage["command"] = "set_channel_exposure";

    QJsonObject params2;
    params2["channel_1_exposure"] = QString::number(MULTIPLY_BY_1000(params.channel_1_exposure_time));
    params2["channel_2_exposure"] = QString::number(MULTIPLY_BY_1000(params.channel_2_exposure_time));
    params2["channel_3_exposure"] = QString::number(MULTIPLY_BY_1000(params.channel_3_exposure_time));
    params2["channel_4_exposure"] = QString::number(MULTIPLY_BY_1000(params.channel_4_exposure_time));
    params2["channel_5_exposure"] = QString::number(MULTIPLY_BY_1000(params.channel_5_exposure_time));
    jsonMessage["params"] = params2;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//修改浓度系数
bool MqttExecutor::setChannelPotency(ENUM_CLASS::PCR_Id id, const ChannelParams &params)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_channel_potency";
    jsonMessage["command"] = "set_channel_potency";

    QJsonObject params2;
    params2["channel_1_potency"] = params.channel_1_potency;
    params2["channel_2_potency"] = params.channel_2_potency;
    params2["channel_3_potency"] = params.channel_3_potency;
    params2["channel_4_potency"] = params.channel_4_potency;
    params2["channel_5_potency"] = params.channel_5_potency;
    jsonMessage["params"] = params2;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//修改推入/推出位
bool MqttExecutor::setBlockPosition(ENUM_CLASS::PCR_Id id, const BlockParams &params)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_photo_position";
    jsonMessage["command"] = "set_photo_position";

    QJsonObject obj;
    obj["photo_position"] = params.getPhotoPosition;
    obj["out_position"] = params.blockOutPosition;
    jsonMessage["params"] = obj;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//保存顶升位
bool MqttExecutor::setCompactPosition(ENUM_CLASS::PCR_Id id, const QString &position)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_compact_position";
    jsonMessage["command"] = "set_compact_position";

    QJsonObject obj;
    obj["position"] = position;
    jsonMessage["params"] = obj;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//保存热盖补偿值
bool MqttExecutor::setCapTemOffset(ENUM_CLASS::PCR_Id id, const QString &value)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_cap_offset";
    jsonMessage["command"] = "set_cap_offset";

    QJsonObject obj;
    obj["offset"] = value;
    jsonMessage["params"] = obj;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//保存传感器补偿值
bool MqttExecutor::setConsistencyOffset(ENUM_CLASS::PCR_Id id, const BlockTemOffset &blockTemOffset)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_consistency_offset";
    jsonMessage["command"] = "set_consistency_offset";

    QJsonObject obj;
    obj["temp1"] = blockTemOffset.temp1;
    obj["temp2"] = blockTemOffset.temp2;
    obj["temp3"] = blockTemOffset.temp3;
    jsonMessage["params"] = obj;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//保存KB
bool MqttExecutor::setAccuracyOffset(ENUM_CLASS::PCR_Id id, const BlockCompenSate &blockCompenSate)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "set_accuracy_offset";
    jsonMessage["command"] = "set_accuracy_offset";

    QJsonObject obj;
    obj["K"] = blockCompenSate.k;
    obj["B"] = blockCompenSate.b;
    jsonMessage["params"] = obj;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//修改ip
bool MqttExecutor::setIpAddress(ENUM_CLASS::PCR_Id id, const QString &address)
{
    if(!MqttCommunication::instance()->isClientConnected(id))
    {
        return false;
    }

    QJsonObject jsonMessage;
    jsonMessage["uuid"] = "ipconfig";
    jsonMessage["command"] = "ipconfig";

    QJsonObject obj;
    obj["ip_address"] = address;
    jsonMessage["params"] = obj;

    QJsonDocument doc(jsonMessage);
    QByteArray message = doc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(id, TC384_DEBUG_DOWN, message);

    return true;
}

//整机复位
bool MqttExecutor::machineRest(ENUM_CLASS::PCR_Id client_id)
{
    if(!MqttCommunication::instance()->isClientConnected(client_id))
    {
        return false;
    }

    MqttCommunication::instance()->publishMessage(client_id, MACHINE_RESET, "");

    return true;
}

//发送载架
bool MqttExecutor::publishCarrier(ENUM_CLASS::PCR_Id client_id, const QString &json)
{
    if(!MqttCommunication::instance()->isClientConnected(client_id))
    {
        return false;
    }

    QJsonObject carrierObj;
    carrierObj["carrier_table_config_name"] = ParseMessageInfo::getCarrierTableName(json);
    QJsonDocument carrierDoc(carrierObj);
    QByteArray carrierMessage = carrierDoc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(client_id, LOAD_CARRIERS, carrierMessage);

    return true;
}

//发送流程
bool MqttExecutor::publishTestProject(ENUM_CLASS::PCR_Id client_id, const QString &json)
{
    if(!MqttCommunication::instance()->isClientConnected(client_id))
    {
        return false;
    }

    MqttCommunication::instance()->publishMessage(client_id, LOAD_TEST_PROJECT, json.toUtf8());

    return true;
}

//发送台面
bool MqttExecutor::publishProcessTable(ENUM_CLASS::PCR_Id client_id, int pipiet_id, const QString &workFlowName)
{
    if(!MqttCommunication::instance()->isClientConnected(client_id))
    {
        return false;
    }

    QJsonObject tableObj;
    tableObj["process_table_name"] = ParseMessageInfo::getProcessTableName(pipiet_id, workFlowName);
    QJsonDocument tableDoc(tableObj);
    QByteArray tableMessage = tableDoc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(client_id, LOAD_PROCESS_TABLE, tableMessage);

    return true;
}

//开启实验
bool MqttExecutor::pubishRunProject(ENUM_CLASS::PCR_Id client_id)
{
    if(!MqttCommunication::instance()->isClientConnected(client_id))
    {
        return false;
    }

    QJsonObject testObj;
    testObj["ui_test_id"] = "1314";
    QJsonDocument testDoc(testObj);
    QByteArray testMessage = testDoc.toJson(QJsonDocument::Compact);

    MqttCommunication::instance()->publishMessage(client_id, RUN_TEST_PROJECT, testMessage);

    return true;
}

