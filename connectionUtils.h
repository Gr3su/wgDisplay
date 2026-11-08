#ifndef CONNECTIONUTILS_H
#define CONNECTIONUTILS_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

enum class ApplicationType {
    XML,
    JSON
};

void prepareHttpClient(HTTPClient &http, WiFiClientSecure &client, const char* url, ApplicationType applicationType);

#endif