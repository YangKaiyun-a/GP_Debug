#include "dp1debugwidget.h"
#include "ui_dp1debugwidget.h"
#include "Controller/Dp1/dp1.h"

#include <QDebug>

DP1DebugWidget::DP1DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP1DebugWidget)
{
    ui->setupUi(this);
}

DP1DebugWidget::~DP1DebugWidget()
{
    delete ui;
}

//DP1模块复位
void DP1DebugWidget::on_pushButton_clicked()
{
    Dp1::instance()->module_reset();
    Dp1_Xy::instance()->module_reset();
}

//X轴运动
void DP1DebugWidget::on_pushButton_2_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedX->value();
    param.targetPosition = ui->targetPositionX->value();

    Dp1_Xy::instance()->motorMove(Dp1_Xy::MOTOR_X, param);
}

//X轴复位
void DP1DebugWidget::on_pushButton_3_clicked()
{
    Dp1_Xy::instance()->motorReset(Dp1_Xy::MOTOR_X);
}

//Y轴运动
void DP1DebugWidget::on_pushButton_4_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedY->value();
    param.targetPosition = ui->targetPositionY->value();

    Dp1_Xy::instance()->motorMove(Dp1_Xy::MOTOR_Y, param);
}

//Y轴复位
void DP1DebugWidget::on_pushButton_5_clicked()
{
    Dp1_Xy::instance()->motorReset(Dp1_Xy::MOTOR_Y);
}

//ADP_Z轴运动
void DP1DebugWidget::on_pushButton_10_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedADPZ->value();
    param.targetPosition = ui->targetPositionADPZ->value();

    Dp1::instance()->motorMove(Dp1::MOTOR_Dp1_Z, param);
}

//ADPZ轴电机复位
void DP1DebugWidget::on_pushButton_11_clicked()
{
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);
}

//ADP吸液
void DP1DebugWidget::on_pushButton_20_clicked()
{
    Dp1::instance()->adp_aspirate(ui->targetPositionAspirate->value(), ui->targetSpeedAspirate->value());
}

//ADP吸液复位
void DP1DebugWidget::on_pushButton_21_clicked()
{
    Dp1::instance()->adp_reset();
}

//ADP喷液
void DP1DebugWidget::on_pushButton_19_clicked()
{
    Dp1::instance()->adp_squirt(ui->targetPositionSquirt->value(), ui->targetSpeedSquirt->value());
}

//ADP喷液复位
void DP1DebugWidget::on_pushButton_22_clicked()
{
    Dp1::instance()->adp_reset();
}

//夹爪Z轴运动
void DP1DebugWidget::on_pushButton_13_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedClawZ->value();
    param.targetPosition = ui->targetPositionClawZ->value();

    Dp1::instance()->motorMove(Dp1::MOTOR_CLAW_Z, param);
}

//夹爪Z轴复位
void DP1DebugWidget::on_pushButton_12_clicked()
{
    Dp1::instance()->motorReset(Dp1::MOTOR_CLAW_Z);
}

//旋转夹爪旋转运动
void DP1DebugWidget::on_pushButton_16_clicked()
{
    Dp1::instance()->claw_spin(ui->spinAngle->value(), ui->spinSpeed->value());
}

//旋转夹爪张合
void DP1DebugWidget::on_pushButton_14_clicked()
{
    Dp1::instance()->release(ui->targetPositionClaw->value());
}

//旋转夹爪复位
void DP1DebugWidget::on_pushButton_15_clicked()
{
    Dp1::instance()->claw_reset();
}

//底部夹爪运动
void DP1DebugWidget::on_pushButton_17_clicked()
{
    Dp1::instance()->claw_2_release(ui->targetPositionClaw_2->value());
}

//底部夹爪复位
void DP1DebugWidget::on_pushButton_18_clicked()
{
    Dp1::instance()->claw_2_reset();
}
