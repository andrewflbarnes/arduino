/**
 * For a binary clock we need:
 * - 4 LEDs for hours (5 for 24 hour)
 * - 6 LEDs for minutes
 * - 6 LEDs for seconds
 *
 * Using 2 daisy chained 8 bit shift registers (74HC595) we can set 16 LEDs using 3
 * pins and have a dedicated PIN for the 16 hour LED.
 */
const boolean HOUR_24 = true;

// Pin connected to ST_CP/RCLK of 74HC595
const int LATCH_PIN = 12;
// Pin connected to SH_CP/SRCLK of 74HC595
const int CLOCK_PIN = 13;
// Pin connected to DS/SER of 74HC595
const int DATA_PIN = 11;
// Dedicated pin for 16 hours LED
const int HOUR16_PIN = 8;

// Track which LEDs are on/off
unsigned long now = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int hoursInDay;

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(HOUR16_PIN, OUTPUT);

  if (HOUR_24) {
    hoursInDay = 24;
  } else {
    hoursInDay = 12;
  }
}

void loop() {
  updateTime();
  updateDisplay();
}

void updateTime() {
  // replace with RTC if you have something which can do that
  now = millis() / 1000;
  hours = (now / (60 * 60)) % hoursInDay;
  minutes = (now / 60) % 60;
  seconds = now % 60;
}

void updateDisplay() {
  unsigned long matrix = (hours << 12);
  matrix += (minutes << 6);
  matrix += seconds;

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (matrix >> 8) & 255);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, matrix & 255);
  digitalWrite(LATCH_PIN, HIGH);

  if (hours >= 16) {
    digitalWrite(HOUR16_PIN, HIGH);
  } else {
    digitalWrite(HOUR16_PIN, LOW);
  }
}
