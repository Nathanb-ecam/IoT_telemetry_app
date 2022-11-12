// Iot_Arduino_MKR_1310
// execute LORA-Dev-EUI.ino to get the app_key




#include <MKRWAN.h>
#include "arduino_secrets.h"

LoRaModem modem;
int sensorPin = A0;
int humiditySensorValue = 0;
int lightSensorValue = 0;
int msg_counter = 0;




void setup() {  
  Serial.begin(115200);
  while (!Serial);
   
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  
  if (modem.version() != ARDUINO_FW_VERSION) {
    Serial.println("Please make sure that the latest modem firmware is installed.");
    Serial.println("To update the firmware upload the 'MKRWANFWUpdate_standalone.ino' sketch.");
  }
  
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  Serial.println("Connecting...");
  int connected = modem.joinOTAA(APP_EUI, APP_KEY);

  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  delay(5000);
}

void loop() {
  msg_counter++;
  Serial.println("Sending message...");
  modem.setPort(3);
  modem.beginPacket();

  humiditySensorValue = analogRead(sensorPin);
  humiditySensorValue = (1023 - humiditySensorValue)/4; 
  humiditySensorValue = humiditySensorValue;
  Serial.println(humiditySensorValue);


  // lightSensorValue = analogRead();
    // lightSensorValue = lightSensorValue/4;
  lightSensorValue = 255;
  Serial.println(lightSensorValue);

  modem.print(lightSensorValue);
  modem.print(humiditySensorValue);


  int error = modem.endPacket(true);
  
  if (error > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
    Serial.println(error);
  }
  delay(5000);
}