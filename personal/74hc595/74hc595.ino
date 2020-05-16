//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

// Track which LEDs are on/offf
int matrix = 0;

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  shiftMatrix(0);
  printPrompt();
}

void printPrompt() {
  Serial.println("Enter led to switch or 'x' to reset");
}

void loop() {
  if (Serial.available()) {
    onAvailable ();
    printPrompt();
  }
}

void onAvailable() {
  char input = Serial.read();

  if (input == 'x') {
    matrix = 0;
  } else {
    int number = input - '0';
    if (number > 0 && number < 9) {
      int bin = 1 << (number - 1);
      matrix = matrix ^ bin;
    } else {
      Serial.println("Invalid input");
    }
  }
  
  shiftMatrix(matrix);
  
  Serial.println(matrix);
}

void shiftMatrix(const unsigned int matrix) {
  digitalWrite(latchPin, LOW);
  myShiftOut(dataPin, clockPin, MSBFIRST, matrix);
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
