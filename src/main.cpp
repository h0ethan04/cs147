#include <Arduino.h>
#include <HttpClient.h>
#include <WiFi.h>
#include <inttypes.h>
#include <stdio.h>

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"

#include <Adafruit_AHTX0.h>
#include <Adafruit_SGP30.h>
#include <Wire.h>

Adafruit_AHTX0 aht;
Adafruit_SGP30 sgp;

char ssid[50]; // your network SSID (name)
char pass[50]; // your network password (use for WPA, or use as key for WEP)

const char server[] = "seg8lvbr1e.execute-api.us-west-1.amazonaws.com";
const uint16_t port = 80;
const char path[] = "/Prod/temp-readings/";

WiFiClient c;
HttpClient http(c);


void nvs_access() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    Serial.printf("\n");
    Serial.printf("Opening Non-Volatile Storage (NVS) handle... ");
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
	    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        Serial.printf("Done\n");
        Serial.printf("Retrieving SSID/PASSWD\n");
        size_t ssid_len;
        size_t pass_len;
        err = nvs_get_str(my_handle, "ssid", ssid, &ssid_len);
        err |= nvs_get_str(my_handle, "pass", pass, &pass_len);
        switch (err) {
        case ESP_OK:
            Serial.printf("Done\n");
            Serial.printf("SSID = %s\n", ssid);
            Serial.printf("PASSWD = %s\n", pass);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            Serial.printf("The value is not initialized yet!\n");
            break;
        default:
            Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
    }

    // Close
    nvs_close(my_handle);
}

void setup() {
    Serial.begin(9600);

    delay(1000);
    Serial.println("\n");


    Serial.begin(9600);
    Wire.begin();
    for (byte i = 8; i < 120; i++) {
      Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0) {
        Serial.print("I2C device found at address 0x");
        if (i < 16) {
          Serial.print("0");
        }
        Serial.print(i, HEX);
        Serial.println(" !");
      }
    }


    // Retrieve SSID/PASSWD from flash before anything else
    nvs_access();
    
    // We start by connecting to a WiFi network
    delay(1000);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, pass);
    
    while (WiFi.status() != WL_CONNECTED) {
    	delay(500);
    	Serial.print(".");
    }
    
    Serial.println("");
    // Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
    // Serial.println("MAC address: ");
    // Serial.println(WiFi.macAddress());

    if (!aht.begin()) {
		Serial.println("Could not find AHTX0 sensor!");
		while (1);
	}
	Serial.println("AHTX0 sensor found!");

	if (!sgp.begin()) {
		Serial.println("SGP30 sensor not found. Check wiring!");
		while (1);
	}
	Serial.println("SGP30 initialized!");
}

void loop() {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degrees C");
    
    // Serial.print("Humidity: ");
    // Serial.print(humidity.relative_humidity);
    // Serial.println("% rH");

    Serial.print("\n");

    if (!sgp.IAQmeasure()) {
      Serial.println("Measurement failed!");
      return;
    }
    Serial.print("TVOC: ");
    Serial.print(sgp.TVOC);
    Serial.println(" ppb");
	

    // Modified code to send recorded data to the web server
    String payload = String("{\"tempReading\":") + temp.temperature +
	",\"gasReading\":" + sgp.TVOC + "}";

    http.beginRequest();
    int err = http.post(server, port, path);
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("Content-Length", payload.length());
    http.write(reinterpret_cast<const uint8_t*>(payload.c_str()), payload.length());
    http.endRequest();

    if (err == 0) {
    	Serial.println("Request sent successfully.");
    
    	int statusCode = http.responseStatusCode();
     	Serial.print("Response status code: ");
     	Serial.println(statusCode);
    
    	// Read the response body
    	while (http.available()) {
    	    char c = http.read();
    	    Serial.print(c);
    	}
    
    } else {
    	Serial.print("Error sending request: ");
    	Serial.println(err);
    }

    Serial.print("\n");

    http.stop();
    delay(5000);

}