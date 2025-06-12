#include <Arduino.h>

#include <WiFi.h> 
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

  
// ------------------ WiFi Setup ------------------ 
const char* ssid = "Alireza";  // Enter SSID here 
const char* password = "Er13496063#";  //Enter Password here 

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
  
AsyncWebServer server(80);
AsyncEventSource events("/events");
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<h1>Smart Home via Router</h1>");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}