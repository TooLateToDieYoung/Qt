#include "Wire.h"

#define DEV_ADDR 0x68

static uint8_t res[16];

void setup() 
{
  Serial.begin(115200);
  delay(10);

  Wire.begin();
  delay(10);
  
  Wire.beginTransmission(DEV_ADDR); //開啟MPU6050的傳輸
  Wire.write(0x6B);                 //指定暫存器地址
  Wire.write(0x00);                 //寫入一個位元組的資料
  Wire.endTransmission(true);       //結束傳輸，true表示釋放匯流排
  
  res[0] = 'A'; 
  res[8] = 'G';
  res[7] = res[15] = 0x00;
}

void loop() 
{
  ReadMPUDatas();

  AssignCrcBit();
  
  Serial.write(res, sizeof(res));

  delay(10); // 10ms pick one data
}

void ReadMPUDatas() 
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(0x3B);
  Wire.requestFrom(DEV_ADDR, 14, true); // read two register(8bits)
  Wire.endTransmission(true);
 
  uint8_t i, temp;
  for(i = 1; i < 7; ++i)  res[i] = Wire.read();
  temp = Wire.read();
  temp = Wire.read();
  for(i = 9; i < 15; ++i) res[i] = Wire.read();  
}

void AssignCrcBit() 
{
  res[7]  &= 0x00;
  res[15] &= 0x00;
  for(int i = 1; i < 7; ++i) 
  {
    res[7]  ^= res[i];
    res[15] ^= res[i + 8];
  }
}
