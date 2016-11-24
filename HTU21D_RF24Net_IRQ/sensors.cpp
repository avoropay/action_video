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

  Уровень СО2 (ppm) 1000 ppm = 0,1%

  Качество воздуха и его влияние на человека
  Атмосферный воздух
  300- 400 ppm
  Идеальный для здоровья человека 400-600 ppm
  Нормальное качество воздуха До 600 ppm
  Уровень. рекомендованный для спален, детских садов и школ 600-800 ppm
  Появляются единичные жалобы на  качество воздуха 800-1000 ppm
  Более частые жалобы на качество воздуха. Выше 1000 ppm

  Общий дискомфорт, слабость, головная боль, проблемы с концентрацией внимания. Растет число ошибок в работе. Начинаются негативные изменения в ДНК.
  Выше  2000 ppm
  Может вызвать серьезные отклонения в здоровье людей. Количество ошибок в работе сильно возрастает. 70% сотрудников не могут сосредоточиться на работе

*/

HTU21D myHTU21D;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensorsDS(&oneWire);

//DHT dht(DHTPIN, DHTTYPE);

DHT dht22(DHTPIN22, DHTTYPE22);

int sensorMQ135Value;

MQ135 gasSensor = MQ135(A0);

Adafruit_BMP085 bmp;

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  return Temp;
}

/* classes */

Sensors::Sensors(void) {};

boolean Sensors::begin() {
  sensorsDS.begin();
  myHTU21D.begin();
  //   dht.begin();
  dht22.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }

  return true;
}

//int sensorTermistorPin = A5; // select the input pin for the potentiometer
int sensorTermistorPin1 = A10; // select the input pin for the potentiometer

void Sensors::getSensors(TempSensorsStruct* buf) {

  /* bmp180 */

  /*Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    double Pressure = bmp.readPressure();
    Serial.print(Pressure);
    Serial.println(" Pa");

    Serial.print("Pressure(mm Hg):");
    Serial.println(Pressure / 133.3);

    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    float altitude_meters = bmp.readAltitude();
    Serial.print(altitude_meters);
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Pressure = bmp.readSealevelPressure(altitude_meters);
    Serial.print(Pressure);
    Serial.println(" Pa");
    Serial.print("Pressure(mm Hg):");
    Serial.println(Pressure / 133.3);
  */
  buf->myBMP180Temperature = bmp.readTemperature();
  buf->myBMP180Pressure = bmp.readPressure();
  buf->myBMP180PressureMmHg = buf->myBMP180Pressure  / 133.3;
  buf->myBMP180Altitude = bmp.readAltitude();


  //  int readTermistorVal = analogRead(sensorTermistorPin);
  //  buf->myTermistorTemperature = Thermistor(readTermistorVal);

  /*readTermistorVal = analogRead(sensorTermistorPin1);
    buf->myTermistorTemperature1 = readTermistorVal; //Thermistor(readTermistorVal);*/

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
  buf->myDSTemperature1 = sensorsDS.getTempCByIndex(1);
  Serial.println(buf->myDSTemperature);

  //  buf->myDTHHumidity = dht.readHumidity();
  //  buf->myDTHTemperature = dht.readTemperature();
  buf->myDTH22Humidity = dht22.readHumidity();
  buf->myDTH22Temperature = dht22.readTemperature();
}

void Sensors::getGasSensors(MQ135Struct* buf, float Temp, float Humidity) {
  buf->ppm = gasSensor.getPPM();
  buf->ppmC = gasSensor.getCorrectedPPM(Temp, Humidity);
  buf->ppmCZ = gasSensor.getCorrectedRZero(Temp, Humidity);
  buf->ppmZ = gasSensor.getRZero();
  buf->myBattery = myHTU21D.batteryStatus();

  // add queue
  pppZQueue.push(buf->ppmZ);
  pppCZQueue.push(buf->ppmCZ);

  buf->ppmAvgZ = pppZQueue.avg();
  buf->ppmAvgCZ = pppCZQueue.avg();
}
