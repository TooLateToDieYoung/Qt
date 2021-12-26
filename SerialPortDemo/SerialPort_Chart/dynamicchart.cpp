#include "dynamicchart.h"

const int DynamicChart::Xrange = 150;

DynamicChart::DynamicChart(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QChart(parent, wFlags), Xmin(0)
{
    InitAxisX();
    InitAxisY();
    InitChart();
    InitSeriesList();
}

DynamicChart::~DynamicChart()
{
    /*
     * those pointers got from outside, so we don't need to relese them.
     *
    for(auto i = 0; i < seriesList.size(); ++i){
        auto temp = seriesList[i];
        delete temp;
    }
    */
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
}

void DynamicChart::pushPoint(const QPointF & data, int indexSeries)
{
    QLineSeries * target = seriesList[indexSeries];
    target->append(data);
    if(target->count() >= 100) rollAxisX();
}

void DynamicChart::InitChart()
{
    this->removeAllSeries();
    this->setTitle("Example Of Chart");
    this->addAxis(axisX, Qt::AlignBottom); // add x-axis
    this->addAxis(axisY, Qt::AlignLeft);   // add y-axis
    this->legend()->hide();                // chart not visible ( waiting for switch on )
    this->setAnimationOptions(QChart::SeriesAnimations); // print datas with Animation
}

void DynamicChart::InitAxisX()
{
    axisX = new QValueAxis();
    axisX->setTitleText("X-Axis");
    axisX->setRange(Xmin, Xmin + Xrange);
    axisX->setTickCount(11);     // split range to 'n' intervals
    axisX->setMinorTickCount(2); // split each interval to 'n' sub parts
    axisX->setLabelFormat("%d"); // diplay intervals index in type of decimal
    axisX->setGridLineVisible(false); // mesh not visible
}

void DynamicChart::InitAxisY()
{
    axisY = new QValueAxis();
    axisY->setTitleText("Y-Axis");
    axisY->setMax(200);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(5);
    axisY->setLabelFormat("%.2lf");
    axisY->setGridLineVisible(true); // mesh visible
}

void DynamicChart::InitSeriesList()
{
    seriesList.clear();
}

void DynamicChart::rollAxisX()
{
    ++Xmin;
    axisX->setRange(Xmin, Xmin + Xrange);

    QList<QLineSeries*>::Iterator it;
    for(it = seriesList.begin(); it != seriesList.end(); ++it)
    {
        (*it)->remove(0);
        //(*it)->attachAxis(axisX); "AxisX already attached to series."
        //(*it)->attachAxis(axisY); "AxisY already attached to series."
    }
}
