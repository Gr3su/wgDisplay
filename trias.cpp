#include "trias.h"

Times extractTimes(String timeTable, String estimTime){
  struct Times times;
  time_t tableTime = isoToTimestamp(timeTable);
  times.timestamp = tableTime;

  if(estimTime.length() > 0){
    time_t timeEstimate = isoToTimestamp(estimTime);
    times.lateness = (timeEstimate - tableTime) / 60;
  }
  else{
    times.lateness = 0;
  }
  timestampToLocaltime(tableTime, times.timetableTime);

  return times;
}

void requestDeparturesForStation(HTTPClient& http, vector<Departure>& departures, const char* payload, const char type){
  int httpCode = http.POST(payload);
  
  if(httpCode == HTTP_CODE_CREATED || httpCode == HTTP_CODE_OK){
      WiFiClient* stream = http.getStreamPtr();
      String line, tableTime, estimTime;

      while(http.connected() && stream->available()){
        line = stream->readStringUntil('>');
        
        if(line.indexOf("<trias:TimetabledTime") != -1){
          if(tableTime.length() > 0){
            Times times = extractTimes(tableTime, "");
            departures.push_back(Departure{type, times});  
          }
          tableTime = stream->readStringUntil('<');
        }
        else if(line.indexOf("<trias:EstimatedTime") != -1){
          estimTime = stream->readStringUntil('<');
          Times times = extractTimes(tableTime, estimTime);
          tableTime = estimTime = "";
          departures.push_back(Departure{type, times});
        }
      }

      if(tableTime.length() != 0 && estimTime.length() == 0){
        Times times = extractTimes(tableTime, "");
        departures.push_back(Departure{type, times}); 
      }
    
  } else{
    departures.push_back(departureError);
    Serial.println("Trias Anfrage fehlgeschlagen");
    return;
  }
}

vector<Departure> sendTriasRequest(){
  const char* url = "https://projekte.kvv-efa.de/grosstrias/trias";
  const int maxAttempts = 2;
  vector<Departure> lastResult;

  for(int attempt = 0; attempt < maxAttempts; ++attempt){
    vector<Departure> departures;

    HTTPClient http;
    prepareHttpClient(http, url, ApplicationType::XML);
    requestDeparturesForStation(http, departures, payloadBusLine, 'b');
    http.end();

    prepareHttpClient(http, url, ApplicationType::XML);
    requestDeparturesForStation(http, departures, payloadTramLine, 't');
    http.end();

    lastResult = departures;

    if(!departures.empty() && !(departures.size() == 1 && departures[0].type == 'e')){
      sort(departures.begin(), departures.end(), [](const Departure &a, const Departure &b){ return a.times.timestamp < b.times.timestamp; });
      return departures;
    }

    Serial.println("Trias Antwort leer, Anfrage wird erneut gesendet");
  }

  return lastResult;
}