#include "RFoutside.h"
#include <UTFT.h>

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

unsigned long last_chack_sensors = 0;
const unsigned long interval_check = 4000; //ms

pack_for_send pack_list[1];


struct initSensorsStruct {
  int Y_PIN;           // Потенциометр оси Y подключен к аналоговому входу
  int X_PIN;           // Потенциометр оси X подключен к аналоговому входу
  int BUTTON_PIN;
};

initSensorsStruct jFirst;
initSensorsStruct jSecond;

struct SensorsStruct {
  int VRX;
  int VRY;
  boolean button;
};

SensorsStruct myData;
SensorsStruct myData1;

void setup()
{
    radioOutside.begin();

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Test init!", CENTER, 1);

  Serial.begin(9600);
  Serial.println(F(""));
  jFirst.Y_PIN = 4;
  jFirst.X_PIN = 5;
  jSecond.Y_PIN = 6;
  jSecond.X_PIN = 7;
}

void loop()
{
  if (firststart) {
    myGLCD.clrScr();
    firststart = false;
  }
  myData.VRY = analogRead(jFirst.Y_PIN);   // Задаем переменную yVal для считывания показаний аналогового значения
  myData.VRX = analogRead(jFirst.X_PIN);
  //myData.button = digitalRead(BUTTON_PIN);  // Считываем не было ли нажатия на джойстик

  myData1.VRY = analogRead(jSecond.Y_PIN);  
  myData1.VRX = analogRead(jSecond.X_PIN);
  //myData.button = digitalRead(BUTTON_PIN);

  unsigned long now = millis();              // If it's time to send a message, send it!
  /*if ( now - last_chack_sensors >= interval_check  )
  {
    last_chack_sensors = now;
    */myGLCD.setColor(VGA_WHITE);

    myGLCD.setFont(SmallFont);
    myGLCD.print("VRX:", LEFT, 0);
    myGLCD.print("VRY:", RIGHT, 0);
    myGLCD.printNumI(myData.VRX, LEFT, 14,  6, ' ');
    myGLCD.printNumI(myData.VRY, RIGHT, 14,  6, ' ');

    myGLCD.printNumI(myData1.VRX, LEFT, 30,  6, ' ');
    myGLCD.printNumI(myData1.VRY, RIGHT, 30,  6, ' ');

    pack_list[0].buf = &myData;
    pack_list[0].len = sizeof(myData);
    pack_list[0].type_pack = 'D'; // DTH
  //}
  radioOutside.sendOut(pack_list, sizeof(pack_list) / sizeof(pack_for_send));
}


