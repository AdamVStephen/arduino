/*
  LiquidCrystal Library - scrollDisplayLeft() and scrollDisplayRight()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 scrollDisplayLeft() and scrollDisplayRight() methods to scroll
 the text.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalScroll

 */

// include the library code:
#include <LiquidCrystal.h>

// TODO: Ticker tape for longer messages

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);
#define DISPLAY_WIDTH 16
#define DELAY 1000
#define SCROLL_DELAY 250

String lcdMessage = "Balshaw's Boat !";
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(lcdMessage.c_str());
  lcd.print(lcdMessage.c_str());
  delay(5*DELAY);
}

void loop() {
  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  int msgLen = lcdMessage.length();
  
  for (int positionCounter = 0; positionCounter <= msgLen; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(SCROLL_DELAY);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter <= msgLen + DISPLAY_WIDTH; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(SCROLL_DELAY);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter <= DISPLAY_WIDTH; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(SCROLL_DELAY);
  }

  // delay at the end of the full loop:
  delay(1000);

}

