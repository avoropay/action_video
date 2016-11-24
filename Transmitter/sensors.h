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
 public:
  /*float ppmCZ, ppmZ;
  queue pppZQueue = queue(200);
  queue pppCZQueue = queue(200);*/

  Sensors(void);
  boolean begin();
  void getSensors(SensorsStruct* buf);
};

#endif


