#ifndef LIBARY_H
#define LIBARY_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <vector>
#include "connectionUtils.h"

using namespace std;

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

struct Libary{
  char name[15];
  int totalSeats;
  int freeSeats;
};

inline const Libary libaryError = Libary{"Fehler", -1, -1};

vector<Libary> sendLibaryRequest();

#endif