#ifndef HEATSEALREADTEMP_H
#define HEATSEALREADTEMP_H

#include "../abstractpart.h"

class HeatsealReadtemp : public AbstractPart
{
    Q_OBJECT

public:
    bool m_bReadTemp;
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
        TEMP_CURRENT = 0x0B
    };

public:
    HeatsealReadtemp();

    static HeatsealReadtemp* instance();

    Q_INVOKABLE void readTemp();

    int motorMove(int motorIndex, const DevicePart::MotorMotionParams &param) override;
    int motorReset(int motorIndex) override;

signals:
    void sigCurrentTemp(QList<float> temps);
};

#endif // HEATSEALREADTEMP_H
