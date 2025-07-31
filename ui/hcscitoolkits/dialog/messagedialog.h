#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include "hcscitoolkits/hspinner.h"
#include <QDialog>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    enum Flags{
        Tip = 0,
        Warning,
        Fault
    };

    explicit MessageDialog(const QString& msg, Flags flag = Tip,QWidget *parent = nullptr);
    ~MessageDialog();
    QString msg() const;
    void setMsg(const QString &msg);

    void setConfirmText(const QString &msg);

    void updateIcon(Flags flag);

    void setBtnEnable(bool enable);

    void showSpinner();

    void hideSpinner();

    void click();

protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_btnConfirm_clicked();

private:
    Ui::MessageDialog *ui;
    QString m_msg;
    HSpinner *m_spinner = nullptr;
};

#endif // MESSAGEDIALOG_H
