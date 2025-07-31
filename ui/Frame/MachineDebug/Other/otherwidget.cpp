#include "otherwidget.h"
#include "ui_otherwidget.h"
#include "Controller/xzclaw/xzclaw_control.h"
#include "Controller/Switch_Signal/switch_signal.h"
#include "Controller/Light_Status/light_status.h"
#include "Controller/air_pressure/air_pressure.h"

OtherWidget::OtherWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OtherWidget)
{
    ui->setupUi(this);
    init();
}

OtherWidget::~OtherWidget()
{
    delete ui;
}

void OtherWidget::init()
{
    if(!m_read_pressure_Thread)
    {
        m_read_pressure_Thread = new readpressure_Thread();
    }

    connect(m_read_pressure_Thread, &readpressure_Thread::newValue, this, [=](QList<float> seq, bool status) {
        if (status == true)
        {
            ui->channel1_pressure->setValue(seq.at(0));
            ui->channel2_pressure->setValue(seq.at(1));
            ui->channel3_pressure->setValue(seq.at(2));
            ui->pcrmix_fan_1_rpm->setValue(seq.at(3));
            ui->pipet_fan_1_rpm->setValue(seq.at(4));
            ui->pipet_fan_2_rpm->setValue(seq.at(5));
            ui->pipet_fan_3_rpm->setValue(seq.at(6));
            ui->qpcr_fan_1_rpm->setValue(seq.at(7));
            ui->qpcr_fan_2_rpm->setValue(seq.at(8));
        }
    });
}

//打开物流3取板门
void OtherWidget::on_pushButton_clicked()
{
    xzclaw_control::instance()->M1_Open_entry();
}

//关闭物流3取板门
void OtherWidget::on_pushButton_2_clicked()
{
    xzclaw_control::instance()->M1_Close_entry();
}

//打开物流3废板门
void OtherWidget::on_pushButton_4_clicked()
{
    xzclaw_control::instance()->M2_Open_entry();
}

//关闭物流3废板门
void OtherWidget::on_pushButton_3_clicked()
{
    xzclaw_control::instance()->M2_Close_entry();
}

//打开废料门
void OtherWidget::on_pushButton_6_clicked()
{
    Switch_Signal::instance()->openCover();
}

//关闭废料门
void OtherWidget::on_pushButton_5_clicked()
{
    Switch_Signal::instance()->closeCover();
}

//打开紫外灯-左侧->0X2C00
void OtherWidget::on_pushButton_7_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA, Light_Status::UVLIGHT_1_OPEN);
}

//关闭紫外灯-左侧->0X2C01
void OtherWidget::on_pushButton_8_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA, Light_Status::UVLIGHT_1_CLOSE);
}

//打开紫外灯-中间-1->0X2C02
void OtherWidget::on_pushButton_9_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA, Light_Status::UVLIGHT_2_OPEN);
}

//关闭紫外灯-中间-1->0X2C03
void OtherWidget::on_pushButton_10_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA, Light_Status::UVLIGHT_2_CLOSE);
}

//打开紫外灯-中间-2->0X2C04
void OtherWidget::on_pushButton_12_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA, Light_Status::UVLIGHT_3_OPEN);
}

//关闭紫外灯-中间-2->0X2C05
void OtherWidget::on_pushButton_11_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA, Light_Status::UVLIGHT_3_CLOSE);
}

//打开紫外灯-右侧->0X2C00
void OtherWidget::on_pushButton_14_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA_RIGHT, Light_Status::UVLIGHT_1_OPEN);
}

//关闭紫外灯-右侧->0X2C01
void OtherWidget::on_pushButton_15_clicked()
{
    Light_Status::instance()->uvlight_control(DevicePart::NODE_LIGHT_STATUA_RIGHT, Light_Status::UVLIGHT_1_CLOSE);
}

//查询门状态
void OtherWidget::on_pushButton_13_clicked()
{
    //左侧门-0x01
    int leftDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Left_Door);
    if(leftDoorStatus == 1)
    {
        ui->leftDoorStatus->setText("已关闭");
    }
    else if(leftDoorStatus == 0)
    {
        ui->leftDoorStatus->setText("未关闭");
    }
    else if(leftDoorStatus == -1)
    {
        ui->leftDoorStatus->setText("检测失败");
    }

    //中间载架门-0x01
    int middleCarrierDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Middle_Carrier_Door);
    if(middleCarrierDoorStatus == 1)
    {
        ui->MiddleCarrierDoorStatus->setText("已关闭");
    }
    else if(middleCarrierDoorStatus == 0)
    {
        ui->MiddleCarrierDoorStatus->setText("未关闭");
    }
    else if(middleCarrierDoorStatus == -1)
    {
        ui->MiddleCarrierDoorStatus->setText("检测失败");
    }

    //荧光检测正大门-0x01
    int lightDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Light_Door);
    if(lightDoorStatus == 1)
    {
        ui->LightDoorStatus->setText("已关闭");
    }
    else if(lightDoorStatus == 0)
    {
        ui->LightDoorStatus->setText("未关闭");
    }
    else if(lightDoorStatus == -1)
    {
        ui->LightDoorStatus->setText("检测失败");
    }

    //左侧载架门-0x02
    int leftCarrierDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Left_Carrier_Door);
    if(leftCarrierDoorStatus == 1)
    {
        ui->LeftCarrierDoorStatus->setText("已关闭");
    }
    else if(leftCarrierDoorStatus == 0)
    {
        ui->LeftCarrierDoorStatus->setText("未关闭");
    }
    else if(leftCarrierDoorStatus == -1)
    {
        ui->LeftCarrierDoorStatus->setText("检测失败");
    }

    //右侧载架门-0x02
    int rightCarrierDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Right_Carrier_Door);
    if(rightCarrierDoorStatus == 1)
    {
        ui->RightCarrierDoorStatus->setText("已关闭");
    }
    else if(rightCarrierDoorStatus == 0)
    {
        ui->RightCarrierDoorStatus->setText("未关闭");
    }
    else if(rightCarrierDoorStatus == -1)
    {
        ui->RightCarrierDoorStatus->setText("检测失败");
    }

    //废板取出门-0x02
    int scrapsDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Scraps_Door);
    if(scrapsDoorStatus == 1)
    {
        ui->ScrapsDoorStatus->setText("已关闭");
    }
    else if(scrapsDoorStatus == 0)
    {
        ui->ScrapsDoorStatus->setText("未关闭");
    }
    else if(scrapsDoorStatus == -1)
    {
        ui->ScrapsDoorStatus->setText("检测失败");
    }

    //正大门-0x03
    int middleDoorStatus = Switch_Signal::instance()->checkDoorStatus(Switch_Signal::Middle_Door);
    if(middleDoorStatus == 1)
    {
        ui->MiddleDoorStatus->setText("已关闭");
    }
    else if(middleDoorStatus == 0)
    {
        ui->MiddleDoorStatus->setText("未关闭");
    }
    else if(middleDoorStatus == -1)
    {
        ui->MiddleDoorStatus->setText("检测失败");
    }
}

//12号板读取参数
void OtherWidget::on_get_environment_param_Btn_clicked()
{
    air_pressure::air_pressure_Params param;

    air_pressure::instance()->air_getParameter(param);

    ui->pcrmix_pressure->setValue(param.pcrmix_pressure_target);
    ui->pcrmix_pressure_office->setValue(param.pcrmix_pressure_offset);
    ui->pcrmix_pressure_duty->setValue(param.pcrmix_pressure_duty);
    ui->pipet_pressure->setValue(param.pipet_pressure_target);
    ui->pipet_pressure_office->setValue(param.pipet_pressure_offset);
    ui->pipet_pressure_duty->setValue(param.pipet_pressure_duty);
    ui->air_pwm_keep->setValue(param.pwm_dytu_ratio);
}

//12号板设置参数
void OtherWidget::on_set_environment_param_Btn_clicked()
{
    air_pressure::air_pressure_Params param;

    param.pcrmix_pressure_target = ui->pcrmix_pressure->value();
    param.pcrmix_pressure_offset = ui->pcrmix_pressure_office->value();
    param.pcrmix_pressure_duty = ui->pcrmix_pressure_duty->value();
    param.pipet_pressure_target = ui->pipet_pressure->value();
    param.pipet_pressure_offset = ui->pipet_pressure_office->value();
    param.pipet_pressure_duty = ui->pipet_pressure_duty->value();
    param.pwm_dytu_ratio = ui->air_pwm_keep->value();

    air_pressure::instance()->air_setParameter(param);
}

//17号板读取参数
void OtherWidget::on_get_pressure_param_xz_Btn_clicked()
{
    air_pressure::air_pressure_Params param;

    air_pressure::instance()->xz_getParameter(param);

    ui->qpcr_pressure->setValue(param.qpcr_pressure_target);
    ui->qpcr_pressure_office->setValue(param.qpcr_pressure_offset);
    ui->qpcr_pressure_duty->setValue(param.qpcr_pressure_duty);
    ui->xz_pwm_keep->setValue(param.pwm_dytu_ratio);
}

//17号板设置参数
void OtherWidget::on_set_pressure_param_xz_Btn_clicked()
{
    air_pressure::air_pressure_Params param;

    param.qpcr_pressure_target = ui->qpcr_pressure->value();
    param.qpcr_pressure_offset = ui->qpcr_pressure_office->value();
    param.qpcr_pressure_duty = ui->qpcr_pressure_duty->value();
    param.pwm_dytu_ratio = ui->xz_pwm_keep->value();

    air_pressure::instance()->xz_setParameter(param);
}

//读取气压与风扇转速
void OtherWidget::on_pushButton_16_clicked()
{
    if(!m_read_pressure_Thread)
    {
        return;
    }

    m_read_pressure_Thread->start();
}

//读取停止
void OtherWidget::on_stop_Button_clicked()
{
    if(!m_read_pressure_Thread)
    {
        return;
    }

    m_read_pressure_Thread->Thread_stop();
    m_read_pressure_Thread->wait();
}

//pcrmix腔室气压启动
void OtherWidget::on_pcrmix_start_Btn_clicked()
{
    air_pressure::instance()->pcrmix_chamber_start(0);
}

//pcrmix腔室气压停止
void OtherWidget::on_pcrmix_stop_Btn_clicked()
{
    air_pressure::instance()->pcrmix_chamber_stop(0);
}

//移液腔室气压启动
void OtherWidget::on_pipette_start_Btn_clicked()
{
    air_pressure::instance()->pipette_chamber_start(0);
}

//移液腔室气压停止
void OtherWidget::on_pipette_stop_Btn_clicked()
{
    air_pressure::instance()->pipette_chamber_stop(0);
}

//qpcr腔室气压启动
void OtherWidget::on_qpcr_start_Btn_clicked()
{
    air_pressure::instance()->qpcr_chamber_start(0);
}

//qpcr腔室气压停止
void OtherWidget::on_qpcr_stop_Btn_clicked()
{
    air_pressure::instance()->qpcr_chamber_stop(0);
}

//固定风扇转速
void OtherWidget::on_Keep_fan_rpm_Btn_clicked()
{
    air_pressure::instance()->keep_fan_rpm(0);
}

