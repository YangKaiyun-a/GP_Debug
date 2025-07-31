#ifndef DP8_XY_H
#define DP8_XY_H
#include <QVector>
#include "../abstractpart.h"

class Dp8_Xy : public AbstractPart
{
public:
    enum
    {
        MOTOR_X = 0,
        MOTOR_Y = 1,
        MOTOR_DP1_Z = 2,
        MOTOR_DP1_PIPET = 3,
    };
    enum
    {
        MODUL_ERESET = 0X00,
        GET_TIP = 0X01,
        REMOVE_TIP = 0X02,
        ASPIRATE = 0x03,
        SQUIRT = 0x04,
        LIQUID_MIX = 0x05,
        PARAMETER = 0x06,
        PAUSE = 0x07,
        RESUME = 0x08,
        RELATIVE_ASPIRATE = 0x14,
        MOTOR_RESET = 9,
        MOTOR_MOVE = 10,
        XY_MOVE = 0,
        XY_MOTOR_RESET = 1,
        XY_MOTOR_MOVE = 2,
    };

    //吸液参数
    struct AspiraterParams {
        int aspirateStrategy;
        int preAspirateVolume;
        int aspirateVolume1;
        int aspirateSpeed1;
        float wait_time;
        int aspirateVolume2;
        int aspirateCompensateVolume;
        int aspirateSpeed2;
        float underLiquidWaitTime; //单位: s
        int squitTrimVolume;
        int squitTrimSpeed;
        float squirt_wait_time;
        float leaveLiquidZPosition;
        int leaveLiquidZSpeed;
        int postAspirateVolume;
        int postAspirateSpeed;
        float aspirateZPositionStart;
        float aspirateZPositionMiddle;
        float aspirateZPositionEnd;
        float sectionalArea;
        int flux;
        float adjustment_k;
        float adjustment_b;
        int through_film_symbol;
        int use_claw_symbol;
        float through_film_z_position;
        float through_film_z_offset;
        int check_channel;
        int check_threshold;
        float finish_z_position;
        int cross_expand_symbol;

        AspiraterParams() {
            aspirateStrategy = 0;
            preAspirateVolume = 20;
            aspirateVolume1 = 10;
            aspirateSpeed1 = 10;
            aspirateVolume2 = 10;
            aspirateCompensateVolume = 10;
            squitTrimVolume = 10;
            aspirateSpeed2 = 10;
            underLiquidWaitTime = 1.0; //单位: s
            leaveLiquidZPosition = 10.0;
            leaveLiquidZSpeed = 10;
            postAspirateVolume = 10;
            postAspirateSpeed = 2;
            postAspirateVolume = 10;
            postAspirateSpeed = 10;
            aspirateZPositionStart = 50.0;
            aspirateZPositionMiddle = 0.0;
            aspirateZPositionEnd = 0.0;
            sectionalArea = 0.0;
            flux = 8;
            adjustment_k = 1.0;
            adjustment_b = 0.0;
            through_film_symbol = 0;
            use_claw_symbol = 0;
            through_film_z_position = 0.0;
            through_film_z_offset = 0.0;
            check_channel = 0;
            check_threshold = 0;
        }
    };

    //喷液参数
    struct SquirtParams {
        int squirtStrategy;
        int squirtVolume_1;
        int squirtSpeed_1;
        float wait_time;
        int squirtVolume_2;
        int squitCompensateVolume;
        int squirtSpeed_2;
        float underLiquidWaitTime; //单位: s
        float leaveLiquidZPosition;
        int leaveLiquidZSpeed;
        int postAspirateVolume;
        int postAspirateSpeed;
        float squirtZPositionStart;
        float squirtZPositionMid;
        float squirtZPositionEnd;
        float sectionalArea;
        int flux;
        float adjustment_k;
        float adjustment_b;
        int check_channel;
        int check_threshold;


        SquirtParams() {
            squirtStrategy = 0;
            squirtVolume_1 = 20;
            squitCompensateVolume = 10;
            squirtSpeed_1 = 20;
            underLiquidWaitTime = 1.0;
            leaveLiquidZPosition = 10.0;
            leaveLiquidZSpeed = 3;
            postAspirateVolume = 20;
            postAspirateSpeed = 10;
            squirtZPositionStart = 50.0;
            sectionalArea = 0.0;
            flux = 8;
        }
    };

    //吹打参数
    struct LiquidMixParams {
        int mixStrategy;
        int preAspirateVolume;
        int firstAspirateVolume;
        int firstAspirateSpeed;
        int mixVolume;
        int mixTimes; //吹打次数
        int mixSpeed;
        int lastSquirtVolume_1;
        int lastSquirtSpeed_1;
        float wait_time;
        int lastSquirtVolume_2;
        int squitCompensateVolume;
        int lastSquirtSpeed_2;
        float underLiquidWaitTime; //单位: s
        float leaveLiquidZPosition;
        int leaveLiquidZSpeed;
        float postLiquidZPosition;
        int postLiquidZSpeed;
        float mixZPosition_sta;
        float mixZPosition_mid;
        float mixZPosition_end;
        float sectionalArea;
        int flux;
        float adjustment_k;
        float adjustment_b;
        int check_channel;
        int check_threshold;

        LiquidMixParams() {
            mixStrategy = 0;
            preAspirateVolume = 20;
            firstAspirateVolume = 20;
            firstAspirateSpeed = 10;
            mixVolume = 20;
            mixTimes = 5; //吹打次数
            mixSpeed = 20;
            lastSquirtVolume_1 = 20;
            lastSquirtSpeed_1 = 10;
            underLiquidWaitTime = 1.0;
            leaveLiquidZPosition = 30.0;
            leaveLiquidZSpeed = 2;
            postLiquidZPosition = 20.0;
            postLiquidZSpeed = 10;
            mixZPosition_sta = 50.0;
            sectionalArea = 0.0;
            flux = 8;
        }
    };

    //支架运动参数
    struct RackMotionParams {
        int posNumber;
        float speed;
        QVector<float> positions; //下标偶序列为X位置，奇序列为Y位置
    };

public:
    Dp8_Xy();
    static Dp8_Xy* instance();

    quint16 select_motor_reset_index(int motor_id);
    quint16 select_motor_move_index(int motor_id);

    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    int module_reset();
    Q_INVOKABLE int getTip(float z_pos); //取头
    Q_INVOKABLE int removeTip(float z_pos); //退头
    int aspirate(const AspiraterParams& param); //吸液
    int squirt(const SquirtParams& param); //喷液
    int liquidMix(const LiquidMixParams& param); //吹打
    int move(const Dp8_Xy::RackMotionParams& param);
    int relative_aspirate(float aspirate_speed, float aspirate_volume);
};

#endif // DP8_XY_H
