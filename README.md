# IoT_telemetry_app

IoT Project to monitor farmers fields using lorawan. The web app  receives real time data from an arduino MKRWAN1310. The data received in measured by two arduino sensors : 
* light sensor : Grove LS06-S Phototransistor
* humidity sensor : arduino Grove Moisture sensor

The data received is displayed in a node server. This data is retreived by using an mqtt client reaching TTN server. ["The Things Network"](https://eu1.cloud.thethings.network/console/)

!["iot_app_ui"](/IoT_mqtt_client/public/images/iot_ui.png)

# The Things Network configuration

Create a new application and add a new device. Set as the “DeviceEUI” the unique identifier of the board given by running the script ‘LORA-Dev-EUI.ino’. <br>
Get the AppKey that will allow you to make the device authentication by a OTAA (Over the Air Activation) request.  
<br>

# Device configuration
To be able to successfully run the ‘Iot_Arduino_send_sensors_data.ino’ script, you need to specify the AppKey (found in ttn configuration) in a new script called ‘arduino_secrets.h’ that you need to create in the ‘Iot_Arduino_send_sensors_data’ directory. The APP_EUI can be set to all zeros in ttn and in the ‘arduino_secrets.h’. 

### File template : 'arduino_secrets.h'
```c
#define APP_EUI "ttn_app_eui"
#define APP_KEY "ttn_app_key"
```

# Web app installation/configuration
### Node dependencies
```
npm i express 
npm i http 
npm i ejs 
npm i socket.io  
npm i mqtt 
```


Create a ‘secrets.json’ file in the IoT_mqtt_client’ directory in which you define a username and the password that you will find in the mqtt integration details ‘Connection credentials’ from ttn. 

### File template : 'secrets.json'
```js
{
    "username":"YourApplicationName@ttn",
    "password":"YourApiKey"
}
```

 