#ifndef DJ1WIDGET_H
#define DJ1WIDGET_H

#include "Function/data/global.h"
#include "Customcontrols/dj1carrierwidget.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class DJ1Widget;
}

class DJ1Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DJ1Widget(QWidget *parent = nullptr);
    ~DJ1Widget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();
    void initConnect();

    //保存转运校准位偏差
    void saveTransportPositionDeviation();

    //保存B架开盖位偏差
    void saveBOpenPositionDeviation();

    //保存B架关盖位偏差
    void saveBClosePositionDeviation();

private slots:
    void onUpdateRackDeviation(int rackId);
    void on_pushButton_17_clicked();

    void on_btnSava_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_18_clicked();

    void on_btnSava_2_clicked();

    void on_pushButton_22_clicked();

    void on_btnSava_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_25_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DJ1Widget *ui;
    //DJ1中用到的机位rack_id
    QVector<int> m_rackVector;
    //机位与位置信息的对应关系
    QMap<int, DJ1CarrierWidget*> m_map;
    //DJ1移液器
    PipetteInfo m_pipetteInfo;
    //夹爪
    ClawInfo m_clawInfo;
    //移液器校准点
    Storage_Info m_storageInfo;
    //转运校准位
    Storage_Info m_transportInfo;
    //B架开盖位
    Storage_Info m_bOpenInfo;
    //B架关盖位
    Storage_Info m_bCloseInfo;
    //存储所有的位置信息
    QVector<Storage_Info> m_storageVector;

    int m_A1_rack_id = -1;
    int m_B1_rack_id = -1;
    int m_clean1_rack_id = -1;
    int m_pcr1_rack_id = -1;        //PCR板转运机位
    int m_B2_rack_id = -1;
    int m_B_open_rack_id = -1;
    int m_B_close_rack_id = -1;

};

#endif // DJ1WIDGET_H
