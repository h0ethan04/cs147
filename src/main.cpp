#include <Arduino.h>
#include <Servo.h>

#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x > y) ? x : y)

constexpr uint8_t servoPort = 13;
constexpr uint8_t analogReadPort = 32;
constexpr uint32_t calibrationTime = 10000;
Servo servo;

uint32_t minLightLevel, maxLightLevel;

void setup() {
    // pinMode(15, OUTPUT);
    Serial.begin(9600);
    // pinMode(servoPort, OUTPUT);
    servo.attach(servoPort);

    // Calibration phase (10 seconds)
    uint32_t begin = millis();
    uint32_t end = begin;
    while ((end = millis()) - begin >= calibrationTime) {
        uint16_t value = analogRead(analogReadPort);
        minLightLevel = min(minLightLevel, value);
        maxLightLevel = max(maxLightLevel, value);
        delay(500);
    }

}

void loop() {
    uint32_t x = analogRead(analogReadPort);
    uint32_t y = map(x, minLightLevel, maxLightLevel, 0, 180);
    servo.write(y);
    sleep(1);
}
