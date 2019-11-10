/*
  Lora Send And Receive
  This sketch demonstrates how to send and receive data with the MKR WAN 1300 LoRa module.
  This example code is in the public domain.
*/

//  Your device EUI is: a8610a3337386b0c
//const char *appEui = "70B3D57ED0024800";
//const char *appKey = "1983ED48628325F566503F3D7541FDED";

#include <Arduino.h>
#include "wiring_private.h"
Uart mySerial1 (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0); // Create the new UART instance assigning it to pin 0 and 1



#include <MKRWAN.h>

LoRaModem modem;

#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup() {
    mySerial1.begin(9600);
  pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
  pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1
    mySerial1.println("LORA FitExample");

  if (!modem.begin(EU868)) {
    mySerial1.println("[ERR] Failed to start module");
    while (1) {}
  };
  delay(200);
  mySerial1.println("Your device EUI is: ");
  mySerial1.println(modem.deviceEUI());
  delay(200);

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    mySerial1.println("[ERR] Can't get GW");
    while (1) {}
  }

  modem.minPollInterval(60); //rm in future?
  mySerial1.println("SETUP END");
}
int  myCounter = 1;
void loop() {
  mySerial1.println();
  delay(200);

  // while (!Serial.available());
  String msg = String(myCounter++);    //Serial.readStringUntil('\n');
  mySerial1.println("New msg: " + myCounter);
  delay(200);

  mySerial1.println();
  mySerial1.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    mySerial1.print(msg[i] >> 4, HEX);
    mySerial1.print(msg[i] & 0xF, HEX);
    mySerial1.print(" ");
  }
  mySerial1.println();
  delay(200);

  int err;
  modem.beginPacket();
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0) {
    mySerial1.println("[OK] SENT!");
  } else {
    mySerial1.println("[ERR] Cant send limit?");
  }
  delay(1000);
  if (!modem.available()) {
    mySerial1.println("[DOWNLINK] NO");
    return;
  }
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
    mySerial1.println("[DOWNLINK] YES");
  // mySerial1.print("RECEIVED!!: ");
  for (unsigned int j = 0; j < i; j++) {
    mySerial1.print(rcv[j] >> 4, HEX);
    mySerial1.print(rcv[j] & 0xF, HEX);
    mySerial1.print(" ");
  }
  mySerial1.println();
}

// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial1.IrqHandler();
}