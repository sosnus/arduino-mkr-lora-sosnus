#include <Arduino.h>
#include "wiring_private.h"
Uart mySerial1 (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0); // Create the new UART instance assigning it to pin 0 and 1

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  mySerial1.begin(9600);
  pinPeripheral(0, PIO_SERCOM); //Assign RX function to pin 0
  pinPeripheral(1, PIO_SERCOM); //Assign TX function to pin 1
}

void loop() {
  Serial.println("Check pins 0(rx) and 1(tx)");
  mySerial1.println("Hello from UART on SERCOM3 pins");
  delay(1000);
}

// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial1.IrqHandler();
}
