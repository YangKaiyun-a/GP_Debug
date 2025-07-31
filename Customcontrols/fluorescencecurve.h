#ifndef FLUORESCENCECURVE_H
#define FLUORESCENCECURVE_H
#include "Function/data/global.h"

#include <QWidget>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>

namespace Ui {
class FluorescenceCurve;
}

QT_CHARTS_USE_NAMESPACE

    class FluorescenceCurve : public QWidget
{
    Q_OBJECT

public:
    explicit FluorescenceCurve(QWidget *parent = nullptr);

    void init();

    void initSeries();

    void appendLineData(const RealTimeTemperature &temp);

    void resetChart();

private:
    double maxY = 120.0;

    QChart *chart;

    QList<QSplineSeries*> m_lineSeriesLst;

    QList<QGraphicsTextItem*> m_textItemLst;
};

#endif // FLUORESCENCECURVE_H
