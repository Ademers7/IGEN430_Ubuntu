#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

// Include custom images
#include "images.h"


// Initialize the OLED display using Wire library
   SSD1306  display(0x3c, D2, D1);

int counter = 0;
bool currentStatus = LOW;
bool prevStatus = LOW;
void setup() {
  pinMode(D8, INPUT);
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  

}

void loop() {
    currentStatus = digitalRead(D8);
    if(prevStatus == LOW && currentStatus == HIGH){
        counter++;
    }
    // clear the display
    display.clear();
    display.drawString(0, 0, "A4P"); // x, y, text -> 0,0 at top left corner
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 14, "Aim For Pass");
    display.drawString(0, 28, String(counter));
    display.display();
    prevStatus = currentStatus;
    delay(250);

 }
