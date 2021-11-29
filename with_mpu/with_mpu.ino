#include <MPU6050_tockn.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;

SoftwareSerial mySerial(6, 7);
String buffers;
File myFile;
int incomingByte = 0; // for incoming serial data
int bytesayi;
MPU6050 mpu6050(Wire);
String readstring;
float spin;
float privSpin = 0.0;
int spincnt = 0;
int pktnum=0;
int a=0;
int16_t gyroX, gyroxRate;
int16_t gyroY, gyroyRate;
int16_t gyroZ, gyrozRate;
float gyroxAngle=0;
float gyroyAngle=0;
float gyrozAngle=0;
unsigned long currTime, prevTime=0, loopTime;
volatile float accAnglex, gyroAnglex, currentAnglex, prevAnglex=0, errorx, prevErrorx=0, errorSumx=0;
volatile float accAngley, gyroAngley, currentAngley, prevAngley=0, errory, prevErrory=0, errorSumy=0;
volatile float accAnglez, gyroAnglez, currentAnglez, prevAnglez=0, errorz, prevErrorz=0, errorSumz=0;
String gelen;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mpu6050.begin();
  mpu6050.setGyroOffsets(-3.48, -1.45, 0.07);
//
  mySerial.begin(9600);
  pinMode(9,OUTPUT);
   myservo1.attach(3,1000,2000);
  myservo2.attach(5,1000,2000);
   myservo1.write(0);
   myservo2.write(0);
  pinMode(10,INPUT_PULLUP);
  SD.begin(4);
//    while(!Serial)
//  {
//    ;
//  }

//  if (!SD.begin(4)) {
//    while (1);
//  }
}


uint32_t timer = millis();

void loop() {
  mpu6050.update();
  
   if (timer > millis())  timer = millis();
//delay(50);
  //mpu6050.update();
  
  if (millis() - timer > 100) { 
    timer = millis(); // reset the timer
if(a%10==0){
  pktnum=pktnum+1;
}

if(mySerial.available()){
    
    buffers = mySerial.readStringUntil('/');
//    while(mySerial.available());
    Serial.print(buffers);
     mpuData();
    Serial.print(",");
    Serial.print(spincnt);
    Serial.print(",");
    Serial.println("Hayır");
    
     myFile = SD.open("test.txt", FILE_WRITE);
     if(myFile)
  {
    myFile.print("#34699");
    myFile.print(",");
    myFile.print(pktnum);
    myFile.print(",");
    myFile.print("15.08.2021");
    myFile.print(",");
    myFile.print(buffers);
    myFile.print(",");
    myFile.print(accAnglex);
    myFile.print(",");
    myFile.print(accAngley);
    myFile.print(",");
    myFile.print(mpu6050.getAngleZ());
    myFile.print(",");  
    myFile.print(spincnt);
    myFile.print(",");
    myFile.print("Hayır");
    myFile.println(",");
    myFile.close();
    
  }
  
     // delay(10);
//     char inByte  = 0;
//    inByte = (char)mySerial.read();
//  // ascii 97 received
//    Serial.print((char)inByte); // => prints an 'a' without the quotes
  }
  if(Serial.available())
  {
    gelen = Serial.readString();
    if(gelen == "motor\n")
    {
    myservo1.write(30);
    myservo2.write(30);
    delay(10000);
    myservo1.write(0);
    myservo2.write(0);
    }
     if(gelen == "tel\n")
    {
      digitalWrite(9,HIGH);
      delay(2000);
      digitalWrite(9,LOW);
    }
    digitalWrite(9,LOW);
  }

    //DÖNÜŞ SAYISI - BAŞLANGIÇ
    spin=mpu6050.getAngleZ();
    if (((spin - privSpin)>200) or ((spin - privSpin)< -200)){
        spincnt++;
      }
     privSpin = spin;
    //DÖNÜŞ SAYISI - BİTİŞ
    //Serial.print();

     
  
  //  delay(500);
   
  buffers="";
  }
//    }
}

void mpuData()
{
  
  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;
  
  accAnglex = atan2(mpu6050.getAccY(), mpu6050.getAccZ())*RAD_TO_DEG;
  gyroxRate = map(mpu6050.getGyroAngleX(), -32768, 32767, -250, 250);
  gyroxAngle = (float)gyroxRate*loopTime/1000;
//  ax=abs(ax);
  currentAnglex = 0.9934*(prevAnglex + gyroxAngle) + 0.0066*(accAnglex);
  Serial.print(",");
  Serial.print(currentAnglex);
  Serial.print(",");
  prevAnglex = currentAnglex;

  accAngley = atan2(mpu6050.getAccZ(), mpu6050.getAccX())*RAD_TO_DEG;
   gyroyRate = map(mpu6050.getGyroAngleY(), -32768, 32767, -250, 250);
  gyroyAngle = gyroyAngle + (float)gyroyRate*loopTime/1000;
//  ay=abs(ay);
  currentAngley = 0.9934*(prevAngley + gyroyAngle) + 0.0066*(accAngley);
  Serial.print(currentAngley);
  Serial.print(",");
  prevAngley = currentAngley;

   accAnglez = atan2(mpu6050.getAccX(), mpu6050.getAccY())*RAD_TO_DEG;
   gyrozRate = map(mpu6050.getGyroAngleZ(), -32768, 32767, -250, 250);
  gyrozAngle = gyrozAngle + (float)gyrozRate*loopTime/1000;
//  ay=abs(ay);
  currentAnglez = 0.9934*(prevAnglez + gyrozAngle) + 0.0066*(accAnglez);
  Serial.print(currentAnglez);
  prevAnglez = currentAnglez;
}
