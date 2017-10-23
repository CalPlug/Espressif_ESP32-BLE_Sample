/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <SimpleDHT.h>
#include <string>

BLECharacteristic *pCharacteristic;
BLECharacteristic *pCharacteristic2;
BLECharacteristic *pCharacteristic3;
BLECharacteristic *pCharacteristic4;

int pinDHT11 = 32;
SimpleDHT11 dht11;
int pinLDR =33;
int pinLED =14;

bool deviceConnected = false;
uint8_t light = 0;
int dhp11 = 0;
byte temperature = 0;
byte humidity = 0;
uint8_t temp = 0;
uint8_t humid= 0;
String led_state;
uint8_t cc =64;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE2_UUID        "45ecddcf-c316-488d-8558-3222e5cb9b3c"
#define CHARACTERISTIC2_UUID "4a78b8dd-a43d-46cf-9270-f6b750a717c8"
#define CHARACTERISTIC3_UUID "d240dbed-7d22-45bb-b810-add58a6c856b"
#define CHARACTERISTIC4_UUID "3abcedb6-e652-4415-9741-ebecc11c4580"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setup() {
  Serial.begin(115200);
  pinMode(pinLED,OUTPUT);
  // Create the BLE Device
  BLEDevice::init("CALPLUG2");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());


  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_INDICATE|
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );
 
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  
  pCharacteristic->setValue(&cc, 1);
  
  BLEService *pService2 = pServer->createService(SERVICE2_UUID);
  pCharacteristic2 = pService2->createCharacteristic(
                    CHARACTERISTIC2_UUID,
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_INDICATE
                  );
  pCharacteristic2->addDescriptor(new BLE2902());

  pCharacteristic3 = pService2->createCharacteristic(
                    CHARACTERISTIC3_UUID,
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_INDICATE
                  );
  pCharacteristic3->addDescriptor(new BLE2902());

  pCharacteristic4 = pService2->createCharacteristic(
                    CHARACTERISTIC4_UUID,
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_INDICATE
                  );
  pCharacteristic4->addDescriptor(new BLE2902());
  pService2->start();

  
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}


void loop() {
  if (deviceConnected) {
 
    light = analogRead(pinLDR);
    Serial.print("Ambient Light: ");Serial.println(light);

    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
      return;
    }
    
    Serial.print((int)temperature); Serial.print(" *C, "); 
    Serial.print((int)humidity); Serial.println(" H");

    temp= (uint8_t)temperature*9/5+32;
    humid = (uint8_t)humidity;

    std::string great = pCharacteristic->getValue();
    Serial.printf("%s\n",great.c_str());
    Serial.print(great.c_str());
    if (great != "%"){
      Serial.println("YES");
        digitalWrite(pinLED, HIGH);
    }else {
        digitalWrite(pinLED,LOW);
    }
    
    pCharacteristic2->setValue(&light, 1);
    pCharacteristic2->notify();
    
    pCharacteristic3->setValue(&temp, 1);
    pCharacteristic3->notify();

    pCharacteristic4->setValue(&humid, 1);
    pCharacteristic4->notify();
  }
  delay(1500);
}
