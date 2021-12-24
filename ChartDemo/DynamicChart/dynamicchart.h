#ifndef DYNAMICCHART_H
#define DYNAMICCHART_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class DynamicChart : public QChart
{
public:
    // copy by QChart() definition
    DynamicChart(QGraphicsItem * parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());

    virtual ~DynamicChart();

    // override QChart::addSeries(QAbstractSeries *series);
    void addSeries(QLineSeries * newSeries);

    void pushPoint(const QPointF& data, int indexSeries = 0);

protected:
    void InitChart();
    void InitAxisX();
    void InitAxisY();
    void InitSeriesList();

    void rollAxisX();

private:
    static const int Xrange;
    int Xmin;
    QValueAxis * axisX;
    QValueAxis * axisY;
    QList<QLineSeries*> seriesList;
};
#endif // DYNAMICCHART_H
