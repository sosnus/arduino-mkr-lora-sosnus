/*
  First Configuration
  This sketch demonstrates the usage of MKR WAN 1300 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>

LoRaModem modem;

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);
const char *devAddr = "2601199F";
const char *nwkSKey = "7F1FA4919BFB835952A3CF935BA83DC1";
const char *appSKey = "8647B2C5784839A06C6679F793B694B9";

//String appEui;
//String appKey;
//String devAddr;
//String nwkSKey;
//String appSKey;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Welcome to MKRWAN1300 first configuration sketch");
  Serial.println("lora_template");
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int mode = 2;
  while (mode != 1 && mode != 2) {
    Serial.println("Are you connecting via OTAA (1) or ABP (2)?");
    while (!Serial.available());
    mode = Serial.readStringUntil('\n').toInt();
  }

  int connected;
  
    connected = modem.joinABP(devAddr, nwkSKey, appSKey);
    
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  delay(5000);

  int err;
  modem.setPort(3);
  modem.beginPacket();
  modem.print("HeLoRA world!");
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }
}

void loop() {

    Serial.println("Send...");
  int err;
 // modem.setPort(3);
  modem.beginPacket();
  modem.print("HeLoRA world!");
  err = modem.endPacket(false);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }
  
  delay(15000);
  
    Serial.println("Wait....");
}
