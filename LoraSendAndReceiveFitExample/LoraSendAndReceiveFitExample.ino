#include <Arduino.h>
#include "wiring_private.h"
#include "arduino_secrets.h"
#include <MKRWAN.h>

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
  mySerial1.println("LORA FitExample");

  // LORA INIT
  if (!modem.begin(EU868))
  {
    mySerial1.println("[ERR] Failed to start module");
    while (1);
  };

  // LORA CONNECT
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected)
  {
    mySerial1.println("[ERR] Can't get GW");
    while (1);
  }

  mySerial1.println("SETUP END");
  digitalWrite(LED_BUILTIN, LOW);
}

int myCounter = 1;
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
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