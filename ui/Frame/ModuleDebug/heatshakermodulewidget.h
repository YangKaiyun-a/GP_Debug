#ifndef HEATSHAKERMODULEWIDGET_H
#define HEATSHAKERMODULEWIDGET_H

#include "Customcontrols/heatshakerdatawidget.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class HeatShakerModuleWidget;
}

class HeatShakerModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeatShakerModuleWidget(QWidget *parent = nullptr);
    ~HeatShakerModuleWidget();
    void init();
    void initStackedWidget();
    void initButton();


private slots:
    void on_btnNumber1_clicked();

    void on_btnNumber2_clicked();

    void on_btnNumber3_clicked();

    void on_btnNumber4_clicked();

    void on_btnNumber5_clicked();

    void on_btnMagnetic_clicked();

    void on_btnMagnetic_2_clicked();

    void on_btnMagnetic_3_clicked();

    void on_btnMagnetic_4_clicked();

private:
    Ui::HeatShakerModuleWidget *ui;
    QMap<int, HeatShakerDataWidget*> m_map;
};

#endif // HEATSHAKERMODULEWIDGET_H
