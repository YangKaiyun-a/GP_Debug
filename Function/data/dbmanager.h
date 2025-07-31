#ifndef DBMANAGER_H
#define DBMANAGER_H
#include "global.h"


#include <QObject>
#include <QSqlDatabase>

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    static DBManager *instance();
    bool connectDataBase();
    QSqlDatabase getDatabaseConnection(const QString& groupName);

    /******************PCR********************/

    //获取顶升位参数（获取热盖下压位置）
    float getCapUpPosition(ENUM_CLASS::PCR_Id id);

    //获取推入/推出位参数
    BlockParams getBlockPosition(ENUM_CLASS::PCR_Id id);

    //获取热盖补偿值
    QString getCapTemOffset(ENUM_CLASS::PCR_Id id);

    //获取传感器温度补偿值
    BlockTemOffset getBlockTemOffset(ENUM_CLASS::PCR_Id id);

    //保存曝光时间
    bool saveExposureTime(ENUM_CLASS::PCR_Id id, const QString &json);
    //读取曝光时间
    QString getExposureTime(ENUM_CLASS::PCR_Id id);

    //保存第一通道参数
    bool saveFirstChannelPosition(ENUM_CLASS::PCR_Id id, const QString &position);
    //获取第一通道参数
    QString getFirstChannelPosition(ENUM_CLASS::PCR_Id id);

    //获取K、B
    BlockCompenSate getBlockCompenSate(ENUM_CLASS::PCR_Id id);

    //获取PCR实时温度
    RealTimeTemperature getRealTimeTemperature(ENUM_CLASS::PCR_Id id);

    //获取校准矩阵
    QString getAdjustMatrix(ENUM_CLASS::PCR_Id id);
    //获取浓度系数
    QString getPotency(ENUM_CLASS::PCR_Id id);



    /******************位置校准********************/

    //根据机位id获取并组合一条位置信息
    void getStorageInfoByCarrierId(Storage_Info &info, const QString &dataBaseName = "Position");
    //根据module_id获取移液器信息
    PipetteInfo getPipetteInfoByModuleId(int moduleId, const QString &dataBaseName = "Position");
    //根据gantry_id获取夹爪相对葫芦头位置
    ClawInfo getClawInfoByGantryId(int gantryId, const QString &dataBaseName = "Position");
    //根据gantry_id获取单通道相对葫芦头位置
    SingleChannelInfo getSingleInfoByGantryId(int gantryId, const QString &dataBaseName = "Position");
    //根据机位id获取机位坐标
    Rack getRackInfo(int id, const QString &dataBaseName = "Position");
    //保存移液器偏差
    bool setPipeteeDeviationValue(const PipetteInfo &info, const QString &dataBaseName = "Position");
    //保存机位偏差值
    bool setDeviationValue(const Storage_Info &info, const QString &dataBaseName = "Position");
    //保存夹爪和葫芦头相对位置的偏差
    bool setClawDeviationValue(const ClawInfo &clawInfo, const QString &dataBaseName = "Position");
    //保存单通道和葫芦头相对位置的偏差
    bool setSingleDeviationValue(const SingleChannelInfo &singleInfo, const QString &dataBaseName = "Position");
    //根据机位id获取机位偏差
    void getRackDevationById(Storage_Info &info, const QString &dataBaseName = "Position");
    //根据载架名称获取载架id
    int getCarrierIdByName(const QString &carrierName, const QString &dataBaseName = "Position");
    //根据机位名称、机位腔室获取机位id
    int getRackIdByNameAndTable(const QString &rackName, int tableId, const QString &dataBaseName = "Position");


    /******************移液流程相关********************/

    //保存补偿策略
    bool saveMaterialStrategy(const QString &workFlowName, float k, float b, const QString &dataBaseName = "Position");
    //获取补偿策略
    QVector<float> getMaterialStrategy(const QString &workFlowName, const QString &dataBaseName = "Position");



private:
    static DBManager* m_dbManager;

};

#endif // DBMANAGER_H
