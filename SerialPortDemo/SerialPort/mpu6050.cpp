#include "mpu6050.h"

MPU6050::MPU6050(int64_t ps, QObject * parent)
    : QSerialPort(parent)
    , precision((double)ps)
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

qreal MPU6050::AcceAngle(Direction index) const
{
    return acos(acceArr[index] / precision) * 57.3;
}

qreal MPU6050::GyroAngle(Direction index) const
{
    return acos(gyroArr[index] / precision) * 57.3;
}

QList<qreal> MPU6050::AcceAngleArr() const
{
    QList<qreal> res;
    for(int i = 0; i < 3; ++i)
        res.append(AcceAngle((Direction)i));
    return res;
}

QList<qreal> MPU6050::GyroAngleArr() const
{
    QList<qreal> res;
    for(int i = 0; i < 3; ++i)
        res.append(GyroAngle((Direction)i));
    return res;
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

    const uint8_t * datas = (const uint8_t*)read(16).operator const void *();

    if( isValid(datas) && isValid(datas + 8) )
    {
        int16_t acceTmp[3], gyroTmp[3];
        if(ParseData(acceTmp, datas) && ParseData(gyroTmp, datas + 8))
        {
            acceArr[0] = acceTmp[0];
            acceArr[1] = acceTmp[1];
            acceArr[2] = acceTmp[2];
            gyroArr[0] = gyroTmp[0];
            gyroArr[1] = gyroTmp[1];
            gyroArr[2] = gyroTmp[2];
        }
    }

    flush();
    clearError();
}

bool MPU6050::ParseData(int16_t * arr, const uint8_t * bytes)
{
    arr[0] = (int)(bytes[1] << 8 | bytes[2]);
    arr[1] = (int)(bytes[3] << 8 | bytes[4]);
    arr[2] = (int)(bytes[5] << 8 | bytes[6]);
    return arr[0] != 0 || arr[1] != 0 || arr[2] != 0;
}

bool MPU6050::isValid(const uint8_t * bytes)
{
    uint8_t crc = 0;
    for(int i = 1; i < 7; ++i) crc ^= bytes[i];
    return crc == bytes[7];
}
