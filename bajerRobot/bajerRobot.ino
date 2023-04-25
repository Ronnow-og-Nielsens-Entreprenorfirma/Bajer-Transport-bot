/*

    Kristoffer N og Jeppe R
     Bajer transport robot
         Version 1.0

          26/04/23
*/

// Importere biblotek til styring af HC12
#include <SoftwareSerial.h>

// HC12 pins og data ind boolean
SoftwareSerial HC12(12, 11);  // TX, RX pins for HC-12
bool data = false;

// ir sensor pins
#define left_IR A0
#define right_IR A1

// echo sensor pins og andre variabler til at syre HC-SR0
#define trigPin 7
#define echoPin 8
float duration, distance;
#define EXE_INTERVAL_ECHO 100
unsigned long lastExecutedHCSR0 = 0;

// start knap pin og tilstands boolean
#define startKnap 2
bool start = false;

// pins til pwm output til hbro
#define venstre_frem 10
#define hojre_frem 9
#define venstre_tilbage 5
#define hojre_tilbage 6

// duty cycle af pwm
#define motor_fart 80

// tilstand af ir sensor booleans
bool left_value = LOW;
bool right_value = LOW;

// til at fortælle hvis route er færdig
bool end = false;

// til at fortælle at den er på vej tilbage
bool tilbage = false;

// array af boolean til at holde styr på hvilke kopper er i
bool kopState[4] = { false, false, false, false };
// Pins til Knapper for kopperne
int kopPins[4] = { 3, 4, 19, 18 };

// Setup køres engang på start af app
void setup() {
  for (int i = 0; i < 4; i++) {            // for loop der definere kop knapperne som INPUT med PULLUP
    pinMode(kopPins[i], INPUT_PULLUP);
  }

  pinMode(startKnap, INPUT_PULLUP); // Definere start knap pin som INPUT med PULLUP

  // Definere ir sensor input pins
  pinMode(left_IR, INPUT);  
  pinMode(right_IR, INPUT);

  // Definere echo sensor send og modtag pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Definere pwm output pins
  pinMode(venstre_frem, OUTPUT);
  pinMode(hojre_frem, OUTPUT);
  pinMode(venstre_tilbage, OUTPUT);
  pinMode(hojre_tilbage, OUTPUT);

  HC12.begin(9600); // initialisere HC12 trådløst kommunikationsmodul med en baudrate på 9600 bits i sekundet
}

// void Loop looper konstant
void loop() {
  if (!start) {  // hvis start knap ikke er trykket
    for (int i = 0; i < 4; i++) {             // for loop der tjekker om kop knapper er trykket
      kopState[i] = !digitalRead(kopPins[i]); // og gemmer i boolean array
    }
  }


  //start knap
  if (digitalRead(startKnap) == LOW) { // hvis start knap er trykket
    start = true;  // set start til true så robotten starter
    tilbage = false; // reset tilbage boolean
    data = false; // reset data så klar til at modtage data
  }

  // ir
  left_value = digitalRead(left_IR); // aflæs venstre ir sensor data
  right_value = digitalRead(right_IR); // aflæs højre ir sensor data

  // echo
  unsigned long currentMillis = millis(); // Definere tid nu
  if (currentMillis - lastExecutedHCSR0 >= EXE_INTERVAL_ECHO) {  //  køre if statemen hvert 100ms
    lastExecutedHCSR0 = currentMillis;  // gemmer sidste kørte tid

    digitalWrite(trigPin, LOW);  // sætter echo send pin til lav
    delayMicroseconds(2);        // venter 2 mikrosekunder
    digitalWrite(trigPin, HIGH); // sætter echo send pin til høj
    delayMicroseconds(10);       // venter 10 mikrosekunder
    digitalWrite(trigPin, LOW);  // sætter echo send pin til lav igen

    duration = pulseIn(echoPin, HIGH); // tager tiden det tager trig at reflektere tilbage til echo
    distance = (duration * .0343) / 2; // omregner tiden til distance i cm
  }

  if (distance < 20) {   // hvis distance er mindre end 20 cm stoppes motorne
    analogWrite(venstre_frem, 0);
    analogWrite(hojre_frem, 0);
    return; // går tilbage til start af void loop
  }

  // motor
  if (!end && start) {   // hvis start knap trykket og ende ikke nået
    switch ((left_value == HIGH) << 1 | (right_value == HIGH)) {  // tager værdi af venstre ir sensor og skubber binær værdi en bit til venstre (<< 1)
                                                                  // og så kombinere det med værdien for højre ir sensor (|)
      case 0b11:  // hvis begge er høje køres case
        // motorne stoppes
        analogWrite(venstre_frem, 0); 
        analogWrite(hojre_frem, 0);
        end = true; // fortæller at enden er nået
        break;
      case 0b10: // hvis venste ir høj køres case
        // højre motor slukkes
        analogWrite(venstre_frem, motor_fart + 50);
        analogWrite(hojre_frem, 0);
        break;
      case 0b01: // hvis højre ir høj køres case
        // venstre motor slukkes
        analogWrite(venstre_frem, 0);
        analogWrite(hojre_frem, motor_fart + 50);
        break;
      case 0b00: // hvis begge ir er lav køres case
        // begge motor kører
        analogWrite(venstre_frem, motor_fart);
        analogWrite(hojre_frem, motor_fart);
        break;
    }
  }

  if (end) {  // hvis enden er nået
    //Send kopper
    if (!tilbage) { // hvis robotten ikke var på vej tilbage
      for (int i = 0; i < 4; i++) { // så sendes byte array med hvilke kop holder der er kopper i til øltårn
        HC12.write(kopState[i]);
      }

      // modtager
      if (HC12.available() > 0) {  // hvis HC12 er klar
        data = HC12.read();  // læser data fra øltårn, hvis modtaget sættes data til true
      }
    }

    
    if (data) {  // hvis signal fra øltårn er modtaget
      delay(3000); // venter 3 sekunder hvis nu øltårnet drypper 
      while (digitalRead(left_IR) == LOW) { // rotere om sig selv indtil venstre ir sensor opdager noget
        analogWrite(hojre_frem, 80);
        analogWrite(venstre_tilbage, 80);
      }
      analogWrite(hojre_frem, 0);          // når venstre ir sensor opdager noget stoppes rotation
      analogWrite(venstre_tilbage, 0);

      delay(1000);  // venter et sekund, så motor når at stå helt stille

      if (tilbage) start = false; // hvis den er på vej tilbage så 
      tilbage = true; // fortæller at den er på vej tilbage
      end = false;  // fortæller at slut frekvens er færdig, så den kan begynde at køre tilbage
    }
  }
}