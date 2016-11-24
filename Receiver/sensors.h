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

struct RF24NetworkHeaderL
{
  uint16_t from_node; /**< Logical address where the message was generated */
  uint16_t to_node; /**< Logical address where the message is going */
  uint16_t id; /**< Sequential message ID, incremented every message */
  unsigned char type; /**< Type of the packet.  0-127 are user-defined types, 128-255 are reserved for system */
  unsigned char reserved; /**< Reserved for future use */

  static uint16_t next_id; /**< The message ID of the next message to be sent */
};

struct TempDTHSensorsStructHeader {
  RF24NetworkHeaderL header;
  SensorsStruct sensor;
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


