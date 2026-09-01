#ifndef QUERIES_H
#define QUERIES_H

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

inline const char payloadBusLine[] PROGMEM =
                  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                  "<Trias version=\"1.1\" xmlns=\"http://www.vdv.de/trias\" xmlns:siri=\"http://www.siri.org.uk/siri\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.vdv.de/trias file:///C:/development/HEAD/extras/TRIAS/TRIAS_1.1/Trias.xsd\">\r\n"
                  "    <ServiceRequest>\r\n"
                  "        <siri:RequestTimeStamp>{timestamp}</siri:RequestTimeStamp>\r\n"
                  "        <siri:RequestorRef>JVsLJdlVt7Bg</siri:RequestorRef>\r\n"
                  "        <RequestPayload>\r\n"
                  "            <StopEventRequest>\r\n"
                  "    <Location>\r\n"
                  "      <LocationRef>\r\n"
                  "      <StopPointRef>de:08212:3011</StopPointRef>\r\n"
                  "      <LocationName>Karlsruhe Waldstadt Elbinger Strasse</LocationName>\r\n"
                  "      </LocationRef>\r\n"
                  "    </Location>\r\n"
                  "    <Params>\r\n"
                  "      <LineFilter>\r\n"
                  "        <Line>\r\n"
                  "          <LineRef>kvv:24030:E:H</LineRef>\r\n"
                  "          <DirectionRef>outward</DirectionRef>\r\n"
                  "        </Line>\r\n"
                  "      </LineFilter>\r\n"
                  "      <NumberOfResults>2</NumberOfResults>\r\n"
                  "      <IncludeRealtimeData>true</IncludeRealtimeData>\r\n"
                  "    </Params>\r\n"
                  "            </StopEventRequest>\r\n"
                  "        </RequestPayload>\r\n"
                  "    </ServiceRequest>\r\n"
                  "</Trias>";

inline const char payloadTramLine[] PROGMEM =
                        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                        "<Trias version=\"1.1\" xmlns=\"http://www.vdv.de/trias\" xmlns:siri=\"http://www.siri.org.uk/siri\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.vdv.de/trias file:///C:/development/HEAD/extras/TRIAS/TRIAS_1.1/Trias.xsd\">\r\n"
                        "    <ServiceRequest>\r\n"
                        "        <siri:RequestTimeStamp>{timestamp}</siri:RequestTimeStamp>\r\n"
                        "        <siri:RequestorRef>JVsLJdlVt7Bg</siri:RequestorRef>\r\n"
                        "        <RequestPayload>\r\n"
                        "            <StopEventRequest>\r\n"
                        "    <Location>\r\n"
                        "      <LocationRef>\r\n"
                        "      <StopPointRef>de:08212:409</StopPointRef>\r\n"
                        "      <LocationName>Karlsruhe Jägerhausstraße</LocationName>\r\n"
                        "      </LocationRef>\r\n"
                        "    </Location>\r\n"
                        "    <Params>\r\n"
                        "      <LineFilter>\r\n"
                        "        <Line>\r\n"
                        "          <LineRef>kvv:21004:E:H</LineRef>\r\n"
                        "          <DirectionRef>outward</DirectionRef>\r\n"
                        "        </Line>\r\n"
                        "      </LineFilter>\r\n"
                        "      <NumberOfResults>2</NumberOfResults>\r\n"
                        "      <IncludeRealtimeData>true</IncludeRealtimeData>\r\n"
                        "    </Params>\r\n"
                        "            </StopEventRequest>\r\n"
                        "        </RequestPayload>\r\n"
                        "    </ServiceRequest>\r\n"
                        "</Trias>";                        
#endif