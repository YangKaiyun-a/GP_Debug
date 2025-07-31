#ifndef PCRLIGHTACCURACYWIDGET_H
#define PCRLIGHTACCURACYWIDGET_H
#include "Function/data/global.h"
#include "temperaturewidget.h"

#include <QWidget>
#include <QTimer>
#include <QAbstractButton>
#include <QProcess>

namespace Ui {
class PCRLightAccuracyWidget;
}

class PCRLightAccuracyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PCRLightAccuracyWidget(ENUM_CLASS::PCR_Id id, QWidget *parent = nullptr);
    ~PCRLightAccuracyWidget();
    void init();
    void initConnect();
    void initStyleSheet();
    void initScrollBar();
    void initTemperatureWidget();
    //托盘推出
    bool blockMoveOut();
    //通道复位
    bool channelReset(bool isUsedForSwitchChannel);
    //切换通道
    bool channelSwitch();
    //处理拍照完成后的逻辑
    void handlePhotoCapture();
    //获取测试底图
    bool getTestPhoto(const QString &path);
    //启动分析软件
    bool startAnalyseSoft(const QString &path);
    //获取方案
    void getScheme();
    //下发方案
    bool setScheme();
    //执行方案
    bool runScheme();
    void showTemperatureWidget();
    //获取校准矩阵
    bool getAdjustMatrix();
    //保存第一通道位置参数
    bool saveFirstChannelPosition(const ChannelParams &params);
    //保存曝光时间
    bool saveChannelExposureTime(const ChannelParams &params);
    //保存浓度系数
    bool saveChannelPotency(const ChannelParams &params);
    //发送sample参数
    bool setSampleParams();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onHandleFinished(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    void onStatusChanged_2(bool enableSample);
    void onHandleSetParams(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    void onButtonClicked(QAbstractButton *button);
    void on_pushButton_2_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_32_clicked();

private:
    Ui::PCRLightAccuracyWidget *ui;
    QTimer *m_timer = nullptr;
    ENUM_CLASS::PCR_Id m_id;
    ENUM_CLASS::Light_Channel m_currentChannel;
    ENUM_CLASS::Light_Channel m_channelOnClose;
    QString m_currentPcrConfig;
    QProcess *m_process = nullptr;
    TemperatureWidget *m_temperatureWidget = nullptr;
    QString m_currentSchemeName = "";

    int m_setFirstChannelStatus = ENUM_CLASS::SetParams_Status::Uninit;
    int m_setExposureTimeStatus = ENUM_CLASS::SetParams_Status::Uninit;
    int m_setPotencyStatus = ENUM_CLASS::SetParams_Status::Uninit;
};

#endif // PCRLIGHTACCURACYWIDGET_H
