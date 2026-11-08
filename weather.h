#ifndef WEATHER_H
#define WEATHER_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

struct WeatherData {
    bool error;
    float currentTemp;
    int weatherCode;
    float maxTemp;
    float minTemp;
    int precipProbMax;
    float hourlyTemp[24];
    float hourlyPrecip[24];
};

WeatherData sendWeatherRequest();

#endif

