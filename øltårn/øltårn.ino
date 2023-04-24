#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // TX, RX pins for HC-12

bool boolArray[4];

void setup() {
  HC12.begin(9600); // initialize HC-12 serial communication at 9600 baud
}

void loop() {

  // modtag 4 bool array
  if (HC12Serial.available() >= 4) {
    // read incoming data into boolean array
    for (int i = 0; i < 4; i++) {
      boolArray[i] = HC12Serial.read();
    }
    delay(7000);
    HC12.write(true);
  }


  

}
