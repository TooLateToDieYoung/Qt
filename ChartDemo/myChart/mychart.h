#ifndef MYCHART_H
#define MYCHART_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QPoint>
#include <QList>
#include <QtCore/QTimer>

QT_CHARTS_USE_NAMESPACE

class myChart : public QChart
{
public:
    myChart(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~myChart();

    void addSeries(QLineSeries * series);

private:
    void InitChart();
    void InitView();
    void InitAxisX();
    void InitAxisY();

    QValueAxis * axisX;
    QValueAxis * axisY;

};

#endif // MYCHART_H
