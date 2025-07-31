#ifndef DP1CARRIERWIDGET_H
#define DP1CARRIERWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class DP1CarrierWidget;
}

class DP1CarrierWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DP1CarrierWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, LaserInfo laserInfo, QWidget *parent = nullptr);
    ~DP1CarrierWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();

private slots:
    void onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry);

    void on_pushButton_33_clicked();

    void on_pushButton_18_clicked();

    void on_btnSava_2_clicked();

private:
    Ui::DP1CarrierWidget *ui;
    Storage_Info m_storageInfo;
    PipetteInfo m_pipetteInfo;
    LaserInfo m_laserInfo;
};

#endif // DP1CARRIERWIDGET_H
