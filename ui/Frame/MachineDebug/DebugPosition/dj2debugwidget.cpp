#include "dj2debugwidget.h"
#include "ui_dj2debugwidget.h"
#include "Controller/Extract_Dj1/extract_dj1.h"

DJ2DebugWidget::DJ2DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DJ2DebugWidget)
{
    ui->setupUi(this);
}

DJ2DebugWidget::~DJ2DebugWidget()
{
    delete ui;
}

void DJ2DebugWidget::init()
{

}

//DJ2模块复位
void DJ2DebugWidget::on_pushButton_clicked()
{
    Extract_DJ1::instance()->module_reset();
}

//X轴运动
void DJ2DebugWidget::on_pushButton_2_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedX->value();
    param.targetPosition = ui->targetPositionX->value();

    Extract_DJ1::instance()->motorMove(Extract_DJ1::MOTOR_X, param);
}

//X轴复位
void DJ2DebugWidget::on_pushButton_3_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_X);
}

//Y轴运动
void DJ2DebugWidget::on_pushButton_4_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedY->value();
    param.targetPosition = ui->targetPositionY->value();

    Extract_DJ1::instance()->motorMove(Extract_DJ1::MOTOR_Y, param);
}

//Y轴复位
void DJ2DebugWidget::on_pushButton_5_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_Y);
}

//Z轴运动
void DJ2DebugWidget::on_pushButton_10_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedZ->value();
    param.targetPosition = ui->targetPositionZ->value();

    Extract_DJ1::instance()->motorMove(Extract_DJ1::MOTOR_Z, param);
}

//Z轴复位
void DJ2DebugWidget::on_pushButton_11_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_Z);
}

//柱塞泵运动
void DJ2DebugWidget::on_pushButton_12_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedPippet->value();
    param.targetPosition = ui->targetPositionPippet->value();

    Extract_DJ1::instance()->motorMove(Extract_DJ1::MOTOR_BIG_PIPET, param);
}

//柱塞泵复位
void DJ2DebugWidget::on_pushButton_15_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_BIG_PIPET);
}

//吸液
void DJ2DebugWidget::on_pushButton_13_clicked()
{
    Extract_DJ1::AspiraterParams param;
    param.aspirateStrategy = ui->Absorb_Strategy_SBox->value();
    param.preAspirateVolume = ui->Absorb_Pre_Vol_SBox->value();
    param.preSquirtVolume = ui->Absorb_Pre_Squirt_Vol_SBox->value();
    param.preAspirateSpeed = ui->Pre_Aspirate_Speed_SBox->value();
    param.preAspirateAccDecNum = ui->Pre_Aspirate_Acc_SBox->value();
    param.aspirateVolume = ui->Absorb_Vol_SBox->value();
    param.aspirateCompensateVolume = ui->Absorb_Compensated_Vol_SBox->value();
    param.aspirateSpeed = ui->Absorb_Speed_SBox->value();
    param.aspirateAccDecNum = ui->Absorb_Acc_SBox->value();
    param.squitTrimVolume = ui->Absorb_Squirt_trim_Vol_SBox->value();
    param.squitTrimSpeed = ui->Absorb_Squirt_trim_Speed_SBox->value();
    param.squirtAccDecNum = ui->Absorb_Squirt_trim_Acc_SBox->value();
    param.leaveLiquidZPosition = ui->Absorb_Leave_Liquid_Z_Position_SBox->value();
    param.leaveLiquidZSpeed = ui->Absorb_Leave_Liquid_Z_Speed_SBox->value();
    param.postAspirateVolume = ui->Absorb_Post_Vol_SBox->value();
    param.postAspirateSpeed = ui->Absorb_Post_Speed_SBox->value();
    param.postAspirateAccDecNum = ui->Absorb_Post_Acc_SBox->value();
    param.aspirateZPreparePosition = ui->Absorb_Liquid_Z_Prepare_Position_SBox->value();
    param.aspirateZPrepareSpeed = ui->Absorb_Liquid_Z_Fast_Speed_SBox->value();
    param.aspirateZPositionStart = ui->Absorb_Liquid_Z_Start_Position_SBox->value();
    param.aspirateZPositionMiddle = ui->Absorb_Liquid_Z_Middle_Position_SBox->value();
    param.aspirateZPositionEnd = ui->Absorb_Liquid_Z_End_Position_SBox->value();
    param.aspirateZSpeed = ui->Absorb_Liquid_Slow_Speed_SBox->value();
    param.sectionalArea = ui->Absorb_Liquid_SectionalArea_SBox->value();
    param.adjustment_k = ui->aspirate_k->value();
    param.adjustment_b = ui->aspirate_b->value();
    param.check_threshold = ui->Aspirate_check_threshold->value();
    param.finish_z_position = ui->Aspirate_ZFinish_Position_DSBox->value();
    param.finish_z_speed = ui->Aspirate_ZFinish_Speed_DSBox->value();

    Extract_DJ1::instance()->aspirate(param);
}

//喷液
void DJ2DebugWidget::on_pushButton_14_clicked()
{
    Extract_DJ1::SquirtParams param;
    param.squirtStrategy = ui->Spray_Strategy_SBox->value();
    param.squirtVolume = ui->Spray_Vol_SBox->value();
    param.squirtSpeed = ui->Spray_Speed_SBox->value();
    param.squitCompensateVolume = ui->Spray_Compensated_Vol_SBox->value();
    param.squirtAccDecNum = ui->Squirt_Acc_SBox->value();
    param.leaveLiquidZPosition = ui->Spray_Leave_Liquid_Z_Position_SBox->value();
    param.leaveLiquidZSpeed = ui->Spray_Leave_Liquid_Z_Speed_SBox->value();

    param.postAspirateVolume = ui->Spray_Post_Absorb_Vol_SBox->value();
    param.postAspirateSpeed = ui->Spray_Post_Absorb_Speed_SBox->value();
    param.postAspirateAccDecNum = ui->Spray_Post_Absorb_Acc_SBox->value();

    param.squirtZPreparePosition = ui->Spray_Liquid_Z_Prepare_Position_SBox->value();
    param.squirtZPrepareSpeed = ui->Spray_Liquid_Z_Fast_Speed_SBox->value();


    param.squirtZPositionStart = ui->Spray_Liquid_Z_Pos_Start->value();
    param.squirtZPositionMid = ui->Spray_Liquid_Z_Pos_Mid->value();
    param.squirtZPositionEnd = ui->Spray_Liquid_Z_Pos_End->value();
    param.squirtZSpeed = ui->Spray_Liquid_Slow_Speed_SBox->value();
    param.sectionalArea = ui->Spray_SectionalArea_SBox->value();

    param.adjustment_k = ui->squirt_k->value();
    param.adjustment_b = ui->squirt_b->value();
    param.check_threshold = ui->Spray_Check_Threshold->value();

    param.finish_z_position = ui->Spray_ZFinish_Position_DSBox->value();
    param.finish_z_speed = ui->Spray_ZFinish_Speed_DSBox->value();

    Extract_DJ1::instance()->squirt(param);
}



