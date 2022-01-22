#ifndef DYNAMICCHART_H
#define DYNAMICCHART_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#define  DynamicChart_Init_Xmin (double)-10
#define  DynamicChart_Xrange    (double)15
#define  DynamicChart_Unit      (double)0.1

QT_CHARTS_USE_NAMESPACE

// 1. also could be inherited
// 2. all of public member functions are able to override
class DynamicChart : public QChart
{
public:
    // copy by QChart() definition
    explicit DynamicChart(QGraphicsItem * parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());
    virtual ~DynamicChart();

    virtual void addSomeEmptySeries(unsigned amount = 1) noexcept; // List 內的數據流全部採用動態記憶體 (內部生成與控管)
    virtual void pushData(double data, int indexSeries = 0);
    virtual void setAxisXRange(double max, double min, int tick = 2) noexcept;
    virtual void setAxisYRange(double max, double min, int tick = 2) noexcept;
    virtual void rollAxisX() noexcept;
    virtual void removeOldestData() noexcept;

protected:
    // override QChart::addSeries(QAbstractSeries *series);
    virtual void addSeries(QLineSeries * newSeries) noexcept /* override */ ; // 內部管理動態記憶體

private:
    void InitChart() noexcept;
    void InitAxisX() noexcept;
    void InitAxisY() noexcept;

    QValueAxis * axisX;
    QValueAxis * axisY;
    QList<QLineSeries*> seriesList; // pointer to data series

};
#endif // DYNAMICCHART_H
