#include "parsemessageinfo.h"
#include "global.h"
#include "dbmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMetaEnum>
#include <QFile>
#include <QCoreApplication>

ParseMessageInfo* ParseMessageInfo::m_parseMessageInfo = nullptr;
QMap<int, CrossingDeviation> ParseMessageInfo::m_mapCrossing;
LaserDistance ParseMessageInfo::m_laserDistance;
float ParseMessageInfo::m_resetZ0;

ParseMessageInfo::ParseMessageInfo(QObject *parent)
    : QObject{parent}
{

}

ParseMessageInfo *ParseMessageInfo::instance()
{
    if(!m_parseMessageInfo)
    {
        m_parseMessageInfo = new ParseMessageInfo();
    }

    return m_parseMessageInfo;
}

//解析mqtt接收消息
MessageInfo ParseMessageInfo::parseMessageReceived(const QString &data)
{
    MessageInfo info;

    QJsonDocument loadDoc = QJsonDocument::fromJson(data.toUtf8());

    if(loadDoc.isObject())
    {
        QJsonObject json = loadDoc.object();
        if(json.contains("uuid"))
        {
            info.uuid = json["uuid"].toString();
        }
        if(json.contains("command"))
        {
            info.command = json["command"].toString();
        }
        if(json.contains("params"))
        {
            QJsonObject paramsObj = json["params"].toObject();
            if(paramsObj.contains("result"))
            {
                info.result = paramsObj["result"].toString();
            }
            if(paramsObj.contains("reset_object"))
            {
                info.resetObject = paramsObj["reset_object"].toString();
            }
            if(paramsObj.contains("experiment_name"))
            {
                info.experiment_name = paramsObj["experiment_name"].toString();
            }
            if(paramsObj.contains("status"))
            {
                info.status = paramsObj["status"].toString();
            }
        }
    }
    return info;
}

//解析中位机s回复
MiddleMessageInfo ParseMessageInfo::parseMiddleReceived(const QString &data)
{
    MiddleMessageInfo info;

    QJsonDocument loadDoc = QJsonDocument::fromJson(data.toUtf8());

    if(loadDoc.isObject())
    {
        QJsonObject json = loadDoc.object();

        if(json.contains("message"))
        {
            info.message = json["message"].toString();
        }

        if(json.contains("status"))
        {
            info.status = json["status"].toInt();
        }
    }

    return info;
}

//解析热盖顶升位参数
float ParseMessageInfo::parseCapUpPositionQString(const QString &mechanismParams)
{
    float compactLocation = -1.0;

    QJsonDocument doc = QJsonDocument::fromJson(mechanismParams.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();
            if(obj.contains("compact_location"))
            {
                QJsonValue value = obj.value("compact_location");
                if(value.isString())
                {
                    QString compactLocationStr = value.toString();
                    QString numberStr = compactLocationStr.remove(QRegExp("[^0-9.]"));
                    compactLocation = numberStr.toFloat();
                }
            }
        }
    }

    return compactLocation;
}

//解析推入位参数
BlockParams ParseMessageInfo::parseBlockPositionQString(const QString &mechanismParams)
{
    BlockParams blockParam;

    QJsonDocument doc = QJsonDocument::fromJson(mechanismParams.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();
            if(obj.contains("get_photo_location"))
            {
                QJsonValue value = obj.value("get_photo_location");
                if(value.isString())
                {
                    QString compactLocationStr = value.toString();
                    QString numberStr = compactLocationStr.remove(QRegExp("[^0-9.]"));
                    blockParam.getPhotoPosition = numberStr;
                }
            }
            if(obj.contains("out_position"))
            {
                QJsonValue value = obj.value("out_position");
                if(value.isString())
                {
                    QString compactLocationStr = value.toString();
                    QString numberStr = compactLocationStr.remove(QRegExp("[^0-9.]"));
                    blockParam.blockOutPosition = numberStr;
                }
            }
        }
    }

    return blockParam;
}

//从文件中读取json并返回普通字符串
QString ParseMessageInfo::readJsonFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open the file for reading.";
        return QString();
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    //返回普通字符串
    return content;
}

//组合完整的RDML
QString ParseMessageInfo::createCompleteJson(const QString &completeJsonFile, const QString &currentPcrConfig)
{
    //完整RDML
    QString completeJson = readJsonFromFile(completeJsonFile);
    QJsonDocument completeDoc = QJsonDocument::fromJson(completeJson.toUtf8());
    if(completeDoc.isNull())
    {
        qDebug() << "Failed to parse JSON from file: " << completeJsonFile;
        return QString();
    }
    QJsonObject completeObj = completeDoc.object();

    //PCR RDML
    QJsonDocument currentDoc = QJsonDocument::fromJson(currentPcrConfig.toUtf8());
    if(currentDoc.isNull())
    {
        qDebug() << "Failed to parse currentDoc";
        return QString();
    }
    QJsonObject currentObj = currentDoc.object();


    if(completeObj.isEmpty() || currentObj.isEmpty())
    {
        qDebug() << "completeObj or currentObj is empty.";
        return QString();
    }

    if(completeObj.contains("thermalCyclingConditions") || currentObj.contains("thermalCyclingConditions"))
    {
        completeObj["thermalCyclingConditions"] = currentObj["thermalCyclingConditions"];
    }

    QJsonDocument doc(completeObj);
    QString message = QString::fromUtf8(doc.toJson(/*QJsonDocument::Compact*/));

    return message;
}

//解析相机曝光时间
void ParseMessageInfo::parseExposureTimeJson(const QString &json, ChannelParams &params)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    if(jsonDoc.isNull())
    {
        params.error = true;
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.contains("channel_1"))
    {
        params.channel_1_exposure_time = jsonObj["channel_1"].toString().toInt() / 1000;
    }
    if(jsonObj.contains("channel_2"))
    {
        params.channel_2_exposure_time = jsonObj["channel_2"].toString().toInt() / 1000;
    }
    if(jsonObj.contains("channel_3"))
    {
        params.channel_3_exposure_time = jsonObj["channel_3"].toString().toInt() / 1000;
    }
    if(jsonObj.contains("channel_4"))
    {
        params.channel_4_exposure_time = jsonObj["channel_4"].toString().toInt() / 1000;
    }
    if(jsonObj.contains("channel_5"))
    {
        params.channel_5_exposure_time = jsonObj["channel_5"].toString().toInt() / 1000;
    }
}

//解析PCR循环步骤
QVector<PcrSteps> ParseMessageInfo::parsePcrConfigParams(const QString &pcrConfig)
{
    QVector<PcrSteps> steps;

    QJsonDocument doc = QJsonDocument::fromJson(pcrConfig.toUtf8());
    if (!doc.isNull())
    {
        QJsonObject rootObj = doc.object();
        QJsonArray conditionsArray = rootObj["thermalCyclingConditions"].toArray();

        QJsonArray stepsArray = conditionsArray[0].toObject()["step"].toArray();

        foreach (const QJsonValue &value, stepsArray)
        {
            QJsonObject stepObj = value.toObject();
            if (stepObj.contains("temperature"))
            {
                PcrSteps step;
                step.index = stepObj["nr"].toInt();
                QJsonObject temperatureObj = stepObj["temperature"].toObject();
                step.temperature = temperatureObj["temperature"].toDouble();
                step.duration = temperatureObj["duration"].toInt();

                steps.append(step);
            }
        }
    }
    else
    {
        qDebug() << "解析PCR循环步骤失败！";
    }

    return steps;
}

//解析PCR实时温度
RealTimeTemperature ParseMessageInfo::parseRealTimeTemperature(const QString &json)
{
    RealTimeTemperature temp;

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();
            if(obj.contains("temp1"))
            {
                temp.block1 = obj["temp1"].toString().toFloat();
            }
            if(obj.contains("temp2"))
            {
                temp.block2 = obj["temp2"].toString().toFloat();
            }
            if(obj.contains("temp3"))
            {
                temp.block3 = obj["temp3"].toString().toFloat();
            }
            if(obj.contains("temp5"))
            {
                temp.heatCap = obj["temp5"].toString().toFloat();
            }
        }
    }

    return temp;
}

//修改PCR方案中的参数，并返回字符串
QString ParseMessageInfo::modifyScheme(const QString &json, int heatLidTemperature, int blockTemperature, int count)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    QJsonArray thermalCyclingConditionsArray = jsonObj.value("thermalCyclingConditions").toArray();

    for(int i = 0; i < thermalCyclingConditionsArray.size(); ++i)
    {
        QJsonObject thermalCyclingCondition = thermalCyclingConditionsArray[i].toObject();
        QJsonArray stepsArray = thermalCyclingCondition.value("step").toArray();

        if(thermalCyclingCondition.contains("lidTemperature"))
        {
            thermalCyclingCondition["lidTemperature"] = heatLidTemperature;
        }

        for(int j = 0; j < stepsArray.size(); ++j)
        {
            QJsonObject step = stepsArray[j].toObject();

            if(step.contains("loop"))
            {
                QJsonObject loop = step.value("loop").toObject();
                loop["repeat"] = count;
                step["loop"] = loop;
                stepsArray[j] = step;
            }

            if(step.contains("temperature"))
            {
                QJsonObject obj = step.value("temperature").toObject();
                obj["temperature"] = blockTemperature;
                step["temperature"] = obj;
                stepsArray[j] = step;
            }
        }

        thermalCyclingCondition["step"] = stepsArray;
        thermalCyclingConditionsArray[i] = thermalCyclingCondition;
    }

    jsonObj["thermalCyclingConditions"] = thermalCyclingConditionsArray;

    QJsonDocument modifiedJsonDoc(jsonObj);
    QString modifiedJsonString = QString::fromUtf8(modifiedJsonDoc.toJson(QJsonDocument::Compact));

    return modifiedJsonString;
}

//解析荧光校准第一通道位置
void ParseMessageInfo::parseFirstChannelPosition(const QString &json, ChannelParams &params)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    if(jsonDoc.isNull())
    {
        params.error = true;
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.contains("channel_gap"))
    {
        params.channel_gap = jsonObj["channel_gap"].toString();
    }

    if(jsonObj.contains("channel1_position"))
    {
        params.channel1_position = jsonObj["channel1_position"].toString();
    }
}

//解析浓度系数
void ParseMessageInfo::parsePotency(const QString &json, ChannelParams &params)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    if(jsonDoc.isNull())
    {
        params.error = true;
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.contains("channel_1"))
    {
        params.channel_1_potency = jsonObj["channel_1"].toString();
    }
    if(jsonObj.contains("channel_2"))
    {
        params.channel_2_potency = jsonObj["channel_2"].toString();
    }
    if(jsonObj.contains("channel_3"))
    {
        params.channel_3_potency = jsonObj["channel_3"].toString();
    }
    if(jsonObj.contains("channel_4"))
    {
        params.channel_4_potency = jsonObj["channel_4"].toString();
    }
    if(jsonObj.contains("channel_5"))
    {
        params.channel_5_potency = jsonObj["channel_5"].toString();
    }
}

//解析热盖补偿值
QString ParseMessageInfo::parseTempOffset(const QString &json)
{
    QString temp_offset = "";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    if(jsonDoc.isNull())
    {
        return temp_offset;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.contains("temp_offset"))
    {
        temp_offset = jsonObj["temp_offset"].toString();
    }

    return temp_offset;
}

//解析k、b
BlockCompenSate ParseMessageInfo::parseBlockCompensate(const QString &json)
{
    BlockCompenSate a;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    if(jsonDoc.isNull())
    {
        a.error = true;
        return a;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.contains("K"))
    {
        a.k = jsonObj["K"].toString();
    }
    if(jsonObj.contains("B"))
    {
        a.b = jsonObj["B"].toString();
    }

    return a;
}

//解析传感器温度补偿值
BlockTemOffset ParseMessageInfo::parseBlockTemOffset(const QString &json)
{
    BlockTemOffset a;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    if(jsonDoc.isNull())
    {
        a.error = true;
        return a;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.contains("temp1_offset"))
    {
        a.temp1 = jsonObj["temp1_offset"].toString();
    }
    if(jsonObj.contains("temp2_offset"))
    {
        a.temp2 = jsonObj["temp2_offset"].toString();
    }
    if(jsonObj.contains("temp3_offset"))
    {
        a.temp3 = jsonObj["temp3_offset"].toString();
    }

    return a;
}

//读取载架十字标记偏差
void ParseMessageInfo::initDeviationFromJson(const QString &jsonPath)
{
    QFile file(jsonPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open file:" << jsonPath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data);
    if(document.isNull() || !document.isObject())
    {
        qWarning() << "Invalid JSON document:" << jsonPath;
        return;
    }

    QJsonObject jsonObject = document.object();

    QJsonArray array = jsonObject["rack_carrier"].toArray();

    for(int i = 0; i < array.size(); ++i)
    {
        QJsonObject obj = array[i].toObject();

        if(obj["rack_name"].toString() == "激光相对葫芦头位置")
        {
            LaserDistance laser;
            laser.distance_x = DIVIDE_BY_1000(obj.value("distance_x").toInt());
            laser.distance_y = DIVIDE_BY_1000(obj.value("distance_y").toInt());
            laser.distance_z = DIVIDE_BY_1000(obj.value("distance_z").toInt());

            m_laserDistance = laser;

            m_resetZ0 = DIVIDE_BY_1000(obj.value("resetZ0").toInt());
        }
        else
        {
            CrossingDeviation deviation;
            deviation.table_id = obj.value("table_id").toInt();
            deviation.rack_name = obj.value("rack_name").toString();
            deviation.carrier_name = obj.value("carrier_name").toString();
            //从数据库中获取rack_id
            deviation.rack_id = DBManager::instance()->getRackIdByNameAndTable(deviation.rack_name, deviation.table_id);
            //从数据库中获取carrier_id
            deviation.carrier_id = DBManager::instance()->getCarrierIdByName(deviation.carrier_name);

            deviation.crossing_x = DIVIDE_BY_1000(obj.value("crossing_x").toInt());
            deviation.crossing_y = DIVIDE_BY_1000(obj.value("crossing_y").toInt());
            deviation.crossing_z = DIVIDE_BY_1000(obj.value("crossing_z").toInt());

            m_mapCrossing.insert(deviation.rack_id, deviation);
        }
    }
}

//根据rack_id来获取标记偏差、载架
void ParseMessageInfo::getCrossingDeviatio(Storage_Info &info)
{
    info.carrier_id = m_mapCrossing.value(info.rack_id).carrier_id;
    info.carrier_name = m_mapCrossing.value(info.rack_id).carrier_name;
    info.rack_name = m_mapCrossing.value(info.rack_id).rack_name;

    info.crossing_x = m_mapCrossing.value(info.rack_id).crossing_x;
    info.crossing_y = m_mapCrossing.value(info.rack_id).crossing_y;
    info.crossing_z = m_mapCrossing.value(info.rack_id).crossing_z;
}

//根据table_id与rack_name获取rack_id
int ParseMessageInfo::getRackIdByTableIdAndRackName(int table_id, const QString &rack_name)
{
    for (auto it = m_mapCrossing.begin(); it != m_mapCrossing.end(); ++it)
    {
        const CrossingDeviation& deviation = it.value();
        if (deviation.table_id == table_id && deviation.rack_name == rack_name)
        {
            return deviation.rack_id;
        }
    }

    qDebug() << "无法在配置文件找到 table_id = " <<  table_id << "，rack_name = " << rack_name << "的记录";

    return -1;
}

//获取DP1的激光相对葫芦头位置
void ParseMessageInfo::getLaserDistance(LaserInfo &info)
{
    info.distance_x = m_laserDistance.distance_x;
    info.distance_y = m_laserDistance.distance_y;
    info.distance_z = m_laserDistance.distance_z;
}

float ParseMessageInfo::resetZ0()
{
    return m_resetZ0;
}

//获取移液流程
QString ParseMessageInfo::getWorkFlowJson(const QString &workFlowName)
{
    QString jsonPath = WORKFLOW_FILE_PATH(workFlowName);

    QFile file(jsonPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open file:" << jsonPath;
        return QString();
    }

    QByteArray data = file.readAll();
    file.close();

    return QString::fromUtf8(data);
}

//从流程中解析出载架名称
QString ParseMessageInfo::getCarrierTableName(const QString &json)
{
    QString carrierName = "";

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();
            if(obj.contains("carrier_table_config_name"))
            {
                carrierName = obj["carrier_table_config_name"].toString();
            }
        }
    }

    return carrierName;
}

//获取台面名称
QString ParseMessageInfo::getProcessTableName(int id, const QString &workFlowName)
{
    QString name = "";

    //切割出Tip头类型
    QStringList parts = workFlowName.split('_');

    switch (id)
    {
    case ENUM_CLASS::DJ1:
        name = "DJ1台面";
        break;
    case ENUM_CLASS::DJ2:
        name = "DJ2台面";
        break;
    case ENUM_CLASS::DP1:
        if(parts[1] == "200Tip")
        {
            name = "DP1_C_C台面";
        }
        else if(parts[1] == "1000Tip")
        {
            name = "DP1_C_D台面";
        }
        break;
    case ENUM_CLASS::DP8:
        name = "DP8台面";
        break;
    default:
        break;
    }

    return name;
}

//获取移液流程中的补偿策略
QString ParseMessageInfo::getMaterialStrategy(const QString &workFlowName)
{
    QString json = getWorkFlowJson(workFlowName);

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());

    if (!doc.isNull())
    {
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            if (obj.contains("manual_task"))
            {
                QJsonArray taskArray = obj["manual_task"].toArray();

                QString aspirate_compen_strategy = "";
                QString dispense_compen_strategy = "";

                for (int i = 0; i < taskArray.count(); ++i)
                {
                    QJsonObject taskObj = taskArray[i].toObject();

                    if (taskObj.contains("task"))
                    {
                        QJsonArray array = taskObj["task"].toArray();

                        for (int j = 0; j < array.count(); ++j)
                        {
                            QJsonObject stepObj = array[j].toObject();

                            if (stepObj.contains("aspirate_compen_strategy"))
                            {
                                aspirate_compen_strategy = stepObj["aspirate_compen_strategy"].toString();
                            }
                            if (stepObj.contains("dispense_compen_strategy"))
                            {
                                dispense_compen_strategy = stepObj["dispense_compen_strategy"].toString();
                            }
                        }
                    }
                }

                if (aspirate_compen_strategy == dispense_compen_strategy)
                {
                    return aspirate_compen_strategy;
                }
            }
        }
    }

    return "未找到策略";
}
