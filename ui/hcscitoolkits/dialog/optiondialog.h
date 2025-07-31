#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include "hcscitoolkits/hspinner.h"
#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(const QString& msg = "", QWidget *parent = nullptr);
    ~OptionDialog();

    QString msg() const;
    void setMsg(const QString &msg);
    void setOkMessage(const QString & msg);
    void setCancelMessage(const QString& msg);
    void setOkEnable(bool enable);
    void setCancelEnable(bool enable);
    void showSpinner();
    void hideSpinner();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_btnCancel_clicked();

    void on_btnConfirm_clicked();

private:
    Ui::OptionDialog *ui;
    QString m_msg;
    HSpinner *m_spinner = nullptr;
};

#endif // OPTIONDIALOG_H
