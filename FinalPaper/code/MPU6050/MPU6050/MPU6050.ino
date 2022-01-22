#include "Wire.h"

#define DEV_ADDR  0x68
#define ACCE_CONF 0x1C
#define ACCE_REG  0x3B

static int32_t arr[3];   // 連續快取，暫存累加資料
static uint8_t res[8];   // { 'A', Acce_X_H, Acce_X_L, Acce_Y_H, Acce_Y_L, Acce_Z_H, Acce_Z_L, Crc };

enum AcceMode { G2 = 0, G4, G8, G16 };

void setup() 
{
  Serial.begin(115200);
  delay(10);

  Wire.begin();
  delay(10);
  
  WakeUpMPU6050();
  delay(50);

  SetAcceMode(AcceMode::G2); // 2g mode
  delay(50);

  for(int i = 0; i < 3; ++i) arr[i] = 0;
  for(int i = 0; i < 8; ++i) res[i] = 0;
  res[0] |= 'A';

}

void loop() 
{
  int i;

  for(i = 0; i < 16; ++i) ReadAcceData(); // pick for 16 times of data
  for(i = 0; i < 3 ; ++i) arr[i] >>= 4; // average ( if 1 of 16 data is getting wrong, mistaking less than 6.5% )

  AssignAcceRes();
  AssignCrcBit();
  
  while(Serial.availableForWrite() < sizeof(res)); // wait for enough tx reg ( 8 bytes )
  Serial.write(res, sizeof(res)); // 送 8 bytes 的時間 = [3 + 8 * (8 + 1) + 1] bits * (1s/115200) = 0.660ms

  for(i = 0; i < 3; ++i) arr[i] &= (int32_t)0x0;
  for(i = 1; i < 8; ++i) res[i] &= (uint8_t)0x0;
}

void WakeUpMPU6050()
{
  Wire.beginTransmission(DEV_ADDR); //開啟MPU6050的傳輸
  Wire.write(0x6B);                 //指定暫存器地址
  Wire.write(0x00);                 //寫入一個位元組的資料
  Wire.endTransmission(true);       //結束傳輸，true表示釋放匯流排
}

void SetAcceMode(uint8_t mode)
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(ACCE_CONF);
  Wire.requestFrom(0x68, 1, true); // 讀出原配置
  uint8_t config = Wire.read();
  config &= 0xE7;                  // Mask bits[4:3]
  config |= mode << 3;
  Wire.write(config);
  Wire.endTransmission(true);
}

void ReadAcceData()
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(ACCE_REG);
  Wire.requestFrom(DEV_ADDR, 3 * 2, true);
  Wire.endTransmission(true);

  uint8_t byte_H, byte_L;
  for(int i = 0; i < 3; ++i)
  {
    byte_H = Wire.read();
    byte_L = Wire.read();
    arr[i] += (int32_t)(byte_H << 8 | byte_L);
  }  
}

void AssignAcceRes() // 轉成無號數，方便丟封包
{
  res[1] |= (arr[0] & 0xFF00) >> 8;
  res[2] |= (arr[0] & 0x00FF);

  res[3] |= (arr[1] & 0xFF00) >> 8;
  res[4] |= (arr[1] & 0x00FF);

  res[5] |= (arr[2] & 0xFF00) >> 8;
  res[6] |= (arr[2] & 0x00FF);
}

void AssignCrcBit() 
{
  for(int i = 1; i < 7; ++i) res[7] ^= res[i]; // 校驗碼
}

