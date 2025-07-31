#include "dp8debugwidget.h"
#include "ui_dp8debugwidget.h"
#include "Controller/Dp8/dp8.h"
#include "Controller/Dp8_xy/dp8_xy.h"

DP8DebugWidget::DP8DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP8DebugWidget)
{
    ui->setupUi(this);
}

DP8DebugWidget::~DP8DebugWidget()
{
    delete ui;
}

//模块复位
void DP8DebugWidget::on_pushButton_clicked()
{
    Dp8::instance()->module_reset();
    Dp8_Xy::instance()->module_reset();
}

//X轴运动
void DP8DebugWidget::on_pushButton_2_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedX->value();
    param.targetPosition = ui->targetPositionX->value();

    Dp8_Xy::instance()->motorMove(Dp8_Xy::MOTOR_X, param);
}

//X轴复位
void DP8DebugWidget::on_pushButton_3_clicked()
{
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_X);
}

//Y轴运动
void DP8DebugWidget::on_pushButton_4_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedY->value();
    param.targetPosition = ui->targetPositionY->value();

    Dp8_Xy::instance()->motorMove(Dp8_Xy::MOTOR_Y, param);
}

//Y轴复位
void DP8DebugWidget::on_pushButton_5_clicked()
{
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_Y);
}

//Z轴运动
void DP8DebugWidget::on_pushButton_10_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedZ->value();
    param.targetPosition = ui->targetPositionZ->value();

    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_Z, param);
}

//Z轴复位
void DP8DebugWidget::on_pushButton_11_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);
}

//退头轴运动
void DP8DebugWidget::on_pushButton_14_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedRemoveTip->value();
    param.targetPosition = ui->targetPositionRemoveTip->value();

    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_TIP, param);
}

//退头轴复位
void DP8DebugWidget::on_pushButton_15_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_TIP);
}

//活塞轴运动
void DP8DebugWidget::on_pushButton_21_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedPippet->value();
    param.targetPosition = ui->targetPositionPippet->value();

    Dp8::instance()->motorMove(Dp8::MOTOR_DP8_PIPET, param);
}

//活塞轴复位
void DP8DebugWidget::on_pushButton_22_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_PIPET);
}

//夹爪运动
void DP8DebugWidget::on_pushButton_16_clicked()
{
    Dp8::instance()->glab(ui->targetPositionClaw->value());
}

//夹爪复位
void DP8DebugWidget::on_pushButton_17_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW);
}

//夹爪Z轴运动
void DP8DebugWidget::on_pushButton_18_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedClawZ->value();
    param.targetPosition = ui->targetPositionClawZ->value();

    Dp8::instance()->motorMove(Dp8::MOTOR_CLAW_Z, param);
}

//夹爪Z轴复位
void DP8DebugWidget::on_pushButton_19_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW_Z);
}

//吸液
void DP8DebugWidget::on_pushButton_20_clicked()
{
    Dp8::AspiraterParams param;
    param.aspirateStrategy = ui->Absorb_Strategy_SBox->value();
    param.flux = ui->Absorb_Flux_SBox->value();
    param.preAspirateVolume = ui->Absorb_Before_Vol_SBox->value();
    param.aspirateVolume1 = ui->Absorb_Vol_1_SBox->value();
    param.aspirateSpeed1 = ui->Absorb_Speed_1_SBox->value();
    param.wait_time = ui->AspirateWaitDoubleSpinBox->value();
    param.aspirateVolume2 = ui->Absorb_Vol_2_SBox->value();
    param.aspirateSpeed2 = ui->Absorb_Speed_2_SBox->value();
    param.squitTrimVolume = ui->Spray_Back_Vol_SBox->value();
    param.squitTrimSpeed = ui->Spray_Back_Speed_SBox->value();
    param.squirt_wait_time = ui->Spray_Back_wait_DSpinBox->value();
    param.aspirateCompensateVolume = ui->Absorb_Compensated_Vol_SBox->value();
    param.underLiquidWaitTime = ui->Absorb_Wait_Liquid_Below_Time_SBox->value();
    param.leaveLiquidZPosition = ui->Absorb_Leave_Liquid_Z_Position_SBox->value();
    param.leaveLiquidZSpeed = ui->Absorb_Leave_Liquid_Z_Speed_SBox->value();
    param.postAspirateVolume = ui->Absorb_After_Vol_SBox->value();
    param.postAspirateSpeed = ui->Absorb_After_Speed_SBox->value();
    param.aspirateZPositionStart = ui->Absorb_Liquid_Z_Start_Position_SBox->value();
    param.aspirateZPositionMiddle = ui->Absorb_Liquid_Z_Middle_Position_SBox->value();
    param.aspirateZPositionEnd = ui->Absorb_Liquid_Z_End_Position_SBox->value();
    param.sectionalArea = ui->Absorb_Liquid_SectionalArea_SBox->value();
    param.through_film_z_position = ui->through_film_z_position->value();
    param.through_film_z_offset = ui->through_film_z_offset->value();
    param.through_film_symbol = ui->through_film->value();
    param.use_claw_symbol = ui->use_claw->value();
    param.adjustment_k = ui->aspirate_k->value();
    param.adjustment_b = ui->aspirate_b->value();
    param.check_channel = ui->check_channel->value();
    param.check_threshold = ui->check_threshold->value();
    param.finish_z_position = ui->ZFinishDSpinBox->value();
    param.cross_expand_symbol = ui->CrossExpandspinBox->value();

    Dp8::instance()->aspirate(param);
}

//喷液
void DP8DebugWidget::on_pushButton_23_clicked()
{
    Dp8::SquirtParams param;
    param.squirtStrategy = ui->Spray_Strategy_SBox->value();
    param.squirtVolume_1 = ui->Spray1_Vol_SBox->value();
    param.squirtSpeed_1 = ui->Spray1_Speed_SBox->value();
    param.wait_time = ui->Spray_wait_DSpinBox->value();
    param.squirtVolume_2 = ui->Spray2_Vol_SBox->value();
    param.squitCompensateVolume = ui->Spray_Compensated_Vol_SBox->value();
    param.squirtSpeed_2 = ui->Spray2_Speed_SBox->value();

    param.underLiquidWaitTime = ui->Spray_Wait_Liquid_Below_Time_SBox->value();
    param.squirtZPositionStart = ui->Spray_Liquid_Z_Pos_Start->value();
    param.squirtZPositionMid = ui->Spray_Liquid_Z_Pos_Mid->value();
    param.squirtZPositionEnd = ui->Spray_Liquid_Z_Pos_End->value();
    param.leaveLiquidZPosition = ui->Spray_Leave_Liquid_Z_Position_SBox->value();
    param.leaveLiquidZSpeed = ui->Spray_Leave_Liquid_Z_Speed_SBox->value();
    param.postAspirateVolume = ui->Spray_After_Vol_SBox->value();
    param.postAspirateSpeed = ui->Spray_After_Speed_SBox->value();
    param.sectionalArea = ui->Spray_SectionalArea_SBox->value();
    param.flux = ui->Spray_Flux_SBox->value();
    param.adjustment_k = ui->squirt_k->value();
    param.adjustment_b = ui->squirt_b->value();
    param.check_channel = ui->Spray_Check_Channel->value();
    param.check_threshold = ui->Spray_Check_Threshold->value();

    Dp8::instance()->squirt(param);
}
