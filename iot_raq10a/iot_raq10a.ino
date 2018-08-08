//Includes for OLED-Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>

//Include our classes
#include "RAQ_BME280.h"
#include "RAQ_BME680.h"
#include "RAQ_CCS811.h"
#include "RAQ_Wifi.h"
#include "RAQ_HttpClient.h"
#include "RAQ_SensorData.h"

//Name of RoomAirQuality-Sensor
#define SENSOR_ID "EU02"                //change this for second Sensor

//Define WiFi SSID and PASS
#define WIFI_SSID "DESKTOP-B7JQT0N 4039"
#define WIFI_PASS "27Q?c465"

//Global vars
Adafruit_SSD1306 oled = Adafruit_SSD1306();

RAQ_BME280 sensor_bme;                  //change type to RAQ_BME680 if comboboard is connected
RAQ_CCS811 sensor_ccs;
RAQ_Wifi   wifi(WIFI_SSID, WIFI_PASS);
RAQ_HttpClient http("192.168.137.1", 9000);

unsigned long millis_wifi = 0;
unsigned long millis_displaySensorData = 0;
unsigned long millis_addSensorData = 0;

SensorDataList sensorDataList;
SensorData currentSensorData;
int ptr_sensorData = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  delay(10);  //Make sure sensors had enough time to turn on.

  //Initialize oled-display
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.display();
  delay(1000);
  
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,0);
  oled.println("Raumluftqualitaet");
  oled.println("Alex Herr");
  oled.println("David Schlegel");
  oled.println("Sebastian Kreutz");
  oled.display();
  

  //Initialize Sensors
  int status_bme = sensor_bme.init();
  int status_ccs = sensor_ccs.init();

  //Stop if problem with Sensors
  if (!status_bme || !status_ccs) {
    while(true);
  }

  //Connect with Hotspot
  wifi.connect();

  //Wait for WiFi to connect
  delay(5000);
}

void loop() {
  //If WiFi is not connected, try every 5sec
  if (wifi.getWifiStatus() != WL_CONNECTED &&
      millis() - millis_wifi >= 5000) {
    wifi.connect();
    millis_wifi = millis();
  }
  
  //Get Sensordata every 1.5sec and print on Serial/OLED
  if (millis() - millis_displaySensorData >= 1500) {
    //Get Sensordata and save into struct
    currentSensorData = {
      millis(), 
      sensor_bme.getTemp(),
      sensor_bme.getPressure(),
      sensor_bme.getHumidity(),
      sensor_ccs.getCO2(),
      sensor_ccs.getTVOC(),
      NULL
    };
    
    //Print Data on Serial.
    Serial.println("-------------------------------");
    Serial.print("Temperatur: ");
    Serial.println(currentSensorData.temp);
    Serial.print("Luftdruck: ");
    Serial.println(currentSensorData.pressure);
    Serial.print("Luftfeuchtigkeit: ");
    Serial.println(currentSensorData.humidity);
    Serial.print("CO2: ");
    Serial.println(currentSensorData.co2);
    Serial.print("TVOC: ");
    Serial.println(currentSensorData.tvoc);
    Serial.println("-------------------------------");

    //Print Data on OLED.
    oled.clearDisplay();
    oled.setCursor(0,0);
    oled.print("Temperatur: ");
    oled.print(currentSensorData.temp);
    oled.println("C");
    oled.print("Luftfeucht.: ");
    oled.print(currentSensorData.humidity);
    oled.println("%");
    oled.print("CO2: ");
    oled.print(currentSensorData.co2);
    oled.println("ppm");
    oled.print("TVOC: ");
    oled.print(currentSensorData.tvoc);
    oled.println("mg/m3");
    oled.display();

    millis_displaySensorData = millis();
  }

  //Add Sensordata every 5sec to list
  if (millis() - millis_addSensorData >= 5000) {
    SensorData* temp = new SensorData{
        currentSensorData.timestamp, 
        currentSensorData.temp,
        currentSensorData.pressure,
        currentSensorData.humidity,
        currentSensorData.co2,
        currentSensorData.tvoc,
        NULL
      };
    
    bool addRes = sensorDataList.addSensorData(temp, true);
    
    //test.....
    if (addRes) {
      Serial.println("Added SensorData to List.");
    } else {
      Serial.println("Failed to add SensorData to List.");
    }
    
    millis_addSensorData = millis();
  }

  //If WiFi is connected, send Sensordata from List to Server
  if (wifi.getWifiStatus() == WL_CONNECTED) {
    //Get Sensordata from List
    SensorData* tempSensorData = sensorDataList.getSensorData();
    
    //If List is not mt
    if (tempSensorData != NULL) {
      
      Serial.println("Prepare to send Data.");
      
      //Make JSON-String (HTTP Request Body)
      String reqBody = "[{\"SensorID\":\"" + String(SENSOR_ID)
        + "\",\"Zeit\":" + String(millis() - tempSensorData->timestamp)
        + ",\"Temperatur\":" + String(tempSensorData->temp)
        + ",\"Luftdruck\":" + String(tempSensorData->pressure)
        + ",\"Luftfeuchtigkeit\":" + String(tempSensorData->humidity)
        + ",\"CO2\":" + String(tempSensorData->co2)
        + ",\"TVOC\":" + String(tempSensorData->tvoc)
        + "}]";
      
      //Print for debug
      Serial.println(reqBody);
      
      //Send Data
      bool res = http.postJSON("/", reqBody);
      
      if (res) {
        Serial.println("Sent Data.");
        delete tempSensorData;      //Deallocate Memory
      } else {
        Serial.println("Failed to send Data.");
        sensorDataList.addSensorData(tempSensorData, false);    //Put back into List
      }
    }
  }
  
}
