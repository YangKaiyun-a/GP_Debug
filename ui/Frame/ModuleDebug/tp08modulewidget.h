#ifndef TP08MODULEWIDGET_H
#define TP08MODULEWIDGET_H

#include <QWidget>

namespace Ui {
class TP08ModuleWidget;
}

class TP08ModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TP08ModuleWidget(QWidget *parent = nullptr);
    ~TP08ModuleWidget();
    void init();
    void initScrollBar();

private slots:
    void on_btnModuleReset_clicked();

    void on_btnHotMachine_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_btnSava_4_clicked();

    void on_btnSava_5_clicked();

    void on_btnHotMachine_2_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_20_clicked();

    void on_Tip_comboBox_4_currentTextChanged(const QString &arg1);

    void on_pushButton_22_clicked();

private:
    Ui::TP08ModuleWidget *ui;
    QTimer *m_timer = nullptr;
};

#endif // TP08MODULEWIDGET_H
