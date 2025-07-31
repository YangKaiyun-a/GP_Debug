#ifndef DP8SINGLEWIDGET_H
#define DP8SINGLEWIDGET_H


#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class DP8SingleWidget;
}

class DP8SingleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DP8SingleWidget(QWidget *parent = nullptr);
    ~DP8SingleWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry);

    void on_pushButton_34_clicked();

    void on_btnSava_3_clicked();

private:
    Ui::DP8SingleWidget *ui;
    Storage_Info m_storageInfo;                 //移液器校准位
    PipetteInfo m_pipetteInfo;                  //DP8移液器
    SingleChannelInfo m_singleInfo;             //单通道坐标

    int m_F2_Rack = -1;
};

#endif // DP8SINGLEWIDGET_H
