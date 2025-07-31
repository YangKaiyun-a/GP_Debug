#ifndef PCRTEMPERATUREACCURACYWIDGET_H
#define PCRTEMPERATUREACCURACYWIDGET_H
#include "Function/data/global.h"
#include "temperaturewidget.h"

#include <QWidget>
#include <QTimer>

namespace Ui {
class PCRTemperatureAccuracyWidget;
}

class PCRTemperatureAccuracyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PCRTemperatureAccuracyWidget(ENUM_CLASS::PCR_Id id, QWidget *parent = nullptr);
    ~PCRTemperatureAccuracyWidget();
    void init();
    void initStyleSheet();
    void initScrollBar();
    void initConnect();
    void initTableWidget();
    void initTemperatureWidget();

    //发送sample参数
    bool setSampleParams();
    //下发方案
    bool setScheme();
    //执行方案
    bool runScheme();
    void updateTableWidget(QVector<PcrSteps> steps);
    void showTemperatureWidget();
    //托盘推入
    void blockIn();
    //托盘推入到原点位置
    void blockToOriginPoint();
    //托盘运动到拍照位
    void blockToPhotoPosition(float position);
    //热盖下压
    void heatLidDown();
    //保存热盖补偿值
    bool saveCapTemOffset();
    //保存传感器补偿值
    bool saveBlockTemOffset();
    //保存KB
    bool saveBlockCompensate();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onHandleSetParams(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    void onHandleFinished(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    void onStatusChanged(bool startHeat);
    void onStatusChanged_2(bool enableSample);
    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_17_clicked();

private:
    Ui::PCRTemperatureAccuracyWidget *ui;
    QTimer *m_timer = nullptr;
    ENUM_CLASS::PCR_Id m_id;
    QString m_currentPcrConfig;
    QVector<PcrSteps> m_pcrSteps;
    TemperatureWidget *m_temperatureWidget = nullptr;
    QString m_currentSchemeName = "";

    int m_setCapTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
    int m_setBlockTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
    int m_setBlockCompensateStatus = ENUM_CLASS::SetParams_Status::Uninit;
};

#endif // PCRTEMPERATUREACCURACYWIDGET_H
