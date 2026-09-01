#define ENABLE_GxEPD2_GFX 1

#include <ESP8266WiFi.h>
#include <time.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "trias.h"
#include "libary.h"
#include "displayUtils.h"


// Display
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 4> display(GxEPD2_750_T7(/*CS=*/ 16, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 4)); // GDEW075T7 800x480, EK79655 (GD7965)

// Constants
const unsigned int retryDelay = 500;
const unsigned int baudrate = 115200;
const unsigned char screenRotation = 0;

// WiFi Zugangsdaten
const char* ssid = "Vodafone-4B9C";
const char* password = "3eFYbJDk3hRJrCND";

void setup() {
  Serial.begin(baudrate);
  Serial.println();
  display.init(baudrate, true, 2, false);
  display.setRotation(0);
  //display.setTextSize(2);
  display.setFont(&FreeSansBold9pt7b);
  display.clearScreen();
  delay(2000);
  
  setupWifi();

  setupSystemtime();

  display.clearScreen();

}

void loop() {
  vector<Departure> departures = sendTriasRequest();
  vector<Libary> libaries = sendLibaryRequest();
  
  startDrawRoutine(display, departures, libaries);
  
  display.powerOff();
  delay(60000);
}

String getTimeStamp() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  char buf[25];
  // Format: 2026-03-27T17:30:00+01:00
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S+01:00", p_tm);
  return String(buf);
}

void displayLoadingMessage(const char* message, int step){
  char result[32];

  int numDots = step % 4;
  snprintf(result, sizeof(result), "%s%.*s", message, numDots, "...");

  displayShortMessage(result);
}

void displayShortMessage(const char* message){
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(message, 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setTextColor(GxEPD_BLACK);
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.firstPage();
  do{
    display.fillScreen(GxEPD_WHITE);
    // Place text so its bottom hits the screen bottom regardless of font metrics
    int bottom = display.height();
    int baseline = bottom - tby - tbh - 5; // baseline = bottom - (textBy + textBh)
    display.setCursor(0, baseline);
    display.print(message);
  }
  while(display.nextPage());
}

void setupWifi(){
WiFi.mode(WIFI_STA);
  int attempt = 0;
  displayLoadingMessage("Verbinde mit WLAN", attempt);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    displayLoadingMessage("Verbinde mit WLAN", ++attempt);
    delay(retryDelay);
  }
}

void setupSystemtime(){
  const int gmtOffsetSec = 3600;
  const int daylightOffsetSec = 3600;
  const char* ntpServer = "europe.pool.ntp.org";
  const char* loadingMessage = "Warte auf NTP-Zeit";

  configTime(gmtOffsetSec, daylightOffsetSec, ntpServer);
  int attempt = 0;
  displayLoadingMessage(loadingMessage, attempt);
  time_t now = time(nullptr);
  while(now < 8 * 3600 * 2){
    displayLoadingMessage(loadingMessage, ++attempt);
    delay(retryDelay);
    now = time(nullptr);
  }
}