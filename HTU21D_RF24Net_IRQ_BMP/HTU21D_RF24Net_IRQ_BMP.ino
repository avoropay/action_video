/**************************************************************************/
//#include <Firmata.h>

#include "systeminfo.h"

byte analogPin;

#include "RFoutside.h"
#include "sensors.h"
#include <RF24.h>
#include <SPI.h>
#include <MemoryFree.h>

#include <UTFT.h>
#include <DS1307.h>

#include "makeGRFs.h"

#include <math.h>
int sensorPin = A1; // select the input pin for the potentiometer
int sensorHallPin = A7; // select the input pin for the potentiometer


// Init the DS1307
DS1307 rtc(4, 5);

DS1307_RAM ramBuffer;

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

TempDTHSensorsStruct myTempDTHData;

queue PressureMmHgQueue = queue(200);

int firststart = 1;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
//RFoutside radioOutside (0);
/**********************************************************/

Sensors mySensors;

TempSensorsStruct myTempData;
MQ135Struct myGasData;

graphic myGRFs(&myGLCD);

//pack_for_send pack_list[2];

//pack_for_send pack_list_read[1];

unsigned long last_chack_sensors;
const unsigned long interval_check = 4000; //ms

unsigned long last_display_instrument_readings;
const unsigned long interval_display_instrument_readings = 5000; //ms

const uint16_t this_node = 00;        // Address of our node in Octal format
const uint16_t other_node = 01;       // Address of the other node in Octal format

volatile uint32_t round_trip_timer = 0;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
RFoutside radioOutside (0);
/**********************************************************/


int display_mode_buttonpin = 46; // define the key switch sensor interface
int display_mode = 1;
int display_mode_button_flag = 0;
boolean display_mode_changed = false;

int buf[158];

void setup()
{
  pinMode (display_mode_buttonpin, INPUT); // define the key switch sensor output interface

  //radioOutside.begin();
  // Set the clock to run-mode
  rtc.halt(false);

  // The following lines can be commented out to use the values already stored in the DS1307
  // rtc.setDOW(THURSDAY);        // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 50, 30);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 5, 2016);   // Set the date to October 3th, 2010

  // Set SQW/Out rate to 1Hz, and enable SQW
  rtc.setSQWRate(SQW_RATE_32K);
  rtc.enableSQW(true);
  rtc.setOutput(true);

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Test init!", CENTER, 1);

  myGRFs.begin();

  Serial.begin(9600);

  mySensors.begin();

  Serial.println(F("HTU21D sensor is present"));
  radioOutside.begin();

  PrintCPUstats();

  while (digitalRead(display_mode_buttonpin) == HIGH) {};

  delay(2000);
}

void loop()
{
  if (firststart) {
    display_mode = 1;
    myGLCD.clrScr();
  }

  display_mode_changed = false;

  if (digitalRead(display_mode_buttonpin) == LOW && display_mode_button_flag == 0) //если кнопка нажата
    // и перемення flag равна 0 , то ...
  {
    display_mode++;
    display_mode_button_flag = 1;
    display_mode_changed = true;

    //это нужно для того что бы с каждым нажатием кнопки
    //происходило только одно действие
    // плюс защита от "дребезга"  100%

    if (display_mode > 2) //ограничим количество режимов
    {
      display_mode = 1; //так как мы используем только одну кнопку,
      // то переключать режимы будем циклично
    }
    myGLCD.clrScr();
  }

  if (digitalRead(display_mode_buttonpin) == HIGH && display_mode_button_flag == 1) //если кнопка НЕ нажата
    //и переменная flag равна - 1 ,то ...
  {
    display_mode_button_flag = 0; //обнуляем переменную "knopka"
  }

  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( (now - last_chack_sensors >= interval_check) || firststart)
  {
    last_chack_sensors = now;
    mySensors.getSensors(&myTempData);
    mySensors.getGasSensors(&myGasData, myTempData.myTemperature, myTempData.myHumidity);
  }

  //radioOutside.sendOut(pack_list, sizeof(pack_list) / sizeof(pack_for_send));

  radioOutside.readIn(&myTempDTHData, sizeof(myTempDTHData));

  myTempData.myDTHHumidity = myTempDTHData.myDTHHumidity;
  myTempData.myDTHTemperature = myTempDTHData.myDTHTemperature;

 // PressureMmHgQueue.push(buf->myBMP180PressureMmHg);

  if ( (now - last_display_instrument_readings >= interval_display_instrument_readings) || firststart || display_mode_changed)
  {
    last_display_instrument_readings = now;

    Serial.print("freeMemory()=");
    Serial.println(freeMemory());

    switch (display_mode)
    {
      case 1:
        PrintModeFirst();
        break;
      case 2:
        if (display_mode_changed) {
          myGRFs.displaySystemAxes();
        }

        myGRFs.displayGRF(&PressureMmHgQueue, 730, 760);

        break;
    }
  }

  if (firststart) {
    firststart = false;
  }
}

void PrintModeFirst()
{
  myGLCD.setColor(VGA_WHITE);

  myGLCD.setFont(SmallFont);
  myGLCD.print("Humidity (%):", LEFT, 0);
  myGLCD.print("Pressure", LEFT, 64);
  myGLCD.print("mm Hg", RIGHT, 64);
  myGLCD.print("Pa", CENTER, 78);
  //myGLCD.print("Altitude (m):", LEFT, 92);
  myGLCD.print("Light: (lux):", LEFT, 92);

  myGLCD.printNumF(myTempData.myHumidity, 2, LEFT, 14, '.', 6, ' ');
  myGLCD.printNumF(myTempData.myCHumidity, 2, CENTER, 14, '.', 6, ' ');

  /*if (isnan(myTempData.myDTHTemperature) || isnan(myTempData.myDTHHumidity)) {
    myGLCD.print("Failed to read from DHT", LEFT, 78);
    } else {*/
  myGLCD.setColor(VGA_BLACK);
  //myGLCD.fillRect(0, 78, 159, 89);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumF(myTempData.myDTHHumidity, 2, RIGHT, 14, '.', 6, ' ');
  myGLCD.printNumF(myTempData.myDTHTemperature, 2, RIGHT, 40, '.', 6, ' ');
  //}

  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Temperature (*C):", LEFT, 26);
  //myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.printNumF(myTempData.myTemperature, 2 , CENTER, 40, '.', 6, ' ');
  myGLCD.setColor(VGA_FUCHSIA);
  //myGLCD.print("DS:", LEFT, 40);
  //myGLCD.printNumF(myTempData.myDSTemperature, 2 , myGLCD.getFontYsize() * 3, 40);
  myGLCD.printNumF(myTempData.myDSTemperature, 2 , LEFT, 40, '.', 6, ' ');

  myGLCD.printNumF(myTempData.myDSTemperature1, 2, LEFT, 52, '.', 6, ' ');

  //    myGLCD.printNumF(myTempData.myTermistorTemperature, 2, CENTER, 78, '.', 6, ' ');

  //    myGLCD.printNumF(myTempData.myTermistorTemperature1, 2, RIGHT, 78, '.', 6, ' ');

  /*    int lightLevel = analogRead(A1);
      myGLCD.printNumI(lightLevel, RIGHT, 78);*/



//  Serial.println(myGasData.ppm);  // prints the value read

  /*myGLCD.setColor(VGA_SILVER);
    myGLCD.printNumF(myGasData.ppmAvgZ, 3, LEFT, 90, '.', 10, ' ');
    myGLCD.printNumF(myGasData.ppmAvgCZ, 3, RIGHT, 90, '.', 10, ' ');
  */
  myGLCD.setColor(VGA_YELLOW);
  //myGLCD.printNumF(myGasData.ppm, 3, LEFT, 118, '.', 10, ' ');
  myGLCD.print("CO2 ppm", LEFT, 116);
  myGLCD.printNumF(myGasData.ppmC, 3, RIGHT, 116, '.', 10, ' ');

  /*  myGLCD.setColor(VGA_SILVER);
    myGLCD.printNumF(myGasData.ppmZ, 3, LEFT, 118, '.', 10, ' ');
    myGLCD.printNumF(myGasData.ppmCZ, 3, RIGHT, 118, '.', 10, ' ');
  */

  myGLCD.setColor(VGA_FUCHSIA);
  //Serial.println(F(""));
  //Serial.println(rtc.getTimeStr());
  //myGLCD.print(rtc.getTimeStr(), LEFT, 52);
  myGLCD.print(rtc.getTimeStr(), LEFT, 104);

  myGLCD.setColor(VGA_AQUA);
  // Display date in the lower right corner
  //Serial.println(rtc.getDateStr());
  //myGLCD.print(rtc.getDateStr(), RIGHT, 104);
  myGLCD.print(rtc.getDateStr(), RIGHT, 104);

  /*myGLCD.setColor(VGA_LIME);
    // Display abbreviated Day-of-Week in the lower left corner
    Serial.println(rtc.getDOWStr(FORMAT_SHORT));
    myGLCD.print(rtc.getDOWStr(FORMAT_SHORT), LEFT, 64);
  */
  /*  myGLCD.setColor(VGA_PURPLE);
    if (myGasData.myBattery == true)
    {
      //    myBatteryStatus[120] = "Battery OK. Level > 2.25v";
      Serial.println("Battery OK. Level > 2.25v");
      myGLCD.print("VCC OK", RIGHT, 64);
    }
    else
    {
      //    myBatteryStatus[] = "Battery LOW. Level < 2.25v";
      Serial.println("Battery LOW. Level < 2.25v");
      myGLCD.print("VCC LOW", RIGHT, 64);
    }
  */
  myGLCD.setColor(VGA_AQUA);
  myGLCD.setFont(SmallFont);
  /*    //if (isnan(myTempData.myDTH22Temperature) || isnan(myTempData.myDTH22Humidity)) {
           //myGLCD.print("Failed to read from DHT", LEFT, 92);
    //      } else {
           myGLCD.print("Humidity (%):", LEFT, 92);
           myGLCD.printNumF(myTempData.myDTH22Humidity, 2, RIGHT, 92);

           myGLCD.print("Temperature *C:", LEFT, 104);
           myGLCD.printNumF(myTempData.myDTH22Temperature, 2, RIGHT, 104);
     // }

  */
  myGLCD.printNumF(myTempData.myDTH22Humidity, 2, RIGHT, 0, '.', 6, ' ');

  myGLCD.printNumF(myTempData.myDTH22Temperature, 2, RIGHT, 52, '.', 6, ' ');

  myGLCD.setColor(255, 128, 0);

  myGLCD.printNumF(myTempData.myBMP180Temperature, 2, CENTER, 52, '.', 6, ' ');


  myGLCD.printNumI(myTempData.myBMP180Pressure, LEFT, 78, 8, ' ');

  myGLCD.printNumF(myTempData.myBMP180PressureMmHg, 2, RIGHT, 78, '.', 6, ' ');

  //myGLCD.printNumF(myTempData.myBMP180Altitude, 2, RIGHT, 92, '.', 6, ' ');
  myGLCD.printNumI(myTempData.lux, RIGHT, 92, 6, ' ');


  //myGLCD.print(myBatteryStatus, RIGHT, 64);

  /*   if (isnan(myTempData.myDTHTemperature) || isnan(myTempData.myDTHHumidity)) {
       myGLCD.setFont(SmallFont);
       myGLCD.print("Failed to read from DHT", LEFT, 92);
     } else {
       myGLCD.setFont(SmallFont);
    //      myGLCD.print("Humidity (%):", LEFT, 92);
    //      myGLCD.printNumF(myTempData.myDTHHumidity, 2, RIGHT, 92);

       myGLCD.print("Temperature *C:", LEFT, 104);
       myGLCD.printNumF(myTempData.myDTHTemperature, 2, RIGHT, 104);
     }*/
  /*    for (int i = 0; i < 56; i++)
        ramBuffer.cell[i] = 0;

      ramBuffer = rtc.readBuffer();
      bufferDump("Buffer read from RAM...");

      int temp;
      Serial.println("Reading address 18 (0x12). This should return 18, 0x12.");
      temp = rtc.peek(18);
      Serial.print("Return value: ");
      Serial.print(temp, DEC);
      Serial.print(", 0x");
      Serial.println(temp, HEX);
  */
}


