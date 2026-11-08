#include "displayUtils.h"
int updateCounter = 0;

void drawDepartures(GxEPD2_GFX &disp, vector<Departure> departures){
  int margin = 5;
  int yOffset = margin;
  Departure dep;
  
  int16_t timeBx, timeBy; uint16_t timeBw, timeBh;
  int16_t textBx, textBy; uint16_t textBw, textBh;

  for(int i = 0; i < departures.size(); i++){
    dep = departures.at(i);

    const unsigned char* image = (dep.type == 'b') ? busLogo : (dep.type == 't') ? tramLogo : errorLogo;
    const char* text = (dep.type == 'b') ? "Elbinger Str." : (dep.type == 't') ? "Jaegerhaus" : "Fehler";
    const char* departTime = dep.times.timetableTime;
    const int lateness = dep.times.lateness;
    disp.getTextBounds(text, 0, 0, &textBx, &textBy, &textBw, &textBh);
    disp.getTextBounds(departTime, 0, 0, &timeBx, &timeBy, &timeBw, &timeBh);

    disp.drawXBitmap(margin, yOffset, image, imageSize, imageSize, GxEPD_BLACK);
    // horizontal: desired left x positions (70 for name, 240 for time, 300 for lateness)
    int nameLeftX = 70;
    int timeLeftX = 240;
    int lateLeftX = 300;
    // vertical: center vertically in the image box
    int centerY = yOffset + imageSize / 2;
    // baseline = centerY - textBy - textBh/2
    int nameBaseline = centerY - textBy - (textBh / 2);
    int timeBaseline = centerY - timeBy - (timeBh / 2);

    disp.setCursor(nameLeftX - textBx, nameBaseline);
    disp.print(text);
    disp.setCursor(timeLeftX - timeBx, timeBaseline);
    disp.print(departTime);
    if(lateness > 0){
      disp.setCursor(lateLeftX - timeBx, timeBaseline);
      disp.print("(");
      disp.print("+");
      disp.print(lateness);
      disp.print(")");
    }

    yOffset += 45;
    disp.drawFastHLine(0, yOffset - 2, 360, GxEPD_BLACK);
  }
  
  disp.drawFastVLine(360, 0,  yOffset - 2, GxEPD_BLACK);
}

void drawClock(GxEPD2_GFX &disp){
  int16_t textBx, textBy; uint16_t textBw, textBh;
  char clockTime[6];
  timestampToLocaltime(time(nullptr), clockTime);

  disp.setTextSize(2);
  disp.getTextBounds(clockTime, 0, 0, &textBx, &textBy, &textBw, &textBh);

  int boxX = 360;
  int boxY = 138;
  int boxW = disp.width() - 360;
  int boxH = 45;

  int centerX = boxX + boxW / 2;
  int centerY = boxY + boxH / 2;

  disp.fillRect(boxX, boxY, boxW, boxH, GxEPD_BLACK);
  disp.setCursor(centerX - (textBw / 2) - textBx, centerY - textBy - (textBh / 2));
  disp.setTextColor(GxEPD_WHITE);
  disp.print(clockTime);
  disp.setTextColor(GxEPD_BLACK);
  disp.setTextSize(1);
}

void drawLibaries(GxEPD2_GFX &disp, vector<Libary> libaries){
  int margin = 5;
  int yOffset = margin;
  int16_t textBx, textBy; uint16_t textBw, textBh;

  if(libaries.empty()){
    disp.drawXBitmap(360 + margin, yOffset, errorLogo, imageSize, imageSize, GxEPD_BLACK);
    disp.setCursor(360 + imageSize + 2 * margin, yOffset);
    disp.print("Biblio-Fehler");
    return;
  }

  if(libaries[0].totalSeats == -1){
    disp.drawXBitmap(360 + margin, yOffset, errorLogo, imageSize, imageSize, GxEPD_BLACK);
    disp.setCursor(360 + imageSize + 2 * margin, yOffset);
    disp.print(libaries[0].name);
    return;
  }

  for(int i = 0; i < libaries.size(); i++){
    disp.getTextBounds(libaries[i].name, 0, 0, &textBx, &textBy, &textBw, &textBh);
    // desired top position for the library name block
    int nameTop = yOffset + 20;
    // include half the text height so the text sits visually centered above the line
    int nameBaseline = nameTop - textBy - (textBh / 2); // baseline from top, adjusted
    disp.setCursor(360 + margin, nameBaseline);
    disp.print(libaries[i].name);
    disp.print(" - ");
    if(libaries[i].totalSeats == 0){
      disp.print("geschlossen");
    } else{
      disp.print(libaries[i].totalSeats - libaries[i].freeSeats);
      disp.print("/");
      disp.print(libaries[i].totalSeats);
    }
    yOffset += 45;
    disp.drawFastHLine(360, yOffset - 2, disp.width() - 360, GxEPD_BLACK);
  }
}

void drawWithChart(GxEPD2_GFX &disp, WeatherData weather, const unsigned char* wLogo){
  int yOffset = 184;
  int margin = 10;

  disp.drawXBitmap(margin, yOffset + 5, wLogo, imageSize, imageSize, GxEPD_BLACK);

  disp.setCursor(margin + 50, yOffset + 30);
  disp.print("Aktuell: ");
  disp.print(weather.currentTemp, 1);
  disp.print("°C");
  
  disp.setCursor(margin + 230, yOffset + 30);
  disp.print("Min: ");
  disp.print(weather.minTemp, 1);
  disp.print("°C  Max: ");
  disp.print(weather.maxTemp, 1);
  disp.print("°C");
  
  disp.setCursor(margin + 480, yOffset + 30);
  disp.print("Regen-Wkt: ");
  disp.print(weather.precipProbMax);
  disp.print("%");
  
  int chartX = 50;
  int chartY = 250;
  int chartW = 700;
  int chartH = 180;
  
  disp.drawFastVLine(chartX, chartY, chartH, GxEPD_BLACK);
  disp.drawFastVLine(chartX + chartW, chartY, chartH, GxEPD_BLACK);
  disp.drawFastHLine(chartX, chartY + chartH, chartW, GxEPD_BLACK);
  
  float tempMinBound = (int)weather.minTemp - 2.0;
  float tempMaxBound = (int)weather.maxTemp + 2.0;
  float tempRange = tempMaxBound - tempMinBound;
  if(tempRange == 0) tempRange = 1;
  
  disp.setCursor(2, chartY + 15);
  disp.print((int)tempMaxBound);
  disp.setCursor(2, chartY + chartH);
  disp.print((int)tempMinBound);
  
  float maxPrecip = 1.0;
  for(int i=0; i<24; i++) {
      if(weather.hourlyPrecip[i] > maxPrecip) maxPrecip = weather.hourlyPrecip[i];
  }
  disp.setCursor(chartX + chartW + 5, chartY + 15);
  disp.print(maxPrecip, 1);
  disp.setCursor(chartX + chartW + 5, chartY + chartH);
  disp.print("0.0");
  
  int prevTX = -1, prevTY = -1;
  int barW = chartW / 24 - 2;
  if (barW < 2) barW = 2;
  
  for(int i=0; i<24; i++) {
      int x = chartX + (i * chartW) / 23;
      
      float t = weather.hourlyTemp[i];
      int ty = chartY + chartH - (int)(((t - tempMinBound) / tempRange) * chartH);
      
      float p = weather.hourlyPrecip[i];
      int py = chartY + chartH - (int)((p / maxPrecip) * chartH);
      
      if (p > 0) {
          int bW = barW;
          if (x - bW/2 < chartX) bW = (x - chartX) * 2;
          disp.fillRect(x - bW/2, py, bW, chartY + chartH - py, GxEPD_BLACK);
      }
      
      if (prevTX != -1) {
          disp.drawLine(prevTX, prevTY, x, ty, GxEPD_BLACK);
          disp.drawLine(prevTX, prevTY-1, x, ty-1, GxEPD_BLACK); // Make line thicker
      }
      
      prevTX = x; prevTY = ty;
      
      if (i % 4 == 0 || i == 23) {
          disp.drawFastVLine(x, chartY + chartH, 5, GxEPD_BLACK);
          disp.setCursor(x - 15, chartY + chartH + 20);
          disp.print(i);
          disp.print("h");
      }
  }
}

void drawWithoutChart(GxEPD2_GFX &disp, WeatherData weather, const unsigned char* wLogo, int randomIndex){
  int yOffset = 250;
  int margin = 10;

  disp.drawXBitmap(margin, yOffset + 5, wLogo, imageSize, imageSize, GxEPD_BLACK);

  disp.setTextSize(2);

  disp.setCursor(margin + 50, yOffset + 35);
  disp.print("Aktuell: ");
  disp.print(weather.currentTemp, 1);
  disp.print("C");
  

  disp.setCursor(margin + 50, yOffset + 75);
  disp.print("Min: ");
  disp.print(weather.minTemp, 1);
  disp.print("°C");
  disp.setCursor(margin + 50, yOffset + 115);
  disp.print("Max: ");
  disp.print(weather.maxTemp, 1);
  disp.print("°C");

  disp.setCursor(margin + 50, yOffset + 155);
  disp.print("Regen-Wkt: ");
  disp.print(weather.precipProbMax);
  disp.print("%");

  disp.setTextSize(1);

  const unsigned char* image = (const unsigned char*) pgm_read_ptr(&(wallpapers[randomIndex]));
  disp.drawXBitmap(360, 184, image, 440, 295, GxEPD_BLACK);
}

void drawWeatherAndChart(GxEPD2_GFX &disp, WeatherData weather, int randomIndex) {
  if (weather.error) {
    disp.setCursor(10, 200);
    disp.print("Wetterdaten Fehler");
    return;
  }
  
  const unsigned char* wLogo = sonneLogo;
  if (weather.weatherCode == 1 || weather.weatherCode == 2) wLogo = leichtbewoelktLogo;
  else if (weather.weatherCode == 3) wLogo = wolkigLogo;
  else if (weather.weatherCode == 45 || weather.weatherCode == 48) wLogo = nebelLogo;
  else if (weather.weatherCode >= 51 && weather.weatherCode <= 67) wLogo = regenLogo;
  else if (weather.weatherCode >= 71 && weather.weatherCode <= 77) wLogo = schneeLogo;
  else if (weather.weatherCode >= 80 && weather.weatherCode <= 82) wLogo = regenLogo;
  else if (weather.weatherCode >= 85 && weather.weatherCode <= 86) wLogo = schneeLogo;
  else if (weather.weatherCode >= 95) wLogo = gewitterLogo;
  
  if(weather.precipProbMax != 0) drawWithChart(disp, weather, wLogo);
  else drawWithoutChart(disp, weather, wLogo, randomIndex);
  
}

void draw(GxEPD2_GFX &disp, vector<Departure> departures, vector<Libary> libaries, WeatherData weather){
  disp.firstPage();

  const size_t wallpaperCount = sizeof(wallpapers) / sizeof(wallpapers[0]);
  int randomIndex = random(0, wallpaperCount);

  do{
    drawDepartures(disp, departures);
    drawLibaries(disp, libaries);
    disp.drawFastHLine(0, 183, disp.width(), GxEPD_BLACK);

    drawClock(disp);
    drawWeatherAndChart(disp, weather, randomIndex);

  } while(disp.nextPage());
}

void startDrawRoutine(GxEPD2_GFX &disp, vector<Departure> departures, vector<Libary> libaries, WeatherData weather){
  updateCounter++;

  if(updateCounter >= 10){
    disp.setFullWindow();
    updateCounter = 0;
  }
  else{
    disp.setPartialWindow(0, 0, disp.width(), disp.height());
  }

  draw(disp, departures, libaries, weather);
}