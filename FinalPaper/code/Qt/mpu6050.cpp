#include "mpu6050.h"

const double MPU6050::RadtoAngle = 180.0 / acos(-1);

MPU6050::MPU6050(MPU6050::AcceMode acceMode, QObject * parent)
    : QSerialPort(parent)
    , modeScalar(acceMode)
{
    setBaudRate(QSerialPort::Baud115200);
    setParity(QSerialPort::NoParity);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setFlowControl(QSerialPort::NoFlowControl);
}

MPU6050::~MPU6050()
{
    if(isOpen())
    {
        clear(QSerialPort::AllDirections);
        clearError();
        close();
    }
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
    return sqrt(acceArr[0] * acceArr[0] + acceArr[1] * acceArr[1] + acceArr[2] * acceArr[2]) / modeScalar;
}

double MPU6050::AcceComponent(int index) const
{
    return (double)acceArr[index] / modeScalar;
}

double MPU6050::AngleToAxis(int index) const
{
    return acos(AcceComponent(index) / Acce()) * RadtoAngle;
}

QList<QString> MPU6050::AllPortsName()
{
    QList<QString> res;
    foreach(auto & info, QSerialPortInfo::availablePorts())
        res.append(info.portName());
    return res;
}

/* 走自訂義通訊格式->8bytes { 'A', Acce_X_H, Acce_X_L, Acce_Y_H, Acce_Y_L, Acce_Z_H, Acce_Z_L, Crc }; */
void MPU6050::readyReadSlot() // spent 0 ~ 1 ms
{
    char c;
    while(peek(&c, 1) && c != 'A') { getChar(&c); } // peek() for find the beginning byte 'A'

    const uint8_t * datas = (const uint8_t*)read(8).operator const void *(); // read whole 8 bytes raw datas

    if( isValid(datas) ) // check to Crc byte
    {
        int16_t acceTmp[3];
        if( ParseData(acceTmp, datas) ) // try parse data & ensure it is not null
        {
            acceArr[0] = acceTmp[0];
            acceArr[1] = acceTmp[1];
            acceArr[2] = acceTmp[2];
            emit readSuccessful();     // if successed, send a signal
        }
    }

    clear(QSerialPort::Input); // clear all datas on receive reg memery
    clearError(); // ignore errors
}

bool MPU6050::ParseData(int16_t * arr, const uint8_t * bytes)
{
    arr[0] = (int16_t)(bytes[1] << 8 | bytes[2]);
    arr[1] = (int16_t)(bytes[3] << 8 | bytes[4]);
    arr[2] = (int16_t)(bytes[5] << 8 | bytes[6]);
    return arr[0] != 0 && arr[1] != 0 && arr[2] != 0;
}

bool MPU6050::isValid(const uint8_t * bytes)
{
    uint8_t crc = 0;
    for(int i = 1; i < 7; ++i) crc ^= bytes[i];
    return crc == bytes[7];
}
