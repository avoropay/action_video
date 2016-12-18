#include "RFoutside.h"
#include "RFoutside.h"
//#include <OLED_I2C.h>
#include <AccelStepper.h>
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
//OLED  //myOLED(SDA, SCL, 8);
AccelStepper stepper1(1, 4, 5); // stepP, dirP
AccelStepper stepper2(1, 6, 7);
AccelStepper stepper3(1, 8, 9);

extern uint8_t TinyFont[];
extern uint8_t SmallFont[];


extern  int VRX_048;
extern  int VRY_048;
extern  boolean IRQ_rx_048;
extern  boolean DIR_x_048;
extern  boolean DIR_y_048;

extern  int VRX_049;
extern  int VRY_049;
extern  boolean IRQ_rx_049;
extern  boolean DIR_x_049;
extern  boolean DIR_y_049;

extern  int VRX_050;
extern  int VRY_050;
extern  boolean IRQ_rx_050;
extern  boolean DIR_x_050;
extern  boolean DIR_y_050;

extern  int VRX_051;
extern  int VRY_051;
extern  boolean IRQ_rx_051;
extern  boolean DIR_x_051;
extern  boolean DIR_y_051;

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
  stepper1.setMaxSpeed(5000.0);
  stepper2.setMaxSpeed(5000.0); 
  stepper3.setMaxSpeed(5000.0);  

  stepper1.setSpeed(0.0);
  stepper2.setSpeed(0.0);
  stepper3.setSpeed(0.0);
  
  radioOutside.begin();
  Serial.begin(9600);
  //myOLED.begin();
  //myOLED.setFont(SmallFont);
  //myOLED.print("Test OLED", LEFT, 24);
  ////myOLED.print(header.toString(), LEFT, 24);
  //myOLED.update();
}

void loop()
{

  if (firststart) {
    //    display_mode = 1;
    //myOLED.clrScr();
    //myOLED.setFont(SmallFont);
  }
  //myOLED.print(F("VRX: "), LEFT, 0);
  //myOLED.print(F("VRY: "), RIGHT, 0);
  //radioOutside.readIn(&myTempDTHData, sizeof(myTempDTHData));
  if (IRQ_rx_048) {
    //myOLED.printNumI(VRY_048, RIGHT, 10);
    //myOLED.printNumI(VRX_048, LEFT, 10);
    IRQ_rx_048 = false;
    
    stepper1.setSpeed(VRY_048 * 5);
    stepper1.setDirection(DIR_y_048);
  }

  int currentSpeed = 0;
  int goalSpeed = 0;
  int accel = 10;
  bool useSmooth = false;
  long prevTime = 0;
  long smoothDelay = 100;
  bool upTrueOrDownFalse = true;
  
  if(useSmooth){
    long currTime = millis();
    if(currTime > prevTime + smoothDelay){
      prevTime = currTime;
      if(upTrueOrDownFalse){
        if(currentSpeed + accel > goalSpeed){
          currentSpeed += accel;
          stepper2.setSpeed(currentSpeed);
        }
      }else{
        if(currentSpeed - accel > goalSpeed){
          currentSpeed -= accel;
          stepper2.setSpeed(currentSpeed);
        }
      }
    }
  }
  
  if (IRQ_rx_049) {
    
    int mSpeed = IRQ_rx_049;
    
    IRQ_rx_049 = false;
    
    if(mSpeed > currentSpeed){
      currentSpeed += accel
      goalSpeed = mSpeed;
      useSmooth = true;
      upTrueOrDownFalse = true;
    }else if(mSpeed < currentSpeed){
      currentSpeed -= accel;
      goalSpeed = mSpeed;
      useSmooth = true;
      upTrueOrDownFalse = false;
    }
    
    stepper2.setSpeed(currentSpeed);
    stepper2.setDirection(DIR_y_049);
  }

  if (IRQ_rx_050) {
    //myOLED.printNumI(VRY_050, RIGHT, 30);
    //myOLED.printNumI(VRX_050, LEFT, 30);
    IRQ_rx_050 = false;
    
    stepper3.setSpeed(VRY_050 * 3);
    stepper3.setDirection(DIR_y_050);
  }
 
  if (IRQ_rx_051) {
    //myOLED.printNumI(VRY_051, RIGHT, 40);
    //myOLED.printNumI(VRX_051, LEFT, 40);
    IRQ_rx_051 = false;
  }
  
  //myOLED.update();
  if (firststart) {
    firststart = false;
  }
  
  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();
  
}
