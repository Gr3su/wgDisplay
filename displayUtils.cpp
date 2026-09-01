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
  int padding = 6;
  char clockTime[6];
  timestampToLocaltime(time(nullptr), clockTime);

  disp.setTextSize(10);
  disp.getTextBounds(clockTime, 0, 0, &textBx, &textBy, &textBw, &textBh);

  int yPos = 183 + (disp.height() - 183) / 2 - textBh / 2;
  // yPos is the top of the text bounding box; baseline = yPos - textBy
  disp.setCursor((disp.width() / 2) - (textBw / 2) - textBx, yPos - textBy);
  disp.setTextColor(GxEPD_WHITE);
  disp.fillRect((disp.width() / 2) - (textBw / 2) - padding, yPos - padding, textBw + padding * 2, textBh + padding * 2, GxEPD_BLACK);
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
  disp.drawLine(360, yOffset +2, disp.width(), yOffset + 43, GxEPD_BLACK);
}

void draw(GxEPD2_GFX &disp, vector<Departure> departures, vector<Libary> libaries){
  disp.firstPage();

  do{
    drawDepartures(disp, departures);
    drawLibaries(disp, libaries);
    disp.drawFastHLine(0, 183, disp.width(), GxEPD_BLACK);

    drawClock(disp);

  } while(disp.nextPage());
}

void startDrawRoutine(GxEPD2_GFX &disp, vector<Departure> departures, vector<Libary> libaries){
  updateCounter++;

  if(updateCounter >= 10){
    disp.setFullWindow();
    updateCounter = 0;
  }
  else{
    disp.setPartialWindow(0, 0, disp.width(), disp.height());
  }

  draw(disp, departures, libaries);
}