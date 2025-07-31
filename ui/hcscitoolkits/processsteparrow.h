#ifndef PROCESSSTEPARROW_H
#define PROCESSSTEPARROW_H

#include <QWidget>

namespace Ui {
class ProcessStepArrow;
}

class ProcessStepArrow : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessStepArrow(QWidget *parent = nullptr);
    explicit ProcessStepArrow(QStringList &text,QWidget *parent = nullptr);
    ~ProcessStepArrow();

    int indexStep() const;

    int stepCount() const;

    void setStepCount(int stepCount);

    void setIndexStep(int indexStep);

    QBrush defaultBrush() const;
    void setDefaultBrush(const QBrush &defaultBrush);

    QBrush fillBrush() const;
    void setFillBrush(const QBrush &fillBrush);

    QStringList stepText() const;
    void setStepText(const QStringList &stepText);

    void setActiveRange(int start,int last);

    void setFinishRange(int start,int last);

    void reFresh();

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawBackground(QPainter* painter);
    void drawStep(QPainter* painter);
    void drawActive(QPainter* painter);
private:
    void init();
private:
    int m_stepCount = 10;
    int m_indexStep;
    int m_activeStartIndex;
    int m_activeLastIndex;
    int m_finishStartIndex;
    int m_finishLastIndex;
    QBrush m_defaultBrush;
    QBrush m_fillBrush;
    QPixmap m_acceptPix;
    QStringList m_stepText;
private:
    Ui::ProcessStepArrow *ui;
};

#endif // PROCESSSTEPARROW_H
