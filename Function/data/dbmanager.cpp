#include "dbmanager.h"
#include "settingfilecreate.h"
#include "parsemessageinfo.h"

#include <QThread>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

DBManager *DBManager::m_dbManager =  nullptr;

DBManager::DBManager(QObject *parent)
    : QObject{parent}
{

}

DBManager *DBManager::instance()
{
    if(nullptr == m_dbManager)
    {
        m_dbManager = new DBManager();
    }
    return m_dbManager;
}

//连接数据库
bool DBManager::connectDataBase()
{
    bool success1 = false;
    bool success2 = false;
    bool success3 = false;

    QSqlDatabase db1 = getDatabaseConnection("M1");
    QSqlDatabase db2 = getDatabaseConnection("M2");
    QSqlDatabase db3 = getDatabaseConnection("Position");

    if(db1.isValid())
    {
        success1 = db1.open();
    }

    if(db2.isValid())
    {
        success2 = db2.open();
    }

    if(db3.isValid())
    {
        success3 = db3.open();
    }

    if(!success1)
    {
        qDebug() << "数据库 M1 打开失败" << db1.lastError();
    }
    else
    {
        qDebug() << "数据库 M1 打开成功";
    }

    if(!success2)
    {
        qDebug() << "数据库 M2 打开失败" << db2.lastError();
    }
    else
    {
        qDebug() << "数据库 M2 打开成功";
    }

    if(!success3)
    {
        qDebug() << "数据库 Position 打开失败" << db3.lastError();
    }
    else
    {
        qDebug() << "数据库 Position 打开成功";
    }

    return success1 && success2 && success3;
}

//根据 模块+线程 获取连接名
QSqlDatabase DBManager::getDatabaseConnection(const QString& groupName)
{
    QString connectionName = QString("%1_DBConnection_%2").arg(groupName).arg((quintptr)QThread::currentThreadId());

    if(!QSqlDatabase::contains(connectionName))
    {
        if(!SettingFileCreate::instance()->sysValue(groupName, "db_connected").toBool())
        {
            return QSqlDatabase();
        }

        QString dbDriver = "QMYSQL";

        if(groupName == "Position")
        {
            dbDriver = "QPSQL";
        }

        QSqlDatabase db = QSqlDatabase::addDatabase(dbDriver, connectionName);
        db.setHostName(SettingFileCreate::instance()->sysValue(groupName, "db_Host").toString());
        db.setPort(SettingFileCreate::instance()->sysValue(groupName, "db_Port").toInt());
        db.setDatabaseName(SettingFileCreate::instance()->sysValue(groupName, "db_Name").toString());
        db.setUserName(SettingFileCreate::instance()->sysValue(groupName, "db_User").toString());
        db.setPassword(SettingFileCreate::instance()->sysValue(groupName, "db_PWD").toString());
        return db;
    }

    return QSqlDatabase::database(connectionName);
}

//获取顶升位参数
float DBManager::getCapUpPosition(ENUM_CLASS::PCR_Id id)
{
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT mechanism_params FROM cap_config_params WHERE key_id = 1");

    QString mechanismParams;

    if(query.exec(str))
    {
        if (query.next())
        {
            mechanismParams = query.value("mechanism_params").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return ParseMessageInfo::parseCapUpPositionQString(mechanismParams);
}

//获取推入/推出位参数
BlockParams DBManager::getBlockPosition(ENUM_CLASS::PCR_Id id)
{
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT mechanism_params FROM block_config_params WHERE key_id = 1");

    QString mechanismParams;

    if(query.exec(str))
    {
        if (query.next())
        {
            mechanismParams = query.value("mechanism_params").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return ParseMessageInfo::parseBlockPositionQString(mechanismParams);
}

//获取热盖补偿值
QString DBManager::getCapTemOffset(ENUM_CLASS::PCR_Id id)
{
    QString json;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT temp_offset FROM cap_config_params WHERE key_id = 1");
    query.prepare(str);

    if(query.exec(str))
    {
        if (query.next())
        {
            json = query.value("temp_offset").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
        return QString();
    }

    return ParseMessageInfo::parseTempOffset(json);
}

//获取传感器温度补偿值
BlockTemOffset DBManager::getBlockTemOffset(ENUM_CLASS::PCR_Id id)
{
    QString json;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT consistency_offset FROM block_config_params WHERE key_id = 1");
    query.prepare(str);

    if(query.exec(str))
    {
        if (query.next())
        {
            json = query.value("consistency_offset").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return ParseMessageInfo::parseBlockTemOffset(json);
}

//读取曝光时间
QString DBManager::getExposureTime(ENUM_CLASS::PCR_Id id)
{
    QString json;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT exposure_time FROM filter_config_params WHERE key_id = 1");

    if(query.exec(str))
    {
        if (query.next())
        {
            json = query.value("exposure_time").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return json;
}

//保存曝光时间
bool DBManager::saveExposureTime(ENUM_CLASS::PCR_Id id, const QString &json)
{
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("UPDATE filter_config_params SET exposure_time = :exposure_time WHERE key_id = 1");

    query.prepare(str);
    query.bindValue(":exposure_time", json);

    if(!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

//获取PCR实时温度
RealTimeTemperature DBManager::getRealTimeTemperature(ENUM_CLASS::PCR_Id id)
{
    RealTimeTemperature temp;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT * FROM real_time_temperature WHERE key_id = 1");

    query.prepare(str);
    if(query.exec(str))
    {
        if (query.next())
        {
            temp = ParseMessageInfo::parseRealTimeTemperature(query.value("current_temps").toString());
            temp.update_Time = query.value("update_Time").toDateTime();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return temp;
}

//获取第一通道参数
QString DBManager::getFirstChannelPosition(ENUM_CLASS::PCR_Id id)
{
    QString json;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT mechanism_params FROM filter_config_params WHERE key_id = 1");
    query.prepare(str);

    if(query.exec(str))
    {
        if (query.next())
        {
            json = query.value("mechanism_params").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
        return QString();
    }

    return json;
}

//获取K、B
BlockCompenSate DBManager::getBlockCompenSate(ENUM_CLASS::PCR_Id id)
{
    QString json;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT accuracy_offset FROM block_config_params WHERE key_id = 1");
    query.prepare(str);

    if(query.exec(str))
    {
        if (query.next())
        {
            json = query.value("accuracy_offset").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return ParseMessageInfo::parseBlockCompensate(json);
}

//保存第一通道参数
bool DBManager::saveFirstChannelPosition(ENUM_CLASS::PCR_Id id, const QString &position)
{
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("UPDATE filter_config_params SET mechanism_params = :mechanism_params WHERE key_id = 1");

    query.prepare(str);
    query.bindValue(":mechanism_params", position);

    if(!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

//获取校准矩阵
QString DBManager::getAdjustMatrix(ENUM_CLASS::PCR_Id id)
{
    QString result;

    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT calibration_file FROM calibration_result WHERE key_id = 1");
    query.prepare(str);

    if(query.exec(str))
    {
        if (query.next())
        {
            result = query.value("calibration_file").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
        return QString();
    }

    return result;
}

//获取浓度系数
QString DBManager::getPotency(ENUM_CLASS::PCR_Id id)
{
    QString json;
    QSqlDatabase db = getDatabaseConnection(ParseMessageInfo::enumToString(id));
    QSqlQuery query(db);

    QString str = QString("SELECT channel_potency FROM channel_potency WHERE key_id = 1");

    if(query.exec(str))
    {
        if (query.next())
        {
            json = query.value("channel_potency").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return json;
}

//根据机位id查询并组合一条位置信息
void DBManager::getStorageInfoByCarrierId(Storage_Info &info, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    QSqlQuery query(db);
    query.prepare("SELECT * FROM public.carrier_in_rack WHERE rack_id = :rack_id AND carrier_id = :carrier_id");
    query.bindValue(":rack_id", info.rack_id);
    query.bindValue(":carrier_id", info.carrier_id);

    if(!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return;
    }

    if(query.next())
    {
        info.loc_x = DIVIDE_BY_1000(query.value("loc_x").toInt());
        info.loc_y = DIVIDE_BY_1000(query.value("loc_y").toInt());
        info.loc_z = DIVIDE_BY_1000(query.value("loc_z").toInt());

        //加上机位坐标
        Rack rackInfo = getRackInfo(info.rack_id);
        info.loc_x += rackInfo.loc_x;
        info.loc_y += rackInfo.loc_y;
        info.loc_z += rackInfo.loc_z;
        info.deviation_x = rackInfo.deviation_x;
        info.deviation_y = rackInfo.deviation_y;
        info.deviation_z = rackInfo.deviation_z;
    }
    else
    {
        qDebug() << "没有在表：carrier_in_rack 中找到机位号为：" << info.rack_id << "且载架号为：" << info.carrier_id << "的记录";
        return;
    }
}

//根据module_id获取移液器信息
PipetteInfo DBManager::getPipetteInfoByModuleId(int moduleId, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    PipetteInfo info;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM public.gantry WHERE module_id = :module_id");
    query.bindValue(":module_id", moduleId);

    if(!query.exec())
    {
        qDebug() << "查询移液器信息失败：" << query.lastError().text();
        return info;
    }

    if(query.next())
    {
        info.module_id = query.value("module_id").toInt();
        info.distance_x_from_orig = DIVIDE_BY_1000(query.value("distance_x_from_orig").toInt());
        info.distance_y_from_orig = DIVIDE_BY_1000(query.value("distance_y_from_orig").toInt());
        info.distance_z_from_orig = DIVIDE_BY_1000(query.value("distance_z_from_orig").toInt());

        //偏差取反
        info.deviate_x = -DIVIDE_BY_1000(query.value("deviate_x").toInt());
        info.deviate_y = -DIVIDE_BY_1000(query.value("deviate_y").toInt());
        info.deviate_z = -DIVIDE_BY_1000(query.value("deviate_z").toInt());
    }
    else
    {
        qDebug() << "没有找到 module_id 为：" << moduleId << "的移液器";
        return info;
    }

    return info;

}

//根据gantry_id获取夹爪相对葫芦头位置
ClawInfo DBManager::getClawInfoByGantryId(int gantryId, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    ClawInfo info;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM public.mechanism_end WHERE gantry_id = :gantry_id AND name = :name");
    query.bindValue(":gantry_id", gantryId);
    query.bindValue(":name", "CLAW");

    if(!query.exec())
    {
        qDebug() << "查询 gantry_id 为：" << gantryId <<"的夹爪相对葫芦头位置失败：" << query.lastError().text();
        return info;
    }

    if(query.next())
    {
        info.gantry_id = query.value("gantry_id").toInt();
        info.name = query.value("name").toString();
        info.distance_x = DIVIDE_BY_1000(query.value("distance_x").toInt());
        info.distance_y = DIVIDE_BY_1000(query.value("distance_y").toInt());
        info.distance_z = DIVIDE_BY_1000(query.value("distance_z").toInt());

        //偏差取反
        info.deviation_x = -DIVIDE_BY_1000(query.value("deviation_x").toInt());
        info.deviation_y = -DIVIDE_BY_1000(query.value("deviation_y").toInt());
        info.deviation_z = -DIVIDE_BY_1000(query.value("deviation_z").toInt());
    }
    else
    {
        qDebug() << "没有找到 gantry_id 为：" << gantryId << "的夹爪信息";
        return info;
    }

    return info;
}

//根据gantry_id获取单通道相对葫芦头位置
SingleChannelInfo DBManager::getSingleInfoByGantryId(int gantryId, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    SingleChannelInfo info;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM public.mechanism_end WHERE gantry_id = :gantry_id AND name = :name");
    query.bindValue(":gantry_id", gantryId);
    query.bindValue(":name", "DP1");

    if(!query.exec())
    {
        qDebug() << "查询单通道相对葫芦头位置失败：" << query.lastError().text();
        return info;
    }

    if(query.next())
    {
        info.gantry_id = query.value("gantry_id").toInt();
        info.name = query.value("name").toString();
        info.distance_x = DIVIDE_BY_1000(query.value("distance_x").toInt());
        info.distance_y = DIVIDE_BY_1000(query.value("distance_y").toInt());
        info.distance_z = DIVIDE_BY_1000(query.value("distance_z").toInt());

        //偏差取反
        info.deviation_x = -DIVIDE_BY_1000(query.value("deviation_x").toInt());
        info.deviation_y = -DIVIDE_BY_1000(query.value("deviation_y").toInt());
        info.deviation_z = -DIVIDE_BY_1000(query.value("deviation_z").toInt());
    }
    else
    {
        qDebug() << "没有找到 gantry_id 为：" << gantryId << "的单通道信息";
        return info;
    }

    return info;
}

//根据机位id获取机位坐标、机位偏差
Rack DBManager::getRackInfo(int id, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    Rack info;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM public.rack WHERE rack_id = :rack_id");
    query.bindValue(":rack_id", id);

    if(!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return info;
    }

    if(query.next())
    {
        info.rack_id = query.value("rack_id").toInt();
        info.table_id = query.value("table_id").toInt();
        info.rack_name = query.value("rack_name").toString();
        info.loc_x = DIVIDE_BY_1000(query.value("loc_x").toInt());
        info.loc_y = DIVIDE_BY_1000(query.value("loc_y").toInt());
        info.loc_z = DIVIDE_BY_1000(query.value("loc_z").toInt());
        info.deviation_x = DIVIDE_BY_1000(query.value("deviation_x").toInt());
        info.deviation_y = DIVIDE_BY_1000(query.value("deviation_y").toInt());
        info.deviation_z = DIVIDE_BY_1000(query.value("deviation_z").toInt());
    }
    else
    {
        qDebug() << "没有在 rack 表中找到机位号为：" << id << "的记录";
        return info;
    }

    return info;
}

//保存移液器偏差
bool DBManager::setPipeteeDeviationValue(const PipetteInfo &info, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);
    QSqlQuery query(db);

    query.prepare("UPDATE public.gantry SET deviate_x = :deviate_x, deviate_y = :deviate_y, deviate_z = :deviate_z WHERE module_id = :module_id");
    query.bindValue(":deviate_x", MULTIPLY_BY_1000(-info.deviate_x));
    query.bindValue(":deviate_y", MULTIPLY_BY_1000(-info.deviate_y));
    query.bindValue(":deviate_z", MULTIPLY_BY_1000(-info.deviate_z));
    query.bindValue(":module_id", info.module_id);

    if(!query.exec())
    {
        qDebug() << "更新 module id 为：" << info.module_id << "的移液器偏差失败：" << query.lastError().text();
        return false;
    }

    return true;
}

//保存机位偏差值
bool DBManager::setDeviationValue(const Storage_Info &info, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);
    QSqlQuery query(db);

    query.prepare("UPDATE public.rack SET deviation_x = :deviation_x, deviation_y = :deviation_y, deviation_z = :deviation_z WHERE rack_id = :rack_id");
    query.bindValue(":deviation_x", MULTIPLY_BY_1000(info.deviation_x));
    query.bindValue(":deviation_y", MULTIPLY_BY_1000(info.deviation_y));
    query.bindValue(":deviation_z", MULTIPLY_BY_1000(info.deviation_z));
    query.bindValue(":rack_id", info.rack_id);

    if(!query.exec())
    {
        qDebug() << "更新机位号为：" << info.rack_id << "的偏差失败：" << query.lastError().text();
        return false;
    }

    return true;
}

//保存夹爪和葫芦头相对位置的偏差
bool DBManager::setClawDeviationValue(const ClawInfo &clawInfo, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);
    QSqlQuery query(db);

    query.prepare("UPDATE public.mechanism_end SET deviation_x = :deviation_x, deviation_y = :deviation_y, deviation_z = :deviation_z WHERE gantry_id = :gantry_id AND name = :name");
    query.bindValue(":deviation_x", -MULTIPLY_BY_1000(clawInfo.deviation_x));
    query.bindValue(":deviation_y", -MULTIPLY_BY_1000(clawInfo.deviation_y));
    query.bindValue(":deviation_z", -MULTIPLY_BY_1000(clawInfo.deviation_z));
    query.bindValue(":gantry_id", clawInfo.gantry_id);
    query.bindValue(":name", clawInfo.name);

    if(!query.exec())
    {
        qDebug() << "更新gantry_id为：" << clawInfo.gantry_id << "，name为：" << clawInfo.name << "的夹爪相对位置失败：" << query.lastError().text();
        return false;
    }

    return true;
}

//保存单通道和葫芦头相对位置的偏差
bool DBManager::setSingleDeviationValue(const SingleChannelInfo &singleInfo, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);
    QSqlQuery query(db);

    query.prepare("UPDATE public.mechanism_end SET deviation_x = :deviation_x, deviation_y = :deviation_y, deviation_z = :deviation_z WHERE gantry_id = :gantry_id AND name = :name");
    query.bindValue(":deviation_x", -MULTIPLY_BY_1000(singleInfo.deviation_x));
    query.bindValue(":deviation_y", -MULTIPLY_BY_1000(singleInfo.deviation_y));
    query.bindValue(":deviation_z", -MULTIPLY_BY_1000(singleInfo.deviation_z));
    query.bindValue(":gantry_id", singleInfo.gantry_id);
    query.bindValue(":name", singleInfo.name);

    if(!query.exec())
    {
        qDebug() << "更新gantry_id为：" << singleInfo.gantry_id << "，name为：" << singleInfo.name << "的单通道相对位置失败：" << query.lastError().text();
        return false;
    }

    return true;
}

//根据机位id获取机位偏差
void DBManager::getRackDevationById(Storage_Info &info, const QString &dataBaseName)
{
    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    QSqlQuery query(db);
    query.prepare("SELECT * FROM public.rack WHERE rack_id = :rack_id");
    query.bindValue(":rack_id", info.rack_id);

    if(!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return;
    }

    if(query.next())
    {
        info.deviation_x = DIVIDE_BY_1000(query.value("deviation_x").toInt());
        info.deviation_y = DIVIDE_BY_1000(query.value("deviation_y").toInt());
        info.deviation_z = DIVIDE_BY_1000(query.value("deviation_z").toInt());
    }
    else
    {
        qDebug() << "没有在 rack 表中找到机位号为：" << info.rack_id << "的记录";
        return;
    }
}

//根据载架名称获取载架id
int DBManager::getCarrierIdByName(const QString &carrierName, const QString &dataBaseName)
{
    int id = -1;

    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    QSqlQuery query(db);
    query.prepare("SELECT carrier_id FROM public.carrier WHERE carrier_name = :carrier_name");
    query.bindValue(":carrier_name", carrierName);

    if(!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return id;
    }

    if(query.next())
    {
        id = query.value("carrier_id").toInt();
    }
    else
    {
        qDebug() << "没有在 carrier 表中找到名为：" << carrierName << "的载架，请检查配置文件中的 carrier_name";
        return id;
    }

    return id;
}

//根据机位名称、机位腔室获取机位id
int DBManager::getRackIdByNameAndTable(const QString &rackName, int tableId, const QString &dataBaseName)
{
    int id = -1;

    QSqlDatabase db = getDatabaseConnection(dataBaseName);

    QSqlQuery query(db);
    query.prepare("SELECT rack_id FROM public.rack WHERE rack_name = :rack_name AND table_id = :table_id");
    query.bindValue(":rack_name", rackName);
    query.bindValue(":table_id", tableId);

    if(!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return id;
    }

    if(query.next())
    {
        id = query.value("rack_id").toInt();
    }
    else
    {
        qDebug() << "没有在 rack 表中找到 rack_name = " << rackName << "，table_id = " << tableId << "的机位，请检查配置文件中的 rack_name";
        return id;
    }

    return id;
}

//保存补偿策略
bool DBManager::saveMaterialStrategy(const QString &workFlowName, float k, float b, const QString &dataBaseName)
{
    //获取流程中的补偿策略
    QString strategyName = ParseMessageInfo::getMaterialStrategy(workFlowName);

    QSqlDatabase db = getDatabaseConnection(dataBaseName);
    QSqlQuery query(db);

    query.prepare("UPDATE public.material_kb SET k_value = :k_value, b_value = :b_value WHERE material_kb_name = :material_kb_name");
    query.bindValue(":k_value", MULTIPLY_BY_10000(k));
    query.bindValue(":b_value", MULTIPLY_BY_10000(b));
    query.bindValue(":material_kb_name", strategyName);

    if(!query.exec())
    {
        qDebug() << "更新补偿策略：" << strategyName << "失败：" << query.lastError().text();
        return false;
    }

    if(query.numRowsAffected() == 0)
    {
        qDebug() << "更新补偿策略：" << strategyName << "失败：未找到匹配的记录";
        return false;
    }

    qDebug() << "更新补偿策略：" << strategyName << "成功";
    return true;
}

//获取补偿策略
QVector<float> DBManager::getMaterialStrategy(const QString &workFlowName, const QString &dataBaseName)
{
    QVector<float> vector;

    //获取流程中的补偿策略
    QString strategyName = ParseMessageInfo::getMaterialStrategy(workFlowName);

    QSqlDatabase db = getDatabaseConnection(dataBaseName);
    QSqlQuery query(db);

    query.prepare("SELECT * FROM public.material_kb WHERE material_kb_name = :material_kb_name");
    query.bindValue(":material_kb_name", strategyName);

    if(!query.exec())
    {
        qDebug() << "查询失败：" << query.lastError().text();
        return vector;
    }

    if(query.next())
    {
        vector << DIVIDE_BY_10000(query.value("k_value").toInt());
        vector << DIVIDE_BY_10000(query.value("b_value").toInt());
    }
    else
    {
        qDebug() << "没有在 material_kb 表中找到 material_kb_name 为：" << strategyName << "的记录";
    }

    return vector;
}
