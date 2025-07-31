#ifndef MACHINEDEBUGWIDGET_H
#define MACHINEDEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class MachineDebugWidget;
}

enum Machine_Parts
{
    ACCURATING_POSITION,
    PIPETTING_PERFORMANCE,
    DEBUG_WIDGET,
    COLDING_PERFORMANCE,
    WORKFLOW_EMPTY_RUN,
    OTHER
};

class MachineDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MachineDebugWidget(QWidget *parent = nullptr);
    ~MachineDebugWidget();

    void init();
    void initConnect();
    void initStackWidget();
    void startSystemDataTime();

private slots:
    void onUpdateSysTime();

    void on_btnHeatShaker_clicked();

    void on_btnExit_clicked();

    void on_btnTP08_clicked();

    void on_btnHeatSeal_clicked();

    void on_btnPCR_clicked();

    void on_btnOther_clicked();

    void on_btnDebug_clicked();

private:
    Ui::MachineDebugWidget *ui;
};

#endif // MACHINEDEBUGWIDGET_H
