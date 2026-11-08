#include "weather.h"
#include "connectionUtils.h"
#include <Arduino.h>

void requestWeatherData(HTTPClient& http, WeatherData& weather) {
    weather.error = true;
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        WiFiClient* stream = http.getStreamPtr();
        
        // Find current temp and weather code
        if(stream->find("\"current\":{")) {
            if(stream->find("\"temperature_2m\":")) {
                weather.currentTemp = stream->parseFloat();
            }
            if(stream->find("\"weather_code\":")) {
                weather.weatherCode = stream->parseInt();
            }
        }
        
        // Find hourly temp and precip
        if(stream->find("\"hourly\":{")) {
            if(stream->find("\"temperature_2m\":[")) {
                for(int i = 0; i < 24; i++) {
                    weather.hourlyTemp[i] = stream->parseFloat();
                }
            }
            if(stream->find("\"precipitation\":[")) {
                for(int i = 0; i < 24; i++) {
                    weather.hourlyPrecip[i] = stream->parseFloat();
                }
            }
        }
        
        // Find daily min/max and prob
        if(stream->find("\"daily\":{")) {
            if(stream->find("\"temperature_2m_max\":[")) {
                weather.maxTemp = stream->parseFloat();
            }
            if(stream->find("\"temperature_2m_min\":[")) {
                weather.minTemp = stream->parseFloat();
            }
            if(stream->find("\"precipitation_probability_max\":[")) {
                weather.precipProbMax = stream->parseInt();
            }
            weather.error = false;
        }
    } else {
        Serial.print("HTTP GET fehlerhaft, Fehlercode: ");
        Serial.println(httpCode);
    }
}

WeatherData sendWeatherRequest() {
    WeatherData weather;
    weather.error = true;
    // URL for Karlsruhe with current temp/code, hourly temp/precip, and daily max/min/precip
    const char* url = "https://api.open-meteo.com/v1/forecast?latitude=49.0069&longitude=8.4037&current=temperature_2m,weather_code&hourly=temperature_2m,precipitation&daily=temperature_2m_max,temperature_2m_min,precipitation_probability_max&timezone=Europe%2FBerlin&forecast_days=1";
    
    for(int attempt = 0; attempt < 2; ++attempt) {
        HTTPClient http;
        WiFiClientSecure client;
        prepareHttpClient(http, client, url, ApplicationType::JSON);
        requestWeatherData(http, weather);
        http.end();
        
        if (!weather.error) {
            return weather;
        }
        Serial.println("Wetter Antwort fehlerhaft, Anfrage wird erneut gesendet");
    }
    return weather;
}

