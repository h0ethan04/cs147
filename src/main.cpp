#include <Arduino.h>
#include <Servo.h>
constexpr uint8_t servoPort = 13;
constexpr uint8_t analogReadPort = 32;
Servo servo;

void setup() {
    // pinMode(15, OUTPUT);
    Serial.begin(9600);
    // pinMode(servoPort, OUTPUT);
    servo.attach(servoPort);

}

void loop() {
    // servo.write(100);
    Serial.print(analogRead(analogReadPort));
    sleep(1);
    for (int i = 0; i < 45; ++i) {
        servo.write(((servo.read()) + i) % 180);
        // Serial.println("loop");
        sleep(1);
    }
    // servo.write(100);
    
}
