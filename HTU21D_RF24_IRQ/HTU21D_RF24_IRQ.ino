/**************************************************************************/
//# include "RFoutside.h"
#include "sensors.h"
#include <RF24.h>
#include <SPI.h>

#include <UTFT.h>
#include <DS1307.h>

#include <math.h>
int sensorPin = A1; // select the input pin for the potentiometer
int sensorHallPin = A7; // select the input pin for the potentiometer

/*#include <IRremote.h>
  IRsend irsend;
*/
#include <IRremote.h>

const int RECV_PIN = 40;

IRrecv irrecv(RECV_PIN);

decode_results results;

RF24 radio(48, 53);

//OLED  myOLED(SDA, SCL, 8);

// Init the DS1307
DS1307 rtc(4, 5);

DS1307_RAM ramBuffer;


//OLED  myOLED(SDA, SCL, 8);
// Usage: myGLCD(<model code>, SDA, SCL, CS, RST[, RS]);
//
// When using the DM-TFT18-101 and shield from DisplayModule you should use the following:
//         UTFT myGLCD(DMTFT18101,2,3,4,6,5);
//
// When using the TFT18SP shield from ElecFreaks you should use the following:
//         UTFT myGLCD(TFT18SHLD,7,6,5,3,4);
//
UTFT myGLCD(ST7735, 11, 13, 10, 8, 9); // Remember to change the model parameter to suit your display module!

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

int firststart = 1;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
//RFoutside radioOutside (0);
/**********************************************************/

Sensors mySensors;

TempSensorsStruct myTempData;
MQ135Struct myGasData;

//pack_for_send pack_list[2];

//pack_for_send pack_list_read[1];

unsigned long last_chack_sensors;
const unsigned long interval_check = 4000; //ms

unsigned long last_display_instrument_readings;
const unsigned long interval_display_instrument_readings = 5000; //ms

/* RGB
  KY-009 3-color full-color LED SMD modules
  using 5050 full color LED with max current of 20mA
*/

int redpin = 22; // select the pin for the red LED
int bluepin = 26; // select the pin for the blue LED
int greenpin = 24; // select the pin for the green LED

//KY016 3-color LED module
int redpin1 = 32; // select the pin for the red LED
int bluepin1 = 30; // select the pin for the blue LED
int greenpin1 = 28 ;// select the pin for the green LED

const uint16_t this_node = 00;        // Address of our node in Octal format
const uint16_t other_node = 01;       // Address of the other node in Octal format

volatile uint32_t round_trip_timer = 0;

void setup()
{
  /* RGB */
  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT);

  //KY016 3-color LED module
  pinMode (redpin1, OUTPUT);
  pinMode (bluepin1, OUTPUT);
  pinMode (greenpin1, OUTPUT);

  //radioOutside.begin();
  // Set the clock to run-mode
  rtc.halt(false);

  // The following lines can be commented out to use the values already stored in the DS1307
  //rtc.setDOW(SATURDAY);        // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 50, 30);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 5, 2016);   // Set the date to October 3th, 2010

  // Set SQW/Out rate to 1Hz, and enable SQW
  rtc.setSQWRate(SQW_RATE_32K);
  rtc.enableSQW(true);
  rtc.setOutput(true);

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Test init!", CENTER, 1);

  Serial.begin(9600);
  Serial.println(F(""));

  mySensors.begin();

  Serial.println(F("HTU21D sensor is present"));

  //irrecv.enableIRIn(); // Start the receiver
  //irrecv.blink13(true);

  SPI.begin();
  radio.begin();
  // Use dynamic payloads to improve response time
  radio.enableDynamicPayloads();
  // Set up the radio the way we want it to look
  radio.setChannel( 90 );
  radio.setDataRate(RF24_1MBPS);
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(this_node);             // communicate back and forth.  One listens on it, the other talks to it.
  radio.openReadingPipe(1,this_node); 
  radio.startListening();
  
  //radio.printDetails();                             // Dump the configuration of the rf unit for debugging

  attachInterrupt(0, check_radio, LOW);             // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver


}

void check_radio(void)                                // Receiver role: Does nothing!  All the work is in IRQ
{
  TempDTHSensorsStruct myTempDTHData;
  
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);                     // What happened?

  Serial.println(F("All the work is in IRQ"));
  // If data is available, handle it accordingly
  if ( rx ){
    if(radio.getDynamicPayloadSize() < 1){
      // Corrupt payload has been flushed
      return; 
    }
    // Read in the data
    uint8_t received;
    radio.read(&myTempDTHData,sizeof(myTempDTHData));
  }

  // Start listening if transmission is complete
  if( tx || fail ){
     radio.startListening(); 
     Serial.println(tx ? F(":OK") : F(":Fail"));
  }  
}

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  return Temp;
}

void loop()
{
  if (firststart) {
    myGLCD.clrScr();
    firststart = false;
  }
  /*myGLCD.setColor(VGA_WHITE);
    myGLCD.setFont(SmallFont);
    myGLCD.print("NEC: ", LEFT, 104);

    if (irrecv.decode(&results)) {
    if (results.decode_type == NEC) {
     Serial.print("NEC: ");
    myGLCD.print("NEC: ", LEFT, 104);
    } else if (results.decode_type == SONY) {
     Serial.print("SONY: ");
    myGLCD.print("SONY: ", LEFT, 104);
    } else if (results.decode_type == RC5) {
     Serial.print("RC5: ");
    myGLCD.print("RC5: ", LEFT, 104);
    } else if (results.decode_type == RC6) {
     Serial.print("RC6: ");
    myGLCD.print("RC6: ", LEFT, 104);
    } else if (results.decode_type == UNKNOWN) {
     Serial.print("UNKNOWN: ");
    }
    //Serial.println(results.value, HEX);
    myGLCD.printNumI(results.value, RIGHT, 104);
    irrecv.resume(); // Receive the next value
    }
  */
  /*   irsend.sendSony(0x68B92, 20);
     delay(100);
     irsend.sendSony(0x68B92, 20);
     delay(100);
     irsend.sendSony(0x68B92, 20);*/

  int readVal = analogRead(sensorHallPin);
  //double temp =  Thermistor(readVal);
  myGLCD.setFont(SmallFont);
  //myGLCD.print("Temp :", LEFT, 92);
  //myGLCD.printNumI(readVal, LEFT, 92);
  //myGLCD.printNumI(temp, RIGHT, 92);

  int val = 0;
  analogWrite (redpin, 255);
  analogWrite (bluepin, 0);
  analogWrite (greenpin, 128);

  /*for (val=255; val>0; val--)
    {
    analogWrite (redpin, val);
    analogWrite (bluepin, 255-val);
    analogWrite (greenpin, 128-val);
    delay (1);
    }
    for (val = 0; val <255; val++)
    {
    analogWrite (redpin, val);
    analogWrite (bluepin, 255-val);
    analogWrite (greenpin, 128-val);
    delay (1);
    }
    Serial.println (val, DEC);*/

  for (val = 255; val > 0; val--)
  {
    analogWrite (redpin1, val);
    analogWrite (bluepin1, 255 - val);
    analogWrite (greenpin1, 128 - val);
    delay (1);
  }
  for (val = 0; val < 255; val++)
  {
    analogWrite (redpin1, val);
    analogWrite (bluepin1, 255 - val);
    analogWrite (greenpin1, 128 - val);
    delay (1);
  }

  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_chack_sensors >= interval_check  )
  {
    last_chack_sensors = now;
    mySensors.getSensors(&myTempData);
    mySensors.getGasSensors(&myGasData, myTempData.myTemperature, myTempData.myHumidity);

    /*pack_list[0].buf = &myGasData;
      pack_list[0].len = sizeof(myGasData);
      pack_list[0].type_pack = 'G';
      pack_list[1].buf = &myTempData;
      pack_list[1].len = sizeof(myTempData);
      pack_list[1].type_pack = 'T';*/
  }

  //radioOutside.sendOut(pack_list, sizeof(pack_list) / sizeof(pack_for_send));
  /*  pack_list_read[0].buf = &myTempDTHData;
    pack_list_read[0].len = sizeof(myTempDTHData);
    pack_list_read[0].type_pack = 'D';*/
  /*radioOutside.readIn(&myTempDTHData, sizeof(myTempDTHData));
      Serial.println(myTempDTHData.myDTHHumidity);
      Serial.println(myTempDTHData.myDTHTemperature);

/*
  while ( network.available() ) {     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out

    network.peek(header);
    Serial.println(header.toString());
    switch (header.type)
    {
      case 'D':
        //network.read(header, &myTempDTHData, sizeof(myTempDTHData));
        network.read(header, &myTempDTHData, maxlen);
        break;
      default:
        // Anything else is unexpected, and ergo a test failure
        network.read(header, 0, 0);
        break;
    };
  }
*/
 /* myTempData.myDTHHumidity = myTempDTHData.myDTHHumidity;
  myTempData.myDTHTemperature = myTempDTHData.myDTHTemperature;*/


  if ( now - last_display_instrument_readings >= interval_display_instrument_readings  )
  {
    myGLCD.setColor(VGA_WHITE);

    myGLCD.setFont(SmallFont);
    myGLCD.print("Humidity (%):", LEFT, 0);
    myGLCD.printNumF(myTempData.myHumidity, 2, LEFT, 14, '.', 6, ' ');
    myGLCD.printNumF(myTempData.myCHumidity, 2, CENTER, 14, '.', 6, ' ');

    if (isnan(myTempData.myDTHTemperature) || isnan(myTempData.myDTHHumidity)) {
      myGLCD.print("Failed to read from DHT", LEFT, 78);
    } else {
      myGLCD.setColor(VGA_BLACK);
      myGLCD.fillRect(0, 78, 159, 89);
      myGLCD.setColor(VGA_RED);
      myGLCD.printNumF(myTempData.myDTHHumidity, 2, RIGHT, 14, '.', 6, ' ');
      myGLCD.printNumF(myTempData.myDTHTemperature, 2, RIGHT, 40, '.', 6, ' ');
    }

    myGLCD.setColor(VGA_WHITE);
    myGLCD.print("Temperature (*C):", LEFT, 26);
    //myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_BLUE);
    myGLCD.printNumF(myTempData.myTemperature, 2 , CENTER, 40, '.', 6, ' ');
    myGLCD.setColor(VGA_FUCHSIA);
    //myGLCD.print("DS:", LEFT, 40);
    //myGLCD.printNumF(myTempData.myDSTemperature, 2 , myGLCD.getFontYsize() * 3, 40);
    myGLCD.printNumF(myTempData.myDSTemperature, 2 , LEFT, 40, '.', 6, ' ');

    Serial.println(F(""));

    Serial.println(myGasData.ppm);  // prints the value read

    /*myGLCD.setColor(VGA_SILVER);
      myGLCD.printNumF(myGasData.ppmAvgZ, 3, LEFT, 90, '.', 10, ' ');
      myGLCD.printNumF(myGasData.ppmAvgCZ, 3, RIGHT, 90, '.', 10, ' ');
    */
    myGLCD.setColor(VGA_YELLOW);
    //myGLCD.printNumF(myGasData.ppm, 3, LEFT, 118, '.', 10, ' ');
    myGLCD.print("CO2 ppm", LEFT, 116);
    myGLCD.printNumF(myGasData.ppmC, 3, RIGHT, 116, '.', 10, ' ');

    /*  myGLCD.setColor(VGA_SILVER);
      myGLCD.printNumF(myGasData.ppmZ, 3, LEFT, 118, '.', 10, ' ');
      myGLCD.printNumF(myGasData.ppmCZ, 3, RIGHT, 118, '.', 10, ' ');
    */

    myGLCD.setColor(VGA_FUCHSIA);
    Serial.println(F(""));
    Serial.println(rtc.getTimeStr());
    myGLCD.print(rtc.getTimeStr(), LEFT, 52);

    myGLCD.setColor(VGA_AQUA);
    // Display date in the lower right corner
    Serial.println(rtc.getDateStr());
    myGLCD.print(rtc.getDateStr(), RIGHT, 52);

    myGLCD.setColor(VGA_LIME);
    // Display abbreviated Day-of-Week in the lower left corner
    Serial.println(rtc.getDOWStr(FORMAT_SHORT));
    myGLCD.print(rtc.getDOWStr(FORMAT_SHORT), LEFT, 64);

    myGLCD.setColor(VGA_PURPLE);
    if (myGasData.myBattery == true)
    {
      //    myBatteryStatus[120] = "Battery OK. Level > 2.25v";
      Serial.println("Battery OK. Level > 2.25v");
      myGLCD.print("VCC OK", RIGHT, 64);
    }
    else
    {
      //    myBatteryStatus[] = "Battery LOW. Level < 2.25v";
      Serial.println("Battery LOW. Level < 2.25v");
      myGLCD.print("VCC LOW", RIGHT, 64);
    }
    //myGLCD.print(myBatteryStatus, RIGHT, 64);

    /*   if (isnan(myTempData.myDTHTemperature) || isnan(myTempData.myDTHHumidity)) {
         myGLCD.setFont(SmallFont);
         myGLCD.print("Failed to read from DHT", LEFT, 92);
       } else {
         myGLCD.setFont(SmallFont);
      //      myGLCD.print("Humidity (%):", LEFT, 92);
      //      myGLCD.printNumF(myTempData.myDTHHumidity, 2, RIGHT, 92);

         myGLCD.print("Temperature *C:", LEFT, 104);
         myGLCD.printNumF(myTempData.myDTHTemperature, 2, RIGHT, 104);
       }*/
    /*    for (int i = 0; i < 56; i++)
          ramBuffer.cell[i] = 0;

        ramBuffer = rtc.readBuffer();
        bufferDump("Buffer read from RAM...");

        int temp;
        Serial.println("Reading address 18 (0x12). This should return 18, 0x12.");
        temp = rtc.peek(18);
        Serial.print("Return value: ");
        Serial.print(temp, DEC);
        Serial.print(", 0x");
        Serial.println(temp, HEX);
    */
  }

}

void bufferDump(char st[])
{
  Serial.write(st);
  Serial.println("");
  for (int i = 0; i < 56; i++)
  {
    Serial.print("0x");
    Serial.print(ramBuffer.cell[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("--------------------------------------------------------");
}

