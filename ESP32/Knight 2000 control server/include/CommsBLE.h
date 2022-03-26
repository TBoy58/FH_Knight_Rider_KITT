#include <Arduino.h>

#include <DigitalPin_ESP32.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

/* Service and Characteristic Universally Unique Identifier (UUID)
generated from - https://www.uuidgenerator.net/ */
#define SERVICE_UUID "c7fa88dc-02f3-434e-8b48-1d08d0b1c644"
#define CHARACTERISTIC_UUID "358269d5-8934-4cd8-8b26-24e2047bd2e9"

DigitalPin ConnectLED(2);

char kittInstr = 'Z';

bool deviceConnected = false;
/* This function handles the server callbacks */
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* MyServer) {
      deviceConnected = true;
      ConnectLED.on();
    };

    void onDisconnect(BLEServer* MyServer) {
      deviceConnected = false;
      ConnectLED.off();
      BLEDevice::startAdvertising();
    }
};

class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      kittInstr = value[0];      
      
    }
};

void startBLEServer() {
  
  ConnectLED.beginOUT();

  BLEDevice::init("KTT");
  BLEServer *pServer = BLEDevice::createServer();
  // Set the function that handles Server Callbacks
  pServer->setCallbacks(new ServerCallbacks());  

  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );


  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
  
  pCharacteristic->setValue("Hello World says Pete");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
}