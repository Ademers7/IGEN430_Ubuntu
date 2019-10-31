#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 15
#define LED_TYPE WS2812B
#define BRIGHTNESS 50
// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  // Uncomment/edit one of the following lines for your leds arrangement.  
  FastLED.addLeds<LED_TYPE, 5, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() { 
  // Turn the LED on, then pause
  for(int i = 0; i < 15; i++){
    leds[i] =CRGB::Blue;
  }
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  for(int i = 0; i < 15; i++){
    leds[i] =CRGB::Black;
  }
  FastLED.show();
  delay(500);
}
