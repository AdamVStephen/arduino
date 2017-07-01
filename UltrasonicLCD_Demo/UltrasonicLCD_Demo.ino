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
   A0 is 12 bit 0-1023

   P (Pa) = rho * g * h
*/

// 3 levels of trace 0-3 (0 = off)
#define DEBUG_TRACE 0

#define VCC 5.0
#define MAX_KPA 10.0
#define COEFF_LIN_KPA 0.09
#define COEFF_OFFSET_KPA 0.04
// Min Vout 0.2 for standard values above
//#define MIN_VOUT (VCC * COEFF_OFFSET_KPA)
static const float MIN_VOUT = (VCC * COEFF_OFFSET_KPA);
// Max Vout 4.7 for standard values above
//#define MAX_VOUT (VCC * ((COEFF_LIN_KPA * MAX_KPA) + COEFF_OFFSET_KPA))
static const float MAX_VOUT = (VCC * ((COEFF_LIN_KPA * MAX_KPA) + COEFF_OFFSET_KPA));

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

int digitalToLevelCentimeters(long d) {
  return int(kpaToLevelCentimeters(voutToKpa(digitalToVout(d))));
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

byte level_01[8] = {0, 0, 0, 0, 0, 0, 0, 31};
byte level_02[8] = {0, 0, 0, 0, 0, 0, 31, 31};
byte level_03[8] = {0, 0, 0, 0, 0, 31, 31, 31};
byte level_04[8] = {0, 0, 0, 0, 31, 31, 31, 31};
byte level_05[8] = {0, 0, 0, 31, 31, 31, 31, 31};
byte level_06[8] = {0, 0, 31, 31, 31, 31, 31, 31};
byte level_07[8] = {0, 31, 31, 31, 31, 31, 31, 31};
byte level_08[8] = {31, 31, 31, 31, 31, 31, 31, 31};



/**
     Display arithmetic
*/

char topTitle[] = "H20 (cm) :";
int topTitleLen = String(topTitle).length();

/**
   Setup -----------------------------------------------------------------------------------
*/
char dbugbuf[50];

static int loops = 4695 - 30;

void setup() {
  Serial.begin(9600);

  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  showConversions();
  // create a new character
  lcd.createChar(0, level_01);
  // create a new character
  lcd.createChar(1, level_02);
  // create a new character
  lcd.createChar(2, level_03);
  // create a new character
  lcd.createChar(3, level_04);
  // create a new character
  lcd.createChar(4, level_05);
  // create a new character
  lcd.createChar(5, level_06);
  // create a new character
  lcd.createChar(6, level_07);
  // create a new character
  lcd.createChar(7, level_08);

  lcd.begin(16, 2);

  // Print a message to the LCD.
  //lcd.setCursor(0, 0);
  //lcd.print("H20 (cm) : ");
#if DEBUG_TRACE
  Serial.println("WaterLevel meter (arduino) : trace debug ON");
  Serial.println("Top Line Title");
  Serial.println(topTitle);
  sprintf("topTitle length = %d chars", toptTitleLen);
  Serial.println(dbugbuf);
#endif
  pinMode(13, OUTPUT);

}

/**
   Loop -----------------------------------------------------------------------------------
*/

int pressureBar(int d) {

#if DEBUG_TRACE
  char sbuf[50];
  sprintf(sbuf, "d %d v %f range %f - %f map 0,8", d, String(digitalToVout(d)).c_str(), String(MIN_VOUT).c_str(), String(MAX_VOUT).c_str());
  Serial.println(sbuf);
  Serial.print(digitalToVout(d));
  Serial.println();
  Serial.print(MIN_VOUT);
  Serial.println();
  Serial.print(MAX_VOUT);
  Serial.println();
#endif

  return constrain(map(digitalToVout(d), MIN_VOUT, MAX_VOUT, 0, 7), 0, 7);
}

int heightBar(int h) {
  return constrain(map(h, 0, 50, 0, 7), 0, 7);
}

char  serbuf[50];
char lcdbuf0[16];
char lcdbuf1[16];


void lcdHelp() {
  char buf[16] = "0123456789ABCEF";
  char * messages[] = {
    "Options for help"
    "Press s to set  ",
    "Press r to reset",
    "Press c to calib",
    "                "
  };
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0, 1);
    lcd.print(messages[i]);
    delay(2000);
  }
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

static int hlo = 100;
static int hhi = 0;
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int hav = 0;

void loop() {

  loops++;

  // Top Line
  lcd.setCursor(topTitleLen + 1, 0);
  lcd.print("   ");
  lcd.setCursor(topTitleLen + 1, 0);
#if DEBUG_TRACE
  float ll = loops * 1.0;
  lcd.print(String(ll).c_str());
  Serial.println("Loop counter from loop");
  Serial.println(String(ll).c_str());
#else

  int a0value;
  a0value = analogRead(0);
  int h = digitalToLevelCentimeters(a0value);
  lcd.setCursor(0, 0);
  sprintf(lcdbuf0, "%s%02d", topTitle, h);
  lcd.print(lcdbuf0);
  //lcd.print(h);
  int hbar = heightBar(h);
  lcd.setCursor(topTitleLen + 2 + 1, 0);
  lcd.write((byte)hbar);

  int j = loops % 8;
  lcd.setCursor(15, 0);
  lcd.write((byte)(7 - j));
  lcd.setCursor(15, 1);
  lcd.write((byte)(7 - j));

  // Update statistics
  if (h < hlo) {
    hlo = h;
  }
  if (h > hhi) {
    hhi = h;
  }
  total = total - readings[readIndex];
  readings[readIndex] = h;
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  float av = total / numReadings;
  hav = (int) av;
  sprintf(lcdbuf1, "%02d-%02d Avg %02d", hlo, hhi, hav);
  lcd.setCursor(0, 1);
  lcd.print(lcdbuf1);
  int avbar = heightBar(hav);
  lcd.setCursor(13, 1);
  lcd.write((byte)avbar);

  sprintf(serbuf, "Analogue value %d Bar %d", a0value, avbar);
  Serial.println(serbuf);

#endif

  int tms = millis();
  int uptimeSS = tms / 1000;
  int uptimeMM = uptimeSS % 60;
  int uptimeHH = uptimeSS / 3600;

  if (loops % 50 == 0) {
    lcdHelp();
  } else if (loops % 37 == 0) {
    lcd.setCursor(0, 1);
    sprintf(lcdbuf1, "%04d %02d:%02d      ", loops, uptimeHH, uptimeSS);
    lcd.print(lcdbuf1);
    delay(3000);
  } else if (loops %  1729 == 0) {
    lcd.setCursor(0, 1);
    lcd.print("1729 : Ramanujan");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("=1^3 + 12^3     ");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("=9^3 + 10^3     ");
    delay(2000);
  } else if (loops % 4695 == 0) {
    lcd.setCursor(0, 1);
    lcd.print("HappyBoatingNick");
    delay(2000);
  }

  Serial.print(loops);
  delay(1000);
  if (loops > 9999) {
    loops = 0;
  }
  return;
}
