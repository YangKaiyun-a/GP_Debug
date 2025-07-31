#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include "hcscitoolkits/hspinner.h"
#include "Function/Data/global.h"
#include "Customcontrols/calibrationwidget.h"


#include <QDialog>

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    enum BlockMoveType
    {
        Uninit,
        TrayIn,
        TrayOut
    };

public:
    explicit CalibrationDialog(ENUM_CLASS::PCR_Id id, QWidget *parent = nullptr);
    ~CalibrationDialog();
    void init();
    void initConnect();
    void setBtnEnable(bool enable);
    void showSpinner();
    void hideSpinner();
    void click();
    void setConfirmText(const QString &msg);
    CalibrationWidget* getCurrentWidget();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onHandleMessageInfo(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    void on_btnConfirm_clicked();

    void on_pushButton_clicked();

signals:
    void handleDialogClose(ENUM_CLASS::PCR_Id id, ENUM_CLASS::Light_Channel currentChannel);

private:
    Ui::CalibrationDialog *ui;
    HSpinner *m_spinner = nullptr;
    ENUM_CLASS::PCR_Id m_id;
    ENUM_CLASS::Light_Channel m_channel = ENUM_CLASS::FAM;  //当前通道
    BlockMoveType m_bockMoveType = Uninit;                  //托盘运动方向
    bool m_isFinished = false;                              //是否完成了校准
};

#endif // CALIBRATIONDIALOG_H
