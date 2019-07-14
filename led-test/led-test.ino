int LED = 3;
int BUTTON = 4;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

}

void loop() {

/*
  if (digitalRead(BUTTON) == LOW) {
    for (int i = 0; i < 128; i++) {
      Serial.println(i);
      analogWrite(LED, i);
      //delay(1);
    }
    analogWrite(LED, LOW);
    delay(1);
  } else {
    analogWrite(LED, LOW);
  }
  */

if (digitalRead(BUTTON) == HIGH) {
  analogWrite(LED, 127);
} else {

  analogWrite(LED,0);
}

  
}
