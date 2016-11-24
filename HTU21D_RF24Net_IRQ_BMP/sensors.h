/**************************************************************************/
/*!
  @file     FRoutside.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/*
     These sensor uses I2C to communicate, 2 pins are required to interface

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

#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <HTU21D.h>
#include "fifoqueue.h"


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 6

#include "MQ135.h"

#include "DHT.h"

//#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11

#define DHTPIN22 22     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE22 DHT11   // DHT 22

#include <Wire.h>
#include <Adafruit_BMP085.h>
// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

struct MQ135Struct {
  float ppm;
  float ppmC;
  float ppmCZ;
  float ppmZ;
  float ppmAvgCZ;
  float ppmAvgZ;
  boolean myBattery;
};

struct TempSensorsStruct {
  float myHumidity;
  float myCHumidity;
  float myTemperature;
  float myDSTemperature;
  float myDSTemperature1;
  float myDTHHumidity;
  float myDTHTemperature;
  float myDTH22Humidity;
  float myDTH22Temperature;
  float myTermistorTemperature;
  float myTermistorTemperature1;
  /* BMP180 */
  float myBMP180Temperature;
  float myBMP180Pressure;
  float myBMP180PressureMmHg;
  float myBMP180Altitude;
  /* BH1750FVI */
  uint16_t lux;
};

struct TempDTHSensorsStruct {
  float myDTHHumidity;
  float myDTHTemperature;
};

struct RF24NetworkHeaderL
{
  uint16_t from_node; /**< Logical address where the message was generated */
  uint16_t to_node; /**< Logical address where the message is going */
  uint16_t id; /**< Sequential message ID, incremented every message */
  unsigned char type; /**< Type of the packet.  0-127 are user-defined types, 128-255 are reserved for system */
  unsigned char reserved; /**< Reserved for future use */

  static uint16_t next_id; /**< The message ID of the next message to be sent */
};

struct TempDTHSensorsStructHeader {
  RF24NetworkHeaderL header;
  float myDTHHumidity;
  float myDTHTemperature;
};

class Sensors {
  public:
    float ppmCZ, ppmZ;
    queue pppZQueue = queue(200);
    queue pppCZQueue = queue(200);
    queue PressureMmHgQueue = queue(200);

    Sensors(void);
    boolean begin();
    void getSensors(TempSensorsStruct* buf);
    void getGasSensors(MQ135Struct* buf, float Temp, float Humidity);
};

#endif


