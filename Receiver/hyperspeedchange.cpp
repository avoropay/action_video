/**************************************************************************/
/*!
  @file     hyperspeedchange.cpp
  @author   A.Voropay
  @license  GNU GPLv3

  hyperbolic speed change

  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/


/* classes */

#include "hyperspeedchange.h"

hyperbolicSpeedChange::hyperbolicSpeedChange(int vmaxTime, int vmaxSteps, int vmaxSpeed) {
  maxTime = vmaxTime;
  maxSteps = vmaxSteps;
  maxSpeed = vmaxSpeed;
};

boolean hyperbolicSpeedChange::begin(int newSpeed) {
  quotientSpeed = (currentSpeed - newSpeed) / pow(maxTime, 2);
  countSteps(currentSpeed - newSpeed);
  currentTime = deltaStep;
 return true;
}

void hyperbolicSpeedChange::countSteps(int deltaSpeed) {
  int newSteps = (deltaSpeed * maxSteps) / maxSpeed;
    deltaStep = maxTime / newSteps;
};

int hyperbolicSpeedChange::getNextSpeed() {
  return quotientSpeed * pow(currentTime,2);
};

