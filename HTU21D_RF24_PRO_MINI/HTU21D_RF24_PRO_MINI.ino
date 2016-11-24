/**************************************************************************/
#include "RFoutside.h"
#include "sensors.h"

int firststart = 1;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
RFoutside radioOutside (0);
/**********************************************************/

Sensors mySensors;

TempSensorsStruct myTempData;

pack_for_send pack_list[1];

unsigned long last_chack_sensors;
const unsigned long interval_check = 4000; //ms

void setup()
{
  radioOutside.begin();

  Serial.begin(9600);
  Serial.println(F(""));

  mySensors.begin();

  Serial.println(F("HTU21D sensor is present"));
}

void loop()
{
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_chack_sensors >= interval_check  )
  {
    last_chack_sensors = now;
    mySensors.getSensors(&myTempData);

    pack_list[0].buf = &myTempData;
    pack_list[0].len = sizeof(myTempData);
    pack_list[0].type_pack = 'D'; // DTH
  }
    radioOutside.sendOut(pack_list, sizeof(pack_list) / sizeof(pack_for_send));
}

