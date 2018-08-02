#ifndef RAQ_WIFI_h
#define RAQ_WIFI_h


#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>

class RAQ_Wifi {
  private:
    WiFiClient wifi;
    HttpClient* httpClient;
    int status = WL_IDLE_STATUS;
    char* ssid;                    // your network SSID (name)
    char* pass;                    // your network password (use for WPA, or use as key for WEP)

  public:
    // Constructor
    RAQ_Wifi(char* ssid, char* pass);

    void connect();
    int getWifiStatus();
};

#endif
