#ifndef QPCRWIDGET_H
#define QPCRWIDGET_H

#include "Function/data/global.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class QpcrWidget;
}

class QpcrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QpcrWidget(QWidget *parent = nullptr);
    ~QpcrWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI(const Storage_Info &info);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_17_clicked();

    void on_btnSava_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

private:
    Ui::QpcrWidget *ui;
    QVector<int> m_rackVector;     //校准的机位号
    QMap<int , Storage_Info> m_storageInfoMap;

    int m_Transport_Pcr_3_Rack = -1;
    int m_QPcr_1_Rack = -1;
    int m_QPcr_2_Rack = -1;
    int m_Pcr_Board_Out_Rack = -1;
};

#endif // QPCRWIDGET_H
