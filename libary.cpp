#include "libary.h"
void requestLibaryData(HTTPClient& http, vector<Libary>& libaries){
  int httpCode = http.GET();
  
  if(httpCode == HTTP_CODE_OK){
    WiFiClient* stream = http.getStreamPtr();
    int i = 0;
    int j = 0;
    vector<vector<const char*>> libariesShortNames = {{"LSG", "LSM", "LST", "LSN", "LSW", "LBS"}, {"L3", "L2", "SAR", "L1", "LEG"}, {"FBI"}};
    vector<const char*> libariesLongNames = {"Hauptbib", "InformatiKOM", "Infobib"};
    const char* stringToSearch;
    String line;
    int occupiedSeats, freeSeats;
    
    while(http.connected() && stream->available() && i < libariesShortNames.size()){
      stringToSearch = libariesShortNames[i][j];
      line = stream->readStringUntil(':');
      
      if(line.indexOf(stringToSearch) != -1){
        if(stream->findUntil("\"occupied_seats\":", "]")){
          occupiedSeats += stream->parseInt();
        }
        if(stream->findUntil("\"free_seats\":","]")){
          freeSeats += stream->parseInt();
        }
        j++;
      }

      if(j >= libariesShortNames[i].size()){
        j = 0;
        
        Libary libary;
        libary.freeSeats = freeSeats;
        libary.totalSeats = freeSeats + occupiedSeats;
        strncpy(libary.name, libariesLongNames[i], sizeof(libary.name) - 1);
        libary.name[sizeof(libary.name) - 1] = '\0';
        libaries.push_back(libary);
        freeSeats = 0;
        occupiedSeats = 0;
        i++;
      }
      
    }
  } else{
    libaries.push_back(libaryError);
    Serial.println("Libary Anfrage fehlgeschlagen");
  }
  
  
}

vector<Libary> sendLibaryRequest(){
  vector<Libary> libaries;
  const char* url = "https://seatfinder.bibliothek.kit.edu/karlsruhe/getdata.php?location[0]=LSG%2CLSM%2CLST%2CLSN%2CLSW%2CLBS%2CBIB-N%2CL3%2CL2%2CSAR%2CL1%2CLEG%2CFBC%2CFBP%2CLAF%2CFBA%2CFBI%2CFBM%2CFBH%2CFBD%2CBLB%2CWIS&values[0]=seatestimate%2Cmanualcount&after[0]=-10800seconds&before[0]=now&limit[0]=-17&location[1]=LSG%2CLSM%2CLST%2CLSN%2CLSW%2CLBS%2CBIB-N%2CL3%2CL2%2CSAR%2CL1%2CLEG%2CFBC%2CFBP%2CLAF%2CFBA%2CFBI%2CFBM%2CFBH%2CFBD%2CBLB%2CWIS&values[1]=location&after[1]=&before[1]=now&limit[1]=1&refresh=&_=1775509589816";

  HTTPClient http;
  prepareHttpClient(http, url, ApplicationType::JSON);
  requestLibaryData(http, libaries);
  http.end();

  return libaries;
}