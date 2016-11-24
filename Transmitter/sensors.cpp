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

Sensors::Sensors(void) {};

boolean Sensors::begin() {
 return true;
}

void Sensors::getSensors(SensorsStruct* buf) {

}

