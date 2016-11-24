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

/**************************************************************************/
/*!
@brief  Default constructor

@param[in] pin  The analog input pin for the readout of the sensor
*/
/**************************************************************************/

/****************** User Config ***************************/
const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(48, 53);
RF24Network network(radio);          // Network uses that radio


const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
};


/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

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
/*  radio.setPALevel(RF24_PA_MAX);

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  radio.startListening();
  radio.printDetails();*/
}

void RFoutside::sendOut(const void* buf, uint8_t len) {

  network.update();                          // Check the network regularly

  
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    payload_t payload = { millis(), packets_sent++ };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header, buf, len);
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }

  /*radio.stopListening();                                    // First, stop listening so we can talk.

  Serial.println(F("Now sending"));

  //buf._micros = micros();
  if (!radio.write( buf, len )) {
    Serial.println(F("failed"));
  }

  radio.startListening();                                    // Now, continue listening

  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

  while ( ! radio.available() ) {                            // While nothing is received
    if (micros() - started_waiting_at > 200000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if ( timeout ) {                                            // Describe the results
    Serial.println(F("Failed, response timed out."));
  } else {
    // Grab the response, compare, and send to debugging spew
    //radio.read( &buf, len );
    unsigned long time = micros();

    // Spew it
    Serial.print(F("Sent "));
    Serial.print(time);
    Serial.print(F(", Got response "));
//    Serial.print(uint8_t len._micros);
    Serial.print(F(", Round-trip delay "));
 //   Serial.print(time - myData._micros);
    Serial.print(F(" microseconds Value "));
    Serial.println(myData.ppm);
    Serial.println(myData.ppmCZ);
    Serial.println(myData.ppmC);
    Serial.println(myData.ppmZ);
  }

  // Try again 1s later
  delay(1000);
  */
}

