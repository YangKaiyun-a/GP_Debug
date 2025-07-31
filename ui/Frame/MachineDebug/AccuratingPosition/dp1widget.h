#ifndef DP1WIDGET_H
#define DP1WIDGET_H

#include "Function/data/global.h"
#include "Customcontrols/dp1carrierwidget.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class DP1Widget;
}

class DP1Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DP1Widget(QWidget *parent = nullptr);
    ~DP1Widget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onUpdateRackDeviation(int rackId);
    void on_pushButton_11_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_33_clicked();

    void on_btnSava_clicked();

    void on_pushButton_18_clicked();

    void on_btnSava_2_clicked();

    void on_pushButton_34_clicked();

    void on_btnSava_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::DP1Widget *ui;
    //DP1中用到的机位
    QVector<int> m_rackVector;
    //机位与位置信息的对应关系
    QMap<int, DP1CarrierWidget*> m_map;
    //DP1移液器
    PipetteInfo m_pipetteInfo;
    //夹爪
    ClawInfo m_clawInfo;
    //激光
    LaserInfo m_laserInfo;
    //移液器校准位
    Storage_Info m_storageInfo;
    //夹爪修正校准位
    Storage_Info m_clawStorageInfo;

    int m_E2_Rack = -1;
    int m_C1_Rack = -1;
    int m_Clean_Pool_2_Rack = -1;
    int m_Magnetic_Beads_Shake_Rack = -1;
    int m_C2_Rack = -1;
    int m_Open_Gate_Waiting_Rack = -1;
    int m_D1_Rack = -1;
    int m_E1_Rack = -1;
    int m_Transport_Add_Sample_Rack = -1;
    int m_D2_Rack = -1;
};

#endif // DP1WIDGET_H
