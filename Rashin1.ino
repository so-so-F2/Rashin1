#include <SparkFunLSM9DS1.h>
#include <Wire.h>
#include <dummy.h>

#define BLYNK_PRINT Serial
 
#define BLYNK_USE_DIRECT_CONNECT
 
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
 
#include <SPI.h>
#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

LSM9DS1 imu;

char auth[] = "wKgHFe7-oEQZujzrgyHpuLes6h153fKq";

float x,y,z;
uint16_t connectionState = 0;

float magx;//マグネットのｘの値
float ax=0.5;//マグネットxの最大値とその後の定数
float bx=0.01;//マグネットｘの最小値
float magy;//マグネットのyの値

int gpsx;//ＧＰＳの経度
int gpsy;//ＧＰＳの緯度
int gpsx2;//計算した後のＧＰＳの経度
int gpsy2;//計算した後のＧＰＳの緯度
//float wanx=139.745433;//目的地の経度東京タワー
//float wany=35.658581;//目的地の緯度
//int wanx=139498598;//目的地の経度わかば中央
//int wany=35505652;//目的地の緯度
//int wanx=139375330;//目的地の経度ランドマーク
//int wany=35271653;//目的地の緯度
int wanx=139.767125;//目的地の経度東京駅
int wany=35.681236;//目的地の緯度
float angl1;//GPSから求まる行きたい角度
float angl2;//マグネットからわかる向いている角度
float many=0;


void setup()
{
  pinMode(22,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  

  Serial.begin(115200);
int SDA = 19;
int SCL = 21;
Wire.begin(SDA, SCL);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  if (connectionState == 0)
  {
    connectionState = imu.begin();
    while (connectionState == 0)
    {
      Serial.println("Failed to communicate with LSM9DS1.");
      Serial.println("Double-check wiring.");
      Serial.println("Default settings in this sketch will "
                     "work for an out of the box LSM9DS1 "
                     "Breakout, but may need to be modified "
                     "if the board jumpers are.");
      Serial.print("Connection Status: ");
      Serial.println(imu.begin());
      delay(1000);
      connectionState = imu.begin();
      Serial.println("------------------------------------------------------\n");
    }if(connectionState!=1){
      Serial.print("connectionState: ");
      Serial.println(connectionState);
    }
  }

  Serial.println("Waiting for connections...");
 
  Blynk.begin(auth);

  //ＬＥＤを全部点灯

 /*
while(many<=2)
{
  
 magx=imu.calcMag(imu.mx);
 if(magx<=0)
 {
  many=many+1;
  while(magx<=0)
  {
    Serial.println("wihe1");
    magx=imu.calcMag(imu.mx);
    Serial.println(magx);
  }
 }

  Serial.println("wihe2");
 if(magx>ax)
 {
  ax=magx;
 }

 if(magx<bx)
 {
  bx=magx;
 }
}
ax=ax-bx;
many=0;


Serial.println("Done");
//LEDを全部３回点滅
*/
}


BLYNK_WRITE(V0)
{
GpsParam gps(param);
  // Print 6 decimal places for Lat
  gpsy = (gps.getLat(), 7)*10000000;
  gpsx = (gps.getLon(),7)*10000000;
  
  Serial.println(gps.getLat(), 7);
  Serial.println(gps.getLon(),7);

  //many=1;
//  float latitude = param[0].asFloat();
//  float longitude = param[1].asFloat();
//  float altitude = param[2].asFloat();
//  float speed = param[3].asFloat();
//
//  Serial.print("latitude");
//  Serial.print(latitude);
//  Serial.print("longitude");
//  Serial.print(longitude);
//  Serial.print("altitude");
//  Serial.print(altitude);
//  Serial.print("speed");
//  Serial.print(speed);
}



void loop()
{

  /*
  imu.readMag(); Serial.print("M: ");
  Serial.print(imu.calcMag(imu.mx), 2);
  Serial.print(", ");
  Serial.print(imu.calcMag(imu.my), 2);
  Serial.print(", ");
  Serial.print(imu.calcMag(imu.mz), 2);
  Serial.println(" gauss");
*/
  
Blynk.run();
    Serial.print("Rasinwan");
    delay(500);
    

    //GPSの値から方向の算出
    imu.readMag(); 
    magx=imu.calcMag(imu.mx);
    magy=imu.calcMag(imu.my);

    double magx = imu.calcMag(imu.mx);
  double magy = imu.calcMag(imu.my);

  //double houi = 180 * atan2(magy,magx)/PI;
  
  //houi = houi * (180.0/PI);
 gpsx2=gpsx-wanx;
 gpsy2=gpsy-wany;
 if(gpsx2>=0)
 {
  if(gpsy2>=0)
  {
   angl1=1;
  }
  else
  {
   angl1=2;
  }
 }
 else
 {
  if(gpsy2>=0)
  {
   angl1=4;
  }
  else
  {
   angl1=3;
  }   
 }


 //マグネットの方向の算出
 if(magy<0.05)//ここ見直し
 {
  if(0.30>magx>0)
  {
   angl2=1;
  }
  else
  {
   angl2=2;
  }
 }
 else
 {
  if(0.30>magx>0)
  {
   angl2=4;
  }
  else
  {
   angl2=3;
  }
 }


 //二つの方向から向くべき方法を算出
 int angl3=angl1-angl2;
 if(angl3<=0)
 {
  angl3=angl3+4;
 }

Serial.println("led");

if(angl3==1)
{
digitalWrite(22,LOW);
digitalWrite(16,HIGH);
digitalWrite(17,LOW);
}
else if(angl3==2)
{
digitalWrite(22,HIGH);
digitalWrite(16,LOW);
digitalWrite(17,LOW);
}
else if(angl3==3)
{
  digitalWrite(22,LOW);
digitalWrite(16,LOW);
digitalWrite(17,LOW);
}
else
{
  digitalWrite(22,LOW);
digitalWrite(16,LOW);
digitalWrite(17,HIGH);
}

 //Serial.println(magy);
 //Serial.println(magx);
Serial.println(angl3);
Serial.println(magx);
Serial.println(magy);
Serial.println(angl2);
//erial.println((magx-bx)/180*bx);
}
