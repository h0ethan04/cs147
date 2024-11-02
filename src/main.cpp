#include <Arduino.h>

constexpr uint32_t BUZZER_PIN = 22;
constexpr uint32_t BUTTON_PIN = 15;
constexpr uint32_t RED_PIN = 32;
constexpr uint32_t YELLOW_PIN = 33;
constexpr uint32_t GREEN_PIN = 25;

enum State {
  RED,
  RED_YELLOW,
  YELLOW,
  GREEN
} tl_state;

enum BuzzerState {
  RED_ON,
  RED_OFF,
  GREEN_ON,
  GREEN_OFF,
  OFF
} buzzer_state;


constexpr uint32_t RED_TIMER = 10000; // Red light is on for 10 seconds
constexpr uint32_t YELLOW_TIMER = 2000; // Yellow/Red-Yellow is on for 2 seconds
constexpr uint32_t GREEN_TIMER = 5000; // Green light is on for 5 seconds AFTER button is pressed

constexpr uint16_t GREEN_BUZZER_TIMER_ON = 500;
constexpr uint16_t GREEN_BUZZER_TIMER_OFF = 1500;

constexpr uint8_t RED_BUZZER = 250;

int buttonState = LOW;
bool buttonPending = false;
unsigned long tl_timer;
unsigned long buzzer_timer;


void setup() {
  Serial.begin(9600);
  delay(2000);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  buzzer_timer = 0;
  buzzer_state = RED_ON; 
  tl_state = RED;
  tl_timer = millis() + RED_TIMER;
}


void loop() {
  if (digitalRead(BUTTON_PIN)) {
    buttonState = HIGH; // button was pressed
  }
  Serial.printf("%d : %d\n", digitalRead(BUTTON_PIN), buttonState);
  
  switch (tl_state) {
    case RED:
      // Turn red light on
      digitalWrite(RED_PIN, HIGH);

      // Turn red light off when timer expires 
      if (millis() > tl_timer) {
        digitalWrite(RED_PIN, LOW);
        buzzer_state = OFF; 
        tl_state = RED_YELLOW;
        tl_timer = millis() + YELLOW_TIMER;
      }
      break;
      
    case RED_YELLOW:
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(RED_PIN, HIGH);

      if (millis() > tl_timer) {
        digitalWrite(YELLOW_PIN, LOW);
        digitalWrite(RED_PIN, LOW);
        buzzer_state = GREEN_ON;
        tl_state = GREEN;
        tl_timer = millis();
      }
      break;
      
    case YELLOW:
      digitalWrite(YELLOW_PIN, HIGH);

      if (millis() > tl_timer) {
        digitalWrite(YELLOW_PIN, LOW);
        buzzer_state = RED_ON;
        tl_state = RED;
        tl_timer = millis() + RED_TIMER;
      }   
      break;
      
    case GREEN:
      // Turn green light on
      digitalWrite(GREEN_PIN, HIGH);

      if (buttonState == HIGH) {
        buttonState = LOW;
        tl_timer = millis() + GREEN_TIMER;
        buttonPending = true;
      }

      if (buttonPending && millis() > tl_timer) {
        digitalWrite(GREEN_PIN, LOW);
        buzzer_state = OFF;
        tl_state = YELLOW;
        tl_timer = millis() + YELLOW_TIMER;
        buttonPending = false;
      }

      // Turn green light off if timer expires AND touch button was pressed
      // if (buttonState == PROCESSED)
      // do magic
      
      break;
    
  }

  if (buzzer_timer < millis()) {
    switch (buzzer_state) {
      case RED_ON:
        buzzer_state = RED_OFF;
        buzzer_timer = millis() + RED_BUZZER;
        tone(BUZZER_PIN, 1000, RED_BUZZER);   
        break;
      case RED_OFF:
        buzzer_state = RED_ON;
        buzzer_timer = millis() + RED_BUZZER;
        break;
      case GREEN_ON:
        buzzer_state = GREEN_OFF;
        buzzer_timer = millis() + GREEN_BUZZER_TIMER_ON;
        tone(BUZZER_PIN, 1000, GREEN_BUZZER_TIMER_ON);
        break;
      case GREEN_OFF:
        buzzer_state = GREEN_ON;
        buzzer_timer = millis() + GREEN_BUZZER_TIMER_OFF;
        break;
      case OFF:
        break;
    }
   
  }
   
}
