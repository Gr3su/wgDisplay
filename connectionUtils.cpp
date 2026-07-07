#include "connectionUtils.h"

void prepareHttpClient(HTTPClient &http, const char* url, ApplicationType applicationType){
  WiFiClientSecure client;
  client.setInsecure();
  const char* type;

  if(applicationType == ApplicationType::XML){
    type = "application/xml";
  }
  else if(applicationType == ApplicationType::JSON){
    type = "application/json";
  }

  http.begin(client, url);
  http.addHeader("Content-Type", type);
  http.addHeader("Accept", type);
}