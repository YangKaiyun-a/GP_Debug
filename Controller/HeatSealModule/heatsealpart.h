#ifndef HEATSEALPART_H
#define HEATSEALPART_H
#include <QThread>
#include "../abstractpart.h"

class HeatSealPart : public AbstractPart
{
    Q_OBJECT

public:
    enum{ //电机索引
        MOTOR_Z = 0,
        MOTOR_TRAY
    };

    int test_count = 0;
    int time1 = 900;
    int time2 = 1800;

    struct HeatSealParams
    {
        float targetTemp;
        float sealTime; //单位: S
        float zDownPosition;
        float trayOutPosition;
        float traySealPosition;
        int zDownSpeed;
        int trayOutSpeed;
        int zResetSpeed;
        int trayResetSpeed;
        float tempCompensationOut;
        float tempCompensationMid;
        float tempCompensationIn;

        HeatSealParams() {
            targetTemp = 175.0;
            sealTime = 1.5; //单位: S
            zDownPosition = 7.5;
            trayOutPosition = 120.0;
            traySealPosition = 0.0;
            zDownSpeed = 6;
            trayOutSpeed = 100;
            zResetSpeed = 3;
            trayResetSpeed = 20;
            tempCompensationOut = 0.0;
            tempCompensationMid = 0.0;
            tempCompensationIn = 0.0;
        }
    };

    enum EdsIndexOffset {
        MOTOR_RESET = 0x01,
        MOTOR_MOVE = 0x02,
        HEAT_START = 0x03,
        HEAT_STOP = 0x04,
        SEALING = 0x05,
        TRAY_OUT = 0x06,
        TRAY_IN = 0x07,
        MODULE_RESET = 0x08,
        PARAMETER = 0x09,
        TEMP_UDLOAD = 0x0A,
        TEMP_CURRENT = 0x0B,
        BOARD_STATUS = 0x0D
    };

public:
    static HeatSealPart* instance();
    void init();

signals:
    void sigCurrentTemp(QList<float> temps);
    void sigTestCurrentCount(int count);

public:
    //业务接口
    int moduleReset();
    int startHeat();
    int stopHeat();
    int sealing();
    int trayPullOut();
    int trayPushIn();
    int tempUpload(int status,  int frequency);
    Q_INVOKABLE void test(int count, bool status);
    int stop_test();

    //参数
    int getParameter(HeatSealParams& params);
    int setParameter(const HeatSealParams& params);

    //调试
    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    //在位检测
    int checkBoardStatus();

    Q_INVOKABLE void readTemp();


private:
    explicit HeatSealPart(QObject *parent = nullptr);
    ~HeatSealPart() {};
    bool m_bReadTemp = false;
    bool m_test = false;
};

#endif // HEATSEALPART_H
