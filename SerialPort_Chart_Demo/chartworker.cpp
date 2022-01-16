#include "chartworker.h"

ChartWorker::ChartWorker(DynamicChart * const targetChart, const MPU6050 * const dev, QObject *parent)
    : QThread(parent)
    , chart(targetChart)
    , device(dev) { }

void ChartWorker::wait()
{
    while(!isFinished());
    delete this;
}

ChartWorkerComponent::ChartWorkerComponent(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent)
    : ChartWorker(targetChart, dev, parent) { start(); }

void ChartWorkerComponent::run()
{
    chart->rollAxisX();
    for(int i = 0; i < 3; ++i)
        chart->pushData(device->AcceComponent(i), i);

    emit finish(this);
}

ChartWorkerAngles::ChartWorkerAngles(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent)
    : ChartWorker(targetChart, dev, parent) { start(); }

void ChartWorkerAngles::run()
{
    chart->rollAxisX();
    for(int i = 0; i < 3; ++i)
        chart->pushData(device->AngleToAxis(i), i);

    emit finish(this);
}

ChartWorkerBasic::ChartWorkerBasic(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent)
    : ChartWorker(targetChart, dev, parent) { start(); }

void ChartWorkerBasic::run()
{
    chart->rollAxisX();
    chart->pushData(device->Acce());

    emit finish(this);
}
