/**************************************************************************/
/*!
@file     FRoutside.h
@author   A.Voropay
@license  GNU GPLv3



@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/

#include "sensors.h"

/*
  HTU21D(resolution)

  resolution:
  HTU21D_RES_RH12_TEMP14 - RH: 12Bit, measuring time 16ms. Temperature: 14Bit, measuring time 50ms
  HTU21D_RES_RH8_TEMP12  - RH: 8Bit,  measuring time 8ms.  Temperature: 12Bit, measuring time 25ms
  HTU21D_RES_RH10_TEMP13 - RH: 10Bit, measuring time 5ms.  Temperature: 13Bit, measuring time 13ms.
  HTU21D_RES_RH11_TEMP11 - RH: 11Bit, measuring time 3ms.  Temperature: 11Bit, measuring time 7ms.


  DEFAULT
  HTU21D(HTU21D_RES_RH12_TEMP14)
*/

HTU21D myHTU21D;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensorsDS(&oneWire);

DHT dht(DHTPIN, DHTTYPE);

int sensorMQ135Value;

MQ135 gasSensor = MQ135(A0);

/* classes */

Sensors::Sensors(void) {};

boolean Sensors::begin() {
     sensorsDS.begin();
     myHTU21D.begin();
     dht.begin();
 return true;
}

void Sensors::getSensors(TempSensorsStruct* buf) {
  myHTU21D.setResolution(HTU21D_RES_RH11_TEMP11);
  buf->myHumidity = myHTU21D.readHumidity();
  buf->myCHumidity = myHTU21D.readCompensatedHumidity();
  buf->myTemperature = myHTU21D.readTemperature();

  Serial.print("Requesting temperatures...");
  sensorsDS.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  buf->myDSTemperature = sensorsDS.getTempCByIndex(0);
  Serial.println(buf->myDSTemperature);

  buf->myDTHHumidity = dht.readHumidity();
  buf->myDTHTemperature = dht.readTemperature();
}

void Sensors::getGasSensors(MQ135Struct* buf, float Temp, float Humidity) {
  buf->ppm = gasSensor.getPPM();
  buf->ppmC = gasSensor.getCorrectedPPM(Temp, Humidity);
  buf->ppmCZ = gasSensor.getCorrectedRZero(Temp, Humidity);
  buf->ppmZ = gasSensor.getRZero();
  buf->myBattery = myHTU21D.batteryStatus();
}
