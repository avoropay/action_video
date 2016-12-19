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
AccelStepper stepper1(1, 8, 9);
AccelStepper stepper2(1, 10, 11);

AccelStepper stepper3(1, 5, 4); // stepP, dirP
AccelStepper stepper4(1, 6, 7);


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
  stepper1.setMaxSpeed(10000.0);
  stepper2.setMaxSpeed(50000.0); 
  stepper3.setMaxSpeed(50000.0);
  stepper4.setMaxSpeed(50000.0);  

  stepper1.setSpeed(0);
  stepper2.setSpeed(0.0);
  stepper3.setSpeed(0.0);
  stepper4.setSpeed(0.0);
  
  radioOutside.begin();
  //Serial.begin(9600);
  //myOLED.begin();
  //myOLED.setFont(SmallFont);
  //myOLED.print("Test OLED", LEFT, 24);
  ////myOLED.print(header.toString(), LEFT, 24);
  //myOLED.update();
}

int currentSpeed = 0;
int goalSpeed = 0;
int accel = 30;
bool useSmooth = false;
long prevTime = 0;
long smoothDelay = 50;
bool upTrueOrDownFalse = true;

void loop()
{

  if(useSmooth){
    long currTime = millis();
    if(currTime > prevTime + smoothDelay){ // check time for one step speed up
      prevTime = currTime;
      if(upTrueOrDownFalse){
        if(currentSpeed < goalSpeed){ // is we steel need apply smooth
          currentSpeed += accel;
          stepper1.setSpeed(currentSpeed);
        }else if(currentSpeed == goalSpeed){ // when we arrive goal speed turn off smoothing and next time we will keep the same speed
          useSmooth = false;
        }
      }else{
        if(currentSpeed > goalSpeed){ // is we steel need apply smooth
          currentSpeed -= accel;
          stepper1.setSpeed(currentSpeed);
        }else if(currentSpeed == goalSpeed){ // when we arrive goal speed turn off smoothing and next time we will keep the same speed
          useSmooth = false;
        }
      }
    }
  }else{ // keep the same speed
    //stepper1.setSpeed(currentSpeed);
  }
  
  if (IRQ_rx_048) {
    
    int mSpeed = VRY_048 / 100 * 100;
    int filter = currentSpeed / 100 * 100;
    IRQ_rx_048 = false;
    
    if(mSpeed > filter){
      currentSpeed += accel;
      goalSpeed = mSpeed;
      useSmooth = true;
      upTrueOrDownFalse = true;
    }else{
      currentSpeed -= accel;
      goalSpeed = mSpeed;
      useSmooth = true;
      upTrueOrDownFalse = false;
    }
    
    //stepper1.setSpeed(currentSpeed);
    stepper1.setDirection(DIR_y_048);
  }
  
  stepper1.runSpeed();
}
