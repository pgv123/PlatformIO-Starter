#include <FastLED.h>

#define SHOW_LOOPS  50
#define LED_PIN1     4
#define LED_PIN2     5
#define LED_PIN3     16
#define LED_PIN4     23
#define LED_PIN5     12
#define LED_PIN6     14
#define NUM_LEDS    10
#define TOT_LEDS    30
#define STEPS       10
#define BRIGHTNESS  100
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB led1[TOT_LEDS];
CRGB led2[TOT_LEDS];
CRGB led3[TOT_LEDS];
CRGB led4[TOT_LEDS];
CRGB led5[TOT_LEDS];
CRGB led6[TOT_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(led1, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(led2, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(led3, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(led4, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN5, COLOR_ORDER>(led5, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN6, COLOR_ORDER>(led6, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(115200);
}

void fadeall() { for(int i = 0; i < TOT_LEDS; i++) { led1[i].nscale8(250); } }

void cylon(int loops){
    static uint8_t hue = 0;
    for(int j=0; j<loops; j++){
    Serial.print(" ");
    Serial.print(j);
    // First slide the led in one direction
    for(int i = 0; i < TOT_LEDS; i++) {
        // Set the i'th led to red 
        led1[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show(); 
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        fadeall();
        // Wait a little bit before we loop around and do it again
        delay(10);
    }
    Serial.print("x");
 
    // Now go in the other direction.  
    for(int i = (TOT_LEDS)-1; i >= 0; i--) {
        // Set the i'th led to red 
        led1[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        fadeall();
        // Wait a little bit before we loop around and do it again
        delay(10);
    }
    }
}

void loop() {
  int i, step_count;
  long horse_pos[6] = {0,0,0,0,0,0};
  Serial.println("*");
  
  FastLED.clear();
  FastLED.show();
  delay(150);
  cylon(SHOW_LOOPS);

for(step_count = 0; step_count < STEPS; step_count++){
    //increment each horsey randomly
    for(i = 0; i < 6; i++) {
      horse_pos[i] = horse_pos[i] + random(NUM_LEDS);
    }


 for (i=0;i < TOT_LEDS;i++){
  if (i < horse_pos[0]) led1[i] = CRGB::Purple;
  else led1[i] = CRGB:: Black;
  if (i < horse_pos[1]) led2[i] = CRGB::Blue;
  else led2[i] = CRGB:: Black;
  if (i < horse_pos[2]) led3[i] = CRGB::Green;
  else led3[i] = CRGB:: Black;
  if (i < horse_pos[3]) led4[i] = CRGB::Red;
  else led4[i] = CRGB:: Black;
  if (i < horse_pos[4]) led5[i] = CRGB::Yellow;
  else led5[i] = CRGB:: Black;
  if (i < horse_pos[5]) led6[i] = CRGB::Aqua;
  else led6[i] = CRGB:: Black;    
  FastLED.show();
  delay(50);
 } 

  delay(1000);
}
}
