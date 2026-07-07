#ifndef TRIAS_H
#define TRIAS_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <vector>
#include "queries.h"
#include "tinyxml2.h"
#include "mytime.h"
#include "connectionUtils.h"

using namespace std;
using namespace tinyxml2;


struct Times{
  char timetableTime[6];
  time_t timestamp;
  int lateness;
};

struct Departure {
  char type;
  Times times;
};

inline const Departure departureError = Departure{'e', Times{"", 0, 0}};

vector<Departure> sendTriasRequest();

#endif