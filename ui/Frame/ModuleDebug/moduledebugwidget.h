#ifndef MODULEDEBUGWIDGET_H
#define MODULEDEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class ModuleDebugWidget;
}

enum Module
{
    HEAT_SHAKER,
    TP08,
    HEAT_SEAL,
    PCR
};

class ModuleDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModuleDebugWidget(QWidget *parent = nullptr);
    ~ModuleDebugWidget();
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

private:
    Ui::ModuleDebugWidget *ui;
};

#endif // MODULEDEBUGWIDGET_H
