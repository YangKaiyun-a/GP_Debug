#include "tp08modulewidget.h"
#include "ui_tp08modulewidget.h"
#include "Controller/Dp8/dp8.h"

#include <QScrollBar>
#include <QTimer>

TP08ModuleWidget::TP08ModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TP08ModuleWidget)
{
    ui->setupUi(this);
    init();
}

TP08ModuleWidget::~TP08ModuleWidget()
{
    delete ui;
}

void TP08ModuleWidget::init()
{
    ui->label->setObjectName("labModuleAction");
    ui->line->setObjectName("blackLine");

    ui->label_2->setObjectName("labModuleDataAction");
    ui->line_2->setObjectName("blueDashLine");

    ui->label_3->setObjectName("labModuleDataAction");
    ui->line_3->setObjectName("blueDashLine");

    ui->label_4->setObjectName("labModuleDataAction");
    ui->line_4->setObjectName("blueDashLine");

    ui->label_14->setObjectName("labModuleDataAction");
    ui->line_5->setObjectName("blueDashLine");

    ui->label_10->setObjectName("labModuleDataAction");
    ui->line_6->setObjectName("blueDashLine");

    ui->labK_4->setObjectName("labK");
    ui->labB_3->setObjectName("labB");

    ui->Tip_comboBox_4->addItems(QStringList() << "200" << "1000");
}

void TP08ModuleWidget::initScrollBar()
{
    ui->scrollArea->verticalScrollBar()->hide();
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if(!m_timer)
    {
        m_timer = new QTimer(this);
    }

    connect(m_timer, &QTimer::timeout, [=]() {
        ui->scrollArea->verticalScrollBar()->hide();
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_timer->stop();
    });

    ui->scrollArea->viewport()->installEventFilter(this);
}

//模块复位
void TP08ModuleWidget::on_btnModuleReset_clicked()
{
    Dp8::instance()->module_reset();
}

//8通道-热机
void TP08ModuleWidget::on_btnHotMachine_clicked()
{
    //吹打10次
    Dp8::LiquidMixParams param;
    param.mixStrategy = 0;              //吹打策略
    param.preAspirateVolume = 50;       //前吸体积
    param.firstAspirateVolume = 800;    //首吸体积
    param.firstAspirateSpeed = 200;     //首吸速度
    param.mixVolume = 500;              //吹打体积
    param.mixTimes = 10;                //吹打次数
    param.mixSpeed = 200;               //吹打速度
    param.lastSquirtVolume_1 = 0;       //末喷体积1
    param.lastSquirtSpeed_1 = 0;        //末喷速度1
    param.wait_time = 0;                //等待时间
    param.lastSquirtVolume_2 = 0;       //末喷体积2
    param.squitCompensateVolume = 0;    //喷液补偿
    param.lastSquirtSpeed_2 = 0;        //末喷速度2
    param.underLiquidWaitTime = 0;      //液面下等待时间
    param.leaveLiquidZPosition = 0;     //离开高度
    param.leaveLiquidZSpeed = 0;        //离开速度
    param.postLiquidZPosition = 0;      //后吸体积
    param.postLiquidZSpeed = 0;         //后吸速度
    param.mixZPosition_sta = 50;        //吹打开始高度
    param.mixZPosition_mid = 0;         //吹打中间高度
    param.mixZPosition_end = 0;         //吹打结束高度
    param.sectionalArea = 0;            //截面积
    param.flux = 8;                     //通量
    param.adjustment_k = 1;             //k
    param.adjustment_b = 0;             //b
    param.check_channel = 0;            //检测通道
    param.check_threshold = 0;          //检测阈值

    Dp8::instance()->liquidMix(param);
}

//8通道-Z轴运动
void TP08ModuleWidget::on_pushButton_7_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = 250;
    param.targetPosition = ui->targetPosition->value();

    Dp8::instance()->motorMove(4, param);   //4:DP8_Z轴
}

//8通道-Z复位
void TP08ModuleWidget::on_pushButton_8_clicked()
{
    Dp8::instance()->motorReset(4);         //4:DP8_Z轴
}

//8通道-取头
void TP08ModuleWidget::on_pushButton_9_clicked()
{
    Dp8::instance()->getTip(ui->getTipPosition->value());
}

//8通道-退头
void TP08ModuleWidget::on_pushButton_10_clicked()
{
    Dp8::instance()->removeTip(ui->removeTipPosition->value());
}

//8通道-吸液
void TP08ModuleWidget::on_pushButton_21_clicked()
{
    Dp8::AspiraterParams param;
    param.aspirateStrategy = 0;             //吸液策略
    param.flux = 8;                         //通量
    param.preAspirateVolume = 100;          //前吸体积
    param.aspirateVolume1 = ui->Tip_comboBox_5->currentText().toInt();              //吸液体积1
    param.aspirateSpeed1 = 200;             //吸液速度1
    param.wait_time = 0;                    //等待时间
    param.aspirateVolume2 = 0;              //吸液体积2
    param.aspirateSpeed2 = 0;               //吸液速度2
    param.squitTrimVolume = 0;              //回喷体积
    param.squitTrimSpeed = 0;               //回喷速度
    param.squirt_wait_time = 0;             //回喷后等待时间
    param.aspirateCompensateVolume = 0;     //吸液补偿
    param.underLiquidWaitTime = 0;          //液面下等待
    param.leaveLiquidZPosition = 10;        //离开高度
    param.leaveLiquidZSpeed = 2;            //离开速度
    param.postAspirateVolume = 0;           //后吸体积
    param.postAspirateSpeed = 0;            //后吸速度
    param.aspirateZPositionStart = ui->targetPosition_10->value();           //吸液开始高度
    param.aspirateZPositionMiddle = 0;      //吸液中间高度
    param.aspirateZPositionEnd = 0;         //吸液结束高度
    param.sectionalArea = 0;                //截面积
    param.through_film_z_position = 0;      //穿刺膜Z位置
    param.through_film_z_offset = 0;        //穿刺膜夹爪Z偏移
    param.through_film_symbol = 0;          //是否穿刺
    param.use_claw_symbol = 0;              //是否使用夹爪
    param.adjustment_k = ui->targetPosition_15->value();     //吸液K值
    param.adjustment_b = ui->targetPosition_11->value();     //吸液B值
    param.check_channel = 0;                //检测通道
    param.check_threshold = 0;              //检测阈值
    param.finish_z_position = 0;            //Z轴结束位置
    param.cross_expand_symbol = 0;          //启用十字画膜

    Dp8::instance()->aspirate(param);
}

//8通道-喷液
void TP08ModuleWidget::on_pushButton_20_clicked()
{
    Dp8::SquirtParams param;
    param.squirtStrategy = 0;               //喷液策略
    param.squirtVolume_1 = ui->Tip_comboBox_5->currentText().toInt() + 100;     //喷液体积1 = 吸液体积1 + 前吸100
    param.squirtSpeed_1 = 300;              //喷液速度1
    param.wait_time = 0;                    //等待时间
    param.squirtVolume_2 = 0;               //喷液体积2
    param.squitCompensateVolume = 0;        //喷液补偿
    param.squirtSpeed_2 = 0;                //喷液速度2
    param.underLiquidWaitTime = 0;          //液面下等待
    param.squirtZPositionStart = ui->targetPosition_13->value();         //喷液开始高度
    param.squirtZPositionMid = 0;           //喷液中间高度
    param.squirtZPositionEnd = 0;           //喷液结束高度
    param.leaveLiquidZPosition = 10;         //离开高度
    param.leaveLiquidZSpeed = 2;            //离开速度
    param.postAspirateVolume = 0;           //后吸体积
    param.postAspirateSpeed = 0;            //后吸速度
    param.sectionalArea = 0;                //截面积
    param.flux = 8;                         //通量
    param.adjustment_k = ui->targetPosition_15->value();                //喷液K值
    param.adjustment_b = ui->targetPosition_11->value();                //喷液B值
    param.check_channel = 0;                //检测通道
    param.check_threshold = 0;              //检测阈值

    Dp8::instance()->squirt(param);
}

//夹爪Z轴运动
void TP08ModuleWidget::on_pushButton_13_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = 80;
    param.targetPosition = ui->targetPosition_2->value();
    Dp8::instance()->motorMove(Dp8::MOTOR_CLAW_Z, param);
}


//夹爪Z轴复位
void TP08ModuleWidget::on_pushButton_14_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW_Z);
}

//夹爪张开
void TP08ModuleWidget::on_btnSava_4_clicked()
{
    Dp8::instance()->glab(ui->deviation_x_SpinBox_4->value());
}

//夹爪复位
void TP08ModuleWidget::on_btnSava_5_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW);
}


//单通道-热机
void TP08ModuleWidget::on_btnHotMachine_2_clicked()
{
    //吹打10次
    Dp8_Xy::LiquidMixParams param;
    param.mixStrategy = 0;              //吹打策略
    param.preAspirateVolume = 50;       //前吸体积
    param.firstAspirateVolume = 800;    //首吸体积
    param.firstAspirateSpeed = 200;     //首吸速度
    param.mixVolume = 500;              //吹打体积
    param.mixTimes = 10;                //吹打次数
    param.mixSpeed = 200;               //吹打速度
    param.lastSquirtVolume_1 = 0;       //末喷体积1
    param.lastSquirtSpeed_1 = 0;        //末喷速度1
    param.wait_time = 0;                //等待时间
    param.lastSquirtVolume_2 = 0;       //末喷体积2
    param.squitCompensateVolume = 0;    //喷液补偿
    param.lastSquirtSpeed_2 = 0;        //末喷速度2
    param.underLiquidWaitTime = 0;      //液面下等待时间
    param.leaveLiquidZPosition = 0;     //离开高度
    param.leaveLiquidZSpeed = 0;        //离开速度
    param.postLiquidZPosition = 0;      //后吸体积
    param.postLiquidZSpeed = 0;         //后吸速度
    param.mixZPosition_sta = 50;        //吹打开始高度
    param.mixZPosition_mid = 0;         //吹打中间高度
    param.mixZPosition_end = 0;         //吹打结束高度
    param.sectionalArea = 0;            //截面积
    param.flux = 8;                     //通量
    param.adjustment_k = 1;             //k
    param.adjustment_b = 0;             //b
    param.check_channel = 0;            //检测通道
    param.check_threshold = 0;          //检测阈值

}

//单通道-Z轴运动
void TP08ModuleWidget::on_pushButton_15_clicked()
{
    DevicePart::MotorMotionParams param;
    param.speed = 8;
    param.targetPosition = ui->targetPosition_3->value();
    Dp8_Xy::instance()->motorMove(Dp8_Xy::MOTOR_DP1_Z, param);
}

//单通道-Z复位
void TP08ModuleWidget::on_pushButton_16_clicked()
{
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_DP1_Z);
}

//单通道-取头
void TP08ModuleWidget::on_pushButton_18_clicked()
{
    Dp8_Xy::instance()->getTip(ui->getTipPosition_2->value());
}

//单通道-退头
void TP08ModuleWidget::on_pushButton_17_clicked()
{
    Dp8_Xy::instance()->removeTip(ui->removeTipPosition_2->value());
}

//单通道-吸液
void TP08ModuleWidget::on_pushButton_12_clicked()
{
    Dp8_Xy::AspiraterParams param;
    param.aspirateStrategy = 0;         //吸液策略
    param.flux = 8;                     //通量
    param.preAspirateVolume = 20;       //前吸体积
    param.aspirateVolume1 = ui->Tip_comboBox_2->currentText().toInt();    //吸液体积1
    param.aspirateSpeed1 = 100;         //吸液速度1
    param.wait_time = 0;                //等待时间
    param.aspirateVolume2 = 0;          //吸液体积2
    param.aspirateSpeed2 = 0;           //吸液速度2
    param.squitTrimVolume = 0;          //回喷体积
    param.squitTrimSpeed = 0;           //回喷速度
    param.squirt_wait_time = 0;         //回喷后等待时间
    param.aspirateCompensateVolume = 0; //吸液补偿
    param.underLiquidWaitTime = 0;      //液面下等待
    param.leaveLiquidZPosition = 10;     //离开高度
    param.leaveLiquidZSpeed = 2;        //离开速度
    param.postAspirateVolume = 0;       //后吸体积
    param.postAspirateSpeed = 0;        //后吸速度
    param.aspirateZPositionStart = ui->targetPosition_6->value();       //吸液开始高度
    param.aspirateZPositionMiddle = 0;  //吸液中间高度
    param.aspirateZPositionEnd = 0;     //吸液结束高度
    param.sectionalArea = 0;            //截面积
    param.through_film_z_position = 0;  //穿刺膜Z位置
    param.through_film_z_offset = 0;    //穿刺膜夹爪Z偏移
    param.through_film_symbol = 0;      //是否穿刺
    param.use_claw_symbol = 0;          //是否使用夹爪
    param.adjustment_k = ui->targetPosition_4->value();                 //吸液K值
    param.adjustment_b = ui->targetPosition_5->value();                 //吸液B值
    param.check_channel = 0;            //检测通道
    param.check_threshold = 0;          //检测阈值
    param.finish_z_position = 0;        //Z轴结束位置
    param.cross_expand_symbol = 0;      //启用十字画膜

    Dp8_Xy::instance()->aspirate(param);
}

//单通道-喷液
void TP08ModuleWidget::on_pushButton_19_clicked()
{
    Dp8_Xy::SquirtParams param;
    param.squirtStrategy = 0;               //喷液策略
    param.squirtVolume_1 = ui->Tip_comboBox_2->currentText().toInt() + 20;   //喷液体积1 = 吸液体积1 + 前吸
    param.squirtSpeed_1 = 100;                //喷液速度1
    param.wait_time = 0;                    //等待时间
    param.squirtVolume_2 = 0;               //喷液体积2
    param.squitCompensateVolume = 0;        //喷液补偿
    param.squirtSpeed_2 = 0;                //喷液速度2
    param.underLiquidWaitTime = 0;          //液面下等待
    param.squirtZPositionStart = ui->targetPosition_7->value();             //喷液开始高度
    param.squirtZPositionMid = 0;           //喷液中间高度
    param.squirtZPositionEnd = 0;           //喷液结束高度
    param.leaveLiquidZPosition = 10;         //离开高度
    param.leaveLiquidZSpeed = 2;            //离开速度
    param.postAspirateVolume = 0;           //后吸体积
    param.postAspirateSpeed = 0;            //后吸速度
    param.sectionalArea = 0;                //截面积
    param.flux = 8;                         //通量
    param.adjustment_k = ui->targetPosition_4->value();                     //喷液K值
    param.adjustment_b = ui->targetPosition_5->value();                     //喷液B值
    param.check_channel = 0;                //检测通道
    param.check_threshold = 0;              //检测阈值

    Dp8_Xy::instance()->squirt(param);
}

void TP08ModuleWidget::on_Tip_comboBox_4_currentTextChanged(const QString &arg1)
{
    ui->Tip_comboBox_5->clear();

    if(arg1 == "200")
    {
        ui->Tip_comboBox_5->addItems(QStringList() << "5" << "20" << "200");
    }
    else if(arg1 == "1000")
    {
        ui->Tip_comboBox_5->addItems(QStringList() << "100" << "500" << "1000");
    }
}

//x、y运动
void TP08ModuleWidget::on_pushButton_22_clicked()
{
    Dp8_Xy::RackMotionParams param;
    param.posNumber = 1;
    param.speed = 100;

    switch(ui->xy_comboBox->currentText().toInt())
    {
    case 1:
        param.positions << ui->targetPosition_9->value() << ui->targetPosition_8->value();
        break;
    case 2:
        param.positions << ui->targetPosition_14->value() << ui->targetPosition_12->value();
        break;
    case 3:
        param.positions << ui->targetPosition_17->value() << ui->targetPosition_16->value();
        break;
    case 4:
        param.positions << ui->targetPosition_19->value() << ui->targetPosition_18->value();
        break;
    }

    Dp8_Xy::instance()->move(param);
}

