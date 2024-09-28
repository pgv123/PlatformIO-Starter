#include <FastLED.h>

#define SHOW_LOOPS  5
#define BUTTON_PIN  34
#define noOfButtons 1     //Exactly what it says; must be the same as the number of elements in buttonPins
#define bounceDelay 20    //Minimum delay before regarding a button as being pressed and debounced
#define minButtonPress 3  //Number of times the button has to be detected as pressed before the press is considered to be valid
#define LED_PIN1     4
#define LED_PIN2     5
#define LED_PIN3     16
#define LED_PIN4     23
#define LED_PIN5     12
#define LED_PIN6     14
#define NUM_LEDS    15
#define TOT_LEDS    92
#define STEPS       10
#define minBrightness  40
#define midBrightness  100
#define maxBrightness  150
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB led1[TOT_LEDS];
CRGB led2[TOT_LEDS];
CRGB led3[TOT_LEDS];
CRGB led4[TOT_LEDS];
CRGB led5[TOT_LEDS];
CRGB led6[TOT_LEDS];

const int buttonPins[] = {BUTTON_PIN};
uint32_t previousMillis[noOfButtons];    //Timer used in button debouncing
uint8_t pressCount[noOfButtons];         //Counts how many the button is detected as pressed, has to be minButtonPress times to be valid
uint8_t testCount[noOfButtons];             //Test count, incremented once per button press
uint8_t buttonFlg[noOfButtons];         //stores the state of the button - 0 = not pressed, 1 = pressed


void setup() {
  pinMode(BUTTON_PIN, INPUT);
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(led1, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(led2, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(led3, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(led4, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN5, COLOR_ORDER>(led5, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN6, COLOR_ORDER>(led6, TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(midBrightness);
  Serial.begin(115200);
}

void chk_button() {
  uint8_t i;
  uint32_t currentMillis = millis();
  for (i = 0; i < noOfButtons; ++i) {
    if (!digitalRead(buttonPins[i])) {
                     //Input is low, button not pressed or in the middle of bouncing and happens to be low
        previousMillis[i] = currentMillis;        //Set previousMillis to millis to reset timeout
        pressCount[i] = 0;                        //Set the number of times the button has been detected as pressed to 0
      } else {
        if (currentMillis - previousMillis[i] > bounceDelay) {
          previousMillis[i] = currentMillis;        //Set previousMillis to millis to reset timeout
          ++pressCount[i];
          if (pressCount[i] == minButtonPress) {
          //  Serial.print("B");            
            buttonFlg[i] = 1;                             //Button has been debounced. Let the world know.
          }
        }
      }
  }
}

void my_delay(int mS){  //routine to delay while button checking
bool done = false;
uint32_t targetMillis;
uint32_t nowMillis, diffMillis;

targetMillis = millis() + mS;

while(!done) {

  chk_button();

  nowMillis = millis();
  diffMillis = targetMillis - nowMillis;
  if (diffMillis > 0) {
    done = false;
    }
  else {
    done = true;
    }

  }

}

void fadeall() {
  int i;

  
  for(i = 0; i < TOT_LEDS; i++) {
     led1[i].nscale8(250);
     led2[i].nscale8(250);
     led3[i].nscale8(250);
     led4[i].nscale8(250);
     led5[i].nscale8(250);
     led6[i].nscale8(250);                
      }
 }

void startline(){
int i;

  FastLED.clear();

  for(i=0; i<4; i++){
    led1[i] = CRGB::Purple;
    led2[i] = CRGB::Blue;
    led3[i] = CRGB::Green;
    led4[i] = CRGB::Red;
    led5[i] = CRGB::Yellow;
    led6[i] = CRGB::Orange;                
  }
  FastLED.show();
//  fadeall();
//  FastLED.show();
}

void cylon(int loops){
    static uint8_t hue = 0;
    for(int j=0; j<loops; j++){
   //   Serial.print(" ");
   //   Serial.print(j);
      // First slide the led in one direction
      for(int i = 0; i < TOT_LEDS; i++) {
        // Set the i'th led to red 
        led1[i] = CHSV(hue++, 255, 255);
        led2[i] = CHSV(hue++, 255, 255);
        led3[i] = CHSV(hue++, 255, 255);
        led4[i] = CHSV(hue++, 255, 255);
        led5[i] = CHSV(hue++, 255, 255);
        led6[i] = CHSV(hue++, 255, 255);                         
        // Show the leds
        FastLED.show(); 
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        fadeall();
        // Wait a little bit before we loop around and do it again
        my_delay(5);
        chk_button();
        if( buttonFlg[0] == 1) {
          buttonFlg[0] = 0;
          break;
        }
        
    }
    // Now go in the other direction.  
    for(int i = (TOT_LEDS)-1; i >= 0; i--) {
        // Set the i'th led to red 
        led1[i] = CHSV(hue++, 255, 255);
        led2[i] = CHSV(hue++, 255, 255);
        led3[i] = CHSV(hue++, 255, 255);
        led4[i] = CHSV(hue++, 255, 255);
        led5[i] = CHSV(hue++, 255, 255);
        led6[i] = CHSV(hue++, 255, 255);                  
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        fadeall();
        // Wait a little bit before we loop around and do it again
        my_delay(5);
        chk_button();        
        if( buttonFlg[0] == 1 ) {
          //buttonFlg[0] = 0;
          break;
        }        
    }
          if( buttonFlg[0] == 1 ) {
            buttonFlg[0] = 0;
            break;
          }
    }
}

void loop() {
  int i, step_count;
  bool finish = false;
  int winner = -1;
  long winner_pos = 0;
  long horse_pos[6] = {0,0,0,0,0,0};
  Serial.println("Hat Day Racing...GiddyUp!!!!");
  
  FastLED.clear();
  FastLED.show();
  my_delay(150);
  
  
  cylon(SHOW_LOOPS);
  finish = false;
  while(buttonFlg[0] == 0) {
    FastLED.setBrightness(midBrightness);    
    startline();
//    Serial.println("Starting delay!");
//    Serial.println(millis());
    my_delay(500);
//    Serial.println("Ending delay!");
//    Serial.println(millis());
    FastLED.setBrightness(minBrightness);
    FastLED.show();
    my_delay(400);
  }

    for(i=0;i < 6;i++) {
      FastLED.setBrightness(maxBrightness);
      startline();
      my_delay(150);
      FastLED.setBrightness(minBrightness);
      FastLED.show();
      my_delay(120);

    }
  FastLED.clear(true);
  FastLED.show();
  my_delay(300);
  FastLED.setBrightness(midBrightness); 

  while(!finish) {

    //for(step_count = 0; step_count < STEPS; step_count++){
    //increment each horsey randomly
    buttonFlg[0] = 0;
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
    if (i < horse_pos[5]) led6[i] = CRGB::Orange;
    else led6[i] = CRGB:: Black;    
    FastLED.show();
    my_delay(5);
  }
  for(i = 0; i < 6; i++) {
    if (horse_pos[i] > winner_pos) {
      winner_pos = horse_pos[i];
      winner = i;
      Serial.print("Currently Winning Lane: ");
      Serial.println(i+1);
      if (horse_pos[i] >= TOT_LEDS - 1) {
          Serial.print("Declaring Lane ");
          Serial.print(i+1);
          Serial.println(" as the Winner!");
          finish = true;
          winner_pos = TOT_LEDS - 1;
      }          
    }
  }
  switch (winner) {
    case 0:
      led1[winner_pos] = CRGB::White;
      break;
    case 1:
      led2[winner_pos] = CRGB::White;
      break;
    case 2:
      led3[winner_pos] = CRGB::White;
      break;            
    case 3:
      led4[winner_pos] = CRGB::White;
      break;
    case 4:
      led5[winner_pos] = CRGB::White;
      break;
    case 5:
      led6[winner_pos] = CRGB::White;
      break; 
   }
    
  FastLED.show();

  while(buttonFlg[0] == 0) { 
    chk_button();
  }
 
  }
 } 


