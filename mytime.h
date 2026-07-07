#ifndef MYTIME_H
#define MYTIME_H

#include <time.h>
#include <Arduino.h>

using namespace std;

time_t myTimegm(struct tm *t);
void timestampToLocaltime(const time_t timestamp, char* outputTime);
time_t isoToTimestamp(const String iso);

#endif