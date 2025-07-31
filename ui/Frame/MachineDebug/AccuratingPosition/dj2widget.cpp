#include "dj2widget.h"
#include "ui_dj2widget.h"
#include "Function/data/dbmanager.h"
#include "Controller/sportmethods.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "Controller/Extract_Dj1/extract_dj1.h"
#include "Function/data/parsemessageinfo.h"
#include "application.h"

#include <QDebug>

DJ2Widget::DJ2Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DJ2Widget)
{
    ui->setupUi(this);
    init();
}

DJ2Widget::~DJ2Widget()
{
    delete ui;
}

void DJ2Widget::init()
{
    initStyleSheet();
    initData();
    initUI();
    initConnect();
}

void DJ2Widget::initStyleSheet()
{
    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");

    ui->label_31->setObjectName("labModuleAction");
}

void DJ2Widget::initData()
{
    m_rackVector.clear();

    //目前只含有磁珠振荡机位
    m_Magnetic_Beads_Shake_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "磁珠振荡机位");
    m_rackVector << m_Magnetic_Beads_Shake_Rack;

    //获取DJ2移液器信息
    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(ENUM_CLASS::Gantry::Gantry_DJ2);

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

        //获取移液器校准点（磁珠振荡机位及其载架）
        if(id == m_Magnetic_Beads_Shake_Rack)
        {
            m_storageInfo = info;
        }
    }
}

void DJ2Widget::initUI()
{
    ui->deviation_x_SpinBox->setValue(m_pipetteInfo.deviate_x);
    ui->deviation_y_SpinBox->setValue(m_pipetteInfo.deviate_y);
    ui->deviation_z_SpinBox->setValue(m_pipetteInfo.deviate_z);
}

void DJ2Widget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updateRackDeviation, this, &DJ2Widget::onUpdateRackDeviation);
}

void DJ2Widget::onUpdateRackDeviation(int rackId)
{
    if(rackId != m_storageInfo.rack_id)
    {
        return;
    }

    DBManager::instance()->getRackDevationById(m_storageInfo);
}

//运动至移液器偏差校准位   -磁珠振荡位
void DJ2Widget::on_pushButton_17_clicked()
{
    SportMethods::instance()->moveToPipetteAccuratingPositionDJ2(m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), ui->deviation_z_SpinBox->value());
}

//保存移液器偏差
void DJ2Widget::on_btnSava_clicked()
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
    }
}

//DJ2模块复位
void DJ2Widget::on_pushButton_clicked()
{
    Extract_DJ1::instance()->module_reset();
}

//X轴电机复位
void DJ2Widget::on_pushButton_2_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_X);
}

//Y轴电机复位
void DJ2Widget::on_pushButton_3_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_Y);
}

//试剂针Z轴电机复位
void DJ2Widget::on_pushButton_4_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_Z);
}

