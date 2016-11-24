/**************************************************************************/
/*!
  @file     FRoutside.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/

#include "RFoutside.h"
#include "sensors.h"
/**************************************************************************/
/*!
  @brief  Default constructor

  @param[in] pin  The analog input pin for the readout of the sensor
*/
/**************************************************************************/

/****************** User Config ***************************/
const uint16_t this_node = 00;        // Address of our node in Octal format
const uint16_t other_node = 01;       // Address of the other node in Octal format

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(48, 53);
RF24Network network(radio);          // Network uses that radio

const unsigned long interval = 4000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
/**********************************************************/

RFoutside::RFoutside(bool radioNumber) {
  _radioNumber = radioNumber;
}

void RFoutside::begin(void)
{
  //printf_begin();
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  /*  radio.setPALevel(RF24_PA_MAX);*/
    radio.printDetails();
}

void RFoutside::readIn(void* message, size_t maxlen) {
  network.update();                  // Check the network regularly

    while ( network.available() ) {     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out


    network.peek(header);
    //Serial.println(header.toString());
//    myOLED.setFont(SmallFont);
    //myOLED.print("Test OLED", LEFT, 24);
    //myOLED.print(header.toString(), LEFT, 24);
  //  myOLED.update();

    switch (header.type)
    {
      case 'G':
    //    network.read(header, &myGasData, sizeof(myGasData));
        break;
      case 'T':
      //  network.read(header, &myTempData, sizeof(myTempData));
        break;
      case 'D':
        //network.read(header, &myTempDTHData, sizeof(myTempDTHData));
        network.read(header, message, maxlen);
       // printf_P(PSTR("%lu: NET Received %s\n\r"),millis(),header.toString());
        break;
      default:
        // Anything else is unexpected, and ergo a test failure
        network.read(header, 0, 0);
        break;
    };
  }
}

void RFoutside::sendOut(const pack_for_send buf[], uint8_t lenArray) {

  network.update();                          // Check the network regularly


  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    for (byte itemBuf = 0; itemBuf < lenArray; itemBuf++) {

      Serial.print("Sending...");
      // payload_t payload = { millis(), packets_sent++ };
      RF24NetworkHeader header(/*to node*/ other_node, buf[itemBuf].type_pack);
      bool ok = network.write(header, buf[itemBuf].buf, buf[itemBuf].len);
      if (ok)
        Serial.println("ok.");
      else
        Serial.println("failed.");
    }
  }
}




