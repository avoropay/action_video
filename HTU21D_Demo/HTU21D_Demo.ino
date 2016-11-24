/**************************************************************************/
#include "RFoutside.h"
#include "sensors.h"

#include <UTFT.h>
#include <DS1307.h>

// Init the DS1307
DS1307 rtc(4, 5);

//OLED  myOLED(SDA, SCL, 8);
// Usage: myGLCD(<model code>, SDA, SCL, CS, RST[, RS]);
//
// When using the DM-TFT18-101 and shield from DisplayModule you should use the following:
//         UTFT myGLCD(DMTFT18101,2,3,4,6,5);
//
// When using the TFT18SP shield from ElecFreaks you should use the following:
//         UTFT myGLCD(TFT18SHLD,7,6,5,3,4);
//
UTFT myGLCD(ST7735, 11, 13, 10, 8, 9); // Remember to change the model parameter to suit your display module!

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

int firststart = 1;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
RFoutside radioOutside (0);
/**********************************************************/

Sensors mySensors;

TempSensorsStruct myTempData;
MQ135Struct myGasData;


void setup()
{
  radioOutside.begin();
  // Set the clock to run-mode
  rtc.halt(false);

  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Test init!", CENTER, 1);

  Serial.begin(9600);
  Serial.println(F(""));

  mySensors.begin();

  Serial.println(F("HTU21D sensor is present"));
  Serial.println(F("<<DEMO: Default settings, %RH: 12Bit, Temperature - 14Bit>>"));

  Serial.print(F("SIZEOF myTempData: "));
  Serial.println(sizeof(myTempData));

  Serial.print(F("SIZEOF myData: "));
  Serial.println(sizeof(myGasData));

}

void loop()
{
  if (firststart) {
    myGLCD.clrScr();
    firststart = false;
  }

/*  myTempData.myHumidity = myHTU21D.readHumidity();
  myTempData.myCHumidity = myHTU21D.readCompensatedHumidity();
  myTempData.myTemperature = myHTU21D.readTemperature();*/

  mySensors.getSensors(&myTempData);
  
  myGLCD.setFont(SmallFont);
  myGLCD.print("Humidity (%):", LEFT, 0);
  myGLCD.printNumF(myTempData.myHumidity, 2, LEFT, 14);
  myGLCD.printNumF(myTempData.myCHumidity, 2, RIGHT, 14);

  //    myGLCD.setFont(SmallFont);
  myGLCD.print("Temperature (*C):", LEFT, 26);
  //myGLCD.setFont(BigFont);
  myGLCD.printNumF(myTempData.myTemperature, 2 , RIGHT, 40);
  myGLCD.print("DS:", LEFT, 40);
  myGLCD.printNumF(myTempData.myDSTemperature, 2 , myGLCD.getFontYsize() * 3, 40);

  // sensorValue = analogRead(0);       // read analog input pin 0
  mySensors.getGasSensors(&myGasData,myTempData.myTemperature, myTempData.myHumidity);
  /*myData.ppm = gasSensor.getPPM();
  myData.ppmC = gasSensor.getCorrectedPPM(myTempData.myTemperature, myTempData.myHumidity);
  myData.ppmCZ = gasSensor.getCorrectedRZero(myTempData.myTemperature, myTempData.myHumidity);
  myData.ppmZ = gasSensor.getRZero();
*/
  Serial.println(F(""));

  Serial.println(myGasData.ppm);  // prints the value read
  myGLCD.printNumF(myGasData.ppm, 3, LEFT, 64);

  myGLCD.printNumF(myGasData.ppmC, 3, RIGHT, 64);

  myGLCD.printNumF(myGasData.ppmZ, 3, LEFT, 118);
  myGLCD.printNumF(myGasData.ppmCZ, 3, RIGHT, 118);

  Serial.println(F(""));
  // Display time centered on the upper line
  //lcd.setCursor(4, 0);
  //lcd.print(rtc.getTimeStr());
  Serial.println(rtc.getTimeStr());
  myGLCD.print(rtc.getTimeStr(), LEFT, 52);

  // Display date in the lower right corner
  //lcd.setCursor(6, 1);
  //lcd.print(rtc.getDateStr());
  Serial.println(rtc.getDateStr());
  myGLCD.print(rtc.getDateStr(), RIGHT, 52);

  // Display abbreviated Day-of-Week in the lower left corner
  //lcd.setCursor(0, 1);
  //lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  Serial.println(rtc.getDOWStr(FORMAT_SHORT));
  //myGLCD.print(rtc.getDOWStr(FORMAT_SHORT), LEFT, 64);

/*
  Serial.println(F(""));
  Serial.print(F("Humidity: "));
  Serial.print(myTempData.myHumidity);
  Serial.println(F(" +-2%RH     in range 20%RH - 80%RH at 25deg.C only"));

  Serial.println(F(""));
  Serial.print(F("Compensated Humidity: "));
  Serial.print(myTempData.myCHumidity);
  Serial.println(F(" +-2%RH     in range 0%RH - 100%RH at tmp. range 0deg.C - 80deg.C"));

  Serial.println(F(""));
  Serial.print(F("Temperature: "));
  Serial.print(myTempData.myTemperature);
  Serial.println(F(" +-0.5 deg.C"));

  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("<<DEMO: %RH: 11Bit, Temperature - 11Bit>>"));
  myHTU21D.setResolution(HTU21D_RES_RH11_TEMP11);

  Serial.println(F(""));
  Serial.print(F("Humidity: "));
  Serial.print(myHTU21D.readHumidity());
  Serial.println(F(" +-2%RH     in range 20%RH - 80%RH at 25deg.C only"));

  Serial.println(F(""));
  Serial.print(F("Compensated Humidity: "));
  Serial.print(myHTU21D.readCompensatedHumidity());
  Serial.println(F(" +-2%RH     in range 0%RH - 100%RH at tmp. range 0deg.C - 80deg.C"));

  Serial.println(F(""));
  Serial.print(F("Temperature: "));
  Serial.print(myHTU21D.readTemperature());
  Serial.println(F(" +-0.5 deg.C"));


  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("<<DEMO: Battery Status>>"));

  myData.myBattery = myHTU21D.batteryStatus();
*/
  //char myBatteryStatus[120] = "";
  if (myGasData.myBattery == true)
  {
    //    myBatteryStatus[120] = "Battery OK. Level > 2.25v";
    Serial.println("Battery OK. Level > 2.25v");
    myGLCD.print("Battery OK > 2.25v", RIGHT, 78);
  }
  else
  {
    //    myBatteryStatus[] = "Battery LOW. Level < 2.25v";
    Serial.println("Battery LOW. Level < 2.25v");
    myGLCD.print("Battery LOW < 2.25v", RIGHT, 78);
  }
  //myGLCD.print(myBatteryStatus, RIGHT, 64);

  if (isnan(myTempData.myDTHTemperature) || isnan(myTempData.myDTHHumidity)) {
    myGLCD.setFont(SmallFont);
    myGLCD.print("Failed to read from DHT", LEFT, 92);
  } else {
    myGLCD.setFont(SmallFont);
    myGLCD.print("Humidity (%):", LEFT, 92);
    myGLCD.printNumF(myTempData.myDTHHumidity, 2, RIGHT, 92);

    myGLCD.setFont(SmallFont);
    myGLCD.print("Temperature *C:", LEFT, 104);
    myGLCD.printNumF(myTempData.myDTHTemperature, 2, RIGHT, 104);
  }

  /* UNCOMENT FOR SENSOR'S FUNCTIONALITY DIAGNOSTIC ONLY. TEMPERATURE SHOULD INCREASE BY 0.5-1.5 deg.C and HUMIDITY DROP
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("<<DEMO: built-in Heater test>>"));

    Serial.println(F(""));
    Serial.println(F("<<built-in Heater OFF>>"));
    myHTU21D.setHeater(OFF);

    Serial.println(F(""));
    Serial.print(F("Compensated Humidity: "));
    Serial.print(myHTU21D.readCompensatedHumidity());
    Serial.println(F(" %RH"));

    Serial.println(F(""));
    Serial.print(F("Temperature: "));
    Serial.print(myHTU21D.readTemperature());
    Serial.println(F(" deg.C"));

    Serial.println(F(""));
    Serial.println(F("<<built-in Heater ON. Wait for 5 sec.>>"));
    myHTU21D.setHeater(ON);
    delay(5000);

    Serial.println(F(""));
    Serial.println(F(""));
    Serial.print(F("Compensated Humidity: "));
    Serial.print(myHTU21D.readCompensatedHumidity());
    Serial.println(F(" %RH"));

    Serial.println(F(""));
    Serial.print(F("Temperature: "));
    Serial.print(myHTU21D.readTemperature());
    Serial.println(F(" deg.C"));

    Serial.println(F(""));
    Serial.println(F("<<built-in Heater OFF. Wait for 5 sec. to cool it down>>"));
    myHTU21D.setHeater(OFF);
    delay(5000);

    Serial.println(F(""));
    Serial.print(F("Compensated Humidity: "));
    Serial.print(myHTU21D.readCompensatedHumidity());
    Serial.println(F(" %RH"));

    Serial.println(F(""));
    Serial.print(F("Temperature: "));
    Serial.print(myHTU21D.readTemperature());
    Serial.println(F(" deg.C"));
  */

  Serial.println(F(""));
  Serial.println(F(""));
  Serial.print(F("DEMO: Start over again in 5 sec."));

  myGasData._micros = micros();
  radioOutside.sendOut(&myGasData, sizeof(myGasData));
  radioOutside.sendOut(&myTempData, sizeof(myTempData));
  delay(4000);
}


