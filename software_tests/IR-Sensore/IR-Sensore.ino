
int ir1 = 3;
int ir2 = 4;


void setup() {
  // put your setup code here, to run once:
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  if (digitalRead(ir1) == LOW) Serial.println("IR 1 NO");
  if (digitalRead(ir2) == LOW) Serial.println("IR 2 NO");
  if (digitalRead(ir1) == HIGH) Serial.println("IR 1 YES");
  if (digitalRead(ir2) == HIGH) Serial.println("IR 2 YES");
}
