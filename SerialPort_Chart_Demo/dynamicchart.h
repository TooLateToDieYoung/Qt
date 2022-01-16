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
    void pushData(double data, int indexSeries = 0);
    void setAxisXRange(double max, double min, int tick = 11);
    void setAxisYRange(double max, double min, int tick = 11);
    void rollAxisX();

    static const double unit;

protected:
    void InitChart();
    void InitAxisX();
    void InitAxisY();

private:
    static const double Xrange;
    QValueAxis * axisX;
    QValueAxis * axisY;
    QList<QLineSeries*> seriesList;
    double counter;
};
#endif // DYNAMICCHART_H
