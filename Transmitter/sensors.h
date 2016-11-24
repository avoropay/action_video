/**************************************************************************/
/*!
@file     sensor.h
@author   A.Voropay
@license  GNU GPLv3



@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef sensors_H
#define sensors_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

struct initSensorsStruct {
  int Y_PIN;           // Потенциометр оси Y подключен к аналоговому входу
  int X_PIN;           // Потенциометр оси X подключен к аналоговому входу
  int BUTTON_PIN;
};

struct SensorsStruct {
  int VRX;
  int VRY;
  boolean button;
};

class Sensors {
  initSensorsStruct _initSensors;
   
 public:
  SensorsStruct SensorsStatus;
  bool isChanged;
  Sensors(int vY_PIN, int vX_PIN, int vBUTTON_PIN);
  boolean begin();
  void getSensors();
};

#endif


