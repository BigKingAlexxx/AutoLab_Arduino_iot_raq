#include "RAQ_HttpClient.h"

RAQ_HttpClient::RAQ_HttpClient(char* server, int port) {
  client = new HttpClient(wifi, server, port);
}

bool RAQ_HttpClient::postJSON(char* url, String json_string) {
  String contentType = "application/json";
  
  client->post(url, contentType, json_string);

  int statusCode = client->responseStatusCode();
  String response = client->responseBody();

  if (statusCode != 200) { //REMOVE TRUE
    Serial.print("HTTP-Error:");
    Serial.println(statusCode);
    Serial.print("Response:");
    Serial.println(response);
    return false;
  }
  
  return true;
}




