/**************************************************************************/
/*!
@file     FRoutside.h
@author   A.Voropay
@license  GNU GPLv3



@section  HISTORY

v1.0 - First release
*/
/*
 *   These sensor uses I2C to communicate, 2 pins are required to interface

  Connect HTU21D to pins :  SDA  SCL
  Uno, Redboard, Pro:       A4   A5
  Mega2560, Due:            20   21
  Leonardo:                 2    3

 */
/**************************************************************************/
#ifndef sensors_H
#define sensors_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <OneWire.h>
#include <Wire.h>
#include "HTU21D.h"

#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 6

#include "MQ135.h"

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 

struct MQ135Struct {
  unsigned long _micros;
  float ppm;
  float ppmC;
  float ppmCZ;
  float ppmZ;
  boolean myBattery;

};

struct TempSensorsStruct {
  unsigned long _micros;
  float myHumidity;
  float myCHumidity;
  float myTemperature;
  float myDSTemperature;
  float myDTHHumidity;
  float myDTHTemperature;
};

class Sensors {
 public:
  Sensors(void);
  boolean begin();
  void getSensors(TempSensorsStruct* buf);
  void getGasSensors(MQ135Struct* buf, float Temp, float Humidity);
};

#endif
