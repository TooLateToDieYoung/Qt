#include "mychart.h"

myChart::myChart(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    InitAxisX();
    InitAxisY();
    InitChart();
}

myChart::~myChart()
{
    delete axisX;
    delete axisY;
}

void myChart::addSeries(QLineSeries * series)
{
    QChart::addSeries(series); // call the parent Func.
    series->attachAxis(axisX); // attach datas to x-axis
    series->attachAxis(axisY); // attach datas to y-axis
}

void myChart::InitChart()
{
    this->removeAllSeries();
    this->setTitle("Example Of Chart");
    this->addAxis(axisX, Qt::AlignBottom); // add x-axis
    this->addAxis(axisY, Qt::AlignLeft);   // add y-axis
    this->legend()->hide();                // chart not visible ( waiting for switch on )
    this->setAnimationOptions(QChart::SeriesAnimations); // print datas with Animation
}

void myChart::InitAxisX()
{
    axisX = new QValueAxis();
    axisX->setTitleText("X-Axis");
    axisX->setRange(0, 100);
    axisX->setTickCount(11);     // split range to 'n' intervals
    axisX->setMinorTickCount(2); // split each interval to 'n' sub parts
    axisX->setLabelFormat("%d"); // diplay intervals index in type of decimal
    axisX->setGridLineVisible(false); // mesh not visible
}

void myChart::InitAxisY()
{
    axisY = new QValueAxis();
    axisY->setTitleText("Y-Axis");
    axisY->setRange(-50, 50);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(5);
    axisY->setLabelFormat("%d");
    axisY->setGridLineVisible(true); // mesh visible
}












