/**************************************************************************/
/*!
  @file     FRoutside.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/
#ifndef systeminfo_H
#define systeminfo_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include  "avr_cpunames.h"
#include <UTFT.h>
#include <MemoryFree.h>

void PrintCPUstats();

#endif

