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

struct pack_for_send {
  const void* buf;
  unsigned long len;
  unsigned char type_pack;
};


class RFoutside {
 private:
  bool _radioNumber;

 public:
  RFoutside(bool radioNumber);
  void begin();
  void sendOut(const pack_for_send buf[], uint8_t lenArray);
};
#endif
