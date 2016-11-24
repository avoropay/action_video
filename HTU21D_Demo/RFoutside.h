/**************************************************************************/
/*!
@file     FRoutside.h
@author   A.Voropay
@license  GNU GPLv3



@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef RFoutside_H
#define RFoutside_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

class RFoutside {
 private:
  bool _radioNumber;

 public:
  RFoutside(bool radioNumber);
  void begin();
  void sendOut(const void* buf, uint8_t len);
};
#endif
