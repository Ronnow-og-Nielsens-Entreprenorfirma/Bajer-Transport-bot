
int puls = 10;
int retning1 = 11;
int retning2 = 12;

bool retningState = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(puls, OUTPUT);
  pinMode(retning1, OUTPUT);
  pinMode(retning2, OUTPUT);
  Serial.begin(9600);
  
  digitalWrite(retning1, LOW);
  digitalWrite(retning2, LOW);
}

int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(puls, HIGH);

  
  i = i + 50;
  Serial.println(i);
  
  analogWrite(puls, i);
  delay(1000);
  digitalWrite(puls, LOW);
  digitalWrite(retning2, !retningState);
  
  digitalWrite(retning1, !retningState);
  delay(300);
  retningState = !retningState;
  
  if (i == 250){
    i = 0;
  }

}
