#ifndef DJ1CARRIERWIDGET_H
#define DJ1CARRIERWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class DJ1CarrierWidget;
}

class DJ1CarrierWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DJ1CarrierWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, QWidget *parent = nullptr);
    ~DJ1CarrierWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();

private slots:
    void onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry);
    void on_pushButton_17_clicked();

    void on_btnSava_clicked();

private:
    Ui::DJ1CarrierWidget *ui;
    Storage_Info m_storageInfo;
    PipetteInfo m_pipetteInfo;
};

#endif // DJ1CARRIERWIDGET_H
