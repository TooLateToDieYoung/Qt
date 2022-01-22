#ifndef MPU6050_H
#define MPU6050_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include <cmath>

using namespace std;

class MPU6050 : public QSerialPort // without Gyro operations, could be inherited
{

    Q_OBJECT

public:
    enum Direction { X, Y, Z };
    enum AcceMode  { G2 = 1 << 14, G4 = 1 << 13, G8 = 1 << 12, G16 = 1 << 11 };

public:
    explicit MPU6050(AcceMode acceMode, QObject * parent = nullptr);
    virtual ~MPU6050();

    void setPort(const QString& portName);
    double Acce() const;
    double AcceComponent(int index) const;
    double AngleToAxis(int index) const;

    QList<QString> static AllPortsName();

private slots:
    void readyReadSlot();

private:
    virtual bool ParseData(int16_t * arr, const uint8_t * bytes); // can be inherited & overrided
    virtual bool isValid(const uint8_t * bytes);                  // can be inherited & overrided

    /* volatile */ int16_t acceArr[3]; // do not need volatile, because of without interruption

    const AcceMode modeScalar; // 控制加速度模式的精度

    static const double RadtoAngle;

signals:
    void readSuccessful();

};

#endif // MPU6050_H
