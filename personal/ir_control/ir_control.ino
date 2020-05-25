/**
 * IR receiver sketch which triggers lights.
 * 
 * Follows the below model which could be used to drive a race (only 1 set of LEDs connected)
 * at the moment so would need expanding to support dual+ relay.
 * - When starting ready LED is on
 * - When the play button is pushed ready LED goes low and go LED turns on for 2 seconds
 * - When func/stop is pushed the DSQ LED goes high
 * - When rewind is pushed resets state and ready LED is on
 * - When power is pressed all LEDs will turn off/on (state is preserved)
 * 
 * Makes use of a 74HC595 which is maybe excessive but provides better forward support (e.g.
 * duals).
 *
 * Pin Map
 *
 * Arduino -> 74HC595
 * 8 -> ST_CP/RCLK (latch)
 * 9 -> SH_CP/SRCLK (shift)
 * 10 -> DS/SER (data)
 *
 * 74HC595 -> LEDs
 * Q1/QA -> LED_DSQ
 * Q2/QB -> LED_READY
 * Q3/QC -> LED_GO
 *
 * Arduino -> AX-1838HS
 * 11 -> 1 (left side)
 */

#include "IRremote.h"
#include "ir_control.h"

// IR receiver pin
const int RECEIVE_PIN = 11;

// 74HC595 pins
const int LATCH_PIN = 8;
const int SHIFT_PIN = 9;
const int DATA_PIN = 10;

// The Q pin to use on the 74HC595 (1 -> Q1/QA, 2-> Q2/QB, etc.) as per Pin Map
const int LED_DSQ = 1;
const int LED_READY = 2;
const int LED_GO = 3;

// Current state
boolean statReady = true;
boolean statDsq = false;
boolean statGo = false;
boolean statOn = true;

unsigned long goTimer = 0;
// How long the go LED will remain on in ms following it's activation
const int GO_TIME_DELAY = 2000;

unsigned long now = 0;

IRrecv irrecv(RECEIVE_PIN);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

void setup() {
  Serial.begin(57600);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  irrecv.enableIRIn();
}

void loop() {
  static unsigned long tmpValue;

  now = millis();

  if (irrecv.decode(&results)) {

    Serial.print(results.value, HEX);
    Serial.print(" ");
    if (IR_REPEAT == results.value) {
      results.value = tmpValue;
      Serial.print("_R");
      Serial.print(results.value, HEX);
      Serial.print(" ");
    }

    tmpValue = results.value;
    for (int i = 0; i < IR_COUNT + 1; i++) {
      if (IR_VALS[i] == results.value) {
        Serial.print(IR_DESCS[i]);
        Serial.print(" ");
      }
    }

    switch (results.value) {
    case IR_POWER:
      triggerOnOff();
      break;
    case IR_FUNC_STOP:
      triggerDsq();
      break;
    case IR_REWIND:
      triggerRestart();
      break;
    case IR_PLAY_PAUSE:
      triggerGo();
      break;
    }

    irrecv.resume(); // receive the next value
    Serial.println();
  }

  updateState();
  updateLeds();
}

void triggerOnOff() {
  statOn = !statOn;
}

void triggerDsq() {
  statDsq = true;
}

void triggerRestart() {
  statReady = true;
  statDsq = false;
  statGo = false;
}

void triggerGo() {
  statReady = false;
  statGo = true;
  goTimer = now + GO_TIME_DELAY;
}

void updateState() {
  if (goTimer > 0 && now > goTimer) {
    goTimer = 0;
    statGo = false;
    statReady = true;
  }
}

void updateLeds() {
  static unsigned int data;

  data = 0;

  if (statOn) {
    data = switchLed(data, statReady, LED_READY);
    data = switchLed(data, statDsq, LED_DSQ);
    data = switchLed(data, statGo, LED_GO);
  }

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, SHIFT_PIN, MSBFIRST, data);
  digitalWrite(LATCH_PIN, HIGH);
}

unsigned int switchLed(int matrix, boolean enable, int pos) {
  if (enable) {
    return matrix + (1 << pos);
  } else {
    return matrix;
  }
}
