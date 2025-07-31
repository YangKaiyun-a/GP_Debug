#ifndef XZCLAW_CONTROL_H
#define XZCLAW_CONTROL_H


#include "../abstractpart.h"


class xzclaw_control : public AbstractPart
{
    Q_OBJECT

public:
    enum
    {
        X_comboBox = 0x00,
        Z_comboBox,
        M1_comboBox,
        M2_comboBox,
        JACK_comboBox,
    };

    enum
    {
        CLAW_RESET = 0x05,

        X_RESET = 0x01,
        X_MOVE = 0x02,

        CLAW_Z_RESET = 0x05,
        CLAW_Z_MOVE = 0x06,

        M1_RESET = 0x01,
        M1_MOVE = 0x02,

        M2_RESET = 0x03,
        M2_MOVE = 0x04,

        JACK_RESET = 0x05,
        JACK_MOVE = 0x06,

        CLAW_PARAM = 0x0E,
        DOOR_OPEN = 0x07,
        DOOR_CLOSE = 0x08,
        JACK_TRANSPORT = 0x0B,
        JACK_WAIT = 0x0C,

        UVLIGHT_1_OPEN = 0x00,
        UVLIGHT_1_CLOSE = 0x01,

        CLAW_TO_Z_GLAB = 0x03,
        CLAW_TO_Z_RELEASE = 0x04,

        CLAW_MOTOR_RESET = 0x05,
        CLAW_MOTOR_MOVE = 0x06,
        CLAW_SPIN = 0x07,
        CLAW_GLAB = 0x00,
        CLAW_RELEASE = 0x01,

        OPEN_CUP = 0x0D,
        CLOSE_CUP = 0x0E,
        };

    struct xzclaw_Params {
        float z_fly_position = 0.0;
        float z_down_position = 0.0;
        float z_speed = 0.0;
        float z_zero_offset = 0.0;
        float M1_Close_Position = 0.0;
        float M1_Close_Speed = 0.0;
        float M2_Close_Position = 0.0;
        float M2_Close_Speed = 0.0;
        float Jack_Transporter_Position = 0.0;
        float Jack_wait_Position = 0.0;
        float Jack_Speed = 0.0;
        float z_door_position = 0.0;
    };

public:
    xzclaw_control();

    static xzclaw_control *instance();

    quint16 select_motor_reset_index(int motor_id);
    quint16 select_motor_move_index(int motor_id);
    int claw_reset();
    int claw_glab(float glabWidth);
    int claw_release(float releaseWidth);
    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams &param);
    int getParameter(xzclaw_control::xzclaw_Params &params);
    int setParameter(const xzclaw_control::xzclaw_Params &params);
    int module_reset();
    int M1_Open_entry();
    int M1_Close_entry();
    int M2_Open_entry();
    int M2_Close_entry();
    int JACK_Transporter_Position_entry();
    int JACK_Wait_Position_entry();
    int uvlight_1_open_entry();
    int uvlight_1_close_entry();
    int toZGlab(float zPosition, float openWidth, float z_fly_Position);
    int toZRelease(float zPosition, float releaseWidth, float z_fly_Position);
};

#endif // XZCLAW_CONTROL_H
