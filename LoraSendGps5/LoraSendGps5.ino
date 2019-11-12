#include <Arduino.h>
#include "wiring_private.h"
#include "arduino_secrets.h"
#include <MKRWAN.h>

#include <TinyGPS++.h>

TinyGPSPlus gps;
Uart mySerial1(&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0); // Create the new UART instance assigning it to pin 0 and 1
LoRaModem modem;

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  // DEBUG INIT
  mySerial1.begin(9600);
  pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
  pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1
  mySerial1.println(">>> GPS 5 <<<");


  // LORA INIT
    mySerial1.println("[INI] Lora module...");
  if (!modem.begin(EU868))
  {
    mySerial1.println("[ERR] Failed to start module");
    while (1);
  };

  // LORA CONNECT
    mySerial1.println("[INI] Lora GW...");
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected)
  {
    mySerial1.println("[ERR] Can't get GW");
    while (1);
  }

  mySerial1.println("SETUP END");

  //GPS init
//  Serial1.begin(9600);
  digitalWrite(LED_BUILTIN, LOW);
}

int myCounter = 1;
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  
    mySerial1.println(String("[I] GPS.ava.:"+Serial1.available()));
 while (Serial1.available() > 0)
 {
   if (gps.encode(Serial1.read()))
   {
      displayInfo();
   }
     if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    mySerial1.println(F("[ERR] GPS: cable"));
    while(true);
  }
 }



  mySerial1.println();
  String msg = String(myCounter++);

  mySerial1.println("Send msg: <" + msg + ">");

  int err;
  modem.beginPacket();
  modem.print(msg);
  modem.endPacket(false); // falce - without confirmation downlink

  digitalWrite(LED_BUILTIN, LOW);

  delay(10000);
}

// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial1.IrqHandler();
}

void displayInfo()
{
  mySerial1.println(F("Location: ")); 
  if (gps.location.isValid())
  {
    mySerial1.print(gps.location.lat(), 6);
    mySerial1.println(F(","));
    mySerial1.println(gps.location.lng(), 6);
  }
  else
  {
    mySerial1.print(F("INVALID"));
  }
}