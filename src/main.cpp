#include <Arduino.h>

constexpr uint32_t BUTTON_PIN = 13;
constexpr uint32_t RED_PIN = 32;
constexpr uint32_t YELLOW_PIN = 33;
constexpr uint32_t GREEN_PIN = 25;

enum State {
  RED,
  RED_YELLOW,
  YELLOW,
  GREEN
} tl_state;

constexpr uint32_t RED_TIMER = 10000; // Red light is on for 10 seconds
constexpr uint32_t YELLOW_TIMER = 2000; // Yellow/Red-Yellow is on for 2 seconds
constexpr uint32_t GREEN_TIMER = 5000; // Green light is on for 5 seconds AFTER button is pressed

int buttonState = LOW;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  tl_state = RED;
  tl_timer = millis() + RED_TIMER;
}


void loop() {
  if (digitalRead(BUTTON_PIN)) {
    buttonState = HIGH; // button was pressed
  }
  
  switch (tl_state) {
  case RED:
    // Turn red light on
    digitalWrite(RED_PIN, HIGH);

    // Turn red light off when timer expires 
    if (millis() > tl_timer) {
      digitalWrite(RED_PIN, LOW);
      
      tl_state = RED_YELLOW;
      tl_timer = millis() + YELLOW_TIMER;
    }
    break;
    
  case RED_YELLOW:
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);

    if (millis() > tl_timer) {
      digitalWrite(YELLOW_PIN, LOW);

      tl_state = YELLOW;
      tl_timer = millis() + YELLOW_TIMER;
    }
    break;
    
  case YELLOW:
    digitalWrite(YELLOW_PIN, HIGH);

    if (millis() > tl_timer) {
      digitalWrite(YELLOW_PIN, LOW);

      tl_state = GREEN;
      tl_timer = millis() + GREEN_TIMER;
    }   
    break;
    
  case GREEN:
    // Turn green light on
    digitalWrite(GREEN_PIN, HIGH);

    if (buttonState == HIGH) {
      buttonState = LOW;
      tl_timer = millis() + GREEN_TIMER;
    }

    // Turn green light off if timer expires AND touch button was pressed
    // if (buttonState == PROCESSED)
    // do magic
    
    break;
    
  }
    
}
