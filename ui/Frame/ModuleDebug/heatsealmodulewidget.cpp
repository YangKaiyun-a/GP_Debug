#include "heatsealmodulewidget.h"
#include "ui_heatsealmodulewidget.h"
#include "Controller/HeatSealModule/heatsealpart.h"
#include "hcscitoolkits/dialog/messagedialog.h"

#include <QDebug>

HeatSealModuleWidget::HeatSealModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeatSealModuleWidget)
{
    ui->setupUi(this);
    init();
}

HeatSealModuleWidget::~HeatSealModuleWidget()
{
    delete ui;
}

void HeatSealModuleWidget::init()
{
    ui->label->setObjectName("labModuleAction");
    ui->line->setObjectName("blackLine");

    initConnect();

    qRegisterMetaType<QList<float> > ("QList<float>");
}

void HeatSealModuleWidget::initConnect()
{
    connect(ui->swControlDuct, &SwitchButton::statusChanged, this, &HeatSealModuleWidget::onStatusChanged);
}

//开始加热/停止加热
void HeatSealModuleWidget::onStatusChanged(bool startHeat)
{
    //先判断是否为空
    if(ui->editTargetTemp->text().isEmpty())
    {
        MessageDialog msg("输入温度不能为空！");
        msg.exec();
        ui->swControlDuct->setChecked(false);
        return;
    }

    if(startHeat)
    {
        //开始加热
        HeatSealPart::instance()->startHeat();
    }
    else
    {
        //停止加热
        HeatSealPart::instance()->stopHeat();
    }
}

//模块复位
void HeatSealModuleWidget::on_btnModuleReset_clicked()
{
    HeatSealPart::instance()->moduleReset();
}

//托盘推出
void HeatSealModuleWidget::on_btnLiquidDetect_clicked()
{
    HeatSealPart::instance()->trayPullOut();
}

//托盘电机运动
void HeatSealModuleWidget::on_pushButton_7_clicked()
{
    //先获取托盘推出速度
    HeatSealPart::HeatSealParams param1;
    HeatSealPart::instance()->getParameter(param1);
    int speed = param1.trayOutSpeed;

    //托盘电机运动
    DevicePart::MotorMotionParams param2;
    param2.speed = speed;
    param2.targetPosition = ui->doubleSpinBox->value();

    HeatSealPart::instance()->motorMove(1, param2);          //0：Z电机  1：托盘电机
}

//Y轴复位
void HeatSealModuleWidget::on_pushButton_8_clicked()
{
    HeatSealPart::instance()->motorReset(1);                //0：Z电机  1：托盘电机
}

//Z轴电机运动
void HeatSealModuleWidget::on_pushButton_15_clicked()
{
    //先获取Z轴下压位置、Z轴下压速度
    HeatSealPart::HeatSealParams param1;
    HeatSealPart::instance()->getParameter(param1);
    int speed = param1.zDownSpeed;

    //Z电机运动
    DevicePart::MotorMotionParams param;
    param.speed = speed;
    param.targetPosition = ui->doubleSpinBox_2->value();

    HeatSealPart::instance()->motorMove(0, param);      //0：Z电机  1：托盘电机
}

//Z轴复位
void HeatSealModuleWidget::on_pushButton_16_clicked()
{
    HeatSealPart::instance()->motorReset(0);            //0：Z电机  1：托盘电机
}

//托盘推入
void HeatSealModuleWidget::on_btnLiquidDetect_2_clicked()
{
    HeatSealPart::instance()->trayPushIn();
}

//热封封膜
void HeatSealModuleWidget::on_pushButton_21_clicked()
{
    HeatSealPart::instance()->sealing();
}

//获取参数
void HeatSealModuleWidget::on_btnLiquidDetect_3_clicked()
{
    HeatSealPart::HeatSealParams param;
    HeatSealPart::instance()->getParameter(param);
    ui->editTargetTemp->setValue(param.targetTemp);             //目标温度
    ui->doubleSpinBox_2->setValue(param.zDownPosition);         //Z轴下压位
    ui->doubleSpinBox->setValue(param.trayOutPosition);         //托盘推出位置
    ui->doubleSpinBox_3->setValue(param.tempCompensationIn);    //温度补偿IN
    ui->doubleSpinBox_4->setValue(param.tempCompensationOut);   //温度补偿OUT
}

//保存参数
void HeatSealModuleWidget::on_btnLiquidDetect_4_clicked()
{
    HeatSealPart::HeatSealParams param;
    param.targetTemp = ui->editTargetTemp->value();             //目标温度
    param.zDownPosition = ui->doubleSpinBox_2->value();         //Z轴下压位
    param.trayOutPosition = ui->doubleSpinBox->value();         //托盘推出位置
    param.tempCompensationIn =  ui->doubleSpinBox_3->value();   //温度补偿IN
    param.tempCompensationOut = ui->doubleSpinBox_4->value();   //温度补偿OUT

    HeatSealPart::instance()->setParameter(param);
}

//读取当前温度
void HeatSealModuleWidget::on_btnLiquidDetect_5_clicked()
{
    QMetaObject::invokeMethod(HeatSealPart::instance(), "readTemp", Qt::QueuedConnection);

    connect(HeatSealPart::instance(), &HeatSealPart::sigCurrentTemp, this, [=](QList<float> temps){
        ui->doubleSpinBox_5->setValue(temps.at(0));     //out
    });
}

//在位检测
void HeatSealModuleWidget::on_btnLiquidDetect_6_clicked()
{
    int status = HeatSealPart::instance()->checkBoardStatus();

    if(status == 1)
    {
        ui->label_17->setText("已在位");
    }
    else if(status == 0)
    {
        ui->label_17->setText("未在位");
    }
    else
    {
        ui->label_17->setText("检测失败");
    }
}

