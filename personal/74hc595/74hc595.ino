//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

// Track which LEDs are on/offf
int matrix = 0;

// NOTE: Serial.print is freaking slow!

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
  // Looks dumb but reduces the wait on serial IO (println) which we only want to print after all input is successufully processed.
  // Empirically I'm seeing a println take ~10ms so it shouldd be avoided - adding "debug" prints is horrific for "performance"
  // As an example try an initial input of 1357 followed by 12345678123456781234567812345678123456781234567812345678123456781234567812345678
  // with some daditional Serial.println statements
  if (Serial.available()) {
    while (Serial.available() > 0) {
      while (Serial.available() > 0) {
        onAvailable();
      }
      delay(1);
    }
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
      Serial.print("Invalid input: ");
      Serial.println(input);
    }
  }
  
  shiftMatrix(matrix);
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
