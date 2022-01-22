#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QThread>

#include "dynamicchart.h"
#include "mpu6050.h"

class ChartWorker : public QThread // pure virtual class, can just be inherited ( for chart printing )
{
    Q_OBJECT

public:
    explicit ChartWorker(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

    void wait(); //  destroy itself as working finished

protected:
    virtual void run() = 0;

    DynamicChart * const chart;   // to printing target
    const MPU6050 * const device; // just offer data ( also compute floating in thread )

signals:
    void finish(ChartWorker * ptr);
};

// just override run() function
class ChartWorkerComponent : public ChartWorker
{
public:
    explicit ChartWorkerComponent(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

protected:
    void run() override;
};

// just override run() function
class ChartWorkerAngles : public ChartWorker
{
public:
    explicit ChartWorkerAngles(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

protected:
    void run() override;
};

 // just override run() function
class ChartWorkerBasic : public ChartWorker
{
public:
    explicit ChartWorkerBasic(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

protected:
    void run() override;
};

#endif // CHARTWORKER_H
