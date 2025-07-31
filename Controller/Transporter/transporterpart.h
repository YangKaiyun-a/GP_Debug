#ifndef TRANSPORTERPART_H
#define TRANSPORTERPART_H

#include "../abstractpart.h"

class TransporterPart : public AbstractPart
{
    Q_OBJECT

public:

    //电机索引
    enum {
        MOTOR_X,
        MOTOR_Y,
        MOTOR_Z
    };
    enum
    {
        MOTOR_RESET = 0x01,
        MOTOR_MOVE = 0x02,
        TRANSPORTER_PROCESS =0X03,
        PARAMETER = 0X04,

    };

    //位置索引
    enum {
        ZERO = 0,
        CHAMBER_1,
        CHAMBER_2,
        CHAMBER_3,
    };

    struct TransportParams {
        float y_to_exit_position = 0.0;
        float y_put_position = 0.0;
        float z_exit_position = 0.0;

        float z_prepare_position = 0.0;
        float z_put_position = 0.0;
        float x_put_position = 0.0;

        float left_tray_to_exit_position = 0.0;
        float right_tray_to_exit_position = 0.0;
        float right_tray_to_pcr_position = 0.0;

        int x_speed = 0;
        int z_speed = 0;
        int y_speed = 0;

        int x_reset_speed = 0;
        int z_reset_speed = 0;
        int y_reset_speed = 0;
    };

    struct testparam{
        float x_start_positon;
        float x_stop_positon;
        float y_start_positon;
        float y_stop_positon;
        float z_start_positon;
        float z_stop_positon;
        int loop_num;
    };

    struct BoardCheckParam{
        int y_status = -1;
        int x_left_status = -1;
        int x_right_status = -1;
    };

    int current_loop_num;
    bool m_test;

public:
    static TransporterPart* instance();
    void init();

    //业务
    int process_1(int positionId, int isPlaceItem);
    int process_2(int positionId);
    int moduleReset();

    //调试
    int motorReset(int motorIndex);
    int motorMove(int motorIndex, const DevicePart::MotorMotionParams&param);

    //参数
    int getParameter(TransportParams& params);
    int setParameter(const TransportParams& params);

    //老化流程
    Q_INVOKABLE void agingStop(bool stop);
    Q_INVOKABLE void test(TransporterPart::testparam param, int is_pause);

    //在位检测
    int checkBoardPlace(BoardCheckParam &params);

    void stoptest();

signals:
    void sigAgingCount(int count);
    void testCount(int);

private:
    explicit TransporterPart(QObject *parent = nullptr);
    ~TransporterPart() {};

    bool m_aging;
};

#endif // TRANSPORTERPART_H
