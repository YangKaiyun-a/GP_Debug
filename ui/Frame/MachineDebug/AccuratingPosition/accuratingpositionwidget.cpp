#include "accuratingpositionwidget.h"
#include "ui_accuratingpositionwidget.h"
#include "dj1widget.h"
#include "dj2widget.h"
#include "dp1widget.h"
#include "dp8widget.h"
#include "logisticswidget.h"
#include "dp8singlewidget.h"
#include "qpcrwidget.h"

#include <QButtonGroup>

AccuratingPositionWidget::AccuratingPositionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AccuratingPositionWidget)
{
    ui->setupUi(this);
    init();
}

AccuratingPositionWidget::~AccuratingPositionWidget()
{
    delete ui;
}

void AccuratingPositionWidget::init()
{
    //复用PCR模块的按钮样式
    ui->btnDJ1->setObjectName("btnPCRModuleFuction");
    ui->btnDJ2->setObjectName("btnPCRModuleFuction");
    ui->btnDP1->setObjectName("btnPCRModuleFuction");
    ui->btnDP8->setObjectName("btnPCRModuleFuction");
    ui->btnDP8_2->setObjectName("btnPCRModuleFuction");
    ui->btnLogistics->setObjectName("btnPCRModuleFuction");
    ui->btnQPCR->setObjectName("btnPCRModuleFuction");

    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->btnDJ1);
    buttonGround->addButton(ui->btnDJ2);
    buttonGround->addButton(ui->btnDP1);
    buttonGround->addButton(ui->btnDP8);
    buttonGround->addButton(ui->btnDP8_2);
    buttonGround->addButton(ui->btnLogistics);
    buttonGround->addButton(ui->btnQPCR);
    buttonGround->setExclusive(true);

    initStackedWidget();

    if(!m_logWidget)
    {
        m_logWidget = new LogWidget(this);
    }

    m_logWidget->hide();
}

void AccuratingPositionWidget::initStackedWidget()
{
    DJ1Widget *dj1 = new DJ1Widget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DJ1, dj1);

    DJ2Widget *dj2 = new DJ2Widget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DJ2, dj2);

    DP1Widget *dp1 = new DP1Widget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DP1, dp1);

    DP8Widget *dp8 = new DP8Widget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DP8, dp8);

    LogisticsWidget *logic = new LogisticsWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::Logistics, logic);

    DP8SingleWidget *single = new DP8SingleWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DP8_Single, single);

    QpcrWidget *qpcr = new QpcrWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::QPCR, qpcr);

    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DJ1);
}

void AccuratingPositionWidget::showLogWidget()
{
    QSize mainWidgetSize = this->size();
    QSize logWidgetSize(mainWidgetSize.width() * 0.85, mainWidgetSize.height() * 0.85);
    m_logWidget->resize(logWidgetSize);

    //获取中心点
    QPoint mainWidgetCenter = this->rect().center();

    //计算左上角位置
    QPoint logWidgetTopLeft(mainWidgetCenter.x() - logWidgetSize.width() / 2, mainWidgetCenter.y() - logWidgetSize.height() / 2);
    m_logWidget->move(logWidgetTopLeft);

    m_logWidget->show();
}

void AccuratingPositionWidget::on_btnDJ1_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DJ1);
}

void AccuratingPositionWidget::on_btnDJ2_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DJ2);
}

void AccuratingPositionWidget::on_btnDP1_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DP1);
}

void AccuratingPositionWidget::on_btnDP8_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DP8);
}

void AccuratingPositionWidget::on_btnLogistics_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::Logistics);
}

void AccuratingPositionWidget::on_btnDP8_2_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DP8_Single);
}

void AccuratingPositionWidget::on_btnQPCR_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::QPCR);
}

//运动日志
void AccuratingPositionWidget::on_btnLog_clicked()
{
    if(!m_logWidget)
    {
        return;
    }

    if(!m_logWidget->isVisible())
    {
        showLogWidget();
    }
    else if(m_logWidget->isVisible())
    {
        m_logWidget->hide();
    }
}

