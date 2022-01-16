#include "dynamicchart.h"

const double DynamicChart::Xrange = 0.0015;
const double DynamicChart::unit   = 0.00001;

DynamicChart::DynamicChart(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QChart(parent, wFlags)
    , counter(0)
{
    InitAxisX();
    InitAxisY();
    InitChart();
    seriesList.clear();
}

DynamicChart::~DynamicChart()
{
    seriesList.clear();
    delete axisX;
    delete axisY;
}

void DynamicChart::addSeries(QLineSeries * series)
{
    QChart::addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    seriesList.append(series);

    for(double i = axisX->min(); i < 0; i += DynamicChart::unit)
        series->append(QPointF{i, 0});
}

void DynamicChart::pushData(double data, int indexSeries)
{
    seriesList[indexSeries]->append(QPointF{counter, data});
}

void DynamicChart::setAxisXRange(double max, double min, int tick)
{
    axisX->setMax(max);
    axisX->setMin(min);
    axisX->setTickCount(tick);
}

void DynamicChart::setAxisYRange(double max, double min, int tick)
{
    axisY->setMax(max);
    axisY->setMin(min);
    axisY->setTickCount(tick);
}

void DynamicChart::InitChart()
{
    this->removeAllSeries();
    this->addAxis(axisX, Qt::AlignBottom); // add x-axis
    this->addAxis(axisY, Qt::AlignLeft);   // add y-axis
    this->legend()->hide();                // chart not visible ( waiting for switch on )
    this->setAnimationOptions(QChart::AnimationOption::NoAnimation);
}

void DynamicChart::InitAxisX()
{
    axisX = new QValueAxis();
    axisX->setRange(-0.001, -0.001 + Xrange);
    axisX->setLabelFormat("%.4lf"); // diplay intervals index in type of double
    axisX->setGridLineVisible(false); // mesh not visible
}

void DynamicChart::InitAxisY()
{
    axisY = new QValueAxis();
    axisY->setRange(100, -100);
    axisY->setTickCount(11);         // split range to 'n' intervals
    axisY->setLabelFormat("%.2lf");
    axisY->setGridLineVisible(true); // mesh visible
}

void DynamicChart::rollAxisX()
{
    counter += unit;
    double min = axisX->min() + unit;
    axisX->setRange(min, min + Xrange);
}
