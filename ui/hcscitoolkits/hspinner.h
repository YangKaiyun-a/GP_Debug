#ifndef HSPINNER_H
#define HSPINNER_H

#include <QScopedPointer>
#include <QWidget>


class HSpinnerPrivate;
class HSpinner : public QWidget
{
    Q_OBJECT
public:
    explicit HSpinner(QWidget *parent = 0);
    ~HSpinner();

    bool isPlaying() const;

public Q_SLOTS:
    void start();
    void stop();
    void setBackgroundColor(QColor color);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *e) override;

private:
    QScopedPointer<HSpinnerPrivate> d_ptr;
    Q_DISABLE_COPY(HSpinner);
    Q_DECLARE_PRIVATE(HSpinner);
};

#endif
