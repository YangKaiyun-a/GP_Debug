#include "logwidget.h"
#include "ui_logwidget.h"

#include "application.h"

LogWidget::LogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogWidget)
{
    ui->setupUi(this);
    init();
}

LogWidget::~LogWidget()
{
    delete ui;
}

void LogWidget::init()
{
    ui->textEdit->setStyleSheet("QTextEdit { color: black; }");

    connect(hApp->m_sigmanager, &SignalManager::showLog, this, &LogWidget::onShowLog);
}

void LogWidget::onShowLog(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, const LaserInfo &laserInfo, float deviationX, float deviationY, float deviationZ, float x, float y, float z, float resetZ0, const SingleChannelInfo &singleInfo, const QString &str)
{
    if(!ui->textEdit->toPlainText().isEmpty())
    {
        ui->textEdit->append("");
        ui->textEdit->append("");
        ui->textEdit->append("");
    }

    ui->textEdit->append(QString("机位名称：        %1").arg(storageInfo.rack_name));
    ui->textEdit->append(QString("校准点坐标：           (%1,   %2,   %3)").arg(storageInfo.loc_x).arg(storageInfo.loc_y).arg(storageInfo.loc_z));
    ui->textEdit->append(QString("机位偏差：           (%1,   %2,   %3)").arg(storageInfo.deviation_x).arg(storageInfo.deviation_y).arg(storageInfo.deviation_z));
    ui->textEdit->append(QString("标记偏差：           (%1,   %2,   %3)").arg(storageInfo.crossing_x).arg(storageInfo.crossing_y).arg(storageInfo.crossing_z));
    ui->textEdit->append(QString("移液器偏差：         (%1,   %2,   %3)").arg(pipetteInfo.deviate_x).arg(pipetteInfo.deviate_y).arg(pipetteInfo.deviate_z));
    ui->textEdit->append(QString("移液器复位位置:      (%1,   %2,   %3)").arg(pipetteInfo.distance_x_from_orig).arg(pipetteInfo.distance_y_from_orig).arg(pipetteInfo.distance_z_from_orig));
    ui->textEdit->append(QString("单通道坐标:          (%1,   %2,   %3)").arg(singleInfo.loc_x).arg(singleInfo.loc_y).arg(singleInfo.loc_z));
    ui->textEdit->append(QString("夹爪坐标:           (%1,   %2,   %3)").arg(clawInfo.loc_x).arg(clawInfo.loc_y).arg(clawInfo.loc_z));
    ui->textEdit->append(QString("夹爪相对位置终值:    (%1,   %2,   %3)").arg(clawInfo.distance_end_x).arg(clawInfo.distance_end_y).arg(clawInfo.distance_end_z));
    ui->textEdit->append(QString("激光坐标:            (%1,   %2,   %3)").arg(laserInfo.loc_x).arg(laserInfo.loc_y).arg(laserInfo.loc_z));
    ui->textEdit->append(QString("激光复位Z0（全局）:   (%1)").arg(resetZ0));
    ui->textEdit->append(QString("输入偏差：           (%1,   %2,   %3)").arg(deviationX).arg(deviationY).arg(deviationZ));
    ui->textEdit->append(QString("运动到：             (%1,   %2,   %3)").arg(x).arg(y).arg(z));
    ui->textEdit->append("");
    ui->textEdit->append(str);
}

//返回
void LogWidget::on_btnBack_clicked()
{
    this->hide();
}

//清除
void LogWidget::on_btnClear_clicked()
{
    ui->textEdit->clear();
}

