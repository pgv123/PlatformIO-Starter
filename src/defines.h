

#define SHOW_LOOPS  5
#define RACE_PAUSE 3
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
#define LEVEL_UP     15
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
int honour_board[LED_STRINGS];

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
