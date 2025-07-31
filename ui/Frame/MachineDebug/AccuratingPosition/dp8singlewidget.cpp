#include "dp8singlewidget.h"
#include "ui_dp8singlewidget.h"
#include "Function/data/dbmanager.h"
#include "Controller/sportmethods.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "application.h"
#include "Function/data/parsemessageinfo.h"

#include <QDebug>

DP8SingleWidget::DP8SingleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP8SingleWidget)
{
    ui->setupUi(this);
    init();
}

DP8SingleWidget::~DP8SingleWidget()
{
    delete ui;
}

void DP8SingleWidget::init()
{
    initStyleSheet();
    initData();
    initUI();
    initConnect();
}

void DP8SingleWidget::initStyleSheet()
{
    ui->label_33->setObjectName("labModuleAction");

    ui->widget_12->setObjectName("stepsWidget");
    ui->widget_13->setObjectName("stepsWidget");
}

void DP8SingleWidget::initData()
{
    //获取DP8移液器信息
    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(ENUM_CLASS::Gantry::Gantry_DP8);

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

    //只有一个校准位-F2机位
    m_F2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "F2机位");
    m_storageInfo.rack_id = m_F2_Rack;

    //获取该机位的载架、标记偏差
    ParseMessageInfo::getCrossingDeviatio(m_storageInfo);

    //根据查询数据库来组合出最终位置坐标
    DBManager::instance()->getStorageInfoByCarrierId(m_storageInfo);

    if(m_storageInfo.loc_x == 0 && m_storageInfo.loc_y == 0 && m_storageInfo.loc_z == 0)
    {
        qDebug() << "rack_id = " << m_storageInfo.rack_id << "，rack_name = " << m_storageInfo.rack_name << "的机位没有用到任何载架，或者最终坐标为（0，0，0）";
    }

    //添加所属移液器id
    m_storageInfo.pipette_id = m_pipetteInfo.module_id;
}

void DP8SingleWidget::initUI()
{
    ui->deviation_x_SpinBox_3->setValue(m_singleInfo.deviation_x);
    ui->deviation_y_SpinBox_3->setValue(m_singleInfo.deviation_y);
    ui->deviation_z_SpinBox_3->setValue(m_singleInfo.deviation_z);
}

void DP8SingleWidget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updatePipetteDeviation, this, &DP8SingleWidget::onUpdatePipetteDeviation);
}

//重新读取移液器偏差
void DP8SingleWidget::onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry)
{
    if(gantry != m_pipetteInfo.module_id)
    {
        return;
    }

    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(m_pipetteInfo.module_id);
}

//运动至调整后的校准位
void DP8SingleWidget::on_pushButton_34_clicked()
{
    SportMethods::instance()->moveToSingleChannelAccuratingPosition(m_storageInfo, m_pipetteInfo, m_singleInfo, ui->deviation_x_SpinBox_3->value(), ui->deviation_y_SpinBox_3->value(), ui->deviation_z_SpinBox_3->value());
}

//保存单通道偏差
void DP8SingleWidget::on_btnSava_3_clicked()
{
    SingleChannelInfo singleInfo = m_singleInfo;

    //更新偏差
    singleInfo.deviation_x = ui->deviation_x_SpinBox_3->value();
    singleInfo.deviation_y = ui->deviation_y_SpinBox_3->value();
    singleInfo.deviation_z = ui->deviation_z_SpinBox_3->value();

    //更新单通道和葫芦头相对位置（原始相对位置 - 显示值）
    singleInfo.distance_end_x = singleInfo.distance_x - singleInfo.deviation_x;
    singleInfo.distance_end_y = singleInfo.distance_y - singleInfo.deviation_y;
    singleInfo.distance_end_z = singleInfo.distance_z - singleInfo.deviation_z;

    //更新单通道位置
    singleInfo.loc_x = m_pipetteInfo.distance_x_from_orig + singleInfo.distance_x;
    singleInfo.loc_y = m_pipetteInfo.distance_y_from_orig + singleInfo.distance_y;
    singleInfo.loc_z = m_pipetteInfo.distance_z_from_orig + singleInfo.distance_z;

    //只保存偏差
    if(!DBManager::instance()->setSingleDeviationValue(singleInfo))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_singleInfo = singleInfo;
    }
}

