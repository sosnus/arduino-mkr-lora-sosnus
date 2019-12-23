
#include <Arduino.h>
#include "wiring_private.h"
Uart mySerial1 (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0); // Create the new UART instance assigning it to pin 0 and 1


#include <Arduino_MKRGPS.h>

void setup() {
  // initialize serial communications and wait for port to open:
  mySerial1.begin(9600);
  
    mySerial1.println("06:03 22-Dec-19");
    mySerial1.println("MKR_GPS_test_02");
  pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
  pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1

  // If you are using the MKR GPS as shield, change the next line to pass
  // the GPS_MODE_SHIELD parameter to the GPS.begin(...)
  if (!GPS.begin(GPS_MODE_SHIELD)) {
    mySerial1.println("Failed to initialize GPS!");
    while (1);
  }
}

void loop() {
  // put the GPS in standby mode
  mySerial1.println("standby");
  GPS.standby();
/*
  // wait for 10 seconds
  mySerial1.print("delay ");
  for (int i = 0; i < 10; i++) {
    delay(1000);
    mySerial1.print(".");
  }
  mySerial1.println();
*/
  // wake up the GPS
  mySerial1.println("wakeup");
  GPS.wakeup();

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
}


// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial1.IrqHandler();
}
