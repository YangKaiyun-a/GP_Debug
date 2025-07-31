#ifndef DP1_H
#define DP1_H
#include "../abstractpart.h"
#include "Controller/Dp1_xy/dp1_xy.h"

class Dp1 : public AbstractPart
{
    Q_OBJECT

public:
    enum
    {
        MOTOR_Dp1_Z = 2,
        MOTOR_CLAW_Z = 3,
        };

    //接口文件地址偏移
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
        MOTOR_RESET = 0x09,
        MOTOR_MOVE = 0x0A,
        DISTANCE = 0x0E,
        ADP_RESET = 0X0F,
        ADP_REMOVE_TIP = 0X10,
        ADP_ASPIRATE = 0X11,
        ADP_SQUIRT = 0X12,
        ADP_SEARCH_LIQUID = 0X13,
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
        CLAW_SPIN = 0x07,
        CLAW_2_GLAB = 0x08,
        CLAW_2_RELEASE = 0x09,
        CLAW_2_RESET = 0x0A,
        OPEN_CUP = 0x0D,
        CLOSE_CUP = 0x0E,
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

    struct testparam{
        float x_start_positon;
        float x_stop_positon;
        float y_start_positon;
        float y_stop_positon;
        float z_start_positon;
        float z_stop_positon;
        float jaw_z_start_positon;
        float jaw_z_stop_positon;
        int loop_num;
    };

    int current_loop_num;
    bool m_test;

public:
    Dp1();
    static Dp1* instance();

    quint16 select_motor_reset_index(int motor_id);
    quint16 select_motor_move_index(int motor_id);

    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    int module_reset();
    Q_INVOKABLE int getTip(float z_pos); //取头
    Q_INVOKABLE int removeTip(float z_pos); //退头
    Q_INVOKABLE int aspirate(const AspiraterParams& param); //吸液
    Q_INVOKABLE int squirt(const SquirtParams& param); //喷液
    Q_INVOKABLE int liquidMix(const LiquidMixParams& param); //吹打
    Q_INVOKABLE int pause(); //暂停
    Q_INVOKABLE int resume(); //恢复
    Q_INVOKABLE int aspirate_waste(float z_pos);
    Q_INVOKABLE void test(Dp1::testparam param, int is_pause);

    void stoptest();

    float get_distance();

    int claw_reset();
    int glab(float glabWidth);
    int release(float releaseWidth);
    int toZ(float zPosition, float openWidth);
    int toZGlab(float zPosition, float openWidth,int force);
    int toZRelease(float zPosition, float releaseWidth);
    int claw_spin(float angle, float speed);

    int claw_2_reset();
    int claw_2_glab(float glabWidth);
    int claw_2_release(float releaseWidth);

    int adp_reset();
    int adp_remove_tip();
    int adp_aspirate(float volume, float speed);
    int adp_squirt(float volume, float speed);
    int adp_sesarch_liquid();
    int open_cup(float z_position);
    int close_cup(float z_position);

signals:
    void testCount(int);
};


#endif // Dp1_H
