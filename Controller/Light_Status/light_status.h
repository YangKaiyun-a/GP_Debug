#ifndef LIGHT_STATUS_H
#define LIGHT_STATUS_H
#include "../abstractpart.h"

class Light_Status : public QObject
{
        Q_OBJECT

public:
        //接口文件地址偏移
        enum
        {
            UVLIGHT_1_OPEN = 0X00,
            UVLIGHT_1_CLOSE = 0X01,
            UVLIGHT_2_OPEN = 0X02,
            UVLIGHT_2_CLOSE = 0x03,
            UVLIGHT_3_OPEN = 0x04,
            UVLIGHT_3_CLOSE = 0x05,
        };
        enum
        {
            PERIPHERALS_BLINK_CONTROL = 0x01,
            LIGHT_1_OPEN = 0x02,
            LIGHT_1_CLOSE = 0x03,
        };

public:
    Light_Status();
    static Light_Status* instance();

    //紫外接口
    int uvlight_control(int node_id, int index_offset);
    int blink_control(int id);

    //照明接口
    int light_control(int index_offset);
};

#endif // LIGHT_STATUS_H
