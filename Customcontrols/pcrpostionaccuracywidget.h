#ifndef PCRPOSTIONACCURACYWIDGET_H
#define PCRPOSTIONACCURACYWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class PCRPostionAccuracyWidget;
}

class PCRPostionAccuracyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PCRPostionAccuracyWidget(ENUM_CLASS::PCR_Id id, QWidget *parent = nullptr);
    ~PCRPostionAccuracyWidget();
    void init();
    void initConnect();
    void blockToOriginPoint();
    void blockToPhotoPosition(float position);

private slots:
    void onHandleSetParams(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    void on_pushButton_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::PCRPostionAccuracyWidget *ui;
    ENUM_CLASS::PCR_Id m_id;

    int m_setPhotoPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
    int m_setCompactPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
};

#endif // PCRPOSTIONACCURACYWIDGET_H
