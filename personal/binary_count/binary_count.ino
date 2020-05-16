
const unsigned int L_1 = 2;
const unsigned int L_2 = 3;
const unsigned int L_3 = 4;
const unsigned int L_4 = 5;
const unsigned int L_5 = 6;
const unsigned int L_6 = 7;
const unsigned int L_COUNT = 6;

const unsigned int L_ALL[] = {L_1, L_2, L_3, L_4, L_5, L_6};

const unsigned int B_MOD = 8;

const int MODE_BIN = 0;
const int MODE_BIN_REV = 1;
int mode = MODE_BIN;

unsigned long now;
unsigned long nowSeconds;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < L_COUNT; i++) {
    pinMode(L_ALL[i], OUTPUT);
  }

  pinMode(B_MOD, INPUT);
}

void loop() {
  now = millis();
  nowSeconds = (now / 100) % 60;

  if (updateButtonState(now)) {
    switch (mode) {
      case MODE_BIN:
        mode = MODE_BIN_REV;
        break;
      case MODE_BIN_REV:
        mode = MODE_BIN;
        break;
    }
  }

  for (int i = 0; i < L_COUNT; i++) {
    digitalWrite(L_ALL[i], LOW);
  }

  switch (mode) {
    case MODE_BIN_REV:
    case MODE_BIN:
    default:
      binaryLights(nowSeconds, mode);
      break;
  }
  
}

void binaryLights(const int activationMatix, const int mode) {
  
  int activationVal = 1;
  
  Serial.print(activationMatix);
  Serial.print(" ");
  
  for (int i = 0; i < L_COUNT; i++) {
    if ((activationMatix & activationVal) > 0) {
      int idx = i;
      if (mode == MODE_BIN_REV) {
        idx = L_COUNT - 1 - i;
      }
      digitalWrite(L_ALL[idx], HIGH);
      Serial.print(activationVal);
    } else {
      Serial.print(0);
    }
    Serial.print(" ");
    activationVal *= 2;
  }
  
  Serial.println();
}


unsigned int activeTime = 0;
unsigned int inactiveTime = 0;
unsigned int lastActive = 0;

boolean updateButtonState(const unsigned int now) {

  boolean pushEvent = false;
  int active = digitalRead(B_MOD);

  if (active == HIGH) {
    activeTime = now;
    if (lastActive == LOW) {
    }
  }

  if (active == LOW) {
    if (lastActive == HIGH && activeTime - inactiveTime > 100) {
      pushEvent = true;
    }
    inactiveTime = now;
  }
  
  lastActive = active;

  return pushEvent;
}
