#ifndef OTHERWIDGET_H
#define OTHERWIDGET_H

#include "Controller/air_pressure/air_pressure.h"

#include <QWidget>

namespace Ui {
class OtherWidget;
}

class OtherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OtherWidget(QWidget *parent = nullptr);
    ~OtherWidget();
    void init();

    readpressure_Thread *m_read_pressure_Thread = nullptr;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_get_environment_param_Btn_clicked();

    void on_set_environment_param_Btn_clicked();

    void on_get_pressure_param_xz_Btn_clicked();

    void on_set_pressure_param_xz_Btn_clicked();

    void on_pushButton_16_clicked();

    void on_stop_Button_clicked();

    void on_pcrmix_start_Btn_clicked();

    void on_pcrmix_stop_Btn_clicked();

    void on_pipette_start_Btn_clicked();

    void on_pipette_stop_Btn_clicked();

    void on_qpcr_start_Btn_clicked();

    void on_qpcr_stop_Btn_clicked();

    void on_Keep_fan_rpm_Btn_clicked();

private:
    Ui::OtherWidget *ui;
};

#endif // OTHERWIDGET_H
