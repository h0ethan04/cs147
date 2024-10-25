#include <Arduino.h>
#include <TFT_eSPI.h>

constexpr int FG = TFT_WHITE;
constexpr int BG = TFT_BLACK;

TFT_eSPI tft = TFT_eSPI();
uint32_t time_elapsed;

void setup() {
    tft.init();
    tft.setRotation(1); // 0 for portrait, 1 for landscape
    tft.fillScreen(BG);

    tft.setTextColor(FG, BG);
    // tft.setTextSize(1); // Default text size is 1

    tft.setCursor(0,0);
}

void loop() {
    for (;;) {
        tft.fillRect(0, 0, 80, 16, BG);
        tft.print(time_elapsed++);
        delay(1000);
    }
}
