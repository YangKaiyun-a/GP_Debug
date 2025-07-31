#ifndef HEATSEALMODULEWIDGET_H
#define HEATSEALMODULEWIDGET_H

#include <QWidget>

namespace Ui {
class HeatSealModuleWidget;
}

class HeatSealModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeatSealModuleWidget(QWidget *parent = nullptr);
    ~HeatSealModuleWidget();
    void init();
    void initConnect();

private slots:
    void onStatusChanged(bool startHeat);
    void on_btnModuleReset_clicked();

    void on_btnLiquidDetect_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_btnLiquidDetect_2_clicked();

    void on_pushButton_21_clicked();

    void on_btnLiquidDetect_3_clicked();

    void on_btnLiquidDetect_4_clicked();

    void on_btnLiquidDetect_5_clicked();

    void on_btnLiquidDetect_6_clicked();

private:
    Ui::HeatSealModuleWidget *ui;
};

#endif // HEATSEALMODULEWIDGET_H
