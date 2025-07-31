#include "coldeperformancewidget.h"
#include "ui_coldeperformancewidget.h"
#include "Controller/CoolStoreModule/coolstorepart.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "hcscitoolkits/dialog/optiondialog.h"
#include "Controller/Amplification_Cool/amplification_cool.h"

#include <QDebug>

ColdePerformanceWidget::ColdePerformanceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColdePerformanceWidget)
{
    ui->setupUi(this);
    init();
}

ColdePerformanceWidget::~ColdePerformanceWidget()
{
    delete ui;
}

void ColdePerformanceWidget::init()
{
    initStyleSheet();
    initConnect();

    //注册类型
    qRegisterMetaType<QList<float> > ("QList<float>");

    m_mapConnection.clear();
}

void ColdePerformanceWidget::initStyleSheet()
{
    ui->widget_9->setObjectName("stepsWidget");
    ui->widget_10->setObjectName("stepsWidget");
    ui->widget_11->setObjectName("stepsWidget");
    ui->widget_12->setObjectName("stepsWidget");
}

void ColdePerformanceWidget::initConnect()
{
    connect(ui->swControlDuct_10, &SwitchButton::statusChanged, this, &ColdePerformanceWidget::onStatusChanged);
    connect(ui->swControlDuct_11, &SwitchButton::statusChanged, this, &ColdePerformanceWidget::onStatusChanged_11);
    connect(ui->swControlDuct_12, &SwitchButton::statusChanged, this, &ColdePerformanceWidget::onStatusChanged_12);
    connect(ui->swControlDuct_13, &SwitchButton::statusChanged, this, &ColdePerformanceWidget::onStatusChanged_13);
}


/*************************************************核酸暂存位-左*************************************************/

//核酸暂存位-左-模块复位
void ColdePerformanceWidget::on_pushButton_40_clicked()
{
    CoolStorePart::instance()->moduleReset();
}

//核酸暂存位制-左-开盖
void ColdePerformanceWidget::on_pushButton_29_clicked()
{
    CoolStorePart::instance()->openLeftCover();
}

//核酸暂存位制-左-关盖
void ColdePerformanceWidget::on_pushButton_30_clicked()
{
    CoolStorePart::instance()->closeLeftCover();
}

//核酸暂存位制-左-开始制冷
void ColdePerformanceWidget::onStatusChanged(bool startCold)
{
    if(startCold)
    {
        //开始制冷
        CoolStorePart::instance()->startLeftCool();
    }
    else
    {
        //停止制冷
        CoolStorePart::instance()->stopLeftCool();
    }
}

//核酸暂存位制-左-获取参数
void ColdePerformanceWidget::on_pushButton_39_clicked()
{
    CoolStorePart::CoolStoreParams param;
    CoolStorePart::instance()->getParameter(param);

    ui->Left_Temp_SBox->setValue(param.left_target_temperature);
    ui->Left_K_SBox->setValue(param.left_k);
    ui->Left_B_SBox->setValue(param.left_b);
    ui->Left_Cover_Position_SBox->setValue(param.left_cover_closePosition);
}

//保存核酸暂存位-左-保存参数
void ColdePerformanceWidget::on_pushButton_37_clicked()
{
    CoolStorePart::CoolStoreParams param;
    param.left_k = ui->Left_K_SBox->value();
    param.left_b = ui->Left_B_SBox->value();
    param.left_cover_closePosition = ui->Left_Cover_Position_SBox->value();
    param.left_target_temperature = ui->Left_Temp_SBox->value();

    CoolStorePart::instance()->setLeftParameter(param);
}

//核酸暂存位制-左-读取温度
void ColdePerformanceWidget::on_pushButton_51_clicked()
{
    if(!m_isGetTemp)
    {
        m_isGetTemp = true;
        QMetaObject::invokeMethod(CoolStorePart::instance(), "readTemp");
    }

    QMetaObject::Connection conn = connect(CoolStorePart::instance(), &CoolStorePart::sigCurrentTemp, this, [=](QList<float> temps){
        ui->PCR_Current_Temp_SBox_1->setValue(temps.at(0));
    });

    ConnectionStatus status;
    status.conn = conn;
    status.isConnected = true;
    m_mapConnection.insert(0, status);    //0：左，1：右，2：体系
}

//核酸暂存位制-左-停止读取
void ColdePerformanceWidget::on_pushButton_52_clicked()
{
    //判断此时是否有连接
    int count = 0;

    for(int i = 1; i < 3; ++i)
    {
        if(m_mapConnection[i].isConnected)
        {
            ++count;
        }
    }

    if(count > 1)
    {
        disconnect(m_mapConnection[0].conn);
        m_mapConnection[0].isConnected = false;;
        ui->PCR_Current_Temp_SBox_1->setValue(0);
    }
    else
    {
        //若所有连接都已断开，则关闭读取温度
        CoolStorePart::instance()->stopReadTemp();
        m_isGetTemp = false;
    }
}


/*************************************************核酸暂存位-右*************************************************/


//核酸暂存位制-右-模块复位
void ColdePerformanceWidget::on_pushButton_42_clicked()
{
    CoolStorePart::instance()->moduleReset();
}

//核酸暂存位制-右-开盖
void ColdePerformanceWidget::on_pushButton_31_clicked()
{
    CoolStorePart::instance()->openRightCover();
}

//核酸暂存位制-右-关盖
void ColdePerformanceWidget::on_pushButton_32_clicked()
{
    CoolStorePart::instance()->closeRightCover();
}

//核酸暂存位制-右-开始制冷
void ColdePerformanceWidget::onStatusChanged_11(bool startCold)
{
    if(startCold)
    {
        //开始制冷
        CoolStorePart::instance()->startRightCool();
    }
    else
    {
        //停止制冷
        CoolStorePart::instance()->stopRightCool();
    }
}

//核酸暂存位制-右-获取参数
void ColdePerformanceWidget::on_pushButton_41_clicked()
{
    CoolStorePart::CoolStoreParams param;
    CoolStorePart::instance()->getParameter(param);

    ui->Right_Temp_SBox->setValue(param.right_target_temperature);
    ui->Right_K_SBox->setValue(param.right_k);
    ui->Right_B_SBox->setValue(param.right_b);
    ui->Right_Cover_Position_SBox->setValue(param.right_cover_closePosition);
}

//核酸暂存位制-右-保存参数
void ColdePerformanceWidget::on_pushButton_38_clicked()
{
    CoolStorePart::CoolStoreParams param;
    param.right_k = ui->Right_K_SBox->value();
    param.right_b = ui->Right_B_SBox->value();
    param.right_cover_closePosition = ui->Right_Cover_Position_SBox->value();
    param.right_target_temperature = ui->Right_Temp_SBox->value();

    CoolStorePart::instance()->setRightParameter(param);
}

//核酸暂存位制-右-读取温度
void ColdePerformanceWidget::on_pushButton_54_clicked()
{
    if(!m_isGetTemp)
    {
        m_isGetTemp = true;
        QMetaObject::invokeMethod(CoolStorePart::instance(), "readTemp");
    }

    QMetaObject::Connection conn = connect(CoolStorePart::instance(), &CoolStorePart::sigCurrentTemp, this, [=](QList<float> temps){
        ui->PCR_Current_Temp_SBox_2->setValue(temps.at(1));
    });

    ConnectionStatus status;
    status.conn = conn;
    status.isConnected = true;
    m_mapConnection.insert(1, status);    //0：左，1：右，2：体系
}

//核酸暂存位制-右-停止读取
void ColdePerformanceWidget::on_pushButton_53_clicked()
{
    //判断此时是否有连接
    int count = 0;

    for(int i = 1; i < 3; ++i)
    {
        if(m_mapConnection[i].isConnected)
        {
            ++count;
        }
    }

    if(count > 1)
    {
        disconnect(m_mapConnection[1].conn);
        m_mapConnection[1].isConnected = false;
        ui->PCR_Current_Temp_SBox_2->setValue(0);
    }
    else
    {
        //若所有连接都已断开，则关闭读取温度
        CoolStorePart::instance()->stopReadTemp();
        m_isGetTemp = false;
    }
}

/*************************************************体系构建位*************************************************/


//体系构建位-开始制冷
void ColdePerformanceWidget::onStatusChanged_12(bool startCold)
{
    if(startCold)
    {
        //开始制冷
        CoolStorePart::instance()->startSysbuildCool();
    }
    else
    {
        //停止制冷
        CoolStorePart::instance()->stopSysbuildCool();
    }
}

//体系构建位-读取温度
void ColdePerformanceWidget::on_pushButton_45_clicked()
{
    if(!m_isGetTemp)
    {
        m_isGetTemp = true;
        QMetaObject::invokeMethod(CoolStorePart::instance(), "readTemp");
    }

    QMetaObject::Connection conn = connect(CoolStorePart::instance(), &CoolStorePart::sigCurrentTemp, this, [=](QList<float> temps){
        ui->Sysbuild_Current_Temp_SBox->setValue(temps.at(2));
    });

    ConnectionStatus status;
    status.conn = conn;
    status.isConnected = true;
    m_mapConnection.insert(2, status);    //0：左，1：右，2：体系
}

//体系构建位-停止读取
void ColdePerformanceWidget::on_pushButton_46_clicked()
{
    //判断此时是否有连接
    int count = 0;

    for(int i = 0; i < 3; ++i)
    {
        if(m_mapConnection[i].isConnected)
        {
            ++count;
        }
    }

    if(count > 1)
    {
        disconnect(m_mapConnection[2].conn);
        m_mapConnection[2].isConnected = false;
        ui->Sysbuild_Current_Temp_SBox->setValue(0);
    }
    else
    {

        //若所有连接都已断开，则关闭读取温度
        CoolStorePart::instance()->stopReadTemp();
        m_isGetTemp = false;
    }
}

//体系构建位-获取参数
void ColdePerformanceWidget::on_pushButton_43_clicked()
{
    CoolStorePart::CoolStoreParams param;
    CoolStorePart::instance()->getParameter(param);

    ui->Sysbuiid_K_SBox->setValue(param.sysbuild_k);
    ui->Sysbuild_B_SBox->setValue(param.sysbuild_b);
    ui->Sysbuild_Temp_SBox->setValue(param.sysbuilding_target_temperature);
}

//体系构建位-保存参数
void ColdePerformanceWidget::on_pushButton_44_clicked()
{
    CoolStorePart::CoolStoreParams param;
    param.sysbuild_k = ui->Sysbuiid_K_SBox->value();
    param.sysbuild_b = ui->Sysbuild_B_SBox->value();
    param.sysbuilding_target_temperature = ui->Sysbuild_Temp_SBox->value();

    CoolStorePart::instance()->setSysbuildParameter(param);
}


/*************************************************扩增制冷位*************************************************/

//扩增开始制冷
void ColdePerformanceWidget::onStatusChanged_13(bool startCold)
{
    if(startCold)
    {
        //开始制冷
        Amplification_Cool::instance()->startCool();
    }
    else
    {
        //停止制冷
        Amplification_Cool::instance()->stopCool();
    }
}

//扩增制冷位-读取温度
void ColdePerformanceWidget::on_pushButton_49_clicked()
{
    QMetaObject::invokeMethod(Amplification_Cool::instance(), "readTemp");

    connect(Amplification_Cool::instance(), &Amplification_Cool::sigCurrentTemp, this, [=](QList<float> temps){
        ui->Add_Current_Temp_SBox_2->setValue(temps.at(0));
    });
}

//扩增制冷位-停止读取
void ColdePerformanceWidget::on_pushButton_50_clicked()
{
    Amplification_Cool::instance()->stopReadTemp();
}

//扩增制冷位-获取参数
void ColdePerformanceWidget::on_pushButton_47_clicked()
{
    Amplification_Cool::CoolStoreParams param;
    Amplification_Cool::instance()->getParameter(param);

    ui->Add_K_SBox->setValue(param.left_k);
    ui->Add_B_SBox->setValue(param.left_b);
    ui->Add_Current_Temp_SBox->setValue(param.left_target_temperature);
}

//扩增制冷位-保存参数
void ColdePerformanceWidget::on_pushButton_48_clicked()
{
    Amplification_Cool::CoolStoreParams params;
    params.left_target_temperature = ui->Add_Current_Temp_SBox->value();
    params.left_k = ui->Add_K_SBox->value();
    params.left_b = ui->Add_B_SBox->value();

    Amplification_Cool::instance()->setParameter(params);
}

