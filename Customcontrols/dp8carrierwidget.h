#ifndef DP8CARRIERWIDGET_H
#define DP8CARRIERWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class DP8CarrierWidget;
}

class DP8CarrierWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DP8CarrierWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, float resetZ0, QWidget *parent = nullptr);
    ~DP8CarrierWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();

private slots:
    void onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry);

    void on_pushButton_18_clicked();

    void on_pushButton_33_clicked();

    void on_btnSava_2_clicked();

private:
    Ui::DP8CarrierWidget *ui;
    Storage_Info m_storageInfo;
    PipetteInfo m_pipetteInfo;
    float m_resetZ0 = 0;
};

#endif // DP8CARRIERWIDGET_H
