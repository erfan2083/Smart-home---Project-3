#include <Arduino.h>

#include <WiFi.h> 
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>

  
// ------------------ WiFi Setup ------------------ 
const char* ssid = "Wokwi-GUEST";  // Enter SSID here 
const char* password = "";  //Enter Password here 

IPAddress local_ip(193, 186, 4, 40);
IPAddress gateway(193, 186, 4, 40);
IPAddress subnet(255, 255, 255, 0);
  
AsyncWebServer server(80);
AsyncEventSource events("/events");
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


 WiFi.begin(ssid, password, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "pages/smart-home.html", "text/html");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}