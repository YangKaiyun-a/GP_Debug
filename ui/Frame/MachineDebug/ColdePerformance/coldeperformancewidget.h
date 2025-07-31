#ifndef COLDEPERFORMANCEWIDGET_H
#define COLDEPERFORMANCEWIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui {
class ColdePerformanceWidget;
}


class ColdePerformanceWidget : public QWidget
{
    Q_OBJECT


public:
    struct ConnectionStatus
    {
        QMetaObject::Connection conn;
        bool isConnected = false;
    };

public:
    explicit ColdePerformanceWidget(QWidget *parent = nullptr);
    ~ColdePerformanceWidget();
    void init();
    void initStyleSheet();
    void initConnect();

private slots:
    void onStatusChanged(bool startCold);
    void onStatusChanged_11(bool startCold);
    void onStatusChanged_12(bool startCold);
    void onStatusChanged_13(bool startCold);
    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_48_clicked();

private:
    Ui::ColdePerformanceWidget *ui;

    QMap<int, ConnectionStatus> m_mapConnection;
    bool m_isGetTemp = false;
};

#endif // COLDEPERFORMANCEWIDGET_H
