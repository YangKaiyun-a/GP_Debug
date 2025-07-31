#ifndef AMPLIFICATION_DJ1_H
#define AMPLIFICATION_DJ1_H
#include "../abstractpart.h"
#include <QVector>

class Amplification_Dj1 : public AbstractPart
{

public:
    enum
    {
        MOTOR_X = 0,
        MOTOR_Y = 1,
        MOTOR_CLAW = 2,
        MOTOR_CLAW_Z = 3,
        MOTOR_DJ1_Z = 4,
        MOTOR_PIPET = 5,
    };
    enum
    {
        XY_MOVE,
        XY_MOTOR_RESET = 1,
        XY_MOTOR_MOVE = 2,
    };

    enum
    {
        CLAW_GLAB = 0x00,
        CLAW_RELEASE = 0x01,
        CLAW_TO_Z = 0x02,
        CLAW_TO_Z_GLAB = 0x03,
        CLAW_TO_Z_RELEASE = 0x04,
        CLAW_MOTOR_RESET = 0x05,
        CLAW_MOTOR_MOVE = 0x06,
    };

        enum
    {
        MODUL_RESET = 0X00,
        GET_TIP = 0X01,
        REMOVE_TIP = 0X02,
        ASPIRATE = 0x03,
        SQUIRT = 0x04,
        LIQUID_MIX = 0x05,
        PARAMETER = 0x06,
        PAUSE = 0x07,
        RESUME = 0x08,
        MOTOR_RESET = 0x09,
        MOTOR_MOVE = 0x0A,
        RELATIVE_ASPIRATE = 0x0B,
        INIT_CLEAN = 0x0E,
        REMAINDER_CHECK_CLEAN = 0x0F,
        ROUTINE_CLEAN = 0x10,
        CLEANING_LIQUID_CLEAN = 0x11,
        OPEN_LITTLE_INSIDE_VALVE = 0X12,
        CLOSE_LITTLE_INSIDE_VALVE = 0X13,
        OPEN_BIG_INSIDE_VALVE = 0X14,
        CLOSE_BIG_INSIDE_VALVE = 0X15,
        OPEN_OUTSIDE_VALVE = 0X16,
        CLOSE_OUTSIDE_VALVE = 0X17,
        OPEN_INSIDE_PUMP = 0X18,
        CLOSE_INSIDE_PUMP = 0X19,
        OPEN_OUTSIDE_PUMP = 0X1A,
        CLOSE_OUTSIDE_PUMP = 0X1B,
        OPEN_WASTE_PUMP = 0X1C,
        CLOSE_WASTE_PUMP = 0X1D,
        OPEN_CLEAN_VALVE = 0X1E,
        CLOSE_CLEAN_VALVE = 0X1F,
        OPEN_CLEAN_PUMP = 0X20,
        CLOSE_CLEAN_PUMP = 0X21,
        };

    struct AmplificationDj1Params
    {
        float little_pump_start_speed_low;
        float little_pump_low_acc_low;
        float little_pump_threshold_speed_low;
        float little_pump_high_acc_low;
        float little_pump_high_dec_low;
        float little_pump_low_dec_low;
        float little_pump_stop_speed_low;

        float little_pump_start_speed_mid;
        float little_pump_low_acc_mid;
        float little_pump_threshold_speed_mid;
        float little_pump_high_acc_mid;
        float little_pump_high_dec_mid;
        float little_pump_low_dec_mid;
        float little_pump_stop_speed_mid;

        float little_pump_start_speed_high;
        float little_pump_low_acc_high;
        float little_pump_threshold_speed_high;
        float little_pump_high_acc_high;
        float little_pump_high_dec_high;
        float little_pump_low_dec_high;
        float little_pump_stop_speed_high;

        float dj1_z_fly_position;
        float dj1_z_speed;
        float routine_clean_z_offset;
        float cleaning_liquid_z_offset;
        float pure_water_z_offset;
        float remainder_check_clean_z_offset;
        float init_clean_z_offset;
        float pump_return_difference_volume;
        float claw_z_speed;
        float routine_clean_aspirate_v1_volume;
        float routine_clean_aspirate_v2_volume;
        float routine_clean_aspirate_speed;
        float routine_clean_z_leave_speed;
        float routine_clean_aspirate_air_volume;
        float cleaning_clean_aspirate_v1_volume;
        float cleaning_clean_aspirate_v2_volume;
        float cleaning_clean_aspirate_speed;
        float cleaning_clean_z_leave_speed;
        float cleaning_clean_aspirate_air_volume;
    };

    //吸液参数
    struct AspiraterParams {
        int aspirateStrategy;
        int preAspirateVolume;
        int preSquirtVolume;
        int preAspirateSpeed;
        int preAspirateAccDecNum;
        int aspirateVolume;
        int aspirateCompensateVolume;
        int aspirateSpeed;
        int aspirateAccDecNum;
        int squitTrimVolume;
        int squitTrimSpeed;
        int squirtAccDecNum;
        float leaveLiquidZPosition;
        int leaveLiquidZSpeed;
        int postAspirateVolume;
        int postAspirateSpeed;
        int postAspirateAccDecNum;
        float aspirateZPreparePosition;
        float aspirateZPrepareSpeed;
        float aspirateZPositionStart;
        float aspirateZPositionMiddle;
        float aspirateZPositionEnd;
        float aspirateZSpeed;
        float sectionalArea;
        float adjustment_k;
        float adjustment_b;
        int check_threshold;
        float finish_z_position;
        float finish_z_speed;
    };

    //喷液参数
    struct SquirtParams {
        int squirtStrategy;
        int squirtVolume;
        int squitCompensateVolume;
        int squirtSpeed;
        int squirtAccDecNum;
        float leaveLiquidZPosition;
        int leaveLiquidZSpeed;
        int postAspirateVolume;
        int postAspirateSpeed;
        int postAspirateAccDecNum;
        float squirtZPreparePosition;
        float squirtZPrepareSpeed;
        float squirtZPositionStart;
        float squirtZPositionMid;
        float squirtZPositionEnd;
        float squirtZSpeed;
        float sectionalArea;
        float adjustment_k;
        float adjustment_b;
        int check_threshold;
        float finish_z_position;
        float finish_z_speed;
    };

    //吹打参数
    struct LiquidMixParams {
        int mixStrategy;
        int preAspirateVolume;
        int preSquirtVolume;
        int preAspirateSpeed;
        int preAspirateAccDecNum;
        int aspirateSpeed;
        int squirtSpeed;
        int pipetAccDecNum;
        int firstAspirateVolume;
        int firstAspirateCompensateVolume;
        float squirtZPosition;
        int squirtVolume;
        float aspirateZPosition;
        int aspirateVolume;
        int mixTimes; //吹打次数
        int lastSquirtVolume;
        int lastSquitCompensateVolume;
        float leaveLiquidZPosition;
        int leaveLiquidZSpeed;
        float postAspirateVolume;
        int postAspirateSpeed;
        int postAspirateAccDecNum;
        float mixZPosition_sta;
        float mixZPosition_mid;
        float mixZPosition_end;
        float mixZSpeed;
        float sectionalArea;
        float adjustment_k;
        float adjustment_b;
        int check_threshold;
        float finish_z_position;
        float finish_z_speed;
    };

    //支架运动参数
    struct RackMotionParams {
        int posNumber;
        float speed;
        QVector<float> positions; //下标偶序列为X位置，奇序列为Y位置
    };

public:
    Amplification_Dj1();

    static Amplification_Dj1* instance();

    quint16 select_motor_reset_index(int motor_id);
    quint16 select_motor_move_index(int motor_id);

    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    int module_reset();
    int aspirate(const AspiraterParams& param); //吸液
    int squirt(const SquirtParams& param); //喷液
    int liquidMix(const LiquidMixParams& param); //吹打
    Q_INVOKABLE int pause(); //暂停
    Q_INVOKABLE int resume(); //恢复
    Q_INVOKABLE int move(Amplification_Dj1::RackMotionParams param);

    int init_clean_start(float z_position);
    int remainder_check_clean_start(float z_position);
    int routine_clean_strart(int time, float z_position);
    int cleaning_liqud_clean_start(int cleaning_liquid_clean_time, int pure_water_clean_time,
                                   float z_position);
    int open_inside_valve();
    int close_inside_valve();
    int open_outside_valve();
    int close_outside_valve();
    int open_inside_pump();
    int close_inside_pump();
    int open_outside_pump();
    int close_outside_pump();
    int open_waste_pump();
    int close_waste_pump();
    int open_clean_pump();
    int close_clean_pump();
    int open_clean_valve();
    int close_clean_valve();
    int glab(float glabWidth);
    int release(float releaseWidth);
    int toZ(float zPosition, float openWidth);
    int toZGlab(float zPosition, float openWidth, float closeWidth, float finish_z_position);
    int toZRelease(float zPosition, float releaseWidth);
    int relative_aspirate(float aspirate_speed, int acc_num, float aspirate_volume);


    //参数
    int getParameter(AmplificationDj1Params& params);
    int setParameter(const AmplificationDj1Params& params);
};

#endif // AMPLIFICATION_DJ1_H
