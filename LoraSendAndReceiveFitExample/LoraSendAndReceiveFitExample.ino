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
    mySerial1.println("ELO!!!");

  // Serial.begin(115200);
  // while (!Serial);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    mySerial1.println("Failed to start module");
    while (1) {}
  };
  delay(200);
  mySerial1.println("Your module version is: ");
  mySerial1.println(modem.version());
  delay(200);
  mySerial1.println("Your device EUI is: ");
  mySerial1.println(modem.deviceEUI());
  delay(200);

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    mySerial1.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
  mySerial1.println("SETUP END");

}

void loop() {
  mySerial1.println();
  mySerial1.println("Enter a message to send to network");
  delay(200);
  // mySerial1.println("(make sure that end-of-line 'NL' is enabled)");

  // while (!Serial.available());
  String msg = "AAA";    //Serial.readStringUntil('\n');
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
    mySerial1.println("Message sent correctly!");
  } else {
    mySerial1.println("Error sending message :(");
    mySerial1.println("(you may send a limited amount of messages per minute, depending on the signal strength");
    mySerial1.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
  }
  delay(1000);
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    return;
  }
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  for (unsigned int j = 0; j < i; j++) {
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
}


// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial1.IrqHandler();
}


// //  Your device EUI is: a8610a3337386b0c
// //const char *appEui = "70B3D57ED0024800";
// //const char *appKey = "1983ED48628325F566503F3D7541FDED";
// #include <Arduino.h>
// #include "wiring_private.h"
// Uart mySerial1 (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0); // Create the new UART instance assigning it to pin 0 and 1

// #include <MKRWAN.h>

// LoRaModem modem;

// #include "arduino_secrets.h"
// // Please enter your sensitive data in the Secret tab or arduino_secrets.h
// String appEui = SECRET_APP_EUI;
// String appKey = SECRET_APP_KEY;

// void setup() {
//   Serial.begin(9600);
//   mySerial1.begin(9600);
//   pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
//   pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1

//     mySerial1.println("Run LoRa...");
//   if (!modem.begin(EU868)) {
//     mySerial1.println("Failed to start module");
//     while (1) {}
//   };
//   mySerial1.println("Your module version is: ");
//   mySerial1.println(modem.version());
//   mySerial1.println("Your device EUI is: ");
//   mySerial1.println(modem.deviceEUI());

//   int connected = modem.joinOTAA(appEui, appKey);
//   if (!connected) {
//     mySerial1.println("Something went wrong; are you indoor? Move near a window and retry");
//     while (1) {}
//   }

//   // Set poll interval to 60 secs.
//   modem.minPollInterval(60);
// }

// void loop() {
  
//   mySerial1.println("--");
//   mySerial1.println("Enter to send...");
//   // mySerial1.println("(make sure that end-of-line 'NL' is enabled)");

//   // mySerial1.println();
//   // mySerial1.println("Enter a message to send to network");
//   // mySerial1.println("(make sure that end-of-line 'NL' is enabled)");

//   while (!Serial.available());
//   String msg = Serial.readStringUntil('\n');

//   mySerial1.println();
//   mySerial1.print("Sending: " + msg + " - ");
//   for (unsigned int i = 0; i < msg.length(); i++) {
//     mySerial1.print(msg[i] >> 4, HEX);
//     mySerial1.print(msg[i] & 0xF, HEX);
//     mySerial1.print(" ");
//   }
//   mySerial1.println();

//   int err;
//   modem.beginPacket();
//   modem.print(msg);
//   err = modem.endPacket(true);
//   if (err > 0) {
//     mySerial1.println("Message sent correctly!");
//   } else {
//     mySerial1.println("Error sending message :(");
//     mySerial1.println("(you may send a limited amount of messages per minute, depending on the signal strength");
//     mySerial1.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
//   }
//   delay(1000);
//   if (!modem.available()) {
//     mySerial1.println("No downlink message received at this time.");
//     return;
//   }
//   char rcv[64];
//   int i = 0;
//   while (modem.available()) {
//     rcv[i++] = (char)modem.read();
//   }
//   mySerial1.print("Received: ");
//   for (unsigned int j = 0; j < i; j++) {
//     mySerial1.print(rcv[j] >> 4, HEX);
//     mySerial1.print(rcv[j] & 0xF, HEX);
//     mySerial1.print(" ");
//   }
//   mySerial1.println();
// }


// // Attach the interrupt handler to the SERCOM
// void SERCOM3_Handler()
// {
//   mySerial1.IrqHandler();
// }
