/**************************************************************************/
/*!
  @file     FRoutside.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/

#include <avr/pgmspace.h>
#include "RFoutside.h"

#include <avr/sleep.h>
#include <avr/power.h>
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


const unsigned long interval_read = 4000; //ms  // How often to send 'hello world to the other unit

unsigned long last_read;             // When did we last send?

volatile  int VRX_048;
volatile  int VRY_048;
volatile  boolean IRQ_rx_048;
volatile  boolean DIR_x_048;
volatile  boolean DIR_y_048;

volatile  int VRX_049;
volatile  int VRY_049;
volatile  boolean IRQ_rx_049;
volatile  boolean DIR_x_049;
volatile  boolean DIR_y_049;

volatile  int VRX_050;
volatile  int VRY_050;
volatile  boolean IRQ_rx_050;
volatile  boolean DIR_x_050;
volatile  boolean DIR_y_050;

volatile  int VRX_051;
volatile  int VRY_051;
volatile  boolean IRQ_rx_051;
volatile  boolean DIR_x_051;
volatile  boolean DIR_y_051;

typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;

/**********************************************************/

RFoutside::RFoutside(bool radioNumber) {
  _radioNumber = radioNumber;
}
void check_radio(void)                                // Receiver role: Does nothing!  All the work is in IRQ
{
  TempDTHSensorsStructHeader myTempDTHData;

  bool tx, fail, rx;

  IRQ_rx_048 = false;
  IRQ_rx_049 = false;
  IRQ_rx_050 = false;
  IRQ_rx_051 = false;
  
  radio.whatHappened(tx, fail, rx);                   // What happened?

  //Serial.println(F("All the work is in IRQ"));
  // If data is available, handle it accordingly
  if ( rx ) {
    
    
    if (radio.getDynamicPayloadSize() < 1) {
      // Corrupt payload has been flushed
      Serial.println(F("Corrupt payload has been flushed"));
      return;
    }

    Serial.println(F("Read in the data IRQ"));
    // Read in the data
    uint8_t received;
    radio.read(&myTempDTHData, sizeof(myTempDTHData));
    
    switch (myTempDTHData.header.type) {
      case 48 :
        VRX_048 = myTempDTHData.sensor.VRX;
        VRY_048 = myTempDTHData.sensor.VRY;
        DIR_y_048 = myTempDTHData.sensor.directionY;
        DIR_x_048 = myTempDTHData.sensor.directionX;
        
        IRQ_rx_048 = true;
        break;
      case 49 :
        VRX_049 = myTempDTHData.sensor.VRX;
        VRY_049 = myTempDTHData.sensor.VRY;
        DIR_y_049 = myTempDTHData.sensor.directionY;
        DIR_x_049 = myTempDTHData.sensor.directionX;
        
        IRQ_rx_049 = true;
        break;
      case 50 :
        VRX_050 = myTempDTHData.sensor.VRX;
        VRY_050 = myTempDTHData.sensor.VRY;
        DIR_y_050 = myTempDTHData.sensor.directionY;
        DIR_x_050 = myTempDTHData.sensor.directionX;
        
        IRQ_rx_050 = true;
        break;
      case 51 :
        VRX_051 = myTempDTHData.sensor.VRX;
        VRY_051 = myTempDTHData.sensor.VRY;
        DIR_y_051 = myTempDTHData.sensor.directionY;
        DIR_x_051 = myTempDTHData.sensor.directionX;
        
        IRQ_rx_051 = true;
        break;
    }

    Serial.print(F("ID: "));
    Serial.print(myTempDTHData.header.id);
    Serial.print(F(" Type: "));
    Serial.print(sprintf("%c ", myTempDTHData.header.type));
    Serial.print(myTempDTHData.header.type);
    Serial.print(F(" VRX: "));
    Serial.print(myTempDTHData.sensor.VRX);
    Serial.print(F(" VRY: "));
    Serial.print(myTempDTHData.sensor.VRY);
    Serial.println(F("."));
  }

  // Start listening if transmission is complete
  if ( tx || fail ) {
    //radio.startListening();
    Serial.println(tx ? F(":OK") : F(":Fail"));
  }
}

void RFoutside::begin(void)
{
  //printf_begin();
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();

  network.begin(/*channel*/ 90, /*node address*/ this_node);

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  /*  radio.setPALevel(RF24_PA_MAX);*/

  radio.startListening();

  attachInterrupt(0, check_radio, LOW);             // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

  /******************************** This is the configuration for sleep mode ***********************/
  //  network.setup_watchdog(wdt_1s);                       //The watchdog timer will wake the MCU and radio every second to send a sleep payload, then go back to sleep

}

void RFoutside::readIn(SensorsStruct* message, size_t maxlen) {
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_read >= interval_read  )
  {
    last_read = now;

    radio.startListening();

    //    message->myDTHHumidity = myDTHHumidity;
    //    message->myDTHTemperature = myDTHTemperature;
  }
  /*  network.update();                  // Check the network regularly

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
    }*/
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




