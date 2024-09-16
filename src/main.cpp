#include <FastLED.h>

#define LED_PIN1     4
#define LED_PIN2     5
#define NUM_LEDS    10
#define TOT_LEDS    20
#define STEPS       2
#define BRIGHTNESS  100
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB led1[TOT_LEDS];
CRGB led2[TOT_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(led1, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(led2, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
}

void loop() {
  int i, step_count;
  long j, j1;
  Serial.println("*");
  
  for(i=0;i < TOT_LEDS;i++) {
    led1[i] = CRGB::Black;
    led2[i] = CRGB::Black;    

  
  }
    FastLED.show();
    delay(150);
j = 0;
j1 = 0;
for(step_count = 0; step_count < STEPS; step_count++){
 //randomSeed(analogRead(0));
 j = j + random(NUM_LEDS);
   j1 = j1 + random(NUM_LEDS);

 for (i=0;i < TOT_LEDS;i++){
  if (i < j) led1[i] = CRGB::Blue;
  else led1[i] = CRGB:: Black;
  if (i < j1) led2[i] = CRGB::Purple;
  else led2[i] = CRGB:: Black;
  FastLED.show();
  delay(100);
 } 

  delay(1000);
}
}
