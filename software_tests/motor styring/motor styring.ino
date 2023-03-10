
int puls = 3;
int retning1 = 4;
int retning2 = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(puls, OUTPUT);
  pinMode(retning1, OUTPUT);
  pinMode(retning2, OUTPUT);
  Serial.begin(9600);
  digitalWrite(retning1, LOW);
  digitalWrite(retning2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(puls, HIGH);
  Serial.println("HIGH");
  delay(500);
  digitalWrite(puls, LOW);
  Serial.println("LOW");
  delay(500);

}
