#include "dj1debugwidget.h"
#include "ui_dj1debugwidget.h"
#include "Controller/Amplification_Dj1/amplification_dj1.h"

DJ1DebugWidget::DJ1DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DJ1DebugWidget)
{
    ui->setupUi(this);
    init();
}

DJ1DebugWidget::~DJ1DebugWidget()
{
    delete ui;
}

void DJ1DebugWidget::init()
{
    initStyleSheet();
}

void DJ1DebugWidget::initStyleSheet()
{

}

//DJ1模块复位
void DJ1DebugWidget::on_pushButton_clicked()
{
    Amplification_Dj1::instance()->module_reset();
}

//X轴运动
void DJ1DebugWidget::on_pushButton_2_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedX->value();
    param.targetPosition = ui->targetPositionX->value();

    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_X, param);
}

//X轴复位
void DJ1DebugWidget::on_pushButton_3_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_X);
}

//Y轴运动
void DJ1DebugWidget::on_pushButton_4_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedY->value();
    param.targetPosition = ui->targetPositionY->value();

    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_Y, param);
}

//Y轴复位
void DJ1DebugWidget::on_pushButton_5_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_Y);
}

//夹爪Z轴运动
void DJ1DebugWidget::on_pushButton_9_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedClawZ->value();
    param.targetPosition = ui->targetPositionClawZ->value();

    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_CLAW_Z, param);
}

//夹爪Z轴复位
void DJ1DebugWidget::on_pushButton_8_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_CLAW_Z);
}

//DJ1Z轴运动
void DJ1DebugWidget::on_pushButton_10_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = ui->targetSpeedZ->value();
    param.targetPosition = ui->targetPositionZ->value();

    Amplification_Dj1::instance()->motorMove(Amplification_Dj1::MOTOR_DJ1_Z, param);
}

//DJ1Z轴复位
void DJ1DebugWidget::on_pushButton_11_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_DJ1_Z);
}

//吸液启动
void DJ1DebugWidget::on_pushButton_13_clicked()
{
    Amplification_Dj1::instance()->relative_aspirate(ui->targetSpeedAspirate->value(), ui->spirateaSpeed->currentIndex(), ui->targetPositionAspirate->value());
}

//吸液复位
void DJ1DebugWidget::on_pushButton_12_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_PIPET);
}

//喷液启动
void DJ1DebugWidget::on_pushButton_14_clicked()
{
    Amplification_Dj1::instance()->relative_aspirate(ui->targetSpeedSquirt->value(), ui->squirtSpeed->currentIndex(), -1 * ui->targetPositionSquirt->value());
}

//喷液复位
void DJ1DebugWidget::on_pushButton_15_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_PIPET);
}

//夹爪张合距离
void DJ1DebugWidget::on_pushButton_20_clicked()
{
    Amplification_Dj1::instance()->glab(ui->targetPositionGlab->value());
}

//夹爪张合距离复位
void DJ1DebugWidget::on_pushButton_21_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_CLAW);
}



