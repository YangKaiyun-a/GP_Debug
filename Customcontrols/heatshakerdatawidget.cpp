#include "heatshakerdatawidget.h"
#include "ui_heatshakerdatawidget.h"
#include "Controller/Heat_Shake/heat_shake.h"
#include "hcscitoolkits/dialog/messagedialog.h"

#include <QRegExpValidator>

HeatShakerDataWidget::HeatShakerDataWidget(int moduleId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeatShakerDataWidget), m_moduleId(moduleId)
{
    ui->setupUi(this);
    init();
}

HeatShakerDataWidget::~HeatShakerDataWidget()
{
    delete ui;
}

void HeatShakerDataWidget::init()
{
    ui->line_9->setObjectName("blackLine");
    ui->line_10->setObjectName("blackLine");

    ui->label->setObjectName("labModuleAction");
    ui->label_2->setObjectName("labModuleAction");

    initConnect();

    //注册类型
    qRegisterMetaType<QList<float> > ("QList<float>");
    qRegisterMetaType<Heat_Shake::testparam> ("Heat_Shake::testparam");
}

void HeatShakerDataWidget::initConnect()
{
    connect(ui->swControlDuct, &SwitchButton::statusChanged, this, &HeatShakerDataWidget::onStatusChanged);
}

void HeatShakerDataWidget::onStatusChanged(bool startHeat)
{
    //先判断是否为空
    if(ui->editTargetTemperature->value() == 0)
    {
        MessageDialog msg("输入温度不能为空！");
        msg.exec();
        ui->swControlDuct->setChecked(false);
        return;
    }

    if(startHeat)
    {
        //开始加热
        Heat_Shake::instance()->start_heat(m_moduleId, ui->editTargetTemperature->value());
    }
    else
    {
        //停止加热
        Heat_Shake::instance()->stop_heat(m_moduleId);
    }
}

//吸磁 = 吸磁推入
void HeatShakerDataWidget::on_btnMagnetic_clicked()
{
    Heat_Shake::instance()->magnet_in(m_moduleId);
}

//复位
void HeatShakerDataWidget::on_btnReset_clicked()
{
    Heat_Shake::instance()->moduleReset(m_moduleId);
}

//高速振荡
void HeatShakerDataWidget::on_btnHighSpeedShake_clicked()
{
    Heat_Shake::instance()->start_shake(m_moduleId, 2000);
}

//低速振荡
void HeatShakerDataWidget::on_btnLowSpeedShake_clicked()
{
    Heat_Shake::instance()->start_shake(m_moduleId, 900);
}

//停止振荡
void HeatShakerDataWidget::on_btnStopShake_clicked()
{
    Heat_Shake::instance()->stop_shake(m_moduleId);
}

//电机复位
void HeatShakerDataWidget::on_btnMagnetic_3_clicked()
{
    Heat_Shake::instance()->motorReset(m_moduleId);
}

//获取参数
void HeatShakerDataWidget::on_btnGetParams_clicked()
{
    Heat_Shake::HeatShakeParams param;

    Heat_Shake::instance()->getParameter(m_moduleId, param);
    ui->doubleSpinBox_7->setValue(param.magnet_speed);
    ui->doubleSpinBox->setValue(param.magnet_in_position_1);        //水平吸磁位
    ui->doubleSpinBox_3->setValue(param.magnet_in_position_3);      //下拉吸磁位
    ui->doubleSpinBox_4->setValue(param.magnet_in_position_4);      //上拉吸磁位
    ui->doubleSpinBox_8->setValue(param.heat_k);
    ui->doubleSpinBox_9->setValue(param.heat_b);
}

//保存参数
void HeatShakerDataWidget::on_btnGetParams_2_clicked()
{
    Heat_Shake::HeatShakeParams param;

    param.magnet_speed = ui->doubleSpinBox_7->value();
    param.magnet_in_position_1 = ui->doubleSpinBox->value();        //水平吸磁位
    param.magnet_in_position_3 = ui->doubleSpinBox_3->value();      //下拉吸磁位
    param.magnet_in_position_4 = ui->doubleSpinBox_4->value();      //上拉吸磁位
    param.heat_k = ui->doubleSpinBox_8->value();
    param.heat_b = ui->doubleSpinBox_9->value();

    Heat_Shake::instance()->setParameter(m_moduleId, param);
}

//吸磁电机运动
void HeatShakerDataWidget::on_btnMagnetic_4_clicked()
{
    //获取参数
    DevicePart::MotorMotionParams param;
    param.speed = 15.0;

    if(ui->comboBox->currentIndex() == 0)
    {
        //水平吸磁位
        if(ui->doubleSpinBox->value() == 0)
        {
            MessageDialog msg("请输入运动距离！");
            msg.exec();
            return;
        }

        param.targetPosition = ui->doubleSpinBox->value();
    }
    else if(ui->comboBox->currentIndex() == 1)
    {
        //返回位
        Heat_Shake::HeatShakeParams param2;
        Heat_Shake::instance()->getParameter(m_moduleId, param2);

        param.targetPosition = param2.magnet_in_position_2;

    }
    else if(ui->comboBox->currentIndex() == 2)
    {
        //下拉吸磁位
        if(ui->doubleSpinBox_3->value() == 0)
        {
            MessageDialog msg("请输入运动距离！");
            msg.exec();
            return;
        }

        param.targetPosition = ui->doubleSpinBox_3->value();
    }
    else if(ui->comboBox->currentIndex() == 3)
    {
        //上拉吸磁位
        if(ui->doubleSpinBox_4->value() == 0)
        {
            MessageDialog msg("请输入运动距离！");
            msg.exec();
            return;
        }

        param.targetPosition = ui->doubleSpinBox_4->value();
    }

    Heat_Shake::instance()->motorMove(m_moduleId, param);
}

//读取温度
void HeatShakerDataWidget::on_btnHighSpeedShake_2_clicked()
{
    //临时措施，先关闭上一个温度读取，结束线程
    Heat_Shake::read_temp_instance()->stop_readTemp();

    QMetaObject::invokeMethod(Heat_Shake::read_temp_instance(), "readTemp", Qt::QueuedConnection, Q_ARG(int, m_moduleId));

    connect(Heat_Shake::read_temp_instance(), &Heat_Shake::sigTemp, this, [=](int moduleIndex, QList<float> temps){
        if(moduleIndex != m_moduleId)
        {
            ui->doubleSpinBox_10->setValue(0);
            return;
        }

        ui->doubleSpinBox_10->setValue(temps.at(0));
    });
}

//开始老化
void HeatShakerDataWidget::on_btnHighSpeedShake_3_clicked()
{
    //先获取参数
    Heat_Shake::HeatShakeParams param;
    Heat_Shake::instance()->getParameter(m_moduleId, param);

    Heat_Shake::testparam test_param;

    //水平吸磁位
    test_param.start_position = param.magnet_in_position_1;
    test_param.start_sleep_time = 0;

    //返回位
    test_param.next_position = param.magnet_in_position_2;
    test_param.next_sleep_time = 0;

    //下拉吸磁位
    test_param.third_position = param.magnet_in_position_3;
    test_param.third_sleep_time = 0;

    //上拉吸磁位
    test_param.fourth_position = param.magnet_in_position_4;
    test_param.fourth_sleep_time = 0;

    //原点位置
    test_param.stop_position = 0.0;
    test_param.stop_sleep_time = 0;

    test_param.loop_num = ui->spinBox->value();
    test_param.cycle_speed = param.magnet_speed;

    //模块复位
    Heat_Shake::instance()->moduleReset(m_moduleId);

    //开始老化
    QMetaObject::invokeMethod(Heat_Shake::instance(), "test", \
                              Qt::QueuedConnection, \
                              Q_ARG(int, m_moduleId), \
                              Q_ARG(Heat_Shake::testparam, test_param), \
                              Q_ARG(int, 0));

    connect(Heat_Shake::instance(), &Heat_Shake::testCount, this, [=](int count){
        ui->label_17->setText(QString::number(count));
    });
}

//停止老化
void HeatShakerDataWidget::on_btnHighSpeedShake_4_clicked()
{
    Heat_Shake::instance()->stoptest();
}

