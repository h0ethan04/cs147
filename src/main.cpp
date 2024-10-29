#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <Adafruit_CAP1188.h>

#define CAP1188_RESET 13

#define CLKPIN 22
#define MISOPIN 21
#define MOSIPIN 17
#define CSPIN 15
// hardware i2c
// sda 21
// sck 22
// vin 3v
Adafruit_CAP1188 cap = Adafruit_CAP1188();
// software spi
// ad ground
// 3vo 3v
// Adafruit_CAP1188 cap(CLKPIN, MISOPIN, MOSIPIN, CSPIN, CAP1188_RESET);

constexpr int FG = TFT_WHITE;
constexpr int BG = TFT_BLACK;
TFT_eSPI tft = TFT_eSPI();
uint32_t time_elapsed;


void setup() {
    Serial.begin(9600);

    delay(1500); // delay so we don't miss serial msgs

    Serial.println("\nStarting CAP1188 check");

    if (!cap.begin()) {
        Serial.println("CAP1188 not found");
        for (;;); // what the fuck is this brian
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
    uint8_t total = 0;
    uint8_t touched = cap.touched();
    if (!touched) return;

    for (uint8_t i = 0; i < 8; i++) {
        if (touched & (1 << i)) ++total;
    }

    tft.fillRect(0, 0, 80, 16, BG);
    tft.setCursor(0, 65);
    tft.print(total);
    delay(100);
}
