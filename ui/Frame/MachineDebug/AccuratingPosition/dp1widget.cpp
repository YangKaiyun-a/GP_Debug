#include "dp1widget.h"
#include "ui_dp1widget.h"

#include "Function/data/dbmanager.h"
#include "Controller/sportmethods.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "application.h"
#include "Customcontrols/dp1carrierwidget.h"
#include "Controller/Dp1/dp1.h"
#include "Function/data/parsemessageinfo.h"

#include <QButtonGroup>
#include <QDebug>

DP1Widget::DP1Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP1Widget)
{
    ui->setupUi(this);
    init();
}

DP1Widget::~DP1Widget()
{
    delete ui;
}

void DP1Widget::init()
{
    initStyleSheet();
    initData();
    initUI();
    initConnect();
}

void DP1Widget::initStyleSheet()
{
    ui->pushButton_9->setObjectName("btnPosition");
    ui->pushButton_11->setObjectName("btnCirclePosition");
    ui->pushButton_25->setObjectName("btnPosition");
    ui->pushButton_26->setObjectName("btnPosition");
    ui->pushButton_29->setObjectName("btnPosition");
    ui->pushButton_30->setObjectName("btnPosition");
    ui->pushButton_12->setObjectName("btnPosition");
    ui->pushButton_13->setObjectName("btnPosition");
    ui->pushButton_27->setObjectName("btnPosition");
    ui->pushButton_28->setObjectName("btnPosition");

    ui->widget->setObjectName("carrierWidget");
    ui->widget_15->setObjectName("carrierWidget");
    ui->widget_5->setObjectName("carrierWidget");
    ui->widget_2->setObjectName("carrierWidget");

    ui->stackedWidget->setObjectName("stepsWidget");
    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");
    ui->widget_9->setObjectName("stepsWidget");
    ui->widget_10->setObjectName("stepsWidget");
    ui->widget_11->setObjectName("stepsWidget");
    ui->widget_12->setObjectName("stepsWidget");
    ui->widget_13->setObjectName("stepsWidget");

    ui->label_29->setObjectName("labModuleAction");
    ui->label_31->setObjectName("labModuleAction");
    ui->label_32->setObjectName("labModuleAction");
    ui->label_33->setObjectName("labModuleAction");

    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->pushButton_9);
    buttonGround->addButton(ui->pushButton_11);
    buttonGround->addButton(ui->pushButton_25);
    buttonGround->addButton(ui->pushButton_26);
    buttonGround->addButton(ui->pushButton_29);
    buttonGround->addButton(ui->pushButton_30);
    buttonGround->addButton(ui->pushButton_12);
    buttonGround->addButton(ui->pushButton_13);
    buttonGround->addButton(ui->pushButton_27);
    buttonGround->addButton(ui->pushButton_28);
    buttonGround->setExclusive(true);
}

void DP1Widget::initData()
{
    m_rackVector.clear();
    m_map.clear();

    m_E2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "E2机位");
    m_C1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "C1机位");
    m_Clean_Pool_2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "清洗池2机位");
    m_Magnetic_Beads_Shake_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "磁珠振荡机位");
    m_C2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "C2机位");
    m_Open_Gate_Waiting_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "开盖等待机位");
    m_D1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "D1机位");
    m_E1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "E1机位");
    m_Transport_Add_Sample_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "中转加样机位");
    m_D2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "D2机位");

    m_rackVector << m_E2_Rack << m_C1_Rack \
                 << m_Clean_Pool_2_Rack << m_Magnetic_Beads_Shake_Rack\
                 << m_C2_Rack << m_Open_Gate_Waiting_Rack \
                 << m_D1_Rack << m_E1_Rack \
                 << m_Transport_Add_Sample_Rack << m_D2_Rack;

    //获取DP1移液器信息
    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(ENUM_CLASS::Gantry::Gantry_DP1);

    //获取夹爪相对葫芦头位置
    m_clawInfo = DBManager::instance()->getClawInfoByGantryId(ENUM_CLASS::Gantry::Gantry_DP1);
    //组合为夹爪相对位置最终值
    m_clawInfo.distance_end_x = m_clawInfo.distance_x - m_clawInfo.deviation_x;
    m_clawInfo.distance_end_y = m_clawInfo.distance_y - m_clawInfo.deviation_y;
    m_clawInfo.distance_end_z = m_clawInfo.distance_z - m_clawInfo.deviation_z;
    //组合为夹爪坐标
    m_clawInfo.loc_x = m_pipetteInfo.distance_x_from_orig + m_clawInfo.distance_x;
    m_clawInfo.loc_y = m_pipetteInfo.distance_y_from_orig + m_clawInfo.distance_y;
    m_clawInfo.loc_z = m_pipetteInfo.distance_z_from_orig + m_clawInfo.distance_z;

    //获取激光相对葫芦头位置
    ParseMessageInfo::getLaserDistance(m_laserInfo);

    //组合为激光坐标
    m_laserInfo.loc_x = m_pipetteInfo.distance_x_from_orig + m_laserInfo.distance_x;
    m_laserInfo.loc_y = m_pipetteInfo.distance_y_from_orig + m_laserInfo.distance_y;
    m_laserInfo.loc_z = m_pipetteInfo.distance_z_from_orig + m_laserInfo.distance_z;


    //根据机位查询载架，组合为一条位置信息Storage_Info
    for(auto id : m_rackVector)
    {
        Storage_Info info;
        info.rack_id = id;

        //获取该机位的载架、标记偏差
        ParseMessageInfo::getCrossingDeviatio(info);

        //根据查询数据库来组合出最终位置坐标
        DBManager::instance()->getStorageInfoByCarrierId(info);

        if(info.loc_x == 0 && info.loc_y == 0 && info.loc_z == 0)
        {
            qDebug() << "rack_id = " << id << "，rack_name = " << info.rack_name << "的机位没有用到任何载架，或者最终坐标为（0，0，0）";
        }

        //添加所属移液器id
        info.pipette_id = m_pipetteInfo.module_id;

        DP1CarrierWidget *wgt = new DP1CarrierWidget(info, m_pipetteInfo, m_laserInfo);
        ui->stackedWidget->addWidget(wgt);

        m_map.insert(id, wgt);

        //获取移液器校准位（C2机位及其载架）
        if(id == m_C2_Rack)
        {
            m_storageInfo = info;
        }

        //获取夹爪相对位置校准位（开盖等待机位）
        if(id == m_Open_Gate_Waiting_Rack)
        {
            m_clawStorageInfo = info;
        }
    }

    ui->stackedWidget->setCurrentWidget(m_map.value(m_C1_Rack));

}

void DP1Widget::initUI()
{
    //移液器偏差
    ui->deviation_x_SpinBox->setValue(m_pipetteInfo.deviate_x);
    ui->deviation_y_SpinBox->setValue(m_pipetteInfo.deviate_y);
    ui->deviation_z_SpinBox->setValue(m_pipetteInfo.deviate_z);

    //夹爪相对位置偏差
    ui->deviation_x_SpinBox_3->setValue(m_clawInfo.deviation_x);
    ui->deviation_y_SpinBox_3->setValue(m_clawInfo.deviation_y);
    ui->deviation_z_SpinBox_3->setValue(m_clawInfo.deviation_z);

    ui->deviation_x_SpinBox_2->setValue(0);
    ui->deviation_y_SpinBox_2->setValue(0);
    ui->deviation_z_SpinBox_2->setValue(0);
}

void DP1Widget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updateRackDeviation, this, &DP1Widget::onUpdateRackDeviation);
}

//更新机位偏差，因修正移液器偏差和校准夹爪需要
void DP1Widget::onUpdateRackDeviation(int rackId)
{
    if(rackId == m_storageInfo.rack_id)
    {
        DBManager::instance()->getRackDevationById(m_storageInfo);
    }
    else if(rackId == m_clawStorageInfo.rack_id)
    {
        DBManager::instance()->getRackDevationById(m_clawStorageInfo);
    }
}

void DP1Widget::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Clean_Pool_2_Rack));
}

void DP1Widget::on_pushButton_26_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Magnetic_Beads_Shake_Rack));
}

void DP1Widget::on_pushButton_29_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Open_Gate_Waiting_Rack));
}


void DP1Widget::on_pushButton_30_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Transport_Add_Sample_Rack));
}


void DP1Widget::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_C1_Rack));
}


void DP1Widget::on_pushButton_25_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_C2_Rack));
}


void DP1Widget::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_D1_Rack));
}


void DP1Widget::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_E1_Rack));
}


void DP1Widget::on_pushButton_27_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_D2_Rack));
}


void DP1Widget::on_pushButton_28_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_E2_Rack));
}

//运动至原点
void DP1Widget::on_pushButton_17_clicked()
{
    SportMethods::instance()->moveToOriginPosition(m_laserInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value());
}

//运动至原点偏移位
void DP1Widget::on_pushButton_33_clicked()
{
    SportMethods::instance()->moveToOriginPositionOffset(m_laserInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value());
}

//保存移液器偏差
void DP1Widget::on_btnSava_clicked()
{
    m_pipetteInfo.deviate_x = ui->deviation_x_SpinBox->value();
    m_pipetteInfo.deviate_y = ui->deviation_y_SpinBox->value();
    m_pipetteInfo.deviate_z = ui->deviation_z_SpinBox->value();

    if(!DBManager::instance()->setPipeteeDeviationValue(m_pipetteInfo))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        //通知DP1的其他机位重新读取移液器偏差
        Q_EMIT hApp->m_sigmanager->updatePipetteDeviation(ENUM_CLASS::Gantry::Gantry_DP1);
    }
}

//运动至移液器偏差校准位
void DP1Widget::on_pushButton_18_clicked()
{
    SportMethods::instance()->moveToPipetteAccuratingPosition(m_storageInfo, m_pipetteInfo);
}

//保存修正的移液器偏差
void DP1Widget::on_btnSava_2_clicked()
{
    PipetteInfo pipetteInfo = m_pipetteInfo;
    pipetteInfo.deviate_x += ui->deviation_x_SpinBox_2->value();
    pipetteInfo.deviate_y += ui->deviation_y_SpinBox_2->value();
    pipetteInfo.deviate_z += ui->deviation_z_SpinBox_2->value();

    if(!DBManager::instance()->setPipeteeDeviationValue(pipetteInfo))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_pipetteInfo = pipetteInfo;

        //通知DP1的其他机位重新读取移液器偏差
        Q_EMIT hApp->m_sigmanager->updatePipetteDeviation(ENUM_CLASS::Gantry::Gantry_DP1);

        initUI();
    }
}

//运动至调整后的夹爪校准位
void DP1Widget::on_pushButton_34_clicked()
{
    SportMethods::instance()->moveToClawAccuratingPositionDP1(m_clawStorageInfo, m_pipetteInfo, m_clawInfo, ui->deviation_x_SpinBox_3->value(), ui->deviation_y_SpinBox_3->value(), ui->deviation_z_SpinBox_3->value());
}

//保存夹爪偏差
void DP1Widget::on_btnSava_3_clicked()
{
    ClawInfo clawInfo = m_clawInfo;

    //更新偏差
    clawInfo.deviation_x = ui->deviation_x_SpinBox_3->value();
    clawInfo.deviation_y = ui->deviation_y_SpinBox_3->value();
    clawInfo.deviation_z = ui->deviation_z_SpinBox_3->value();

    //更新夹爪和葫芦头相对位置（原始相对位置 - 显示值）
    clawInfo.distance_end_x = clawInfo.distance_x - clawInfo.deviation_x;
    clawInfo.distance_end_y = clawInfo.distance_y - clawInfo.deviation_y;
    clawInfo.distance_end_z = clawInfo.distance_z - clawInfo.deviation_z;

    //更新夹爪位置
    clawInfo.loc_x = m_pipetteInfo.distance_x_from_orig + clawInfo.distance_x;
    clawInfo.loc_y = m_pipetteInfo.distance_y_from_orig + clawInfo.distance_y;
    clawInfo.loc_z = m_pipetteInfo.distance_z_from_orig + clawInfo.distance_z;

    //只保存偏差
    if(!DBManager::instance()->setClawDeviationValue(clawInfo))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_clawInfo = clawInfo;
    }
}

//DP1模块复位
void DP1Widget::on_pushButton_clicked()
{
    Dp1::instance()->module_reset();
}

//X轴电机复位
void DP1Widget::on_pushButton_2_clicked()
{
    Dp1_Xy::instance()->motorReset(Dp1_Xy::MOTOR_X);
}

//Y轴电机复位
void DP1Widget::on_pushButton_3_clicked()
{
    Dp1_Xy::instance()->motorReset(Dp1_Xy::MOTOR_Y);
}

//ADPZ轴电机复位
void DP1Widget::on_pushButton_4_clicked()
{
    Dp1::instance()->motorReset(Dp1::MOTOR_Dp1_Z);
}

//夹爪Z轴复位
void DP1Widget::on_pushButton_6_clicked()
{
    Dp1::instance()->motorReset(Dp1::MOTOR_CLAW_Z);
}

//底部夹爪张开
void DP1Widget::on_pushButton_20_clicked()
{
    //数值待确认
    Dp1::instance()->claw_2_release(ui->deviation_x_SpinBox_5->value());
}

//底部夹爪复位
void DP1Widget::on_pushButton_19_clicked()
{
    Dp1::instance()->claw_2_reset();
}

//夹爪张开（旋转夹爪）
void DP1Widget::on_pushButton_21_clicked()
{
    //数值待确认
    Dp1::instance()->release(ui->deviation_x_SpinBox_4->value());
}

//夹爪复位（旋转夹爪）
void DP1Widget::on_pushButton_23_clicked()
{
    Dp1::instance()->claw_reset();
}
