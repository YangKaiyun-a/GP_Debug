#include "dp8widget.h"
#include "ui_dp8widget.h"
#include "Function/data/dbmanager.h"
#include "Controller/sportmethods.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "application.h"
#include "Customcontrols/dp8carrierwidget.h"
#include "Customcontrols/dp8pcrwidget.h"
#include "Controller/Dp8/dp8.h"
#include "Function/data/parsemessageinfo.h"

#include <QButtonGroup>
#include <QDebug>

DP8Widget::DP8Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP8Widget)
{
    ui->setupUi(this);
    init();
}

DP8Widget::~DP8Widget()
{
    delete ui;
}

void DP8Widget::init()
{
    initStyleSheet();
    initData();
    initUI();
    initConnect();
}

void DP8Widget::initStyleSheet()
{
    ui->widget_2->setObjectName("carrierWidget");
    ui->widget_8->setObjectName("carrierWidget");
    ui->widget_5->setObjectName("carrierWidget");

    ui->pushButton_36->setObjectName("btnPosition");
    ui->pushButton_37->setObjectName("btnPosition");
    ui->pushButton_39->setObjectName("btnPosition");
    ui->pushButton_40->setObjectName("btnPosition");
    ui->pushButton_41->setObjectName("btnPosition");
    ui->pushButton_44->setObjectName("btnPosition");
    ui->pushButton_45->setObjectName("btnPosition");
    ui->pushButton_42->setObjectName("btnPosition");
    ui->pushButton_43->setObjectName("btnPosition");
    ui->pushButton_46->setObjectName("btnPosition");
    ui->pushButton_47->setObjectName("btnPosition");
    ui->pushButton_48->setObjectName("btnPosition");
    ui->pushButton_49->setObjectName("btnPosition");
    ui->pushButton_50->setObjectName("btnPosition");
    ui->pushButton_38->setObjectName("btnPosition");

    ui->label_31->setObjectName("labModuleAction");
    ui->label_32->setObjectName("labModuleAction");
    ui->label_33->setObjectName("labModuleAction");

    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");
    ui->widget_9->setObjectName("stepsWidget");
    ui->widget_11->setObjectName("stepsWidget");
    ui->stackedWidget->setObjectName("stepsWidget");
    ui->stackedWidget_2->setObjectName("stepsWidget");
    ui->widget_12->setObjectName("stepsWidget");
    ui->widget_13->setObjectName("stepsWidget");


    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->pushButton_36);
    buttonGround->addButton(ui->pushButton_37);
    buttonGround->addButton(ui->pushButton_39);
    buttonGround->addButton(ui->pushButton_40);
    buttonGround->addButton(ui->pushButton_41);
    buttonGround->addButton(ui->pushButton_44);
    buttonGround->addButton(ui->pushButton_45);
    buttonGround->addButton(ui->pushButton_42);
    buttonGround->addButton(ui->pushButton_43);
    buttonGround->addButton(ui->pushButton_46);
    buttonGround->addButton(ui->pushButton_47);
    buttonGround->addButton(ui->pushButton_48);
    buttonGround->addButton(ui->pushButton_49);
    buttonGround->addButton(ui->pushButton_50);
    buttonGround->addButton(ui->pushButton_38);
    buttonGround->setExclusive(true);
}

void DP8Widget::initData()
{
    m_rackVector.clear();
    m_map.clear();

    m_G1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "G1机位");
    m_Nucleic_Storage_2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "核酸暂存2机位");
    m_F2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "F2机位");
    m_Nucleic_Storage_1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "核酸暂存1机位");
    m_F1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "F1机位");
    m_System_Building_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "体系构建机位");
    m_Transport_Pcr_2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "PCR板转运机位");
    m_Transport_Pcr_Put_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "PCR板转运机位放置位");
    m_Magnet_Shake_1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "吸磁震荡1机位");
    m_Magnet_Shake_2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "吸磁震荡2机位");
    m_Magnet_Shake_3_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "吸磁震荡3机位");
    m_Magnet_Shake_4_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "吸磁震荡4机位");
    m_Magnet_Shake_5_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "吸磁震荡5机位");
    m_Trash_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "废料仓机位");
    m_Heat_Seal_Waiting_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "热封等待机位");

    m_rackVector << m_G1_Rack << m_Nucleic_Storage_2_Rack \
                 << m_F2_Rack << m_Nucleic_Storage_1_Rack \
                 << m_F1_Rack << m_System_Building_Rack \
                 << m_Transport_Pcr_2_Rack << m_Transport_Pcr_Put_Rack \
                 << m_Magnet_Shake_3_Rack << m_Magnet_Shake_4_Rack \
                 << m_Magnet_Shake_5_Rack << m_Trash_Rack \
                 << m_Magnet_Shake_2_Rack << m_Magnet_Shake_1_Rack \
                 << m_Heat_Seal_Waiting_Rack;

    //获取DP8移液器信息
    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(ENUM_CLASS::Gantry::Gantry_DP8);

    //获取激光复位位置Z0（全局）
    m_resetZ0 = ParseMessageInfo::resetZ0();

    //获取夹爪相对葫芦头位置
    m_clawInfo = DBManager::instance()->getClawInfoByGantryId(ENUM_CLASS::Gantry::Gantry_DP8);
    //组合为夹爪相对位置最终值
    m_clawInfo.distance_end_x = m_clawInfo.distance_x - m_clawInfo.deviation_x;
    m_clawInfo.distance_end_y = m_clawInfo.distance_y - m_clawInfo.deviation_y;
    m_clawInfo.distance_end_z = m_clawInfo.distance_z - m_clawInfo.deviation_z;
    //组合为夹爪坐标
    m_clawInfo.loc_x = m_pipetteInfo.distance_x_from_orig + m_clawInfo.distance_x;
    m_clawInfo.loc_y = m_pipetteInfo.distance_y_from_orig + m_clawInfo.distance_y;
    m_clawInfo.loc_z = m_pipetteInfo.distance_z_from_orig + m_clawInfo.distance_z;

    //获取单通道相对葫芦头位置
    m_singleInfo = DBManager::instance()->getSingleInfoByGantryId(ENUM_CLASS::Gantry::Gantry_DP8);
    //组合为单通道相对位置最终值
    m_singleInfo.distance_end_x = m_singleInfo.distance_x - m_singleInfo.deviation_x;
    m_singleInfo.distance_end_y = m_singleInfo.distance_y - m_singleInfo.deviation_y;
    m_singleInfo.distance_end_z = m_singleInfo.distance_z - m_singleInfo.deviation_z;
    //组合为单通道坐标
    m_singleInfo.loc_x = m_pipetteInfo.distance_x_from_orig + m_singleInfo.distance_x;
    m_singleInfo.loc_y = m_pipetteInfo.distance_y_from_orig + m_singleInfo.distance_y;
    m_singleInfo.loc_z = m_pipetteInfo.distance_z_from_orig + m_singleInfo.distance_z;

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

        if(id == m_Heat_Seal_Waiting_Rack || id == m_Transport_Pcr_2_Rack || id == m_Transport_Pcr_Put_Rack)
        {
            DP8PcrWidget *wgt = new DP8PcrWidget(info, m_pipetteInfo, m_clawInfo);
            ui->stackedWidget_2->addWidget(wgt);
            m_pcrMap.insert(id, wgt);
        }
        else
        {
            DP8CarrierWidget *wgt = new DP8CarrierWidget(info, m_pipetteInfo, m_resetZ0);
            ui->stackedWidget->addWidget(wgt);
            m_map.insert(id, wgt);
        }

        //获取移液器校准位（F2机位）
        if(id == m_F2_Rack)
        {
            m_storageInfo = info;
        }

        //获取夹爪相对位置校准位（体系构建位）
        if(id == m_System_Building_Rack)
        {
            m_clawStorageInfo = info;
        }
    }

    ui->stackedWidget->setCurrentWidget(m_map.value(m_F1_Rack));
    ui->stackedWidget_2->setCurrentWidget(m_pcrMap.value(m_Heat_Seal_Waiting_Rack));
}

void DP8Widget::initUI()
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

void DP8Widget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updateRackDeviation, this, &DP8Widget::onUpdateRackDeviation);
}

//更新机位偏差，因修正移液器偏差和校准夹爪需要
void DP8Widget::onUpdateRackDeviation(int rackId)
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

void DP8Widget::on_pushButton_37_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_System_Building_Rack));
}

void DP8Widget::on_pushButton_45_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Magnet_Shake_1_Rack));
}


void DP8Widget::on_pushButton_44_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Magnet_Shake_2_Rack));
}


void DP8Widget::on_pushButton_39_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Magnet_Shake_3_Rack));
}


void DP8Widget::on_pushButton_40_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Magnet_Shake_4_Rack));
}


void DP8Widget::on_pushButton_41_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Magnet_Shake_5_Rack));
}


void DP8Widget::on_pushButton_46_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Trash_Rack));
}


void DP8Widget::on_pushButton_50_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Nucleic_Storage_1_Rack));
}


void DP8Widget::on_pushButton_47_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_F1_Rack));
}


void DP8Widget::on_pushButton_38_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_Nucleic_Storage_2_Rack));
}


void DP8Widget::on_pushButton_48_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_F2_Rack));
}


void DP8Widget::on_pushButton_49_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_G1_Rack));
}


void DP8Widget::on_pushButton_36_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(m_pcrMap.value(m_Heat_Seal_Waiting_Rack));
}


void DP8Widget::on_pushButton_42_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(m_pcrMap.value(m_Transport_Pcr_2_Rack));
}


void DP8Widget::on_pushButton_43_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(m_pcrMap.value(m_Transport_Pcr_Put_Rack));
}

//运动至原点校准位
void DP8Widget::on_pushButton_17_clicked()
{
    SportMethods::instance()->moveToOriginPosition(m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), m_resetZ0);
}

//运动至原点偏移位
void DP8Widget::on_pushButton_33_clicked()
{
    SportMethods::instance()->moveToOriginPositionOffset(m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), m_resetZ0);
}

//保存
void DP8Widget::on_btnSava_clicked()
{
    PipetteInfo pipetteInfo = m_pipetteInfo;

    pipetteInfo.deviate_x = ui->deviation_x_SpinBox->value();
    pipetteInfo.deviate_y = ui->deviation_y_SpinBox->value();
    pipetteInfo.deviate_z = ui->deviation_z_SpinBox->value();

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

        //通知DP8的其他机位重新读取移液器偏差
        Q_EMIT hApp->m_sigmanager->updatePipetteDeviation(ENUM_CLASS::Gantry::Gantry_DP8);
    }
}

//运动至移液器偏差校准位   -F2
void DP8Widget::on_pushButton_18_clicked()
{
    SportMethods::instance()->moveToPipetteAccuratingPositionDP8(m_storageInfo, m_pipetteInfo, m_resetZ0);
}

//保存修正后的移液器偏差
void DP8Widget::on_btnSava_2_clicked()
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

        //通知DP8的其他机位重新读取移液器偏差
        Q_EMIT hApp->m_sigmanager->updatePipetteDeviation(ENUM_CLASS::Gantry::Gantry_DP8);

        initUI();
    }
}

//运动至调整后的夹爪校准位-体系构建位
void DP8Widget::on_pushButton_34_clicked()
{
    SportMethods::instance()->moveToClawAccuratingPositionDP8(m_clawStorageInfo, m_pipetteInfo, m_clawInfo, ui->deviation_x_SpinBox_3->value(), ui->deviation_y_SpinBox_3->value(), ui->deviation_z_SpinBox_3->value());
}

//保存夹爪偏差
void DP8Widget::on_btnSava_3_clicked()
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

        //通知DP8的其他机位重新读夹爪
        Q_EMIT hApp->m_sigmanager->updateClawInfo(ENUM_CLASS::Gantry::Gantry_DP8);
    }
}

//DP8模块复位
void DP8Widget::on_pushButton_clicked()
{
    Dp8::instance()->module_reset();
    Dp8_Xy::instance()->module_reset();
}

//X轴电机复位
void DP8Widget::on_pushButton_2_clicked()
{
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_X);
}

//Y轴电机复位
void DP8Widget::on_pushButton_3_clicked()
{
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_Y);
}

//八通道Z轴复位（DP8-Z轴复位）
void DP8Widget::on_pushButton_4_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_DP8_Z);
}

//单通道Z轴复位（DP1-Z轴复位）
void DP8Widget::on_pushButton_6_clicked()
{
    Dp8_Xy::instance()->motorReset(Dp8_Xy::MOTOR_DP1_Z);
}

//夹爪张开
void DP8Widget::on_btnSava_4_clicked()
{
    Dp8::instance()->glab(ui->deviation_x_SpinBox_4->value());
}

//夹爪复位
void DP8Widget::on_btnSava_5_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW);
}

//夹爪Z轴复位
void DP8Widget::on_pushButton_7_clicked()
{
    Dp8::instance()->motorReset(Dp8::MOTOR_CLAW_Z);
}

