#ifndef PARSEMESSAGEINFO_H
#define PARSEMESSAGEINFO_H
#include "global.h"

#include <QObject>
#include <QMetaEnum>

#define ParseInstance ParseMessageInfo::instance()


class ParseMessageInfo : public QObject
{
    Q_OBJECT
public:
    explicit ParseMessageInfo(QObject *parent = nullptr);
    static ParseMessageInfo* instance();

    /**********************解析mqtt************************/

    //解析PCR接收消息
    static MessageInfo parseMessageReceived(const QString &data);
    //解析中位机回复
    static MiddleMessageInfo parseMiddleReceived(const QString &data);



    /***************优雅地将枚举转换为字符串*****************/
    template<typename T>
    static QString enumToString(T id)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        return metaEnum.valueToKey(static_cast<int>(id));
    }


    /************************解析各种*************************/
    //解析顶升位参数（热盖下压位置）
    static float parseCapUpPositionQString(const QString &mechanismParams);
    //解析推入/推出位置参数
    static BlockParams parseBlockPositionQString(const QString &mechanismParams);
    //从文件中读取json并返回普通字符串
    static QString readJsonFromFile(const QString& fileName);
    //组合完整的RDML
    static QString createCompleteJson(const QString &completeJsonFile, const QString &currentPcrConfig);
    //解析相机曝光时间
    static void parseExposureTimeJson(const QString &json, ChannelParams &params);
    //解析PCR循环步骤
    static QVector<PcrSteps> parsePcrConfigParams(const QString &pcrConfig);
    //解析PCR实时温度
    static RealTimeTemperature parseRealTimeTemperature(const QString &json);
    //修改PCR方案中的参数
    static QString modifyScheme(const QString &json, int heatLidTemperature, int blockTemperature, int count);
    //解析荧光校准第一通道位置
    static void parseFirstChannelPosition(const QString &json, ChannelParams &params);
    //解析浓度系数
    static void parsePotency(const QString &json, ChannelParams &params);
    //解析热盖补偿值
    static QString parseTempOffset(const QString &json);
    //解析k、b
    static BlockCompenSate parseBlockCompensate(const QString &json);
    //解析传感器温度补偿值
    static BlockTemOffset parseBlockTemOffset(const QString &json);



    /************************载架十字标记偏差*************************/
    //初始化载架十字标记偏差、激光相对葫芦头位置
    static void initDeviationFromJson(const QString &jsonPath);
    //根据rack_id来获取标记偏差、载架
    static void getCrossingDeviatio(Storage_Info &info);
    //根据table_id与rack_name获取rack_id
    static int getRackIdByTableIdAndRackName(int table_id, const QString &rack_name);
    //获取DP1的激光相对葫芦头位置
    static void getLaserDistance(LaserInfo &info);
    //获取DP8的激光复位Z0（全局）
    static float resetZ0();



    /************************解析中位机相关*************************/
    //获取移液流程
    static QString getWorkFlowJson(const QString &workFlowName);
    //根据流程json获取载架名称
    static QString getCarrierTableName(const QString &json);
    //获取台面名称
    static QString getProcessTableName(int id, const QString &workFlowName);
    //获取移液流程中的补偿策略
    static QString getMaterialStrategy(const QString &workFlowName);


private:
    static ParseMessageInfo *m_parseMessageInfo;
    static QMap<int, CrossingDeviation> m_mapCrossing;  //存储标记偏差、机位与载架的对应关系，以rack_id为索引
    static LaserDistance m_laserDistance;               //激光相对葫芦头位置
    static float m_resetZ0;                             //激光复位Z0（全局）
};

#endif // PARSEMESSAGEINFO_H
