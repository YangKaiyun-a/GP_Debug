#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QObject>
#include <QDateTime>


/***************************宏**************************/
#define JSON_FILE_PATH(fileName) (QCoreApplication::applicationDirPath() + "/json/" + fileName)
#define WORKFLOW_FILE_PATH(workFlowName) (QCoreApplication::applicationDirPath() + "/json/workFlowConfig/" + workFlowName + ".json")

#define DIVIDE_BY_1000(x) ((x) / 1000.0)
#define DIVIDE_BY_10000(x) ((x) / 10000.0)

#define MULTIPLY_BY_1000(x) ((x) * 1000)
#define MULTIPLY_BY_10000(x) ((x) * 10000)

#define MULTIPLY_BY_60(x) ((x) * 60)


/***************************枚举类型******************************/

class ENUM_CLASS : public QObject {
    Q_OBJECT

public:
    explicit ENUM_CLASS(QObject *parent = nullptr);

    //PCR的三个部件
    enum PCR_Fuction_Type
    {
        Position_Accuracy,
        Temperature_Accuracy,
        Light_Accuracy
    };

    //当前语言
    enum Current_Language
    {
        Chinese,
        English
    };

    //PCR模块
    enum PCR_Id
    {
        M1,
        M2,
        Middle      //为了保持mqqt服务的一致性
    };
    Q_ENUM(PCR_Id)

    //PCR电机
    enum PCR_Motor
    {
        Cap,
        Block,
        Filter
    };

    //光学校准通道
    enum Light_Channel
    {
        NONE = -1,
        FAM = 0,
        HEX,
        TAMRA,
        ROX,
        CY5
    };
    Q_ENUM(Light_Channel)

    //位置校准的模块
    enum Position_Module
    {
        DJ1,
        DJ2,
        DP1,
        DP8,
        Logistics,
        DP8_Single,
        QPCR,
    };

    //移液器
    enum Gantry
    {
        Gantry_DP8 = 1,
        Gantry_DP1,
        Gantry_DJ2,
        Gantry_DJ1
    };

    //设置参数的返回值
    enum SetParams_Status
    {
        Uninit = -1,
        failed,
        Success
    };
};


/***************************结构体********************************/
//PCR中位机回复
struct MessageInfo
{
    QString uuid;
    QString command;
    QString result;
    QString resetObject;

    QString experiment_name = "";
    QString status = "";
};

//中位机回复
struct MiddleMessageInfo
{
    QString message = "";
    int status = -1;
};

//推入/推出位置参数
struct BlockParams
{
    QString getPhotoPosition = "";
    QString blockOutPosition = "";
};

struct PcrSteps
{
    int index;
    float temperature;
    int duration;
};

//实时温度
struct RealTimeTemperature
{
    QDateTime update_Time;
    float block1 = -1;
    float block2 = -1;
    float block3 = -1;
    float heatCap = -1;
};

//金属浴K、B
struct BlockCompenSate
{
    QString k = "1.0000";
    QString b = "0";

    bool error = false;
};

//传感器温度补偿值
struct BlockTemOffset
{
    QString temp1 = "1.0000";
    QString temp2 = "1.0000";
    QString temp3 = "1.0000";

    bool error = false;
};

//一些PCR的参数
struct ChannelParams
{
    QString channel1_position = "0";            //第一通道位置
    QString channel_gap = "0";                  //通道间隔

    int channel_1_exposure_time = 0;            //通道1曝光时间
    int channel_2_exposure_time = 0;            //通道2曝光时间
    int channel_3_exposure_time = 0;            //通道3曝光时间
    int channel_4_exposure_time = 0;            //通道4曝光时间
    int channel_5_exposure_time = 0;            //通道5曝光时间

    QString channel_1_potency = "0";            //通道1浓度系数
    QString channel_2_potency = "0";            //通道1浓度系数
    QString channel_3_potency = "0";            //通道1浓度系数
    QString channel_4_potency = "0";            //通道1浓度系数
    QString channel_5_potency = "0";            //通道1浓度系数

    bool error = false;
};

//Sample参数
struct SampleParams
{
    float k = 0.00;
    float b = 0.00;
    int duration = 0;
};

//机位
struct Rack
{
    int rack_id = 0;            //机位号
    int table_id = 0;           //台面号

    QString rack_name = "";     //机位名称

    float loc_x = 0;
    float loc_y = 0;
    float loc_z = 0;

    float deviation_x = 0;      //机位偏差
    float deviation_y = 0;
    float deviation_z = 0;
};

//载架
struct Carrier
{
    int carrier_id = 0;         //载架号

    float loc_x = 0;
    float loc_y = 0;
    float loc_z = 0;

    float crossing_x = 0;       //十字标记偏差坐标
    float crossing_y = 0;
    float crossing_z = 0;
};

//一条位置信息（机位 + 载架）
struct Storage_Info
{
    int pipette_id = 0;         //该位置所属移液器

    int rack_id = 0;            //机位号
    QString rack_name = "";     //机位名称

    int carrier_id = 0;         //载架号
    QString carrier_name = "";  //载架名称

    float loc_x = 0;            //理论坐标（机位坐标 + 载架坐标）
    float loc_y = 0;
    float loc_z = 0;

    float crossing_x = 0;       //十字标记偏差坐标
    float crossing_y = 0;
    float crossing_z = 0;

    float deviation_x = 0;      //板位偏差
    float deviation_y = 0;
    float deviation_z = 0;
};

//一个移液器的位置信息
struct PipetteInfo
{
    int module_id = 0;                //移液器id

    float distance_x_from_orig = 0;   //移液器坐标（移液器复位位置）
    float distance_y_from_orig = 0;
    float distance_z_from_orig = 0;

    float deviate_x = 0;              //移液器偏差
    float deviate_y = 0;
    float deviate_z = 0;
};

//夹爪位置信息
struct ClawInfo
{
    int gantry_id = 0;
    QString name = "";

    float distance_x = 0;           //夹爪相对葫芦头位置
    float distance_y = 0;
    float distance_z = 0;

    float distance_end_x = 0;       //夹爪相对葫芦头位置的最终值
    float distance_end_y = 0;
    float distance_end_z = 0;

    float loc_x = 0;                //夹爪位置 = 移液器坐标 + 夹爪相对葫芦头位置
    float loc_y = 0;
    float loc_z = 0;

    float deviation_x = 0;          //夹爪偏差
    float deviation_y = 0;
    float deviation_z = 0;
};

//激光位置信息
struct LaserInfo
{
    float distance_x = 0;           //激光相对葫芦头位置
    float distance_y = 0;
    float distance_z = 0;

    float loc_x = 0;                //激光位置
    float loc_y = 0;
    float loc_z = 0;
};

//单通道位置信息
struct SingleChannelInfo
{
    int gantry_id = 0;
    QString name = "";

    float distance_x = 0;           //单通道相对葫芦头位置
    float distance_y = 0;
    float distance_z = 0;

    float distance_end_x = 0;       //单通道相对葫芦头位置的最终值
    float distance_end_y = 0;
    float distance_end_z = 0;

    float loc_x = 0;                //单通道位置
    float loc_y = 0;
    float loc_z = 0;

    float deviation_x = 0;          //单通道偏差
    float deviation_y = 0;
    float deviation_z = 0;
};

//十字标记偏差
struct CrossingDeviation
{
    int rack_id = 0;            //机位id
    int table_id = 0;           //机位所处腔室
    QString rack_name = "";     //机位名称

    int carrier_id = 0;         //载架id
    QString carrier_name = "";  //载架名称

    float crossing_x = 0;       //十字标记偏差坐标
    float crossing_y = 0;
    float crossing_z = 0;
};

//激光相对于葫芦头位置
struct LaserDistance
{
    float distance_x = 0;           //激光相对葫芦头位置
    float distance_y = 0;
    float distance_z = 0;
};



/*****************************PCR-接口Topic**************************/
//上位机发布
#define TC384_CONTROL_DOWN "/tc384/control/down"

//中位机回复
#define TC384_CONTROL_UP "/tc384/control/up"

//上位机发布
#define TC384_DEBUG_DOWN "/tc384/debug/down"

//中位机位机回复
#define TC384_DEBUG_UP "/tc384/debug/up"

//中位机发布
#define TC384_CHANGE_UP "/tc384/change/up"

//上位机回复
#define TC384_CHANGE_DOWN "/tc384/change/down"


/*****************************中位机-接口Topic**************************/

//发送载架
#define LOAD_CARRIERS "hcsci/ctrl/cmd/load_carriers"
//中位机回复
#define LOAD_CARRIERS_REPLY "hcsci/ctrl/reply/load_carriers"

//发送流程
#define LOAD_TEST_PROJECT "hcsci/ctrl/cmd/load_test_project"
//中位机回复
#define LOAD_TEST_PROJECT_REPLY "hcsci/ctrl/reply/load_test_project"

//发送台面
#define LOAD_PROCESS_TABLE "hcsci/ctrl/cmd/load_process_table"
//中位机回复
#define LOAD_PROCESS_TABLE_REPLY "hcsci/ctrl/reply/load_process_table"

//运行流程
#define RUN_TEST_PROJECT "hcsci/ctrl/cmd/run_test_project"
//中位机回复
#define RUN_TEST_PROJECT_REPLY "hcsci/ctrl/reply/run_test_project"

//整机复位
#define MACHINE_RESET "hcsci/ctrl/cmd/machine_reset"
//中位机回复
#define MACHINE_RESET_REPLY "hcsci/ctrl/reply/machine_reset"



#endif // GLOBAL_H
