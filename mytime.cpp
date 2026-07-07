#include "mytime.h"

// Eigene Hilfsfunktion als Ersatz für timegm
time_t myTimegm(struct tm *t) {
  int year = t->tm_year + 1900;
  int month = t->tm_mon; // 0-11
  uint32_t days = t->tm_mday - 1;

  // Tage pro Monat (ohne Schaltjahr)
  static const int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // Tage für die vergangenen Jahre seit 1970 addieren
  for (int i = 1970; i < year; i++) {
    days += 365;
    // Schaltjahr-Check
    if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
      days++;
    }
  }

  // Tage für die vergangenen Monate im aktuellen Jahr addieren
  for (int i = 0; i < month; i++) {
    days += month_days[i];
    // Schaltjahr-Check für den Februar
    if (i == 1 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
      days++;
    }
  }

  // Alles in Sekunden umrechnen (UL = Unsigned Long, verhindert Overflow)
  time_t epoch = (time_t)days * 86400UL;
  epoch += (time_t)t->tm_hour * 3600UL;
  epoch += (time_t)t->tm_min * 60UL;
  epoch += t->tm_sec;

  return epoch;
}

time_t isoToTimestamp(const String iso){
  struct tm t;
  int year, month, day, hour, minute, second;

  if (sscanf(iso.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6) {
    t.tm_year = year - 1900;
    t.tm_mon = month - 1; 
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = 0;     

    return myTimegm(&t); 
  }

  return 0;
}

void timestampToLocaltime(const time_t timestamp, char* outputTime){
  struct tm* timeinfo;

  timeinfo = localtime(&timestamp);

  if(timeinfo != nullptr){
    snprintf(outputTime, 6, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
  }
  else{
    snprintf(outputTime, 6, "--:--");
  }
}