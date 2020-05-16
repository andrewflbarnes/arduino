#define ENABLE 2
#define IN1 5
#define IN2 6
#define BUTTON_ENABLE 12

boolean forward = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENABLE, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BUTTON_ENABLE, INPUT);
}

void loop() {
  Serial.print(forward);
  Serial.print(" ");
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON_ENABLE)) {
    Serial.print("press ");
    while (digitalRead(BUTTON_ENABLE));
    Serial.print("release ");
    forward = !forward;
    delay(100);
  } else {
    Serial.print("pass ");
  }

  Serial.print(forward);
  Serial.print(" ");
  digitalWrite(ENABLE, HIGH);
  if (forward) {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, HIGH);
    Serial.println("forward LOW HIGH");
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.println("backward HIGH LOW");
  }
}
