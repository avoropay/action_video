#include "RFoutside.h"
#include "sensors.h"

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


/*  joysticks array */
const int joystick_counts = 4;
Sensors* joysticks[joystick_counts];

SensorsStruct myData[joystick_counts];

pack_for_send pack_list[4];

int jChanged = 0;

void setup()
{
  radioOutside.begin();
  joysticks[0] = new Sensors(A4, A5, 4, 500, 522, 1000);
  joysticks[1] = new Sensors(A6, A7, 5, 500, 530, 1000);
  joysticks[2] = new Sensors(A8, A9, 6, 500, 522, 1000);
  joysticks[3] = new Sensors(A10, A11, 6, 500, 522, 1000);

  for (byte itemJoysticks = 0; itemJoysticks < joystick_counts; itemJoysticks++) {
    joysticks[itemJoysticks]->begin();
  }

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Test init!", CENTER, 1);

  Serial.begin(9600);
  Serial.println(F(""));
}

void loop()
{
  if (firststart) {
    myGLCD.clrScr();
    firststart = false;
  }

  jChanged = 0;

  for (byte itemJoysticks = 0; itemJoysticks < joystick_counts; itemJoysticks++) {
    joysticks[itemJoysticks]->getSensors();
    if (joysticks[itemJoysticks]->isChanged) {
      myData[itemJoysticks] = joysticks[itemJoysticks]->SensorsStatus;
      pack_list[jChanged].buf = &myData[itemJoysticks];
      pack_list[jChanged].len = sizeof(SensorsStruct);
      pack_list[jChanged].type_pack = itemJoysticks;
      joysticks[itemJoysticks]->isChanged = false;
      jChanged++;
    }
  }

  radioOutside.sendOut(pack_list, jChanged);

  myGLCD.setColor(VGA_WHITE);

  myGLCD.setFont(SmallFont);
  myGLCD.print("VRX:", LEFT, 0);
  myGLCD.print("VRY:", RIGHT, 0);
  uint8_t  FontXsize = myGLCD.getFontXsize();


  for (byte itemJoysticks = 0; itemJoysticks < joystick_counts; itemJoysticks++) {
    uint8_t xPos = (FontXsize * (itemJoysticks + 2) + (itemJoysticks + 2) * 3);

    myGLCD.printNumI(joysticks[itemJoysticks]->SensorsStatus.VRX, LEFT, xPos,  6, ' ');
    myGLCD.printNumI(joysticks[itemJoysticks]->SensorsStatus.VRY, RIGHT, xPos,  6, ' ');
  }
}


