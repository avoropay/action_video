/**************************************************************************/
/*!
  @file     hyperspeedchange.h
  @author   A.Voropay
  @license  GNU GPLv3

  hyperbolic speed change

  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/
#ifndef hyperspeedchange_H
#define hyperspeedchange_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class hyperbolicSpeedChange {
  public:
  hyperbolicSpeedChange(int vmaxTime, int vmaxSteps, int vmaxSpeed);
  int maxTime;
  int maxSteps;
  int maxSpeed;
  int deltaStep;
  int quotientSpeed;
  int currentSpeed;
  int newSpeed;
  int currentTime;
  boolean hyperbolicSpeedChange::begin(int newSpeed);
  void countSteps(int deltaSpeed);
  int getNextSpeed();
  
};

#endif
