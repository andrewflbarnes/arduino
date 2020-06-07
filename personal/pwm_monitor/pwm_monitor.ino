/**
 * Simple tool for monitoring PWM duty cycle - we don't all own oscilloscopes!
 *
 * Every time a cycle completes (detected as rising edge) the low time, high time
 * and cycle time are output to Serial. This makes it easy to monitor in the
 * Arduino IDE though alternatively some other device could be connected to
 * graph the information.
 *
 * When monitoring the change in duty cycle for fixed frequency PWM with the
 * Arduino IDE it is useful to enable the thresholds. These should be set to just
 * below the minimum on/off time and just above the cycle time to prevent the
 * y axis from being auto-adjusted.
 *
 * e.g. For a 100Hz PWM (10 ms cycle time) with 5-95% adjustable duty cycle the
 * values 0 and 11000 can be used.
 *
 * Note this has a very low resolution and as such, depending on the frequency,
 * duty cycle and Serial baud rate, not all data may be displayed.
 *
 * As an example, monitoring a PWM with frequency of 800-900Hz at 115200 baud
 * only the threshold min (assumoing 0) and 2 other values can be consistently
 * reported (own experience). Can likely be improved with lower level code and
 * packing Serial data (to be processed by some other program).
 */
#define __pwm_monitor_threshold_min__ 0
//#define __pwm_monitor_threshold_max__ 13000
#define __pwm_monitor_time_low__
#define __pwm_monitor_time_high__
#define __pwm_monitor_time_total__

const int INTERRUPT_PIN = 2;
const unsigned long BAUD_RATE = 115200;

void setup() {
  pinMode(INTERRUPT_PIN, INPUT);
  Serial.begin(BAUD_RATE);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), checkDuty, CHANGE);
}

void loop() {}

void checkDuty() {
  static long lastHigh = micros();
  static long lastLow = micros();
  static long newHigh;
  static long lowTime;
  static long highTime;
  static long high = false;

  static int pwm;
  pwm = digitalRead(INTERRUPT_PIN);

  if (pwm == HIGH) {
    newHigh = micros();
    lowTime = max(newHigh - lastLow, 0);
    lastHigh = newHigh;
    high = true;

#ifdef __pwm_monitor_threshold_min__
    Serial.print(__pwm_monitor_threshold_min__);
    Serial.print(" ");
#endif
#ifdef __pwm_monitor_threshold_max__
    Serial.print(__pwm_monitor_threshold_max__);
    Serial.print(" ");
#endif
#ifdef __pwm_monitor_time_low__
    Serial.print(lowTime);
    Serial.print(" ");
#endif
#ifdef __pwm_monitor_time_high__
    Serial.print(highTime);
    Serial.print(" ");
#endif
#ifdef __pwm_monitor_time_total__
    Serial.print(lowTime + highTime);
#endif
    Serial.println();
} else {
    lastLow = micros();
    highTime = max(lastLow - lastHigh, 0);
    high = false;
  }
}
