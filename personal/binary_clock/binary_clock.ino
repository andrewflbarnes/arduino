
const int SECOND_PINS[] = {7, 8, 9, 10}; // +2 managed by 74HC595 as below
const int SECOND_COUNT = 4;
const int HOUR_PINS[] = {2, 3, 4, 5, 6};
const int HOUR_COUNT = 5;
const boolean HOUR_24 = true;

// minutes are managed by 74HC595. the two most significant bits are for seconds as well, bit 8 -> 2s, bit 7 -> 1s
//Pin connected to ST_CP of 74HC595
int latchPin = 11;
//Pin connected to SH_CP of 74HC595
int clockPin = 13;
////Pin connected to DS of 74HC595
int dataPin = 12;
// timing PINs

// Track which LEDs are on/offf
unsigned long matrix = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  int i;
  for (i = 0; i < SECOND_COUNT; i++) {
    pinMode(SECOND_PINS[i], OUTPUT);
  }
  for (i = 0; i < HOUR_COUNT; i++) {
    pinMode(HOUR_PINS[i], OUTPUT);
  }
}

void loop() {
  matrix = 0;
  long now = millis() / 1000;
  setHours(now);
  setMinutes(now);
  setSeconds(now);
  updateMatrix();
}

void setHours(long now) {
  // these are set per pin, not off the matrix
  int hours = getHours(now);
  for (int i = 0; i < HOUR_COUNT; i++) {
    int led = HOUR_PINS[i];
    if (bitRead(hours, i) > 0) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
  }
}

void setMinutes(long now) {
  // these are set on the lower 6 bits of the matrix, no bit shift required
  matrix = matrix + (getMinutes(now) << 0);
}

void setSeconds(long now) {
  // upper 2 bits of the matrix and on 4 dedicated pins
  // shift 6 bits (minutes bits)
  // it doesn't matter about the upper 4 bits as they won't get used
  int seconds = getSeconds(now);
  matrix = matrix + (seconds << 6);

  // now set the dedicated pins
  for (int i = 0; i < SECOND_COUNT; i++) {
    int led = SECOND_PINS[i];
    // add 2 to the bit shift as the bottom 2 bits are set already
    if (bitRead(seconds, i + 2)) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
  }
}

// Can be replaced with RTC later
int getHours(long now) {
  int hours;
  if (HOUR_24) {
    hours = 24;
  } else {
    hours = 12;
  }
  return (now / (60 * 60)) % hours;
}

// Can be replaced with RTC later
int getMinutes(long now) {
  return (now / 60) % 60;
}

// Can be replaced with RTC later
int getSeconds(long now) {
  return now % 60;
}

void updateMatrix() {
  digitalWrite(latchPin, LOW);
  myShiftOut(dataPin, clockPin, MSBFIRST, matrix & 255);
//  shiftOut(dataPin, clockPin, MSBFIRST, matrix);
  digitalWrite(latchPin, HIGH);
}

// Arduino already provides shiftOut as well as SPI but useful to know what's going on
void myShiftOut(int dataPin, int clockPin, int order, int matrix) {
  int bitValue;
  for (int i = 0; i < 8; i++) {
    if (order == MSBFIRST) {
      bitValue = !!(matrix & (1 << (7 - i)));
    } else { //LSBFIRST
      bitValue = !!(matrix & (1 << i));
    }

    digitalWrite(dataPin, bitValue);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}
