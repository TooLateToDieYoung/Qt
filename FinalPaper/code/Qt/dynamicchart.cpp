#include "dynamicchart.h"

DynamicChart::DynamicChart(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QChart(parent, wFlags)
{
    InitAxisX();
    InitAxisY();
    InitChart();
    seriesList.clear();
}

DynamicChart::~DynamicChart()
{
    while(!seriesList.empty())
    {
        QLineSeries * ptr = seriesList.first();
        seriesList.removeFirst();
        ptr->clear();
        delete ptr;
    }
    seriesList.clear();
    delete axisX;
    delete axisY;
}

void DynamicChart::addSomeEmptySeries(unsigned amount) noexcept
{
    for(unsigned i = 0; i < amount; ++i)
        DynamicChart::addSeries( new QLineSeries() );
}

void DynamicChart::addSeries(QLineSeries * series) noexcept
{
    QChart::addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    seriesList.append(series);

    double beginning = DynamicChart_Init_Xmin;
    while(beginning < 0) // push 100 "zero points" into sereies
    {
        series->append(QPointF{beginning, 0});
        beginning += DynamicChart_Unit;
    }
}

void DynamicChart::pushData(double data, int indexSeries) // without checking to index value, maybe throw error!
{
    seriesList[indexSeries]->append( QPointF{axisX->min() - DynamicChart_Init_Xmin, data} );
}

void DynamicChart::setAxisXRange(double max, double min, int tick) noexcept
{
    axisX->setMax(max);
    axisX->setMin(min);
    axisX->setTickCount(tick); // split range to 'tick' intervals
}

void DynamicChart::setAxisYRange(double max, double min, int tick) noexcept
{
    axisY->setMax(max);
    axisY->setMin(min);
    axisY->setTickCount(tick);
}

void DynamicChart::InitChart() noexcept
{
    this->removeAllSeries();
    this->addAxis(axisX, Qt::AlignBottom); // add x-axis
    this->addAxis(axisY, Qt::AlignLeft);   // add y-axis
    this->legend()->hide();                // chart not visible ( waiting for switch on )
    this->setAnimationOptions(QChart::AnimationOption::NoAnimation);
}

void DynamicChart::InitAxisX() noexcept
{
    axisX = new QValueAxis();
    axisX->setRange(DynamicChart_Init_Xmin, DynamicChart_Init_Xmin + DynamicChart_Xrange);
    axisX->setTickCount(2);
    axisX->setLabelFormat("%.1lf");   // diplay intervals index in type of decimal
    axisX->setGridLineVisible(false); // mesh not visible
}

void DynamicChart::InitAxisY() noexcept
{
    axisY = new QValueAxis();
    axisY->setLabelFormat("%.2lf");  // diplay intervals index in type of float
    axisY->setGridLineVisible(true); // mesh visible
}

void DynamicChart::rollAxisX() noexcept
{
    double min = axisX->min() + DynamicChart_Unit;
    axisX->setRange(min, min + DynamicChart_Xrange);
}

void DynamicChart::removeOldestData() noexcept
{
    QList<QLineSeries*>::iterator it = seriesList.begin();
    for(; it != seriesList.end(); ++it) (*it)->remove(0);
}
