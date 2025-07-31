#ifndef DP8WIDGET_H
#define DP8WIDGET_H

#include "Function/data/global.h"
#include "Customcontrols/dp8carrierwidget.h"
#include "Customcontrols/dp8pcrwidget.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class DP8Widget;
}

class DP8Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DP8Widget(QWidget *parent = nullptr);
    ~DP8Widget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onUpdateRackDeviation(int rackId);
    void on_pushButton_37_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_43_clicked();

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

    void on_pushButton_6_clicked();

    void on_btnSava_4_clicked();

    void on_btnSava_5_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::DP8Widget *ui;
    QVector<int> m_rackVector;     //DP8中用到的机位
    //机位与位置信息的对应关系
    QMap<int, DP8CarrierWidget*> m_map;
    QMap<int, DP8PcrWidget*> m_pcrMap;
    PipetteInfo m_pipetteInfo;                  //DP8移液器
    ClawInfo m_clawInfo;                        //夹爪
    Storage_Info m_storageInfo;                 //移液器校准位
    Storage_Info m_clawStorageInfo;             //夹爪修正校准位
    SingleChannelInfo m_singleInfo;             //单通道
    float m_resetZ0 = 0;

    int m_G1_Rack = -1;
    int m_Nucleic_Storage_2_Rack = -1;
    int m_F2_Rack = -1;
    int m_Nucleic_Storage_1_Rack = -1;
    int m_F1_Rack = -1;
    int m_System_Building_Rack = -1;
    int m_Transport_Pcr_2_Rack = -1;
    int m_Transport_Pcr_Put_Rack = -1;
    int m_Magnet_Shake_3_Rack = -1;
    int m_Magnet_Shake_4_Rack = -1;
    int m_Magnet_Shake_5_Rack = -1;
    int m_Trash_Rack = -1;
    int m_Magnet_Shake_2_Rack = -1;
    int m_Magnet_Shake_1_Rack = -1;
    int m_Heat_Seal_Waiting_Rack = -1;
};

#endif // DP8WIDGET_H
