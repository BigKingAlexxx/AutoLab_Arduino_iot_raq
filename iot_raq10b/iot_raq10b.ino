#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>

#include "RAQ_BME280.h"
#include "RAQ_BME680.h"
#include "RAQ_CCS811.h"
#include "RAQ_Wifi.h"
#include "RAQ_HttpClient.h"
#include "RAQ_SensorData.h"

#define SENSOR_ID "LI008"

#define WIFI_SSID "DESKTOP-B7JQT0N 4039"
#define WIFI_PASS "27Q?c465"

Adafruit_SSD1306 oled = Adafruit_SSD1306();

RAQ_BME680 sensor_bme;                  //change type to RAQ_BME280 if combo board is connected
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

  if (!status_bme || !status_ccs) {
    while(true);
  }

  wifi.connect();

  delay(5000);
}

void loop() {
  if (wifi.getWifiStatus() != WL_CONNECTED &&
      millis() - millis_wifi >= 5000) {
    wifi.connect();
    millis_wifi = millis();
  }
  
  if (millis() - millis_displaySensorData >= 1500) {
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
    oled.println(currentSensorData.temp);
    oled.print("Luftdruck: ");
    oled.println(currentSensorData.pressure);
    oled.print("Luftfeucht.: ");
    oled.println(currentSensorData.humidity);
    oled.print("CO2: ");
    oled.println(currentSensorData.co2);
    oled.print("TVOC: ");
    oled.println(currentSensorData.tvoc);
    oled.display();

    millis_displaySensorData = millis();
  }

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
    if (addRes) {
      Serial.println("Added SensorData to List.");
    } else {
      Serial.println("Failed to add SensorData to List.");
    }
    millis_addSensorData = millis();
  }

  if (wifi.getWifiStatus() == WL_CONNECTED) {
    SensorData* tempSensorData = sensorDataList.getSensorData();
    if (tempSensorData != NULL) {
      Serial.println("Prepare to send Data.");
      String reqBody = "[{\"SensorID\":\"" + String(SENSOR_ID)
        + "\",\"Zeit\":" + String(millis() - tempSensorData->timestamp)
        + ",\"Temperatur\":" + String(tempSensorData->temp)
        + ",\"Luftdruck\":" + String(tempSensorData->pressure)
        + ",\"Luftfeuchtigkeit\":" + String(tempSensorData->humidity)
        + ",\"CO2\":" + String(tempSensorData->co2)
        + ",\"TVOC\":" + String(tempSensorData->tvoc)
        + "}]";
      Serial.println(reqBody);
      bool res = http.postJSON("/", reqBody);
      if (res) {
        Serial.println("Sent Data.");
        delete tempSensorData;
      } else {
        Serial.println("Failed to send Data.");
        sensorDataList.addSensorData(tempSensorData, false);
      }
    }
  }
  
}
