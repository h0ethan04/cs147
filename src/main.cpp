#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <Wire.h>
#include <SparkFunLSM6DSO.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define LED 13

LSM6DSO myIMU;
BLEService *pService;
bool in_step = false;
long steps = 0;
float down_step_accel;
float up_step_accel;

// send data from esp32 to phone
void setup() {
    Serial.begin(9600);
    delay(2000);
    Serial.println("Starting BLE work!");
    BLEDevice::init("Ethan");
    BLEServer *pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );
    pCharacteristic->setValue("Server Example - CS147"); //What gets transfered
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x0);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");

    delay(500); 
    
    Wire.begin();
    delay(10);
    if( myIMU.begin() )
        Serial.println("Ready.");
    else { 
        Serial.println("Could not connect to IMU.");
        Serial.println("Freezing");
    }

    if( myIMU.initialize(BASIC_SETTINGS) )
        Serial.println("Loaded Settings.");

    // init down_step_accel and up_step_accel
    down_step_accel = 0.85;
    up_step_accel = 1.15;

}
void loop() {
    //Get all parameters
    Serial.print("\nAccelerometer:\n");
    // Serial.print(" X = ");
    // Serial.println(myIMU.readFloatAccelX(), 3);
    // Serial.print(" Y = ");
    // Serial.println(myIMU.readFloatAccelY(), 3);
    Serial.print(" Z = ");
    float accelZ = myIMU.readFloatAccelZ();
    Serial.println(accelZ, 3);
    if (in_step && accelZ < down_step_accel) {
        // pService->getCharacteristic(CHARACTERISTIC_UUID)->setValue(String(myIMU.readFloatAccelZ()).c_str());
        in_step = false;
        steps++;
        Serial.println("Step down");
    } else if (!in_step && accelZ > up_step_accel) {
        in_step = true;
        Serial.println("Step up");
        // pService->getCharacteristic(CHARACTERISTIC_UUID)->setValue(String(myIMU.readFloatAccelZ()).c_str());
    }    
    // Serial.print("\nGyroscope:\n");
    // Serial.print(" X = ");
    // Serial.println(myIMU.readFloatGyroX(), 3);
    // Serial.print(" Y = ");
    // Serial.println(myIMU.readFloatGyroY(), 3);
    // Serial.print(" Z = ");
    // Serial.println(myIMU.readFloatGyroZ(), 3);

    delay(500);
}









// part 1 complete
// send data from phone to esp32

// class MyCallbacks: public BLECharacteristicCallbacks {
//     void onWrite(BLECharacteristic *pCharacteristic) {
//         std::string value = pCharacteristic->getValue();
//         if (value.length() > 0) {
//             Serial.println("*********");
//             Serial.print("New value: ");
//             for (int i = 0; i < value.length(); i++)
//             Serial.print(value[i]);
//             Serial.println();
//             Serial.println("*********");
//         }
//         switch (value[0]) {
//             case '1':
//                 digitalWrite(LED, HIGH);
//                 break;
//             case '0':
//                 digitalWrite(LED, LOW);
//                 break;
//         }
//     }
// };
// void setup() {
//     Serial.begin(9600);
//     delay(2000);

//     pinMode(LED, OUTPUT);

//     Serial.println("1- Download and install an BLE scanner app in your phone");
//     Serial.println("2- Scan for BLE devices in the app");
//     Serial.println("3- Connect to MyESP32");
//     Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
//     Serial.println("5- See the magic =)");
//     BLEDevice::init("Ethan");
//     BLEServer *pServer = BLEDevice::createServer();
//     BLEService *pService = pServer->createService(SERVICE_UUID);
//     BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//         CHARACTERISTIC_UUID,
//         BLECharacteristic::PROPERTY_READ |
//         BLECharacteristic::PROPERTY_WRITE
//     );
//     pCharacteristic->setCallbacks(new MyCallbacks());
//     pCharacteristic->setValue("Hello World");
//     pService->start();
//     BLEAdvertising *pAdvertising = pServer->getAdvertising();
//     pAdvertising->start();
// }
// void loop() {
//     delay(2000);
// }