#include "temperaturewidget.h"
#include "Function/data/global.h"
#include "Function/data/dbmanager.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>

TemperatureWidget::TemperatureWidget(ENUM_CLASS::PCR_Id id, QWidget *parent)
    : QWidget{parent}, m_id(id)
{
    init();
}

void TemperatureWidget::init()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    topLayout->addStretch();

    QPushButton *btn2 = new QPushButton();
    btn2->setFixedSize(75, 50);
    btn2->setText("读取温度");
    topLayout->addWidget(btn2);
    connect(btn2, &QPushButton::clicked, [=](){
        startGetTemperature();
    });

    QPushButton *btn = new QPushButton();
    btn->setFixedSize(75, 50);
    btn->setText("清空曲线");
    topLayout->addWidget(btn);
    connect(btn, &QPushButton::clicked, [=](){
        reset();
    });

    QPushButton *btn3 = new QPushButton();
    btn3->setFixedSize(75, 50);
    btn3->setText("关闭");
    topLayout->addWidget(btn3);
    connect(btn3, &QPushButton::clicked, [=](){
        this->hide();
    });

    if(!m_wgt)
    {
        m_wgt = new FluorescenceCurve();
    }

    if(!m_timer)
    {
        m_timer = new QTimer(this);
    }

    bottomLayout->addWidget(m_wgt);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
}

void TemperatureWidget::startGetTemperature()
{
    if(!m_timer)
    {
        return;
    }

    m_conn = connect(m_timer, &QTimer::timeout, this, [=](){

        RealTimeTemperature temp = DBManager::instance()->getRealTimeTemperature(m_id);
        if(!temp.update_Time.isNull() && m_lastDateTime != temp.update_Time)
        {
            m_lastDateTime = temp.update_Time;

            //临时打印出温度
            qDebug() << "block1: " << temp.block1 << ", block2: " << temp.block2 << ", block3: " << temp.block3 <<  ", heatCap: " << temp.heatCap;

            m_wgt->appendLineData(temp);
        }
    });

    m_timer->start(1000);
}

void TemperatureWidget::reset()
{
    if(!m_timer || !m_wgt)
    {
        return;
    }

    m_timer->stop();
    disconnect(m_conn);
    m_wgt->resetChart();
}
