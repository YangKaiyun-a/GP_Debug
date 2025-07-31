#ifndef HEATSHAKERDATAWIDGET_H
#define HEATSHAKERDATAWIDGET_H

#include <QWidget>

namespace Ui {
class HeatShakerDataWidget;
}

class HeatShakerDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeatShakerDataWidget(int moduleId, QWidget *parent = nullptr);
    ~HeatShakerDataWidget();
    void init();
    void initConnect();

private slots:
    void onStatusChanged(bool startHeat);
    void on_btnMagnetic_clicked();

    void on_btnReset_clicked();

    void on_btnHighSpeedShake_clicked();

    void on_btnLowSpeedShake_clicked();

    void on_btnStopShake_clicked();

    void on_btnMagnetic_3_clicked();

    void on_btnGetParams_clicked();

    void on_btnGetParams_2_clicked();

    void on_btnMagnetic_4_clicked();

    void on_btnHighSpeedShake_2_clicked();

    void on_btnHighSpeedShake_3_clicked();

    void on_btnHighSpeedShake_4_clicked();

private:
    Ui::HeatShakerDataWidget *ui;
    int m_moduleId;

    QMetaObject::Connection m_connection;
};

#endif // HEATSHAKERDATAWIDGET_H
