#include "calibrationwidget.h"
#include "ui_calibrationwidget.h"
#include "Function/Data/parsemessageinfo.h"

CalibrationWidget::CalibrationWidget(ENUM_CLASS::Light_Channel channel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationWidget), m_channel(channel)
{
    ui->setupUi(this);
    init();
}

CalibrationWidget::~CalibrationWidget()
{
    delete ui;
}

void CalibrationWidget::init()
{
    setLabMessage(m_channel);
}

void CalibrationWidget::setLabMessage(ENUM_CLASS::Light_Channel channel)
{
    ui->label->setText("请放置" + ParseMessageInfo::enumToString(channel) + "校准板");
    m_channel = channel;
}

void CalibrationWidget::setResultMessage(const QString &msg)
{
    ui->label_2->setText(msg);
}

