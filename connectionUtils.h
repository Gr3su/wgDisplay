#ifndef CONNECTION_H
#define CONNECTION_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

enum class ApplicationType{
  XML,
  JSON
};

void prepareHttpClient(HTTPClient &http, const char* url, ApplicationType applicationType);

#endif