#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define the UUIDs for the BLE service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Define the digital pin to control
#define CONTROL_PIN 8

// Callback function to handle write events
class ControlPinCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String value = pCharacteristic->getValue(); // Get the value as a String
        if (value.length() > 0) {
            int pinState = value.toInt(); // Convert the String to an integer
            digitalWrite(CONTROL_PIN, pinState); // Set the pin state
            Serial.print("Pin state set to: ");
            Serial.println(pinState);
        }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize the BLE device
  BLEDevice::init("ESP32C3_Pin_Controller");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  // Set the initial value of the characteristic
  pCharacteristic->setValue("0");
  pCharacteristic->setCallbacks(new ControlPinCallback()); // Set the callback function

  // Start the service and advertising
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read/write it in your phone!");

  // Set the control pin as an output
  pinMode(CONTROL_PIN, OUTPUT);
  digitalWrite(CONTROL_PIN, LOW); // Initialize the pin to LOW
}

void loop() {
    // Nothing to do here
    delay(2000);
}