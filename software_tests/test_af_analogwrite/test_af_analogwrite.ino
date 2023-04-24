int PWM = 0;

// test om man kan slukke for en analogWrite pin.

void setup() {
}

void loop() {

  analogWrite(9, PWM); // analogWrite start
  
  PWM++; // For hver gang vores loop køres gør vi vores puls bredere

  if (PWM == 255){ // når rammer vores maximale pulsbrede
    PWM = 0; // sættes den til 0

/*  ved at fortælle arduinoen at pin 9 er et output
  og derefter gøre pin 9 lav
  så kan vi stoppe vores PWM
  
  Vi kan starte vores PWM igen, ved at køre analogWirte() funktionen igen    */

    pinMode(9, OUTPUT); 
    digitalWrite(9, LOW); 

    delay(5000);
  }

  delay(50);
}
