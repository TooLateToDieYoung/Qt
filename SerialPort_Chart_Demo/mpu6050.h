#ifndef MPU6050_H
#define MPU6050_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include <cmath>

using namespace std;

enum Direction { X, Y, Z };

class MPU6050 : public QSerialPort
{

    Q_OBJECT

public:
    MPU6050(qreal unitScalar, QObject *parent = nullptr);
    virtual ~MPU6050();

    void setPort(const QString& portName);

    double Acce() const;
    double Gyro() const;

    QList<double> AcceComponentsList();
    double AcceComponent(int index) const;
    double GyroComponent(int index) const;

    QList<double> AnglesList();
    double AngleToAxis(int index) const;

    QList<QString> static AllPortsName();

private slots:
    void readyReadSlot();

private:
    bool static ParseData(int16_t * arr, const uint8_t * bytes);
    bool static isValid(const uint8_t * bytes);

    volatile int16_t acceArr[3];
    volatile int16_t gyroArr[3];

    const double scalar;

    static const double rad;

signals:
    void readSuccessful();

};

#endif // MPU6050_H
