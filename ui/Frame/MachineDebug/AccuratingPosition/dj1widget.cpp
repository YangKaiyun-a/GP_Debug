#include "dj1widget.h"
#include "ui_dj1widget.h"
#include "Function/data/dbmanager.h"
#include "Controller/sportmethods.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "application.h"
#include "Customcontrols/dj1carrierwidget.h"
#include "Controller/Amplification_Dj1/amplification_dj1.h"
#include "Function/data/parsemessageinfo.h"
#include "Function/data/global.h"

#include <QButtonGroup>
#include <QDebug>



DJ1Widget::DJ1Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DJ1Widget)
{
    ui->setupUi(this);
    init();
}

DJ1Widget::~DJ1Widget()
{
    delete ui;
}

void DJ1Widget::init()
{
    initStyleSheet();
    initData();
    initUI();
    initConnect();
}

void DJ1Widget::initStyleSheet()
{
    ui->pushButton_6->setObjectName("btnPosition");
    ui->pushButton_7->setObjectName("btnPosition");
    ui->pushButton_8->setObjectName("btnPosition");
    ui->pushButton_9->setObjectName("btnPosition");
    ui->pushButton_11->setObjectName("btnCirclePosition");
    ui->pushButton_12->setObjectName("btnPosition");
    ui->pushButton_13->setObjectName("btnPosition");

    ui->widget->setObjectName("carrierWidget");
    ui->widget_2->setObjectName("carrierWidget");

    ui->stackedWidget->setObjectName("stepsWidget");
    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");
    ui->widget_9->setObjectName("stepsWidget");
    ui->widget_11->setObjectName("stepsWidget");
    ui->widget_12->setObjectName("stepsWidget");
    ui->widget_13->setObjectName("stepsWidget");

    ui->label_31->setObjectName("labModuleAction");
    ui->label_32->setObjectName("labModuleAction");
    ui->label_33->setObjectName("labModuleAction");

    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->pushButton_6);
    buttonGround->addButton(ui->pushButton_7);
    buttonGround->addButton(ui->pushButton_8);
    buttonGround->addButton(ui->pushButton_9);
    buttonGround->addButton(ui->pushButton_11);
    buttonGround->addButton(ui->pushButton_12);
    buttonGround->addButton(ui->pushButton_13);
    buttonGround->setExclusive(true);
}

void DJ1Widget::initData()
{
    m_rackVector.clear();
    m_map.clear();
    m_storageVector.clear();

    //根据table_id与rack_name获取到rack_id
    m_A1_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "A1机位");
    m_B1_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "B1机位");
    m_clean1_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "清洗池1机位");
    m_pcr1_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "PCR板转运机位");
    m_B2_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "B2机位");
    m_B_open_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "B架开盖位");
    m_B_close_rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(1, "B架关盖位");

    m_rackVector << m_A1_rack_id << m_B1_rack_id \
                 << m_clean1_rack_id << m_pcr1_rack_id \
                 << m_B2_rack_id << m_B_open_rack_id \
                 << m_B_close_rack_id;

    //获取DJ1移液器信息
    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(ENUM_CLASS::Gantry::Gantry_DJ1);

    //获取夹爪相对葫芦头位置
    m_clawInfo = DBManager::instance()->getClawInfoByGantryId(ENUM_CLASS::Gantry::Gantry_DJ1);
    //组合为夹爪相对位置最终值
    m_clawInfo.distance_end_x = m_clawInfo.distance_x - m_clawInfo.deviation_x;
    m_clawInfo.distance_end_y = m_clawInfo.distance_y - m_clawInfo.deviation_y;
    m_clawInfo.distance_end_z = m_clawInfo.distance_z - m_clawInfo.deviation_z;
    //组合为夹爪坐标
    m_clawInfo.loc_x = m_pipetteInfo.distance_x_from_orig + m_clawInfo.distance_x;
    m_clawInfo.loc_y = m_pipetteInfo.distance_y_from_orig + m_clawInfo.distance_y;
    m_clawInfo.loc_z = m_pipetteInfo.distance_z_from_orig + m_clawInfo.distance_z;

    //根据机位查询载架，组合为位置信息Storage_Info
    for(auto id : m_rackVector)
    {
        Storage_Info info;
        info.rack_id = id;

        //根据rack_id获取该机位的载架信息、标记偏差
        ParseMessageInfo::getCrossingDeviatio(info);

        //根据根据rack_id、carrier_id获取最终位置坐标
        DBManager::instance()->getStorageInfoByCarrierId(info);

        if(info.loc_x == 0 && info.loc_y == 0 && info.loc_z == 0)
        {
            qDebug() << "rack_id = " << id << "，rack_name = " << info.rack_name << "的机位没有用到任何载架，或者最终坐标为（0，0，0）";
        }

        //添加所属移液器id
        info.pipette_id = m_pipetteInfo.module_id;

        DJ1CarrierWidget *wgt = new DJ1CarrierWidget(info, m_pipetteInfo);
        ui->stackedWidget->addWidget(wgt);

        m_map.insert(id, wgt);

        //获取移液器校准点（A1机位及其载架）
        if(id == m_A1_rack_id)
        {
            m_storageInfo = info;
        }

        //获取转运校准点（PCR板转运机位）
        if(id == m_pcr1_rack_id)
        {
            m_transportInfo = info;
        }

        //B架开盖位
        if(id == m_B_open_rack_id)
        {
            m_bOpenInfo = info;
        }

        //B架关盖位
        if(id == m_B_close_rack_id)
        {
            m_bCloseInfo = info;
        }
    }

    ui->stackedWidget->setCurrentWidget(m_map.value(m_A1_rack_id));
}

void DJ1Widget::initUI()
{
    ui->deviation_x_SpinBox->setValue(m_pipetteInfo.deviate_x);
    ui->deviation_y_SpinBox->setValue(m_pipetteInfo.deviate_y);
    ui->deviation_z_SpinBox->setValue(m_pipetteInfo.deviate_z);

    //夹爪相对位置偏差
    ui->deviation_x_SpinBox_2->setValue(m_clawInfo.deviation_x);
    ui->deviation_y_SpinBox_2->setValue(m_clawInfo.deviation_y);
    ui->deviation_z_SpinBox_2->setValue(m_clawInfo.deviation_z);

    ui->comboBox->addItems(QStringList() << "转运校准位" << "B架开盖位" << "B架关盖位");
}

void DJ1Widget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updateRackDeviation, this, &DJ1Widget::onUpdateRackDeviation);
}

//保存转运校准位偏差
void DJ1Widget::saveTransportPositionDeviation()
{
    Storage_Info info = m_transportInfo;
    info.deviation_x = ui->deviation_x_SpinBox_3->value();
    info.deviation_y = ui->deviation_y_SpinBox_3->value();
    info.deviation_z = ui->deviation_z_SpinBox_3->value();

    if(!DBManager::instance()->setDeviationValue(info))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_transportInfo = info;
    }
}

//保存B架开盖位偏差
void DJ1Widget::saveBOpenPositionDeviation()
{
    Storage_Info info = m_bOpenInfo;
    info.deviation_x = ui->deviation_x_SpinBox_3->value();
    info.deviation_y = ui->deviation_y_SpinBox_3->value();
    info.deviation_z = ui->deviation_z_SpinBox_3->value();

    if(!DBManager::instance()->setDeviationValue(info))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_bOpenInfo = info;
    }
}

//保存B架关盖位偏差
void DJ1Widget::saveBClosePositionDeviation()
{
    Storage_Info info = m_bCloseInfo;
    info.deviation_x = ui->deviation_x_SpinBox_3->value();
    info.deviation_y = ui->deviation_y_SpinBox_3->value();
    info.deviation_z = ui->deviation_z_SpinBox_3->value();

    if(!DBManager::instance()->setDeviationValue(info))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_bCloseInfo = info;
    }
}

//更新机位偏差，因修正移液器偏差和校准夹爪需要，注意：DJ1的移液器校准位和夹爪校准位同为A1机位
void DJ1Widget::onUpdateRackDeviation(int rackId)
{
    if(rackId == m_storageInfo.rack_id)
    {
        DBManager::instance()->getRackDevationById(m_storageInfo);
    }
}

//运动至移液器偏差校准位
void DJ1Widget::on_pushButton_17_clicked()
{
    SportMethods::instance()->moveToPipetteAccuratingPosition(m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), ui->deviation_z_SpinBox->value());
}

//保存移液器偏差
void DJ1Widget::on_btnSava_clicked()
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

        //通知DJ1的其他机位重新读取移液器偏差
        Q_EMIT hApp->m_sigmanager->updatePipetteDeviation(ENUM_CLASS::Gantry::Gantry_DJ1);
    }
}


void DJ1Widget::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_A1_rack_id));
}

void DJ1Widget::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_A1_rack_id));
}

void DJ1Widget::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_A1_rack_id));
}

void DJ1Widget::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_A1_rack_id));
}

void DJ1Widget::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_B1_rack_id));
}

void DJ1Widget::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_B2_rack_id));
}

void DJ1Widget::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_map.value(m_clean1_rack_id));
}

//夹爪张开
void DJ1Widget::on_pushButton_20_clicked()
{
    //张开距离
    Amplification_Dj1::instance()->glab(ui->deviation_x_SpinBox_4->value());
}

//夹爪复位
void DJ1Widget::on_pushButton_21_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_CLAW);
}

//运动至夹爪校准位   -A1
void DJ1Widget::on_pushButton_18_clicked()
{
    SportMethods::instance()->moveToClawAccuratingPosition(m_storageInfo, m_pipetteInfo, m_clawInfo, ui->deviation_x_SpinBox_2->value(), ui->deviation_y_SpinBox_2->value(), ui->deviation_z_SpinBox_2->value());
}

//保存夹爪偏差
void DJ1Widget::on_btnSava_2_clicked()
{
    //更新夹爪和葫芦头相对位置
    ClawInfo clawInfo = m_clawInfo;

    //更新偏差
    clawInfo.deviation_x = ui->deviation_x_SpinBox_2->value();
    clawInfo.deviation_y = ui->deviation_y_SpinBox_2->value();
    clawInfo.deviation_z = ui->deviation_z_SpinBox_2->value();

    //更新夹爪相对葫芦头位置的最终值（原始相对位置 - 显示值）
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

//运动至三个校准位
void DJ1Widget::on_pushButton_22_clicked()
{
    Storage_Info info;

    switch (ui->comboBox->currentIndex())
    {
    case 0:
        info = m_transportInfo;
        break;
    case 1:
        info = m_bOpenInfo;
        break;
    case 2:
        info = m_bCloseInfo;
        break;
    default:
        break;
    }

    SportMethods::instance()->moveToTransportAccuratingPosition(info, m_pipetteInfo, m_clawInfo, ui->deviation_x_SpinBox_3->value(), ui->deviation_y_SpinBox_3->value(), ui->deviation_z_SpinBox_3->value());
}

//保存三个校准位偏差
void DJ1Widget::on_btnSava_3_clicked()
{
    switch (ui->comboBox->currentIndex())
    {
    case 0:
        //保存转运校准位偏差
        saveTransportPositionDeviation();
        break;
    case 1:
        //保存B架开盖位偏差
        saveBOpenPositionDeviation();
        break;
    case 2:
        //保存B架关盖位偏差
        saveBClosePositionDeviation();
        break;
    default:
        break;
    }
}

//显示3个校准位的机位偏差
void DJ1Widget::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        //转运校准位偏差
        ui->deviation_x_SpinBox_3->setValue(m_transportInfo.deviation_x);
        ui->deviation_y_SpinBox_3->setValue(m_transportInfo.deviation_y);
        ui->deviation_z_SpinBox_3->setValue(m_transportInfo.deviation_z);
        break;
    case 1:
        //B架开盖位偏差
        ui->deviation_x_SpinBox_3->setValue(m_bOpenInfo.deviation_x);
        ui->deviation_y_SpinBox_3->setValue(m_bOpenInfo.deviation_y);
        ui->deviation_z_SpinBox_3->setValue(m_bOpenInfo.deviation_z);
        break;
    case 2:
        //B架关盖位偏差
        ui->deviation_x_SpinBox_3->setValue(m_bCloseInfo.deviation_x);
        ui->deviation_y_SpinBox_3->setValue(m_bCloseInfo.deviation_y);
        ui->deviation_z_SpinBox_3->setValue(m_bCloseInfo.deviation_z);
        break;
    default:
        break;
    }
}

//DJ1模块复位
void DJ1Widget::on_pushButton_clicked()
{
    Amplification_Dj1::instance()->module_reset();
}

//X轴电机复位
void DJ1Widget::on_pushButton_2_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_X);
}

//Y轴电机复位
void DJ1Widget::on_pushButton_3_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_Y);
}

//试剂针Z轴电机复位
void DJ1Widget::on_pushButton_4_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_DJ1_Z);
}

//夹爪Z复位
void DJ1Widget::on_pushButton_25_clicked()
{
    //移液器夹爪Z复位
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_CLAW_Z);
}
