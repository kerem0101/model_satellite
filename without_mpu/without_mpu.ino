#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <I2Cdev.h> //I2C kütüphanesini ekledik
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// #include <MPU6050_tockn.h>


SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true
#define SEALEVELPRESSURE_HPA (1013.25)
//MPU6050 mpu6050(Wire);

int16_t pktnum=0;
#define potpin A0 //Potansiyometreyi A0 pinine tanımlıyoruz
int deger = 0; //"Değer" adlı 0 başlangıçlı bir değişken tanımlıyoruz

float old_alt=0.0;
float new_alt;
char* uydu_stat="Beklemede";
bool ayrilmadurum=false;
String myData="";
//float spin;
//float privSpin = 0.0;
//int spincnt = 0;

//char uydustatchar[50]={" "};
//char bmePress[20]={" "};
//char bmeAlt[20]={" "};
//char bmeTemp[20]={" "};
//char gpspeed[20]={" "};
//char degerchar[20]={" "};
//char mpux[20]={" "};
//char mpuy[20]={" "};
//char mpuz[20]={" "};
//char gpsmin[20]={" "};
//char gpsday[20]={" "};
//char gpsmonth[20]={" "};
//char gpsyear[20]={" "};
Adafruit_BME280 bme;
uint32_t timer = millis();
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy
long int zaman;
long int vzaman;
void setup()  
{
  Wire.begin();
  Serial.begin(9600);
  GPS.begin(9600);

  //mpu6050.begin();
  //mpu6050.setGyroOffsets(-3.48, -1.45, 0.07);
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);

  //delay(1000);
  mySerial.println(PMTK_Q_RELEASE);
   
   if (!bme.begin(0x76)) {
    while (1);
  }

  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);


}


SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();

}

void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

void loop()                     // run over and over again
{
  if (! usingInterrupt) {
    char c = GPS.read();    
  }
    if (GPS.newNMEAreceived()) {
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }


  //mpu6050.update();
  


    pktnum=pktnum+1;

    deger = analogRead(potpin);
    
    //UYDU STATÜSÜ - BAŞLANGIÇ
    new_alt =bme.readAltitude(SEALEVELPRESSURE_HPA);
    
    if((new_alt-old_alt)<0.2 && ((new_alt-old_alt)>-0.2)){
      char* uydu_stat="Beklemede";
    }
    else if ((new_alt-old_alt)>0.5){
      char* uydu_stat="Yukselme";
    }
    else if((!ayrilmadurum) && ((old_alt-new_alt)>0.5)){
      char* uydu_stat="Model Uydu Inis";
    }
    else if((ayrilmadurum)==true){
      char* uydu_stat="Ayrilma";
      
    }
    else if ((ayrilmadurum) && ((old_alt-new_alt)>0.5)){
      char* uydu_stat="Gorev Yuku Inis";
    }
    else{
      char* uydu_stat="Kurtarma";
    }
    old_alt = new_alt;
      //UYDU STATÜSÜ - BİTİŞ
/*
    //DÖNÜŞ SAYISI - BAŞLANGIÇ
    spin=mpu6050.getAngleZ();
    if (((spin - privSpin)>200) or ((spin - privSpin)< -200)){
        spincnt++;
      }
     privSpin = spin;
    //DÖNÜŞ SAYISI - BİTİŞ  */

//    dtostrf((bme.readPressure() / 100.0F), 4, 2, bmePress);
//    dtostrf(bme.readAltitude(SEALEVELPRESSURE_HPA), 4, 2, bmeAlt);
//    dtostrf(bme.readTemperature(), 4, 2, bmeTemp);
//    dtostrf((deger/68.2), 4, 2, degerchar);
//    dtostrf(GPS.speed, 4, 2, gpspeed);
//    dtostrf(GPS.minute, 4, 2, gpsmin);

    
   // char *all[]={"#34699,",pktnum,",",GPS.day,"/",GPS.month,"/20",GPS.year,",",GPS.hour,"/",GPS.minute,"/",GPS.seconds,",",bmePress,",",bmeAlt,",",gpspeed,","};
    //Serial.write(*all);
//
 if (timer > millis())  timer = millis();
//delay(50);
  //mpu6050.update();
  
  if (millis() - timer > 100) { 
   // timer = millis(); // reset the timer
//  myData+="#34699";
//  myData+="|";
//  myData+=pktnum;
//  myData+="|";
//  myData+=GPS.day;
//  myData+="|";
//  myData+=GPS.month;
//  myData+="|";
//  myData+=GPS.year;
//  myData+="|";
//  myData+=GPS.hour;
//  myData+="|";
//  myData+=GPS.minute;
//  myData+="|";
//  myData+=GPS.seconds;
//  myData+="|";
  myData+=bme.readPressure() / 100.0F;
  myData+=",";
  myData+=bme.readAltitude(SEALEVELPRESSURE_HPA);
  myData+=",";
  myData+=GPS.speed;
  myData+=",";
  myData+=(String)bme.readTemperature();
  myData+=",";
  myData+=(String)deger;
  myData+=",";
  myData+=(String)GPS.latitude;
  myData+=",";
  myData+=(String)GPS.longitude;
  myData+=",";
  myData+=(String)GPS.altitude;
  myData+=",";
  myData+=uydu_stat;
  myData+="/";
//  int sizee = sprintf(buffers,"%s,%d,%d.%d.20%d %d:%d:%d,%d.%02d,%d.%02d,%d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%s/","#34699",pktnum,GPS.day,GPS.month,GPS.year,GPS.hour,GPS.minute,GPS.seconds,
//  (int)(bme.readPressure() / 100.0F), (int)((bme.readPressure() / 100.0F)*-100)%100,(int)bme.readAltitude(SEALEVELPRESSURE_HPA), (int)(bme.readAltitude(SEALEVELPRESSURE_HPA)*100)%100,
//  (int)GPS.speed,(int)bme.readTemperature(), (int)(bme.readTemperature()*100)%100,(int)deger, (int)(deger*100)%100,(int)GPS.latitude/100, (int)(GPS.latitude)%100,(int)GPS.longitude/100, (int)(GPS.longitude)%100,
//  (int)GPS.altitude, (int)(GPS.altitude*100)%100,uydu_stat);
  zaman = millis();
  if(zaman-vzaman>500)
  {
  Serial.print(myData);
   vzaman = millis();
  }
   myData="";
   digitalWrite(8,LOW);
 // Serial.print(buffers);
 // Serial.println(GPS.longitude);
  //Serial.print(uydu_stat);
//  Serial.write(pktnum);
//  Serial.write(",");
//  Serial.write(bmeTemp);
  }
   /* 
    Serial.write("#34699,");
    Serial.write(pktnum);
    Serial.write(",");
    Serial.write(GPS.day); Serial.print('/');
    Serial.write(GPS.month); Serial.print("/20");
    Serial.write(GPS.year);
    Serial.write(",");
    Serial.write(GPS.hour); Serial.print('/');
    Serial.write(GPS.minute); Serial.print('/');
    Serial.write(GPS.seconds); Serial.print(',');
    dtostrf((bme.readPressure() / 100.0F), 4, 2, bmePress);
    Serial.write(bmePress);
    Serial.write(",");
    dtostrf(bme.readAltitude(SEALEVELPRESSURE_HPA), 4, 2, bmeAlt);
    Serial.write(bmeAlt);
    Serial.write(",");
    Serial.write(char(GPS.speed));
    dtostrf(bme.readAltitude(SEALEVELPRESSURE_HPA), 4, 2, gpspeed);
    Serial.write(",");
    dtostrf(bme.readTemperature(), 4, 2, bmeTemp);
    Serial.write(bmeTemp);
    Serial.write(",");
    dtostrf((deger/68.2), 4, 2, degerchar);
    Serial.write(degerchar);
    Serial.write(",");
    if (GPS.fix) {
      Serial.write(char(GPS.latitude, 10)); Serial.print(GPS.lat);
     Serial.write(","); 
      Serial.write(char(GPS.longitude, 10)); Serial.print(GPS.lon);
      Serial.write(","); 
      Serial.write(char(GPS.altitude));
      Serial.write(",");
    }
    uydu_stat.toCharArray( uydustatchar,50);
    Serial.write(uydustatchar);
    Serial.write(",");
    /*dtostrf(mpu6050.getAngleX(), 4, 2, mpux);
    Serial.write(char(mpu6050.getAngleX()));
    Serial.write(",");
    dtostrf(mpu6050.getAngleY(), 4, 2, mpuy);
    Serial.write(char(mpu6050.getAngleY()));
    Serial.write(",");
    dtostrf(mpu6050.getAngleZ(), 4, 2, mpuz);
    Serial.write(char(mpu6050.getAngleZ()));
    Serial.write(",");  
    Serial.write(spincnt);
    Serial.write(",");
    Serial.write("???");
    Serial.write(",");*/

}
