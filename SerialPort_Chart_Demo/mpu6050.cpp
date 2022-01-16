#include "mpu6050.h"

const double MPU6050::rad = 180.0 / acos(-1);

MPU6050::MPU6050(qreal unitScalar, QObject * parent)
    : QSerialPort(parent)
    , scalar(unitScalar)
{
    setBaudRate(QSerialPort::Baud115200);
    setParity(QSerialPort::NoParity);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setFlowControl(QSerialPort::NoFlowControl);
}

MPU6050::~MPU6050()
{
    clear();
    clearError();
}

void MPU6050::setPort(const QString & portName)
{
    foreach(const auto& info, QSerialPortInfo::availablePorts())
        if(info.portName() == portName) QSerialPort::setPort(info);
    clearError();
    connect(this, &QSerialPort::readyRead, this, &MPU6050::readyReadSlot);
}

double MPU6050::Acce() const
{
    return sqrt(acceArr[0] * acceArr[0] + acceArr[1] * acceArr[1] + acceArr[2] * acceArr[2]) / scalar;
}

double MPU6050::Gyro() const
{
    return sqrt(gyroArr[0] * gyroArr[0] + gyroArr[1] * gyroArr[1] + gyroArr[2] * gyroArr[2]);
}

QList<double> MPU6050::AcceComponentsList()
{
    QList<qreal> res;
    for(int i = 0; i < 3; ++i)
        res.append(AcceComponent(i));
    return res;
}

double MPU6050::AcceComponent(int index) const
{
    return acceArr[index] / scalar;
}


double MPU6050::GyroComponent(int index) const
{
    return gyroArr[index];
}

QList<double> MPU6050::AnglesList()
{
    QList<qreal> res;
    for(int i = 0; i < 3; ++i)
        res.append(AngleToAxis(i));
    return res;
}

double MPU6050::AngleToAxis(int index) const
{
    return acos(AcceComponent(index) / Acce()) * rad;
}

QList<QString> MPU6050::AllPortsName()
{
    QList<QString> res;
    foreach(auto & info, QSerialPortInfo::availablePorts())
        res.append(info.portName());
    return res;
}

void MPU6050::readyReadSlot()
{
    char c;
    while(peek(&c, 1) && c != 'A') { getChar(&c); }

    const uint8_t * datas = (const uint8_t*)read(8).operator const void *();

    if( isValid(datas) )
    {
        int16_t acceTmp[3];
        if(ParseData(acceTmp, datas))
        {
            acceArr[0] = acceTmp[0];
            acceArr[1] = acceTmp[1];
            acceArr[2] = acceTmp[2];
            emit readSuccessful();
        }
    }

    flush();
    clearError();
}

bool MPU6050::ParseData(int16_t * arr, const uint8_t * bytes)
{
    arr[0] = (int16_t)(bytes[1] << 8 | bytes[2]);
    arr[1] = (int16_t)(bytes[3] << 8 | bytes[4]);
    arr[2] = (int16_t)(bytes[5] << 8 | bytes[6]);
    return arr[0] != 0 || arr[1] != 0 || arr[2] != 0;
}

bool MPU6050::isValid(const uint8_t * bytes)
{
    uint8_t crc = 0;
    for(int i = 1; i < 7; ++i) crc ^= bytes[i];
    return crc == bytes[7];
}
