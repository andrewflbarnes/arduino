/**
 * Measures the temp and humidity from a DHT11 sensor and displays it on
 * a LCD1602 in Celsius and % respectively.
 * 
 * This code assumes humidity and temperature will always be two digits
 * and is displayed to 1dp (though the DHT11 specifically doesn't
 * support this precision, other models do).
 * 
 * This is untested against negative temperatures, single digit temp/humid
 * and triple digit temp/humid. These will likely display fine until a digit
 * is dropped e.g. going from 10% to 9% humidity. This will likely present
 * itself as addtional (zeroed) DPs.
 * 
 * Pin Map
 * 
 * VCC: LCD1602[VDD, A], 10K Pot.[left], DHT11[middle]
 * GND: LCD1602[VSS, RW, K], 10K Pot.[right], DHT11[right]
 * 
 * Arduino -> LCD1602
 * 7 -> RS
 * 8 -> E
 * 9 -> D4
 * 10 -> D5
 * 11 -> D6
 * 12 -> D7
 * 
 * 10K Potentiometer -> LCD 1602
 * wiper -> V0
 * 
 * Arduino -> DHT11
 * 6 -> DATA (left)
 * 
 */

// Modify below lib for faster responses: cooldown time = 0; high impedance delay = 10
#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

#define DHT_TYPE DHT_TYPE_11

#define DHT_PIN 2
#define DHT_MEASURE_COOLDOWN 1000

#define LCD_RS 7
#define LCD_E 8
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12

#define FANCY_INIT true

// which column the temperature and humidity data should start at
const int LCD_DATA_POS = 7;
const int LCD_ROWS = 2;
const int LCD_COLS = 16;

DHT_nonblocking dht_sensor(DHT_PIN, DHT_TYPE);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);
  // According to the data sheet/timing we shouldn't attempt a reading for the first second
  waitForSensorsInit(1000);
  templateLcd();
}

void loop() {
  static float temperature;
  static float humidity;

  if (measure_environment(&temperature, &humidity)) {
    updateLcd(temperature, humidity);
  }
}

/**
 * Blocking method which waits for sensors to intialise and prints
 * an intialising loader to the LCD.
 * 
 * This specifically expects a 2x16 LCD
 */
void waitForSensorsInit(long delayMs) {
  long left = delayMs;
  long until = millis() + delayMs;
  int pos;

  if (FANCY_INIT) {
    lcd.setCursor(0, 0);
    lcd.print("  INITIALISING  ");
  } else {
    templateLcd();
  }

  while (left > 0) {
    if (FANCY_INIT) {
      // Fill the bottom row with a "fully loaded" bar
      lcd.setCursor(0, 1);
      lcd.print("................");

      // Fill the end with spaces depending on how much time is left.
      // The more time which is left, the lower the colulmn position,
      // so the more spaces are written and the fewer "."s display
      pos = map(left, delayMs, 0, 0, LCD_COLS);
      lcd.setCursor(pos, 1);
      lcd.print("                ");
    }

    // Wait a while
    delay(10);
    left = until - millis();
  }
}

/**
 * Sets up the LCD template
 */
void templateLcd() {
  lcd.setCursor(0, 0);
  lcd.print("Temp.: --.- C   ");
  lcd.setCursor(0, 1);
  lcd.print("Humi.: --.- %   ");
}

/**
 * Updates the LCD display with the new humidity and temperature
 */
void updateLcd(float temperature, float humidity) {
  // refresh the template so any transmissions errors are cleared from the screen
  templateLcd();
  lcd.setCursor(LCD_DATA_POS, 0);
  lcd.print(temperature, 1);
  lcd.setCursor(LCD_DATA_POS, 1);
  lcd.print(humidity, 1);
}

/**
 * Returns true if a new measurement is available
 */
bool measure_environment(float *temperature, float *humidity ) {
  static unsigned long lastMeasurement = 0;

  if (millis() - lastMeasurement > DHT_MEASURE_COOLDOWN) {
    if (dht_sensor.measure(temperature, humidity)) {
      lastMeasurement = millis( );
      return true;
    }
  }

  return false;
}
