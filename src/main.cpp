#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <Adafruit_CAP1188.h>

constexpr int CAP1188_RESET = 13;
Adafruit_CAP1188 cap = Adafruit_CAP1188();


constexpr int FG = TFT_WHITE;
constexpr int BG = TFT_BLACK;
TFT_eSPI tft = TFT_eSPI();
uint32_t time_elapsed;


void setup() {
    Serial.begin(9600);

    if (!cap.begin()) {
        Serial.println("CAP1188 not found");
        for (;;);
    }
    Serial.println("CAP1188 found!");


    // Initialize OLED display
    tft.init();
    tft.setRotation(1); // 0 for portrait, 1 for landscape
    tft.fillScreen(BG);

    tft.setTextColor(FG, BG);
    tft.setTextSize(3); // Default text size is 1

    tft.setCursor(0,65);
}

void loop() {
    // uint8_t touched = cap.touched();
    // if (touched == 0) {
    //     // No touch detected
    //     return;
    // }

    // for (uint8_t i=0; i<8; i++) {
    //     if (touched & (1 << i)) {
    //         Serial.print("C");
    //         Serial.print(i+1);
    //         Serial.print("\t");
    //     }
    // }
    // Serial.println();

    tft.fillRect(0, 0, 80, 16, BG);
    tft.setCursor(0,65);
    tft.print(time_elapsed++);
    delay(1000);
}
