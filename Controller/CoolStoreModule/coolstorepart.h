#ifndef COOLSTOREPART_H
#define COOLSTOREPART_H

#include "../abstractpart.h"

class CoolStorePart : public AbstractPart
{
    Q_OBJECT

public:
    enum {
        STORE_ORIGINAL_TUBE = 0,
        STORE_NUCLEIC_ACID,
        STORE_LIBRARY
    };

    enum {
        MOTOR_LEFT = 0,
        MOTOR_RIGHT
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

    enum EdsIndexOffset {
        COOL_LEFT_START = 0X00,
        COOL_LEFT_STOP = 0X01,
        COOL_RIGHT_START = 0x02,
        COOL_RIGHT_STOP = 0x03,
        COOL_SYSBUILD_START = 0x04,
        COOL_SYSBUILD_STOP = 0x05,
        COVER_LEFT_OPEN = 0x06,
        COVER_LEFT_CLOSE = 0x07,
        COVER_RIGHT_OPEN = 0x08,
        COVER_RIGHT_CLOSE = 0x09,
        MOTOR_RESET = 0x0A,
        MOTOR_MOVE = 0x0B,
        PARAMETER = 0x0C,
        CURRENT_TEMP = 0x0D
    };

    struct testparam{
        int loop_num;
    };

    int current_loop_num;
    bool m_test;

public:
    static CoolStorePart* instance();
    void init();

    inline void stopReadTemp() { m_bReadTemp = false; }
    //业务接口
    int moduleReset();
    int startLeftCool();
    int startRightCool();
    int startSysbuildCool();
    int stopLeftCool();
    int stopRightCool();
    int stopSysbuildCool();

    int openLeftCover();
    int closeLeftCover();

    int openRightCover();
    int closeRightCover();

    int startReportTemp(int storeIndex, int status, int frequency);

    void stoptest();

    //调试接口
    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    //参数
    int setParameter(const CoolStoreParams& params);
    int getParameter(CoolStoreParams& params);

    //保存核酸暂存位-左参数
    int setLeftParameter(const CoolStoreParams& params);
    //保存核酸暂存位-右参数
    int setRightParameter(const CoolStoreParams& params);
    //体系构建位-保存参数
    int setSysbuildParameter(const CoolStoreParams& params);

    //读取温度
    Q_INVOKABLE void readTemp();

signals:
    void sigCurrentTemp(QList<float> temps);
    void testCount(int);

public slots:
    void test(CoolStorePart::testparam param, int is_pause);

private:
    explicit CoolStorePart(QObject *parent = nullptr);
    ~CoolStorePart() override {};

    bool m_bReadTemp = false;
};

#endif // COOLSTOREPART_H
