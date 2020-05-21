/**
 * Stop watch with stop, start and reset functionality via a button.
 *
 * Includes a configurable decimal point (compile/upload time).
 *
 * To display each digit is loaded into the 74HC595 then the pin
 * corresponding to that digit is pulsed HIGH for 1ms
 */

// 74HC595 -> 5461AS
// Q0 -> 11
// Q1 -> 7
// Q2 -> 4
// Q3 -> 2
// Q4 -> 1
// Q5 -> 10
// Q6 -> 5
// Q7 -> 3

// Arduino -> 5461AS (each via 220 ohm resistor)
// 3 -> 12
// 4 -> 9
// 5 -> 8
// 6 -> 6

const long POWER_10[] = {1, 10, 100, 1000, 10000};

const int DIGIT_COUNT = 4;
int digits[DIGIT_COUNT] = {0, 0, 0, 0};

const int LATCH_PIN = 9;  //74HC595  pin 9 STCP
const int CLOCK_PIN = 10; //74HC595  pin 10 SHCP
const int DATA_PIN = 8;   //74HC595  pin 8 DS
const int BUTTON_PIN = 2; // start, stop, reset with 10k pulldown resistor
const int DIGIT_PINS[DIGIT_COUNT] = {3, 4, 5, 6};

const long ON_TIME = 2000;
long lastOn = 0;

// decimal places: max 3, min 0
const int DP = 2;

boolean stopped = false;
boolean reset = true;

unsigned long start = 0;

int displayDigit = 0;
int lastDigit = DIGIT_COUNT - 1;

const unsigned char TABLE[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  
  for (int i = 0 - 1; i < DIGIT_COUNT; i++) {
    pinMode(DIGIT_PINS[i], OUTPUT);
    digitalWrite(DIGIT_PINS[i], HIGH);
  }
  
  pinMode(BUTTON_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), switchPress, RISING);
}

void loop() {
  if (!stopped && !reset) {
    updateClock();
  }
  refreshDisplay();
}

void switchPress() {
  if (reset) {
    // was reset -> transition to running
    reset = false;
    stopped = false;
    start = millis();
  } else if (stopped) {
    // was stopped -> transition to reset
    stopped = false;
    reset = true;
    start = 0;
    for (int i = 0; i < DIGIT_COUNT; i++) {
      digits[i] = 0;
    }
  } else {
    // was running -> transition to stopped
    stopped = true;
    reset = false;
  }
}

void updateClock() {
  static unsigned long nowUnits;
  
  nowUnits = (millis() - start) / POWER_10[DIGIT_COUNT - DP - 1];
  for (int i = 0; i < DIGIT_COUNT; i++) {
    digits[i] = (int)(nowUnits / POWER_10[DIGIT_COUNT - i - 1]) % 10;
  }
}

void refreshDisplay() {
  static unsigned long now;
  
  now = micros();

  // only update if the current digit/segment has been on for at least ON_TIME microseconds
  if (now - ON_TIME > lastOn) {
    lastOn = now;

    lastDigit = displayDigit;
    displayDigit = (displayDigit + 1) % DIGIT_COUNT;

    digitalWrite(DIGIT_PINS[lastDigit], HIGH);
    
    if (displayDigit == DIGIT_COUNT - DP - 1) {
      // enable the decimal point
      shiftDigit(digits[displayDigit], 0x80);
    } else {
      shiftDigit(digits[displayDigit]);
    }
    
    digitalWrite(DIGIT_PINS[displayDigit], LOW);
  }
}

void shiftDigit(unsigned char num) {
  shiftDigit(num, 0);
}

void shiftDigit(unsigned char num, unsigned char modifier) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, TABLE[num] | modifier);
  digitalWrite(LATCH_PIN, HIGH);
}
