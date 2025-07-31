#ifndef HEAT_SHAKE_H
#define HEAT_SHAKE_H
#include "../abstractpart.h"

class Heat_Shake : public AbstractPart
{
    Q_OBJECT

public:
    enum EdsIndexOffset {
        MOTOR_RESET = 0x01,
        MOTOR_MOVE = 0x02,
        HEAT_START = 0x03,
        HEAT_STOP = 0x04,
        SHAKE_START = 0x05,
        SHAKE_STOP = 0x06,
        MAGNET_OUT = 0x07,
        MAGNET_IN = 0x08,
        MODULE_RESET = 0x09,
        PARAMETER = 0x0A,
        TEMP_CURRENT = 0x0B

    };

    struct HeatShakeParams
    {
        float magnet_out_position;
        float magnet_in_position;
        float magnet_speed;
        float magnet_in_position_1;
        float magnet_in_position_2;
        float magnet_in_position_3;
        float magnet_in_position_4;
        float wait_time_1;
        float wait_time_2;
        float wait_time_3;
        float wait_time_4;
        float heat_k;
        float heat_b;

        HeatShakeParams() {
            magnet_out_position = 0;
            magnet_in_position = 0;
            magnet_speed = 0;
        }
    };

    struct testparam{
        float start_position;
        int start_sleep_time;
        float next_position;
        int next_sleep_time;
        float third_position;
        int third_sleep_time;
        float fourth_position;
        int fourth_sleep_time;
        float stop_position;
        int stop_sleep_time;
        int loop_num;
        float cycle_speed;
    };

    int current_loop_num;
    bool m_test;
    bool m_bReadTemp = false;

public:
    Heat_Shake();
    static Heat_Shake* instance();
    static Heat_Shake* read_temp_instance();

signals:
    void sigTemp(int moduleIndex, QList<float> temps);
    void testCount(int);

public:
    int moduleReset(int moduleIndex);

    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams &param);

    int start_heat(int moduleIndex, float target_temperature);
    int stop_heat(int moduleIndex);
    int start_shake(int moduleIndex, int target_rpm);
    int stop_shake(int moduleIndex);
    int magnet_out(int moduleIndex);
    int magnet_in(int moduleIndex);
    void stop_readTemp();

    void stoptest();

    //参数
    int getParameter(int moduleIndex, HeatShakeParams& params);
    int setParameter(int moduleIndex, const HeatShakeParams& params);

    Q_INVOKABLE void readTemp(int moduleIndex);
    Q_INVOKABLE void test(int moduleIndex, Heat_Shake::testparam param, int is_pause);


};

#endif // HEAT_SHAKE_H
