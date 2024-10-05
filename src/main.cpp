#include <FastLED.h>

// Define the min macro
#define min(a,b) ((a) < (b) ? (a) : (b))

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
#define LED_STRINGS 6
#define NUM_LEDS    15
#define TOT_LEDS    92
#define RUN_LEDS   4
#define STEPS       10
#define LEVEL_UP     10
#define MERCY_LEVEL 15
#define MERCY_START 3
#define minBrightness  40
#define midBrightness  100
#define maxBrightness  150
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define STRING1_COLOUR CRGB::Purple
#define STRING2_COLOUR CRGB::Blue
#define STRING3_COLOUR CRGB::Green
#define STRING4_COLOUR CRGB::Red
#define STRING5_COLOUR CRGB::Yellow
#define STRING6_COLOUR CRGB::Pink

CRGB leds[LED_STRINGS][TOT_LEDS];
CRGB (*leds_ptr)[TOT_LEDS] = leds;

long horse_pos[LED_STRINGS][2];

const uint8_t dataPins[LED_STRINGS] = {LED_PIN1, LED_PIN2, LED_PIN3, LED_PIN4, LED_PIN5, LED_PIN6};
//CRGB led2[TOT_LEDS];
//CRGB led3[TOT_LEDS];
//CRGB led4[TOT_LEDS];
//CRGB led5[TOT_LEDS];
//CRGB led6[TOT_LEDS];

const int buttonPins[] = {BUTTON_PIN};
uint32_t previousMillis[noOfButtons];    //Timer used in button debouncing
uint8_t pressCount[noOfButtons];         //Counts how many the button is detected as pressed, has to be minButtonPress times to be valid
uint8_t testCount[noOfButtons];             //Test count, incremented once per button press
uint8_t buttonFlg[noOfButtons];         //stores the state of the button - 0 = not pressed, 1 = pressed


void setup() {
  Serial.println(dataPins[2]);
int i;

  pinMode(BUTTON_PIN, INPUT);
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds[0], TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds[1], TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(leds[2], TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(leds[3], TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN5, COLOR_ORDER>(leds[4], TOT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN6, COLOR_ORDER>(leds[5], TOT_LEDS).setCorrection(TypicalLEDStrip);
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
  int i, j;

  
  for(i = 0; i < LED_STRINGS; i++) {
    for(j = 0; j < TOT_LEDS; j++) {
     leds[i][j].nscale8(250);
     //led2[i].nscale8(250);
     //led3[i].nscale8(250);
     //led4[i].nscale8(250);
     //led5[i].nscale8(250);
     //led6[i].nscale8(250);                
    }
  }
}

void running_all(){
int i, c, counter, ledIndex;
int hp_start[LED_STRINGS], hp_finish[LED_STRINGS] ;
bool no_movement[LED_STRINGS];
bool done;

done = false;

for(c = 0; c < LED_STRINGS; c++) {
  hp_start[c] = horse_pos[c][1] - RUN_LEDS + 1;
  hp_finish[c] = horse_pos[c][0] - RUN_LEDS + 1;
  no_movement[c] = false;

}

FastLED.clear(true);
FastLED.show();

while(!done){

  for(c = 0; c < LED_STRINGS; c++) {
    if (hp_start[c] > hp_finish[c]) {
      no_movement[c] = true;
      Serial.print(" Lane ");
      Serial.print(c+1);
      Serial.println(" Done: ");
    }
    else {
      no_movement[c] = false;
      Serial.print(" Lane ");
      Serial.print(c+1);
      Serial.print(" Not Done: ");      
    }
  
    if(!no_movement[c]) {
      counter = RUN_LEDS;
 
    
//      Serial.print("Lane: ");
//      Serial.print(c + 1);
      Serial.print(" Horse Pos St: ");
      Serial.print(horse_pos[c][1]);
      Serial.print(" F: ");
      Serial.print(horse_pos[c][0]);  
      Serial.print(" hp_start: ");
      Serial.print(hp_start[c]);
      Serial.print(" hp_finish: ");
      Serial.print(hp_finish[c]);
      Serial.print(" Counter: ");
      Serial.println(counter);
    
      for( i = 0; i < counter; i++ ) {
        ledIndex = hp_start[c] + i;
        if( ledIndex >= 0) {
  //    Serial.print("LedIndex: ");
  //    Serial.println(ledIndex);
  //    Serial.print("i: ");
  //    Serial.println(i);
          if ((i == 0) && (ledIndex > 0))  leds[c][ledIndex - 1] = CRGB::Black;
  //   Serial.println("About to switch!");
          switch(c) {
            case 0: 
             leds[0][ledIndex] = STRING1_COLOUR;
             break;
            case 1:
             leds[1][ledIndex] = STRING2_COLOUR;
             break;
            case 2:
             leds[2][ledIndex] = STRING3_COLOUR;
             break;
            case 3:
             leds[3][ledIndex] = STRING4_COLOUR;
             break;
            case 4: 
             leds[4][ledIndex] = STRING5_COLOUR;
             break;
            case 5: 
             leds[5][ledIndex] = STRING6_COLOUR;
             break;
          }
        }
      }
      hp_start[c]++;
    }

  }
  FastLED.show();
  my_delay(50);
  done = true;
  for(c = 0; c < LED_STRINGS; c++) {
    if(no_movement[c] == false) done = false;
  }
}
FastLED.show();
my_delay(50);
}


void leader(int w1, int w_pos, int counter, bool tog1){
int i, c;

  if((w_pos - counter) < 0) {
    counter = w_pos;
  }

  for(i= w_pos ; i> (w_pos - counter); i--){
    switch(w1) {
      case 0: 
              leds[0][i] = STRING1_COLOUR;
              break;
      case 1:
              leds[1][i] = STRING2_COLOUR;
              break;
      case 2:
              leds[2][i] = STRING3_COLOUR;
              break;
      case 3:
              leds[3][i] = STRING4_COLOUR;
              break;
      case 4: 
              leds[4][i] = STRING5_COLOUR;
              break;
      case 5: 
              leds[5][i] = STRING6_COLOUR;
              break;
    }
    if (!tog1) {
      leds[w1][i].nscale8(20);
    }
  }
  FastLED.show();

}

void startline(){
int i;

  FastLED.clear();

  for(i=0; i<RUN_LEDS; i++){
    leds[0][i] = STRING1_COLOUR;
    leds[1][i] = STRING2_COLOUR;
    leds[2][i] = STRING3_COLOUR;
    leds[3][i] = STRING4_COLOUR;
    leds[4][i] = STRING5_COLOUR;
    leds[5][i] = STRING6_COLOUR;                
  }
  FastLED.show();
//  fadeall();
//  FastLED.show();
}

void cylon(int loops){
static uint8_t hue = 0;
int i,j,k;

  for(k=0; k<loops; k++){
   //   Serial.print(" ");
   //   Serial.print(j);
      // First slide the led in one direction

    for(j = 0; j < TOT_LEDS; j++) {
         // Set the i'th led to red 
      for(i = 0; i < LED_STRINGS; i++) {
        leds[i][j] = CHSV(hue++, 255, 255);
      }
        //led5[i] = CHSV(hue++, 255, 255);
        //led6[i] = CHSV(hue++, 255, 255);                         
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
  
    for(j = (TOT_LEDS)-1; j >= 0; j--) {
      for(i = 0; i < LED_STRINGS; i++) {
        // Set the i'th led to red 
        leds[i][j] = CHSV(hue++, 255, 255);
      }  
        //led2[i] = CHSV(hue++, 255, 255);
        //led3[i] = CHSV(hue++, 255, 255);
        //led4[i] = CHSV(hue++, 255, 255);
        //led5[i] = CHSV(hue++, 255, 255);
        //led6[i] = CHSV(hue++, 255, 255);                  
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
  bool finish = false, toggle = true;
  int winner = -1, helper = 0, rounds = 0;
  long winner_pos = 0;
  
  Serial.println("Hat Day Racing...GiddyUp!!!!");
  
  FastLED.clear();
  FastLED.show();
  my_delay(150);
  
  for(i=0;i < LED_STRINGS; i++){
    horse_pos[i][0] = 0;
    horse_pos[i][1] = 0;
  }

  rounds = 0;
  winner = -1;
  winner_pos = 0;
  helper = 0;
  
  finish = false;

  cylon(SHOW_LOOPS);
  
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
    rounds++;
    for(i = 0; i < LED_STRINGS; i++) {
      if((horse_pos[i][0] < (horse_pos[winner][0] - MERCY_LEVEL)) && (rounds > MERCY_START)) {
        helper = LEVEL_UP;
      }
      else {
        helper = 0;
      }
      horse_pos[i][1] = horse_pos[i][0];  //remember the value this lane was at.
      horse_pos[i][0] = horse_pos[i][0] + helper + random(NUM_LEDS);
      winner_pos = max(winner_pos,horse_pos[i][0]);
      if (winner_pos == horse_pos[i][0]) {
        winner = i;
        if (winner_pos > (TOT_LEDS - 1)) {
          Serial.print("Declaring Lane ");
          Serial.print(i+1);
          Serial.println(" as the Winner!");
          finish = true;
          winner_pos = TOT_LEDS - 1;
        }
      }
      if (horse_pos[i][0] > (TOT_LEDS - 1)) horse_pos[i][0] = TOT_LEDS - 1;
    }
 
  if (!finish) {
      Serial.print("Currently Winning Lane: ");
      Serial.println(winner+1);
      Serial.print("Current Position on Track: ");
      Serial.println(winner_pos);
      Serial.print("Previous Position on Track: ");
      Serial.println(horse_pos[winner][1]);
  }
  
  running_all(); //this will bump everyone along    
  
  FastLED.show();
  my_delay(50);

  while(buttonFlg[0] == 0) {
    toggle = !toggle;
    if(finish){
      leader(winner, winner_pos, TOT_LEDS, toggle);
    }
    else {
      leader(winner, winner_pos, RUN_LEDS, toggle);
    }
    my_delay(250); 
    chk_button();
  }
 
  }
 } 


