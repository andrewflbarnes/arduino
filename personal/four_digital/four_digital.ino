//www.elegoo.com
//2016.12.12

int latch=9;  //74HC595  pin 9 STCP
int clock=10; //74HC595  pin 10 SHCP
int data=8;   //74HC595  pin 8 DS

int d1 = 5;
int d2 = 4;
int d3 = 3;
int d4 = 6;

int stopPin = 2;
boolean stopped = false;

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};//Display digital data

void setup() {
  Serial.begin(9600);
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  pinMode(d1,OUTPUT);
  pinMode(d2,OUTPUT);
  pinMode(d3,OUTPUT);
  pinMode(d4,OUTPUT);
  pinMode(stopPin,INPUT);

  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(stopPin), switchStop, RISING);
}
/*   The most common method of using 74CH595
 *   latch->LOW : Begin transmitting signals.
 *   shiftOut(dataPin, clockPin, bitOrder, value)
 *   dataPin: the pin on which to output each bit. Allowed data types: int.
 *   clockPin: the pin to toggle once the dataPin has been set to the correct value. Allowed data types: int.
 *   bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST. (Most Significant Bit First, or, Least Significant Bit First).
 *   value: the data to shift out. Allowed data types: byte. 
 *   latch->HIch : The end of the transmission signal.
*/
void Display(unsigned char num)
{

  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);
}

void switchStop() {
  stopped = !stopped;
}

int dig1 = 0;
int dig2 = 0;
int dig3 = 0;
int dig4 = 0;
void updateClock() {
  int now = millis() % 1000;
  dig1 = (now / 1000) % 10;
  dig2 = (now / 100) % 10;
  dig3 = (now / 10) % 10;
  dig4 = now % 10;
}

void loop() {
  Serial.println(stopped);
  if (!stopped) {
    updateClock();
  }
  Display(dig1);
  digitalWrite(d1, LOW);
  delay(1);
  digitalWrite(d1, HIGH);
  
  Display(dig2);
  digitalWrite(d2, LOW);
  delay(1);
  digitalWrite(d2, HIGH);
  
  Display(dig3);
  digitalWrite(d3, LOW);
  delay(1);
  digitalWrite(d3, HIGH);
  
  Display(dig4);
  digitalWrite(d4, LOW);
  delay(1);
  digitalWrite(d4, HIGH);
}
