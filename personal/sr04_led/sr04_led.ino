/**
 * Activates an LED if something comes within a certain distance of the SR04
 * ultrasound sensor.
 *
 * The closer the object the brighter the LED.
 */

// Note: this particular lib is slow because of some artificial delays
#include "SR04.h"

const int TRIG_PIN = 12;
const int ECHO_PIN = 11;
const int LED_PIN = 10;

// Maximum distance in cm before LED will turn on
const int THRESHOLD_CM = 40;

const SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

void setup() {
 pinMode(LED_PIN, OUTPUT);
}

void loop() {
  static unsigned long a;
  static unsigned int adj;
  // cm
  a = sr04.Distance();

  adj = map(a, 0, THRESHOLD_CM, 255, 0);

  if (a <= THRESHOLD_CM) {
    analogWrite(LED_PIN, adj);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
