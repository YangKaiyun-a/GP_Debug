#include "qpcrwidget.h"
#include "ui_qpcrwidget.h"
#include "Function/data/parsemessageinfo.h"
#include "Function/data/dbmanager.h"
#include "Controller/sportmethods.h"
#include "application.h"
#include "Controller/xzclaw/xzclaw_control.h"
#include "hcscitoolkits/dialog/messagedialog.h"

#include <QDebug>

QpcrWidget::QpcrWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QpcrWidget)
{
    ui->setupUi(this);
    init();
}

QpcrWidget::~QpcrWidget()
{
    delete ui;
}

void QpcrWidget::init()
{
    initStyleSheet();
    initData();
}

void QpcrWidget::initStyleSheet()
{
    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");
    ui->widget_9->setObjectName("stepsWidget");
    ui->widget_10->setObjectName("stepsWidget");

    ui->label_31->setObjectName("labModuleAction");
    ui->label_32->setObjectName("labModuleAction");
}

void QpcrWidget::initData()
{
    m_rackVector.clear();

    m_Transport_Pcr_3_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(3, "PCR板转运机位");
    m_QPcr_1_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(3, "QPCR模块1机位");
    m_QPcr_2_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(3, "QPCR模块2机位");
    m_Pcr_Board_Out_Rack = ParseMessageInfo::getRackIdByTableIdAndRackName(3, "PCR板出口机位");

    m_rackVector << m_Transport_Pcr_3_Rack \
                 << m_QPcr_1_Rack \
                 << m_QPcr_2_Rack \
                 << m_Pcr_Board_Out_Rack;

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

        m_storageInfoMap.insert(info.rack_id, info);
    }
}

void QpcrWidget::initUI(const Storage_Info &info)
{
    ui->deviation_x_SpinBox->setValue(info.deviation_x);
    ui->deviation_z_SpinBox->setValue(info.deviation_z);
}

void QpcrWidget::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        initUI(m_storageInfoMap.value(m_Transport_Pcr_3_Rack));
        break;
    case 1:
        initUI(m_storageInfoMap.value(m_QPcr_1_Rack));
        break;
    case 2:
        initUI(m_storageInfoMap.value(m_QPcr_2_Rack));
        break;
    case 3:
        initUI(m_storageInfoMap.value(m_Pcr_Board_Out_Rack));
        break;
    default:
        break;
    }
}

//运动至调整后的校准位
void QpcrWidget::on_pushButton_17_clicked()
{
    int rackId = -1;

    switch (ui->comboBox->currentIndex())
    {
    case 0:
        rackId = m_Transport_Pcr_3_Rack;
        break;
    case 1:
        rackId = m_QPcr_1_Rack;
        break;
    case 2:
        rackId = m_QPcr_2_Rack;
        break;
    case 3:
        rackId = m_Pcr_Board_Out_Rack;
        break;
    default:
        break;
    }

    SportMethods::instance()->moveToQPcrAccuratingPosition(m_storageInfoMap.value(rackId), ui->deviation_x_SpinBox->value(), ui->deviation_z_SpinBox->value());
}

//保存机位偏差
void QpcrWidget::on_btnSava_clicked()
{
    int rackId = -1;

    switch (ui->comboBox->currentIndex())
    {
    case 0:
        rackId = m_Transport_Pcr_3_Rack;
        break;
    case 1:
        rackId = m_QPcr_1_Rack;
        break;
    case 2:
        rackId = m_QPcr_2_Rack;
        break;
    case 3:
        rackId = m_Pcr_Board_Out_Rack;
        break;
    default:
        break;
    }

    Storage_Info storageInfo = m_storageInfoMap.value(rackId);

    storageInfo.deviation_x = ui->deviation_x_SpinBox->value();
    storageInfo.deviation_z = ui->deviation_z_SpinBox->value();

    if(!DBManager::instance()->setDeviationValue(storageInfo))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_storageInfoMap[rackId] = storageInfo;

        //保存成功后更新DP8Widget类中的坐标，因修正移液器偏差和校准夹爪需要
        Q_EMIT hApp->m_sigmanager->updateRackDeviation(rackId);
    }
}

//模块复位
void QpcrWidget::on_pushButton_13_clicked()
{
    xzclaw_control::instance()->module_reset();
}

//夹爪张开
void QpcrWidget::on_pushButton_14_clicked()
{
    xzclaw_control::instance()->claw_glab(ui->Claw_Width_SBox->value());
}

//夹爪复位
void QpcrWidget::on_pushButton_15_clicked()
{
    xzclaw_control::instance()->claw_reset();
}

//打开物流3取板门
void QpcrWidget::on_pushButton_16_clicked()
{
    xzclaw_control::instance()->M1_Open_entry();
}

//关闭物流3取板门
void QpcrWidget::on_pushButton_18_clicked()
{
    xzclaw_control::instance()->M1_Close_entry();
}

//打开物流3废板门
void QpcrWidget::on_pushButton_19_clicked()
{
    xzclaw_control::instance()->M2_Open_entry();
}

//关闭物流3废板门
void QpcrWidget::on_pushButton_20_clicked()
{
    xzclaw_control::instance()->M2_Close_entry();
}

//Z轴复位
void QpcrWidget::on_pushButton_21_clicked()
{
    xzclaw_control::instance()->motorReset(xzclaw_control::Z_comboBox);
}

