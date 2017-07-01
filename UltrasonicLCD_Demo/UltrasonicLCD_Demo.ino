/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

// include the library code:

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//#include "SR04.h"
//#define TRIG_PIN 12
//#define ECHO_PIN 11
//SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
long min = 10;


static float sim_level = 25.0;

/**

   Sensor Transformations

   Range 0.2V = 0 kPa to 4.7V = 10.0 kPa
   A0 is 12 bit 0-1023 ADC

   P (Pa) = rho * g * h
*/

#define VCC 5.0
#define MAX_KPA 10000.0
#define COEFF_LIN_KPA 0.09
#define COEFF_OFFSET_KPA 0.04
// Min Vout 0.2 for standard values above
#define MIN_VOUT (VCC * COEFF_OFFSET_KPA)
// Max Vout 4.7 for standard values above
#define MAX_VOUT (VCC * ((COEFF_LIN_KPA * MAX_KPA) + COEFF_OFFSET_KPA))

// Physical constants
// Gravitational acceleration ms^-2
#define G_ACC 9.81
// Density of water
#define RHO_WATER 1000.0

float digitalToVout(long d) {
  return (VCC * d) / 1023.0;
}

float voutToDigital(float v) {
  return (v * 1023.0) / VCC;
}

float kpaToVout(float kpa) {
  // Clip to max range
  if (kpa > MAX_KPA) {
    kpa = MAX_KPA;
  }
  return VCC * (COEFF_LIN_KPA * kpa + COEFF_OFFSET_KPA);
}

float voutToKpa(float v) {
  // Clip to range
  if (v < MIN_VOUT) {
    v = MIN_VOUT;
  }
  if (v > MAX_VOUT) {
    v = MAX_VOUT;
  }
  return ( v - MIN_VOUT ) / (VCC * COEFF_LIN_KPA);
}

float voutToPa(float v) {
  return 1000.0 * voutToKpa(v);
}

float kpaToLevelMeters(float kpa) {
  return (kpa * 1000.) / (RHO_WATER * G_ACC);
}

float kpaToLevelCentimeters(float kpa) {
  return (100.0 * kpaToLevelMeters(kpa));
}

float digitalToLevelCentimeters(long d) {
  return kpaToLevelCentimeters(voutToKpa(digitalToVout(d)));
}


char *ftoa(char *a, double f, int precision)
{
  long p[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

void showConversions() {
  long d;
  float vout;
  float v;
  float kpa;
  float h;
  float H;
  long sampleValue[] = {32, 64, 128, 256, 512, 768, 1023};
  int sampleCount = 7;
  for (int i = 0; i < sampleCount; i++) {
    d = sampleValue[i];
    vout = digitalToVout(d);
    kpa = voutToKpa(vout);
    h = kpaToLevelCentimeters(kpa);
    vout = 3.14;
    char outbuf[60];
    sprintf(outbuf, "| ADC %ld : V %s V : P %s kPa : H %s cm; %s", d, String(vout).c_str(), String(kpa).c_str(), String(h).c_str());
    Serial.println(outbuf);
    sprintf(outbuf, "| ADC %ld : V %s V",  d, String(vout).c_str());
    Serial.println(outbuf);
    Serial.println(v);

  }
  delay(5000);
}

// make some custom characters:
byte level_0[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte level_00[8] = {0, 0, 0, 0, 0, 0, 0, 0};
byte level_01[8] = {0, 0, 0, 0, 0, 0, 0, 31};
byte level_02[8] = {0, 0, 0, 0, 0, 0, 31, 31};
byte level_03[8] = {0, 0, 0, 0, 0, 31, 31, 31};
byte level_04[8] = {0, 0, 0, 0, 31, 31, 31, 31};
byte level_05[8] = {0, 0, 0, 31, 31, 31, 31, 31};
byte level_06[8] = {0, 0, 31, 31, 31, 31, 31, 31};
byte level_07[8] = {0, 31, 31, 31, 31, 31, 31, 31};
byte level_08[8] = {31, 31, 31, 31, 31, 31, 31, 31};


byte level_1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};



/**
   Setup -----------------------------------------------------------------------------------
*/

void setup() {
  Serial.begin(9600);

  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  showConversions();
  // create a new character
  lcd.createChar(0, level_00);
  // create a new character
  lcd.createChar(1, level_01);
  // create a new character
  lcd.createChar(2, level_02);
  // create a new character
  lcd.createChar(3, level_03);
  // create a new character
  lcd.createChar(4, level_04);
  // create a new character
  lcd.createChar(5, level_05);
  // create a new character
  lcd.createChar(6, level_06);
  // create a new character
  lcd.createChar(7, level_07);
  // create a new character
  lcd.createChar(8, level_08);

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("H20 (cm) : ");
  pinMode(13, OUTPUT);

}

/**
   Loop -----------------------------------------------------------------------------------
*/


void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(6, 1);
  //lcd.print("    ");
  //lcd.setCursor(6, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  //a = sr04.Distance();
  // a = 50.; //levelToVoltage(5.0);
  //lcd.print(a);
  if (a < min) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  showConversions();
  delay(250);
  // Turn off the blinking cursor:
  lcd.noBlink();
  delay(3000);
  // Turn on the blinking cursor:
  lcd.blink();
  delay(3000);
  lcd.setCursor(0, 1);
  for (int i = 0; i < 9; i++) {
    lcd.write((byte)i);

  }
}
