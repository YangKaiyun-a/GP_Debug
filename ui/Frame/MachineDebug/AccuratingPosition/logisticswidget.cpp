#include "logisticswidget.h"
#include "ui_logisticswidget.h"
#include "Controller/Transporter/transporterpart.h"
#include "Controller/xzclaw/xzclaw_control.h"

#include <QDebug>

LogisticsWidget::LogisticsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogisticsWidget)
{
    ui->setupUi(this);
    init();
}

LogisticsWidget::~LogisticsWidget()
{
    delete ui;
}

void LogisticsWidget::init()
{
    initStyleSheet();
    initData();
    initUI();
}

void LogisticsWidget::initStyleSheet()
{
    ui->label_31->setObjectName("labModuleAction");
    ui->label_33->setObjectName("labModuleAction");

    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");
    ui->widget_11->setObjectName("stepsWidget");
    ui->widget_12->setObjectName("stepsWidget");
}

void LogisticsWidget::initData()
{

}

void LogisticsWidget::initUI()
{

}

//模块复位
void LogisticsWidget::on_pushButton_clicked()
{
    TransporterPart::instance()->moduleReset();
}

//在位检测
void LogisticsWidget::on_pushButton_2_clicked()
{
    TransporterPart::BoardCheckParam params;
    TransporterPart::instance()->checkBoardPlace(params);

    auto setLabelText = [](QLabel* label, int status) {
        switch (status) {
        case -1:
            label->setText("检测失败");
            break;
        case 1:
            label->setText("已在位");
            break;
        case 0:
            label->setText("不在位");
            break;
        default:
            label->setText("未知状态");
            break;
        }
    };

    setLabelText(ui->label_2, params.y_status);         //第1物流
    setLabelText(ui->label_3, params.x_left_status);    //左托盘
    setLabelText(ui->label_5, params.x_right_status);   //右托盘
}

//运动至物流1工作位置2Y = 物流1Y轴转板位置
void LogisticsWidget::on_pushButton_5_clicked()
{
    //获取运动速度、物流1Z轴准备位置
    TransporterPart::TransportParams param1;
    TransporterPart::instance()->getParameter(param1);

    //Z轴先运动至与物流2对接的飞行高度，即物流1Z轴准备位置
    DevicePart::MotorMotionParams param2;
    param2.speed = param1.z_speed;
    param2.targetPosition = param1.z_prepare_position;
    TransporterPart::instance()->motorMove(TransporterPart::MOTOR_Z, param2);

    //Y轴再运动至物流1Y轴转板位置
    DevicePart::MotorMotionParams param3;
    param3.speed = param1.y_speed;
    param3.targetPosition = ui->doubleSpinBox->value();
    TransporterPart::instance()->motorMove(TransporterPart::MOTOR_Y, param3);
}

//运动至物流2工作位置1X
void LogisticsWidget::on_pushButton_6_clicked()
{
    //获取运动速度
    TransporterPart::TransportParams param1;
    TransporterPart::instance()->getParameter(param1);

    DevicePart::MotorMotionParams param2;
    param2.speed = param1.x_speed;
    param2.targetPosition = ui->doubleSpinBox_2->value();
    TransporterPart::instance()->motorMove(TransporterPart::MOTOR_X, param2);
}

//运动至物流2工作位置2X
void LogisticsWidget::on_pushButton_8_clicked()
{
    //获取运动速度
    TransporterPart::TransportParams param1;
    TransporterPart::instance()->getParameter(param1);

    DevicePart::MotorMotionParams param2;
    param2.speed = param1.x_speed;
    param2.targetPosition = ui->doubleSpinBox_3->value();
    TransporterPart::instance()->motorMove(TransporterPart::MOTOR_X, param2);
}

//运动至物流2工作位置3X
void LogisticsWidget::on_pushButton_7_clicked()
{
    //获取运动速度
    TransporterPart::TransportParams param1;
    TransporterPart::instance()->getParameter(param1);

    DevicePart::MotorMotionParams param2;
    param2.speed = param1.x_speed;
    param2.targetPosition = ui->doubleSpinBox_4->value();
    TransporterPart::instance()->motorMove(TransporterPart::MOTOR_X, param2);
}

//运动至物流2工作位置4X
void LogisticsWidget::on_pushButton_9_clicked()
{
    //获取运动速度
    TransporterPart::TransportParams param1;
    TransporterPart::instance()->getParameter(param1);

    DevicePart::MotorMotionParams param2;
    param2.speed = param1.x_speed;
    param2.targetPosition = ui->doubleSpinBox_5->value();
    TransporterPart::instance()->motorMove(TransporterPart::MOTOR_X, param2);
}

//物流1Z轴复位
void LogisticsWidget::on_pushButton_12_clicked()
{
    TransporterPart::instance()->motorReset(TransporterPart::MOTOR_Z);
}

//获取参数
void LogisticsWidget::on_pushButton_3_clicked()
{
    TransporterPart::TransportParams param;

    TransporterPart::instance()->getParameter(param);
    ui->doubleSpinBox->setValue(param.y_put_position);                          //物流1工作位置2Y = 物流1Y轴转板位置
    ui->doubleSpinBox_2->setValue(param.x_put_position);                        //物流2工作位置1X = 物流2X轴转板位置
    ui->doubleSpinBox_3->setValue(param.left_tray_to_exit_position);            //物流2工作位置2X = 物流2左托盘在2仓门位置
    ui->doubleSpinBox_4->setValue(param.right_tray_to_exit_position);           //物流2工作位置3X = 物流2右托盘在2仓门位置
    ui->doubleSpinBox_5->setValue(param.right_tray_to_pcr_position);            //物流2工作位置4X = 物流2右托盘在pcr仓门位置
}

//保存参数
void LogisticsWidget::on_pushButton_4_clicked()
{
    TransporterPart::TransportParams param;
    param.y_put_position = ui->doubleSpinBox->value();                          //物流1工作位置2Y = 物流1Y轴转板位置
    param.x_put_position = ui->doubleSpinBox_2->value();                        //物流2工作位置1X = 物流2X轴转板位置
    param.left_tray_to_exit_position = ui->doubleSpinBox_3->value();            //物流2工作位置2X = 物流2左托盘在2仓门位置
    param.right_tray_to_exit_position = ui->doubleSpinBox_4->value();           //物流2工作位置3X = 物流2右托盘在2仓门位置
    param.right_tray_to_pcr_position = ui->doubleSpinBox_5->value();            //物流2工作位置4X = 物流2右托盘在pcr仓门位置

    TransporterPart::instance()->setParameter(param);
}

//物流1运动至第一交互位
void LogisticsWidget::on_pushButton_10_clicked()
{
    TransporterPart::instance()->process_1(1, ui->comboBox->currentIndex());
}

//物流1和物流2交互，然后运动至物流2工作位置2X
void LogisticsWidget::on_pushButton_11_clicked()
{
    TransporterPart::instance()->process_1(2, ui->comboBox->currentIndex());
}

//中转顶升模块-模块复位
void LogisticsWidget::on_pushButton_16_clicked()
{
    xzclaw_control::instance()->motorReset(xzclaw_control::JACK_comboBox);
}

//中转顶升模块-取板等待位
void LogisticsWidget::on_pushButton_24_clicked()
{
    xzclaw_control::xzclaw_Params params;
    xzclaw_control::instance()->getParameter(params);

    DevicePart::MotorMotionParams param;
    param.speed = params.Jack_Speed;
    param.targetPosition = params.Jack_wait_Position;
    xzclaw_control::instance()->motorMove(4, param);
}

