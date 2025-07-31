#ifndef SWITCH_SIGNAL_H
#define SWITCH_SIGNAL_H
#include "../abstractpart.h"

class Switch_Signal : public AbstractPart
{
    Q_OBJECT

public:
    enum EdsIndexOffset {
        COVER_OPEN = 0x00,
        COVER_CLOSE = 0x01,
        MOTOR_RESET = 0x02,
        MOTOR_MOVE = 0x03,
        WASTE_PUMP_OPEN = 0x04,
        WASTE_PUMP_CLOSE = 0x05,
        TRAY_LIGHT_OPEN = 0x07,
        TRAY_LIGHT_CLOSE = 0x08,
        DOOR_STATUS = 0x09,
        PARAMETER = 0x0C,
    };

    enum DoorIndex {
        Left_Door,              //左侧门
        Middle_Carrier_Door,    //中间载架门
        Light_Door,             //荧光检测正大门

        Left_Carrier_Door,      //左侧载架门
        Right_Carrier_Door,     //右侧载架门
        Scraps_Door,            //废板取出门

        Middle_Door             //正大门
    };

    struct Switch_SignalParams
    {
        float cover_openPosition;
        float cover_closePosition;
        float cover_speed;
    };

public:
    Switch_Signal();
    static Switch_Signal* instance();

    int moduleReset();
    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    //打开废料门
    int openCover();
    //关闭废料门
    int closeCover();

    int waste_pump_open();
    int waste_pump_close();

    int tray_light_open();
    int tray_light_close();

    //参数
    int getParameter(Switch_SignalParams& params);
    int setParameter(const Switch_SignalParams& params);

    //查询门状态
    int checkDoorStatus(int doorIndex);

};

#endif // SWITCH_SIGNAL_H
