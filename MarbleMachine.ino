#include <FastLED.h>

#define NUM_LEDS 20     
#define DATA_PIN 6
CRGB LEDs[NUM_LEDS];
static const CRGB neutral = CRGB::Green;
static const CRGB passed  = CRGB::Red;

int pot = A15;
int treshhold = 800;
byte state[10];

const int treshDiff = 100;
volatile int initialStates[10];
volatile int differences[10] = {0,0,0,0,0,0,0,0,0,0};
static const uint8_t analog_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9};

void setup() {
  Serial.begin(57600);
  
  FastLED.delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(LEDs, NUM_LEDS); 

  allColor(CRGB::Black);
  delay(100);
  getInitialStates();  
  delay(100);
  allColor(neutral);    
}

void loop() {    

  for (int i = 0; i < 10; i++) { 
    int value = analogRead(analog_pins[i]);
    int diff = initialStates[i] - value;
    int ledL = getLeftLed(i);
    int ledR = getRightLed(i);

    if (abs(diff) > treshDiff) {
      if (i==0)
        resetRun();
      
      Serial.print  ("Sensor: ");
      Serial.print  (i);
      Serial.print  (" | Diff: ");
      Serial.print  (diff);      
      Serial.print  (" | Left: ");
      Serial.print  (LEDs[ledL] == neutral);
      Serial.print  (" | Right: ");
      Serial.println(LEDs[ledR] == neutral);      

      if (diff > 0) {
        LEDs[ledL] = neutral;
        LEDs[ledR] = passed;
      } else {
        LEDs[ledL] = passed;
        LEDs[ledR] = neutral;
      }
      
      initialStates[i] = value;   
    }    

    FastLED.show();
    delay(10);    
  }  
  delay(10);
}

int getLeftLed(int i) {
  if (i == 0 || i == 3 || i == 4 || i == 5) {
    return i * 2;
  } else {
    return (i * 2) + 1;
  }  
}

int getRightLed(int i) {
  if (i == 0 || i == 3 || i == 4 || i == 5) {
    return (i * 2) + 1; 
  } else {
    return i * 2;
  }  
}

void allColor(CRGB value) {
   for (int i = 0; i < NUM_LEDS; i++) {
    LEDs[i] = value; 
  }  
  FastLED.show();
  delay(100);
}

void getInitialStates() {

  for (int i=0; i<10; i++) {
    initialStates[i] = analogRead(analog_pins[i]);   
    delay(20);
  }
}

void resetRun() {
  // Check if one of the lowest row is passed
  if (LEDs[12] == passed || LEDs[13] == passed || LEDs[14] == passed || LEDs[15] == passed || LEDs[16] == passed || LEDs[17] == passed || LEDs[18] == passed || LEDs[19] == passed) {
    allColor(neutral);  
  }
}