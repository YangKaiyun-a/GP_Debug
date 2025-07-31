#ifndef DJ2WIDGET_H
#define DJ2WIDGET_H

#include "Function/data/global.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class DJ2Widget;
}

class DJ2Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DJ2Widget(QWidget *parent = nullptr);
    ~DJ2Widget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();
    void initConnect();

private slots:
    void onUpdateRackDeviation(int rackId);
    void on_pushButton_17_clicked();

    void on_btnSava_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::DJ2Widget *ui;
    //DJ2中用到的机位
    QVector<int> m_rackVector;
    //DJ2移液器
    PipetteInfo m_pipetteInfo;
    //移液器校准点
    Storage_Info m_storageInfo;

    int m_Magnetic_Beads_Shake_Rack = -1;   //磁珠振荡机位
};

#endif // DJ2WIDGET_H
