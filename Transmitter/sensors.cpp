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

Sensors::Sensors(int vY_PIN, int vX_PIN, int vBUTTON_PIN) {
  _initSensors.Y_PIN = vY_PIN;
  _initSensors.X_PIN = vX_PIN;
  _initSensors.BUTTON_PIN = vBUTTON_PIN;
};

boolean Sensors::begin() {
  pinMode(_initSensors.BUTTON_PIN, INPUT_PULLUP);
 return true;
}

void Sensors::getSensors() {
  int vrX = analogRead(_initSensors.X_PIN);
  int vrY = analogRead(_initSensors.Y_PIN);

  isChanged = (SensorsStatus.VRY != vrY) || (SensorsStatus.VRX != vrX);

  SensorsStatus.VRY = vrY;  
  SensorsStatus.VRX = vrX;

 SensorsStatus.button = digitalRead( _initSensors.BUTTON_PIN);
}

