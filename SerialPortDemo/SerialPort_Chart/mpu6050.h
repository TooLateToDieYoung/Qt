#ifndef MPU6050_H
#define MPU6050_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include <cmath>

using namespace std;

enum class Direction { X = 0, Y, Z };

class MPU6050 : public QSerialPort
{
public:
    MPU6050(int64_t ps, QObject *parent = nullptr);
    virtual ~MPU6050();

    void setPort(const QString& portName);

    qreal AcceAngle(Direction index) const;
    qreal GyroAngle(Direction index) const;

    QList<qreal> AcceAngleArr() const;
    QList<qreal> GyroAngleArr() const;

    QList<QString> static AllPortsName();

signals:
    inline void updated() {}

private slots:
    void readyReadSlot();

private:
    bool static ParseData(int16_t * arr, const uint8_t * bytes);
    bool static isValid(const uint8_t * bytes);

    volatile int16_t acceArr[3];
    volatile int16_t gyroArr[3];
    qreal precision;
};

#endif // MPU6050_H
