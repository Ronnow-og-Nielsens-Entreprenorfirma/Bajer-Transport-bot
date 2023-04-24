/*

Kode af Kristoffer N og Jeppe R
     Bajer transport robot
         Version 1.0

          26/04/23
*/



// ir pins
#define left_IR A0
#define right_IR A1

// echo
#define trigPin 7
#define echoPin 8
float duration, distance;
#define EXE_INTERVAL_ECHO 100
unsigned long lastExecutedHCSR0 = 0;

// start knap
#define startKnap 2
bool start = false;

// dc motor pwm
#define venstre_frem 10
#define hojre_frem 9
#define venstre_tilbage 5
#define hojre_tilbage 6

// PWM speed
#define motor_fart 80

//ir start værdi
bool left_value = LOW;
bool right_value = LOW;

// hvis route er færdig
bool end = false;

// tilbage
bool tilbage = false;

// kop kanpper
bool kopState[4] = { false, false, false, false };
int kopPins[4] = { 3, 4, 19, 18 };

// hc12
#include <SoftwareSerial.h>
SoftwareSerial HC12(12, 11);  // TX, RX pins for HC-12
bool data = false;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(kopPins[i], INPUT_PULLUP);
  }
  pinMode(startKnap, INPUT_PULLUP);

  pinMode(left_IR, INPUT);
  pinMode(right_IR, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(venstre_frem, OUTPUT);
  pinMode(hojre_frem, OUTPUT);
  pinMode(venstre_tilbage, OUTPUT);
  pinMode(hojre_tilbage, OUTPUT);

  HC12.begin(9600);
}


void loop() {
  if (!start) {
    for (int i = 0; i < 4; i++) {
      kopState[i] = !digitalRead(kopPins[i]);  // Read button states and invert them because pull-up resistors are used
    }
  }


  //start knap
  if (digitalRead(startKnap) == LOW) {
    start = true;
    tilbage = false;
    data = false; // klar til at modtage data
  }

  // ir
  left_value = digitalRead(left_IR);
  right_value = digitalRead(right_IR);

  // echo
  unsigned long currentMillis = millis();
  if (currentMillis - lastExecutedHCSR0 >= EXE_INTERVAL_ECHO) {
    lastExecutedHCSR0 = currentMillis;  // save the last executed time

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;
  }

  if (distance < 20) {
    analogWrite(venstre_frem, 0);
    analogWrite(hojre_frem, 0);
    return;
  }

  // motor
  if (!end && start) {
    switch ((left_value == HIGH) << 1 | (right_value == HIGH)) {
      case 0b11:
        // begge høj
        analogWrite(venstre_frem, 0);
        analogWrite(hojre_frem, 0);
        end = true;
        break;
      case 0b10:
        // venstre høj
        analogWrite(venstre_frem, motor_fart + 50);
        analogWrite(hojre_frem, 0);
        break;
      case 0b01:
        // højre høj
        analogWrite(venstre_frem, 0);
        analogWrite(hojre_frem, motor_fart + 50);
        break;
      case 0b00:
        // begge  low
        analogWrite(venstre_frem, motor_fart);
        analogWrite(hojre_frem, motor_fart);
        break;
    }
  }

  if (end) {
    //Send kopper
    if (!tilbage) {
      for (int i = 0; i < 4; i++) {
        HC12.write(kopState[i]);
      }

      //Vent på opfyldning
      if (HC12.available() > 0) {
        data = HC12.read();  // read data from the other Arduino via HC-12
      }
    }


    if (data) {
      tilbage = true;

      delay(1000);
      unsigned long time = millis();
      while (digitalRead(left_IR) == LOW) {
        analogWrite(hojre_frem, 80);
        analogWrite(venstre_tilbage, 80);
      }
      analogWrite(hojre_frem, 0);
      analogWrite(venstre_tilbage, 0);

      delay(1000);
      end = false;
    }
  }
}