#ifndef ABSTRACTPART_H
#define ABSTRACTPART_H

#include <QObject>

const int CONTROL_VALUE = 0X01;
const int STOP_VALUE = 0X00;
#define ACTION_CONTROL (const_cast<int*>(&CONTROL_VALUE))
#define STOP_CONTROL (const_cast<int*>(&STOP_VALUE))

#define DOUBLE_TO_OD_DATA(value) (value / 1000.0)
#define INT_TO_OD_DATA(value) (value / 1000.0)
#define DOUBLE_FROM_OD_DATA(value) (value * 1000)
#define INT_FROM_OD_DATA(value) (value * 1000)
#define BOARD_INDEX_SPACE (0x20)

/**********Motion module*************/
//board node
#define MOTION_NODE_ID(motionIndex) ((motionIndex) == (0) ? (DevicePart::NODE_MOTION_1) : (DevicePart::NODE_MOTION_2))
//dp24 index
const quint16 DP24_BASE_INDEX = 0x2400;
#define DP24_INDEX(dp24Index, offset) (DP24_BASE_INDEX + (offset))
//rack index
const quint16 RACK_BASE_INDEX = 0x2900;
#define RACK_INDEX(rackIndex, offset) (RACK_BASE_INDEX + (offset))

/**********Heatseal module*************/
//index
const quint16 HEATSEAL_BASE_INDEX = 0x2680;
#define HEATSEAL_INDEX(offset) (HEATSEAL_BASE_INDEX + (offset))

/**********Heatshake module*************/
//index
const quint16 HEATSHAKE_BASE_INDEX = 0x2E80;
#define HEATSHAKE_INDEX(offset) (HEATSHAKE_BASE_INDEX + (offset))

/**********COLDSTORE module*************/
//index
const quint16 COLDSTORE_BASE_INDEX = 0x2700;
#define COLDSTORE_INDEX(offset) (COLDSTORE_BASE_INDEX + (offset))

/**********STACK module********************/
const quint16 STACK_PROFESSION_BASE_INDEX = 0x2A00;
#define STACK_INDEX_PROFESSION(store, offset) (STACK_PROFESSION_BASE_INDEX + (offset))
const quint16 STACK_DEBUD_BASE_INDEX = 0x29A0;
#define STACK_INDEX_DEBUG(store, offset) (STACK_DEBUD_BASE_INDEX + (offset))
#define NODE_STACK(part) ((part) == (DevicePart::CHAMBER_1) ? (DevicePart::NODE_STACK_A) : (DevicePart::NODE_STACK_B))

/**********STACK module********************/
const quint16 TRANSPORTER_BASE_INDEX = 0x2A80;
#define TRANSPORTER_INDEX(offset) (TRANSPORTER_BASE_INDEX + (offset))

/**********AIR module*************/
//index
const quint16 AIR_BASE_INDEX = 0x2C80;
#define AIR_INDEX(offset) (AIR_BASE_INDEX + (offset))

/**********MULTI FUCTION******************/
#define NODE_MULTIFUCTION(part) ((part) == (DevicePart::CHAMBER_1) ? (DevicePart::NODE_MULTI_FUCTION_A) : (DevicePart::NODE_MULTI_FUCTION_B))


#define NODE_UVLIGHT(part) ((part) == (DevicePart::CHAMBER_1) ? (DevicePart::NODE_UVLIGHT_LEFT) : (DevicePart::NODE_UVLIGHT_RIGHT))


//磁力架
const quint16 MAGNENTIC_BASE_INDEX = 0x2780;
#define MAGNENTIC_INDEX(offset) (MAGNENTIC_BASE_INDEX + (offset))
//卡扣
const quint16 LOCKS_BASE_INDEX = 0x2B00;
#define LOCKS_INDEX(offset) (LOCKS_BASE_INDEX + (offset))
//利器盒、废液槽
const quint16 DUSTBIN_BASE_INDEX = 0x2D80;
#define DUSTBIN_INDEX(offset) (DUSTBIN_BASE_INDEX + (offset))
//腔室二夹爪
const quint16 CHAMBER2_BASE_INDEX = 0x2880;
#define CHAMBER2_INDEX(offset) (CHAMBER2_BASE_INDEX + (offset))
const quint16 CHAMBER2_MODULE_RESET_INDEX = 0x2400;
//气压控制
const quint16 AIRPRESSURE_BASE_INDEX = 0x2C80;
#define AIRPRESSURE_INDEX(offset) (AIRPRESSURE_BASE_INDEX + (offset))
//外设
const quint16 PERIPHERALS_BASE_INDEX = 0x2B80;
#define PERIPHERALS_INDEX(offset) (PERIPHERALS_BASE_INDEX + (offset))
//紫外灯
const quint16 UVLIGHT_BASE_INDEX = 0x2C00;
#define UVLIGHT_INDEX(offset) (UVLIGHT_BASE_INDEX + (offset))
//DP1
const quint16 DP1_BASE_INDEX = 0x2500;
#define DP1_INDEX(offset) (DP1_BASE_INDEX + (offset))
//DP8
const quint16 DP8_BASE_INDEX = 0x2580;
#define DP8_INDEX(offset) (DP8_BASE_INDEX + (offset))
//XY
const quint16 XY_BASE_INDEX = 0x2900;
#define XY_INDEX(offset) (XY_BASE_INDEX + (offset))
//CLAW
const quint16 CLAW_BASE_INDEX = 0x2880;
#define CLAW_INDEX(offset) (CLAW_BASE_INDEX + (offset))
//DJ1
const quint16 DJ1_BASE_INDEX = 0x2E00;
#define DJ1_INDEX(offset) (DJ1_BASE_INDEX + (offset))
//SWITCH_SIGNAL
const quint16 SWITCH_SIGNAL_BASE_INDEX = 0x2F00;
#define SWITCH_SIGNAL_INDEX(offset) (SWITCH_SIGNAL_BASE_INDEX + (offset))
//XZ_CLAW
const quint16 XZ_CLAW_BASE_INDEX = 0x2890;
#define XZ_CLAW_INDEX(offset) (XZ_CLAW_BASE_INDEX + (offset))

#define MODULE_RESET_INDEX 		0x5F01
#define MODULE_PAUSE_INDEX 		0x5F02
#define MODULE_RESUME_INDEX 	0x5F03

//单个部件
namespace DevicePart {
    //部件所属腔室
    enum {
        CHAMBER_1 = 0,
        CHAMBER_3 = 1,
        CHAMBER_2 = 2
    };

    //电机运动参数
    struct MotorMotionParams {
        float targetPosition;
        float speed;
        int sleep_time;

        MotorMotionParams() {
            targetPosition = 0.0;
            speed = 0.0;
        }
    };

    //故障的错误值
    enum ErrorCode {
        NO = 0,     //正常
        REPEATED,   //重复执行
        WRITE,      //写错误
        READ,       //读错误
        TIMEOUT     //超时
    };

    enum Module_Id{
        NODE_DP8_XY = 0x03,
        NODE_DP8 = 0x04,
        NODE_DP1_XY = 0x23,
        NODE_DP1_JAW = 0x24,
        NODE_EXTRACT_DJ1 = 0x05,
        NODE_AMPLIFICATION_DJ1 = 0x07,

        NODE_EXTRACT_COOLSTORE = 0x08,          //冷存模块
        NODE_AMPLIFICATION_COOLSTORE = 0x09,    //扩增制冷位

        NODE_SWITCH_SIGNAL = 0x0A,              //废料门
        NODE_XZ_CLAW = 0x0E,                    //QPCR夹爪
        NODE_ENVIRONMENT_MONITOR = 0x0C,

        NODE_HEAT_SHAKE_1 = 0x26,               //振荡的6个模块
        NODE_HEAT_SHAKE_2 = 0x27,
        NODE_HEAT_SHAKE_3 = 0x28,
        NODE_HEAT_SHAKE_4 = 0x29,
        NODE_HEAT_SHAKE_5 = 0x2A,
        NODE_HEAT_SHAKE_6 = 0x2B,

        NODE_LIGHT_STATUA = 0X0B,               //左侧、中间紫外灯/照明灯
        NODE_LIGHT_STATUA_RIGHT = 0X0E,         //右侧紫外灯
        NODE_PRESSURE_CAPACITANCE= 0x2D,
        NODE_DJ_CAPACITANCE_1= 0x2E,
        NODE_DJ_CAPACITANCE_2= 0x2F,

        NODE_HEATSEAL = 0x06,                   //热封
        NODE_TRANSPORTER = 0x22,                //转运模块
    };

    enum Temperature_Compensation_Number
    {
        tempCompensateion_1,
        tempCompensateion_2
    };
}

class AbstractPart : public QObject
{
    Q_OBJECT

public:
    explicit AbstractPart(QObject *parent = nullptr);
    ~AbstractPart() { };

public:
    virtual int motorReset(int motorIndex) = 0;
    virtual int motorMove(int motorIndex, const DevicePart::MotorMotionParams&) = 0;
};

#endif // ABSTRACTPART_H
