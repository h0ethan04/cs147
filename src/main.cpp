#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <Wire.h>
#include <SparkFunLSM6DSO.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define LED 13

#define calibration_steps 10

LSM6DSO myIMU;
BLEService *pService;
bool in_step = false;
long steps = 0;
float down_step_accel;
float up_step_accel;
BLECharacteristic *pCharacteristic;


void calibrate_threshold() {
    Serial.println("Starting calibration...");
    Serial.println("Take 10 steps to calibrate.");


    delay(100);
    down_step_accel = 0.85;

    float maxUpAccel = -100.0;
    float maxDownAccel = 100.0;
    int stepsCounted = 0;

    while (stepsCounted < calibration_steps) {
       float accelZ = myIMU.readFloatAccelZ();
       Serial.printf("calibrating z = %f\n", accelZ);

       if (accelZ > maxUpAccel) maxUpAccel = accelZ;
       if (accelZ < maxDownAccel) maxDownAccel = accelZ;

       ++stepsCounted;
       delay(1000);
    }
    up_step_accel = 1.1 * maxUpAccel;
    down_step_accel = 0.9 * maxDownAccel;
}


// send data from esp32 to phone
void setup() {
    Serial.begin(9600);
    delay(2000);
    Serial.println("Starting BLE work!");
    BLEDevice::init("Ethan");
    BLEServer *pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
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

    delay(20000);


    // init down_step_accel and up_step_accel
    calibrate_threshold();

    Serial.println("Calibration complete.");

    // Serial.println(up_step_accel);
    // Serial.println(down_step_accel);

    delay(1000);
}
void loop() {
    //Get all parameters
    float accelZ = 0;
    for (int i = 0; i < 10; i++) {
        accelZ += myIMU.readFloatAccelZ();
        delay(5);
    }
    accelZ /= 10;
    // Serial.println(accelZ, 3);
    if (in_step && accelZ < down_step_accel) {
        ++steps;
        pCharacteristic->setValue(String(steps).c_str());
        in_step = false;
        // Serial.println("Step Up");
        pCharacteristic->notify();
    } else if (!in_step && accelZ > up_step_accel) {
        in_step = true;
        // Serial.println("Step Down");
    }    

    delay(200);
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
