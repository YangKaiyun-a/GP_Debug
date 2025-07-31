#ifndef AIR_PRESSURE_H
#define AIR_PRESSURE_H

#include "../abstractpart.h"
#include <QThread>

class readpressure_Thread : public QThread
{
    Q_OBJECT;

private:

    bool m_Paused = false;
    bool T_stop = false;

public:

    int moduleIndex = 0;

    int M_test_Count = 0;           //预设老化次数
    int Chamber_Box = 0;            //腔室
    int M_test_Position = 0;
    int M_test_speed = 0;
    int M_test_waiting_time = 0;    //等待时间

protected:
    void run() Q_DECL_OVERRIDE;

public:
    readpressure_Thread();

    void Thread_continue(); //继续
    void Thread_dice();  //暂停
    void Thread_stop();  //停止

signals:
    //自定义信号
    void newValue(QList<float> seq, bool status);
};



class air_pressure : public QObject
{
    Q_OBJECT

public:
    air_pressure();

public:
    enum EdsIndexOffset {
        PCRMIX_START = 0X00,
        PCRMIX_STOP = 0X01,
        PIPET_START = 0X02,
        PIPET_STOP = 0X03,
        QPCR_START = 0X04,
        QPCR_STOP = 0X05,
        PARAMETER = 0x06,
        CURRENT_PRESSURE = 0x07,
        CURRENT_FAN = 0x08,
        SPECIAL_STOP = 0x09,
    };

    struct air_pressure_Params
    {
        float pcrmix_pressure_target;
        float pipet_pressure_target;
        float qpcr_pressure_target;
        float pcrmix_pressure_offset;
        float pipet_pressure_offset;
        float qpcr_pressure_offset;
        float pwm_dytu_ratio;
        float pcrmix_pressure_duty;
        float pipet_pressure_duty;
        float qpcr_pressure_duty;
    };

    struct air_get_Params
    {
        float pcrmix_chamber;
        float pipet_chamber;
        float qpcr_chamber;
        float pcrmix_chamber_1;
        float pipet_chamber_1;
        float pipet_chamber_2;
        float pipet_chamber_3;
        float pipet_chamber_4;
        float pipet_chamber_5;
        float pipet_chamber_6;
        float pipet_chamber_7;
        float qpcr_chamber_1;
        float qpcr_chamber_2;
        float qpcr_chamber_3;
        float qpcr_chamber_4;
    };

    static air_pressure *instance();

    Q_INVOKABLE void readpressure(int storeIndex);
    void stop_readpressure(void);

    //参数
    int air_setParameter(const air_pressure_Params& params);
    int air_getParameter(air_pressure_Params& params);
    int xz_setParameter(const air_pressure_Params& params);
    int xz_getParameter(air_pressure_Params& params);
    int pcrmix_chamber_start(int storeIndex);
    int pipette_chamber_start(int storeIndex);
    int qpcr_chamber_start(int storeIndex);
    int pcrmix_chamber_stop(int storeIndex);
    int pipette_chamber_stop(int storeIndex);
    int qpcr_chamber_stop(int storeIndex);
    int keep_fan_rpm(int storeIndex);

private:
    bool m_bReadTemp = false;
signals:
    void sigCurrentPressure(QList<float> pressures);
};

#endif // AIR_PRESSURE_H
