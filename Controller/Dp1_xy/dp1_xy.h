#ifndef DP1_XY_H
#define DP1_XY_H
#include <QVector>
#include "../abstractpart.h"

class Dp1_Xy : public AbstractPart
{
public:
    enum
    {
        MOTOR_X = 0,
        MOTOR_Y = 1,
    };
    enum
    {
        MOTOR_RESET = 9,
        MOTOR_MOVE = 10,
        XY_MOVE = 0,
        XY_MOTOR_RESET = 1,
        XY_MOTOR_MOVE = 2,
        MODUL_ERESET = 3,
    };


    //支架运动参数
    struct RackMotionParams {
        int posNumber;
        float speed;
        QVector<float> positions; //下标偶序列为X位置，奇序列为Y位置
    };

public:
    Dp1_Xy();
    static Dp1_Xy* instance();

    quint16 select_motor_reset_index(int motor_id);
    quint16 select_motor_move_index(int motor_id);

    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    int module_reset();
    Q_INVOKABLE int move(const RackMotionParams& param);
};

#endif // Dp1_XY_H
