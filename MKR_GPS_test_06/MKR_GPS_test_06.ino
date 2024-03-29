#include <Arduino.h>
#include "wiring_private.h"
Uart mySerial1 (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0); // Create the new UART instance assigning it to pin 0 and 1


#include <Arduino_MKRGPS.h>
#include <MKRWAN.h>
LoRaModem modem;

#include "arduino_secrets.h"
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void errorBlink(int blink_delay)
{
  while (1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(blink_delay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(blink_delay);
  }
}


int tempValue = 111;
//send test LoRa Msg ########
void sendLoraMsg(String msg, bool confirmed)
{
  mySerial1.println("LoRaTEST send on modem...");
  int err;
  modem.beginPacket();
  modem.print(msg);
  modem.print(tempValue);
  err = modem.endPacket(confirmed);
  if (err > 0) {
    mySerial1.println("Message sent correctly!");
  } else {
    mySerial1.println("Error sending message :(");
    mySerial1.println("(you may send a limited amount of messages per minute, depending on the signal strength");
    mySerial1.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
  }
}
// end ######################

void setup() {
  // serialport_init();  // #####
  pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
  pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1

  mySerial1.begin(9600);
  mySerial1.println("INF: Serial init done");
  mySerial1.println("08:16 23-Dec-19");
  mySerial1.println("MKR_GPS_test_03");


  // lora_init();  // #####
  if (!modem.begin(EU868))
  {
    mySerial1.println("ERR: Failed to start module");
    errorBlink(250);
  }

  mySerial1.println("Your dev EUI is:");
  String tempDevEui = modem.deviceEUI();
  mySerial1.println(tempDevEui);

  mySerial1.println("modem.joinOTAA(appEui, appKey)...");

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected)
  {
    mySerial1.println("[ERR] !connected"); //Something went wrong; are you indoor? Move near a window and retry");
    mySerial1.println("[WARN] Maybe no respond");
    //  errorBlink(250);
  }

  //modem.minPollInterval(60);
  delay(100);
  mySerial1.println("OTAA Connected!");
  mySerial1.println("INF: LoRa init done");

  delay(100);


  if (!GPS.begin(GPS_MODE_SHIELD)) {
    mySerial1.println("Failed to initialize GPS!");
    errorBlink(250);
  }
}

void loop() {
  // wake up the GPS
  mySerial1.println("wakeup");
  GPS.wakeup();

  sendLoraMsg("GPS-proj-test", false);


  mySerial1.println("wait location ... ");

  // wait for new GPS data to become available
  unsigned long startMillis = millis();
  while (!GPS.available());
  unsigned long endMillis = millis();

  mySerial1.print(endMillis - startMillis);
  mySerial1.println(" ms");

  // read GPS values
  float latitude   = GPS.latitude();
  float longitude  = GPS.longitude();
  float altitude   = GPS.altitude();
  int   satellites = GPS.satellites();

  tempValue   = GPS.altitude();

  // print GPS values
  mySerial1.println();
  mySerial1.print("Location: ");
  mySerial1.print(latitude, 7);
  mySerial1.print(", ");
  mySerial1.println(longitude, 7);

  mySerial1.print("Altitude: ");
  mySerial1.print(altitude);
  mySerial1.println("m");

  mySerial1.print("Number of satellites: ");
  mySerial1.println(satellites);

  mySerial1.println();
  
  // put the GPS in standby mode
  mySerial1.println("standby");
  GPS.standby();

  delay(15000);
}


// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial1.IrqHandler();
}
