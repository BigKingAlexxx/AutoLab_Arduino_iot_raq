#ifndef RAQ_HTTPCLIENT_h
#define RAQ_HTTPCLIENT_h

#include <WiFi101.h>
#include <ArduinoHttpClient.h>                 

class RAQ_HttpClient {
  private:
    WiFiClient wifi;
    HttpClient* client; 

  public:
    // Constructor
    RAQ_HttpClient(char* serverAddress, int port);
    
    bool postJSON(char* host, String json_string);

};

#endif
