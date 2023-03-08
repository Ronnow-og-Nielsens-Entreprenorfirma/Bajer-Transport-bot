#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // TX, RX pins for HC-12

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 baud
  HC12.begin(9600); // initialize HC-12 serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    char data = Serial.read(); // read serial data from the computer
    HC12.write(data); // send the data to the other Arduino via HC-12
  }

  if (HC12.available() > 0) {
    char data = HC12.read(); // read data from the other Arduino via HC-12
    Serial.write(data); // send the data to the computer
  }
}
