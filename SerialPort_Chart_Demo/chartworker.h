#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QThread>

#include "dynamicchart.h"
#include "mpu6050.h"

class ChartWorker : public QThread
{
    Q_OBJECT

public:
    explicit ChartWorker(DynamicChart * const targetChart, const MPU6050 * const dev, QObject *parent = nullptr);
    void wait();

protected:
    virtual void run() = 0;

    DynamicChart * const chart;
    const MPU6050 * const device;

signals:
    void finish(ChartWorker * ptr);

};


class ChartWorkerComponent : public ChartWorker
{
public:
    explicit ChartWorkerComponent(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

protected:
    void run() override;
};

class ChartWorkerAngles : public ChartWorker
{
public:
    explicit ChartWorkerAngles(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

protected:
    void run() override;

};

class ChartWorkerBasic : public ChartWorker
{
public:
    explicit ChartWorkerBasic(DynamicChart * const targetChart, const MPU6050 * const dev, QObject * parent = nullptr);

protected:
    void run() override;

};

#endif // CHARTWORKER_H
