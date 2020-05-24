/**
 * For a binary clock we need:
 * - 4 LEDs for hours (5 for 24 hour)
 * - 6 LEDs for minutes
 * - 6 LEDs for seconds
 *
 * Using 2 daisy chained 8 bit shift registers (74HC595) we can set 16 LEDs using 3
 * pins and have a dedicated PIN for the 16 hour LED.
 */

/*
 * Pin map
 *
 * Assume we have 15 LEDs as below in a row (s - seconds, m - minutes, h - hours)
 * with high bits on the left
 * HHHHH MMMMMM SSSSSS
 *
 * For example above the leftmost LED is H5 (hours bit 5 / 16 hour) and the rightmost bit
 * is S1 (seconds bit 1 / 1 second)
 *
 * Use 2 74HC595. We will use 595(1) for the low bits (S1 - M2) and 595(2) for the high
 * bits (M3 - H4).
 *
 * 595 - both
 * 8 - LOW (GND)
 * 10 - HIGH (reset)
 * 13 - LOW (output enable)
 * 16 - HIGH (VCC)
 *
 * 595(1) -> LED
 * 1 -> S1
 * 2 -> S2
 * 3 -> S3
 * 4 -> S4
 * 5 -> S5
 * 6 -> S6
 * 7 -> M1
 * 8 -> M2
 *
 * 595(2) -> LED
 * 1 -> M1
 * 2 -> M2
 * 3 -> M3
 * 4 -> M4
 * 5 -> H1
 * 6 -> H2
 * 7 -> H3
 * 8 -> H4
 *
 * Arduino -> LED
 * 8 -> H5
 *
 * 595(1) -> 595(2)
 * 11 -> 11
 * 12 -> 12
 * 9 -> 14 (Q7/QH out -> DS/SER)
 *
 * Arduino -> 595(1)
 * 11 -> 14
 * 12 -> 12
 * 13 -> 11
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
