#include "RFoutside.h"
#include "RFoutside.h"
#include <OLED_I2C.h>

/*

  Разрешение дисплея 128*64
  Напряжение питания: 3.3-6V
  Цоколевка разъема:
  GND: Общий
  VCC: Напряжение питания
  SDA (20): Шина данных
  SCL (21): Шина тактирования данных

  Источник http://arduino-ua.com/prod1263-oled-displei-modyl-belii
*/
OLED  myOLED(SDA, SCL, 8);

extern uint8_t TinyFont[];
extern uint8_t SmallFont[];


extern int VRX;
extern int VRY;
extern int IRQ_rx;


int firststart = 1;
/*
  GND - VCC
  CE(48)  - CSN(53)
  SCK(52) - MOSI(51)
  MISO(50) - IRQ
*/


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
RFoutside radioOutside (0);
/**********************************************************/

SensorsStruct myTempDTHData;

void setup()
{
  radioOutside.begin();
  Serial.begin(9600);
  myOLED.begin();
  myOLED.setFont(SmallFont);
  myOLED.print("Test OLED", LEFT, 24);
  //myOLED.print(header.toString(), LEFT, 24);
  myOLED.update();
}

void loop()
{

  if (firststart) {
    //    display_mode = 1;
    myOLED.clrScr();
  myOLED.setFont(SmallFont);
  }

  //radioOutside.readIn(&myTempDTHData, sizeof(myTempDTHData));
  if (IRQ_rx) {
   Serial.print(F("VRX: "));
    Serial.print(VRX);
    Serial.print(F("VRY: "));
    Serial.print(VRY);
    Serial.println(F("."));
    myOLED.print(F("VRX: "), LEFT, 0);
    myOLED.print(F("VRY: "), RIGHT, 0);
    myOLED.print(sprintf("%d",VRX), LEFT, 10);
    myOLED.print(sprintf("%d",VRY), RIGHT, 10);
    myOLED.update();

    IRQ_rx = false;
  }
  if (firststart) {
    firststart = false;
  }
}
