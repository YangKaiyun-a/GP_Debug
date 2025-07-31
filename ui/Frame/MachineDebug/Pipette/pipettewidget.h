#ifndef PIPETTEWIDGET_H
#define PIPETTEWIDGET_H
#include "Function/data/global.h"

#include <QWidget>
#include <QMap>

namespace Ui {
class PipetteWidget;
}

enum SendWorkFlowStage {
    Uninit = -1,
    Load_Carrier,
    Load_Project,
    Load_Process_Table,
    Run_Project
};

struct CurrentWorkFlowInfo
{
    int currentStage = -1;           //当前阶段
    int currentPipette = 0;         //当前移液器
    QString workFlowName = "";      //当前流程名称
    QString workflowJson = "";      //当前流程json
};

class PipetteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PipetteWidget(QWidget *parent = nullptr);
    ~PipetteWidget();
    void init();
    void initData();
    void initStyleSheet();
    void initConnect();

    //发送流程的处理逻辑
    void handleRunWorkFlow(ENUM_CLASS::Position_Module pipiet_id, const QString &workFlowName);
    //发送载架
    void sendCarrier();
    //发送流程
    void sendWorkFlow();
    //发送台面
    void sendProcessTable();
    //开启实验
    void sendRunProject();

private slots:
    void onStatusChanged_2(bool status);
    void onStatusChanged_3(bool status);
    void onStatusChanged_4(bool status);
    void onStatusChanged_5(bool status);
    void onStatusChanged_6(bool status);
    void onStatusChanged_7(bool status);
    void onStatusChanged_8(bool status);
    void onStatusChanged_10(bool status);
    void onStatusChanged_11(bool status);
    void onStatusChanged_13(bool status);
    void onStatusChanged_14(bool status);
    void onStatusChanged_16(bool status);
    void onStatusChanged_9(bool status);
    void onStatusChanged_12(bool status);
    void onStatusChanged_15(bool status);
    void onHandleMiddleInfo(const MiddleMessageInfo &info, ENUM_CLASS::PCR_Id id);
    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_56_clicked();

    void on_comboBox_DJ1_currentTextChanged(const QString &arg1);

    void on_comboBox_DJ2_currentTextChanged(const QString &arg1);

    void on_comboBox_DP1_currentTextChanged(const QString &arg1);

    void on_comboBox_DP8_currentTextChanged(const QString &arg1);

private:
    Ui::PipetteWidget *ui;
    //存储移液器和流程
    QMap<QString, QList<QString>> m_workFlowMap;
    CurrentWorkFlowInfo m_currentWorkFlowInfo;
    //存储每个阶段的错误信息
    QMap<int, QString> m_stageErrorMsg;

};

#endif // PIPETTEWIDGET_H
