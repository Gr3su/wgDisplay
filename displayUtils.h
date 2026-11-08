#ifndef DISPLAYUTILS_H
#define DISPLAYUTILS_H

#include <GxEPD2_BW.h>
#include <vector>
#include "GxEPD2_GFX.h"
#include <time.h>

#include "queries.h"
#include "images.h"
#include "mytime.h"
#include "weather.h"
#include "libary.h"
#include "trias.h"

using namespace std;

void startDrawRoutine(GxEPD2_GFX &disp, vector<Departure> departures, vector<Libary> libaries, WeatherData weather);

#endif