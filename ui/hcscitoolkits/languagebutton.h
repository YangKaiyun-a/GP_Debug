#ifndef LANGUAGEBUTTON_H
#define LANGUAGEBUTTON_H

#include <QWidget>


class LanguageButton : public QWidget
{
    Q_OBJECT
public:
    explicit LanguageButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

private:
    bool m_leftSideSelected = true;

};

#endif // LANGUAGEBUTTON_H
