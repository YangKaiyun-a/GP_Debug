#ifndef AMPLIFICATION_COOL_H
#define AMPLIFICATION_COOL_H

#include "../abstractpart.h"

class Amplification_Cool : public QObject
{
    Q_OBJECT

public:

    enum EdsIndexOffset {
        COOL_LEFT_START = 0X00,
        COOL_LEFT_STOP = 0X01,
        TRAY_LIGHT_OPEN = 0x07,
        TRAY_LIGHT_CLOSE = 0x08,
        PARAMETER = 0x0C,
        CURRENT_TEMP = 0x0D
    };

    struct CoolStoreParams
    {
        float left_k;
        float left_b;
        float right_k;
        float right_b;
        float sysbuild_k;
        float sysbuild_b;
        float left_cover_openPosition;
        float left_cover_closePosition;
        float right_cover_openPosition;
        float right_cover_closePosition;
        float left_cover_speed;
        float right_cover_speed;
        float left_target_temperature;
        float right_target_temperature;
        float sysbuilding_target_temperature;
    };

    bool m_bReadTemp = false;

public:
    Amplification_Cool();
    static Amplification_Cool* instance();

    inline void stopReadTemp() { m_bReadTemp = false; }

    int moduleReset();
    int startCool();
    int stopCool();

    int tray_light_open();
    int tray_light_close();

    //参数
    int setParameter(const CoolStoreParams& params);
    int getParameter(CoolStoreParams& params);

    Q_INVOKABLE void readTemp();

signals:
    void sigCurrentTemp(QList<float> temps);




};

#endif // AMPLIFICATION_COOL_H
