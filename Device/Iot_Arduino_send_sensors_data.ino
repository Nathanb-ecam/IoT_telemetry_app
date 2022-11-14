#include <MKRWAN.h>
#include "arduino_secrets.h"


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

LoRaModem modem;
int humiditySensorPin = A0;
// int lightSensorPin = ;
int humiditySensorValue = 0;
uint16_t lightSensorValue = 0;
// int msg_counter = 0;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);// for light sensor 



void configureLightSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  // /* Update these values depending on what you've set above! */  
  // Serial.println("------------------------------------");
  // Serial.print  ("Gain:         "); Serial.println("Auto");
  // Serial.print  ("Timing:       "); Serial.println("13 ms");
  // Serial.println("------------------------------------");
}




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
  Serial.println("Light Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  //use tsl.begin() to default to Wire, 
  //tsl.begin(&Wire2) directs api to use Wire2, etc.
  if(!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Setup the sensor gain and integration time */
  configureLightSensor();
}

void loop() {

  Serial.println("Sending message...");
  modem.setPort(3);
  modem.beginPacket();


  // 0 à 255 
  humiditySensorValue = analogRead(humiditySensorPin);
  humiditySensorValue = (1023 - humiditySensorValue)/4; 
  humiditySensorValue = humiditySensorValue;
  Serial.println(humiditySensorValue/2.55);


  /* Get a new Light sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
  if (event.light)
  {
    // Serial.println(event.light);
    // Serial.println(event.light);
    //0 à 65536
    lightSensorValue = event.light;
    Serial.println(lightSensorValue);
    // lightSensorValue_tosend = lightSensorValue*(255/65535);
    // Serial.println(lightSensorValue_tosend);
    // lightSensorValue = event.light;
    
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    // lightSensorValue =0; 
    Serial.println("Sensor overload");
  }

  
  // Serial.println(lightSensorValue);
  modem.write(char(humiditySensorValue));
  modem.write(lightSensorValue);
  

  // modem.write(event.light);




  int error = modem.endPacket(true);
  
  if (error > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
    Serial.println(error);
  }
  delay(5000);
}