/**************************************************************************/
/*!
  @file     sensors.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/

#include "sensors.h"


/* classes */

Sensors::Sensors(int vY_PIN, int vX_PIN, int vBUTTON_PIN, int vzMin, int vzMax, int vmaxMap) {
  _initSensors.Y_PIN = vY_PIN;
  _initSensors.X_PIN = vX_PIN;
  _initSensors.BUTTON_PIN = vBUTTON_PIN;
  _initSensors.zMin = vzMin;
  _initSensors.zMax = vzMax;
  _initSensors.maxMap = vmaxMap;
};

boolean Sensors::begin() {
  pinMode(_initSensors.BUTTON_PIN, INPUT_PULLUP);
  return true;
}

void Sensors::getSensors() {
  boolean directionX, directionY;
  vrXa = analogRead(_initSensors.X_PIN);
  vrYa = analogRead(_initSensors.Y_PIN);
  int vrX;
  int vrY;
  if (vrXa > _initSensors.zMax) {
    vrX = map(vrXa - 512, 0, 512, 0, _initSensors.maxMap);
    directionX = false;
  } else if (vrXa < _initSensors.zMin) {
    vrX = map(512 - vrXa, 0, 512, 0, _initSensors.maxMap);
    directionX = true;
  } else {
    vrX   = 0;
  }

  if (vrYa > _initSensors.zMax) {
    vrY = map(vrYa - 512, 0, 512, 0, _initSensors.maxMap);
    directionY = false;
  } else if (vrYa < _initSensors.zMin) {
    vrY = map(512 - vrYa, 0, 512, 0, _initSensors.maxMap);
    directionY = true;
  } else {
    vrY   = 0;
  }

  isChanged = (SensorsStatus.VRY != vrY) || (SensorsStatus.VRX != vrX) || 
  (SensorsStatus.directionX != directionX) || (SensorsStatus.directionY != directionY);

  SensorsStatus.VRY = vrY;
  SensorsStatus.VRX = vrX;
  SensorsStatus.directionX = directionX;
  SensorsStatus.directionY = directionY;

  SensorsStatus.button = digitalRead( _initSensors.BUTTON_PIN);
}

