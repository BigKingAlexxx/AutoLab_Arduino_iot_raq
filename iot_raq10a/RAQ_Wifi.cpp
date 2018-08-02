#include "RAQ_Wifi.h"

RAQ_Wifi::RAQ_Wifi(char* ssid, char* pass) {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
  
  this->ssid = ssid;
  this->pass = pass;
}

void RAQ_Wifi::connect() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present.");
    return;
  }
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network:
  status = WiFi.begin(ssid, pass);
}

int RAQ_Wifi::getWifiStatus(){
  return WiFi.status();
}

