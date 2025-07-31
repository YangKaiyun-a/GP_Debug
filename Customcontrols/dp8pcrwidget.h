#ifndef DP8PCRWIDGET_H
#define DP8PCRWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class DP8PcrWidget;
}

class DP8PcrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DP8PcrWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, ClawInfo clawInfo, QWidget *parent = nullptr);
    ~DP8PcrWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();

private slots:
    void onUpdateClawInfo(ENUM_CLASS::Gantry gantry);
    void onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry);

    void on_pushButton_35_clicked();

    void on_btnSava_4_clicked();

private:
    Ui::DP8PcrWidget *ui;
    Storage_Info m_storageInfo;
    PipetteInfo m_pipetteInfo;
    ClawInfo m_clawInfo;
};

#endif // DP8PCRWIDGET_H
