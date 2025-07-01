#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <DHT.h>
#include <htmlPage.h>


// ------------------------ Pin Definitions ------------------------
#define DHTPIN 21
#define DHTTYPE DHT22
#define PIR_LIVING 12
#define PIR_BEDROOM 13
#define PIR_BATHROOM 0

#define LED_LIVING_1 2
#define LED_LIVING_2 4
#define LED_LIVING_3 8
#define LED_BEDROOM_1 7
#define LED_BEDROOM_2 32
#define LED_BATHROOM 33
#define LED_AC 27

// ------------------------ Global Variables ------------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

AsyncWebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

float temperature = 20.0;
float humidity = 40.0;

// Mode flags (manual = false, auto = true)
bool modeLiving = false;
bool modeBedroom = false;
bool modeAC = false;

// Manual state (only used if mode is manual)
bool livingLights[3] = {false, false, false};
bool bedroomLights[2] = {false, false};
bool acState = false;

// PIR values
bool pirLiving = false;
bool pirBedroom = false;
bool pirBathroom = false;
bool bathroomLight = false;

// ------------------------ RTOS Tasks ------------------------
void TaskDHT(void *param) {
  while (true) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) temperature = t;
    if (!isnan(h)) humidity = h;
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void TaskPIRLiving(void *param) {
  while (true) {
    pirLiving = digitalRead(PIR_LIVING);
    if (modeLiving) {
      digitalWrite(LED_LIVING_1, pirLiving);
      digitalWrite(LED_LIVING_2, pirLiving);
      digitalWrite(LED_LIVING_3, pirLiving);
    } else {
      digitalWrite(LED_LIVING_1, livingLights[0]);
      digitalWrite(LED_LIVING_2, livingLights[1]);
      digitalWrite(LED_LIVING_3, livingLights[2]);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void TaskPIRBedroom(void *param) {
  while (true) {
    pirBedroom = digitalRead(PIR_BEDROOM);
    if (modeBedroom) {
      digitalWrite(LED_BEDROOM_1, pirBedroom);
      digitalWrite(LED_BEDROOM_2, pirBedroom);
      Serial.println("LED are on");
    } else {
      digitalWrite(LED_BEDROOM_1, bedroomLights[0]);
      digitalWrite(LED_BEDROOM_2, bedroomLights[1]);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void TaskPIRBathroom(void *param) {
  while (true) {
    pirBathroom = digitalRead(PIR_BATHROOM);
    bathroomLight = pirBathroom;
    digitalWrite(LED_BATHROOM, bathroomLight);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void TaskACAuto(void *param) {
  while (true) {
    if (modeAC) {
      if (temperature >= 27) acState = true;
      else if (temperature <= 22) acState = false;
    }
    digitalWrite(LED_AC, acState);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ------------------------ Setup ------------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIR_LIVING, INPUT);
  pinMode(PIR_BEDROOM, INPUT);
  pinMode(PIR_BATHROOM, INPUT);
  pinMode(LED_LIVING_1, OUTPUT);
  pinMode(LED_LIVING_2, OUTPUT);
  pinMode(LED_LIVING_3, OUTPUT);
  pinMode(LED_BEDROOM_1, OUTPUT);
  pinMode(LED_BEDROOM_2, OUTPUT);
  pinMode(LED_BATHROOM, OUTPUT);
  pinMode(LED_AC, OUTPUT);

  dht.begin();

  WiFi.begin(ssid, password, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send(200, "text/html", smartHomeHTML);
  });

  server.on("/api/light", HTTP_GET, [](AsyncWebServerRequest *req){
    if (req->hasParam("id") ){
      String id = req->getParam("id")->value();

      if (id == "bedroom-light-1-btn"){
        bedroomLights[0] = !bedroomLights[0];
        digitalWrite(LED_BEDROOM_1, bedroomLights[0]);
      }
      if (id == "bedroom-light-2-btn") {
        bedroomLights[1] = !bedroomLights[1];
        digitalWrite(LED_BEDROOM_2, bedroomLights[1]);
      }
      if (id == "living-room-light-1-btn") {
        livingLights[0] = !livingLights[0];
        digitalWrite(LED_LIVING_1, livingLights[0]);
      }
      if (id == "living-room-light-2-btn") {
        livingLights[1] = !livingLights[1];
        digitalWrite(LED_LIVING_2, livingLights[1]);
      }
      if (id == "living-room-light-3-btn") {
        livingLights[2] = !livingLights[2];
        digitalWrite(LED_LIVING_3, livingLights[2]);
      }
    } 

    req->send(200, "text/plain", "light done");
  });

  server.on("/api/ac/light", HTTP_GET, [](AsyncWebServerRequest *req){
    acState = !acState;
    digitalWrite(LED_AC, acState);
    
    req->send(200, "text/plain", "light done");
  });

  server.on("/api/status/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  String json = "{";
  json += "\"temperature\": " + String(temperature);
  json += "}";
  request->send(200, "application/json", json);
});

server.on("/api/status/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
  String json = "{";
  json += "\"humidity\": " + String(humidity);
  json += "}";
  request->send(200, "application/json", json);
});

  // server.on("/api/control", HTTP_POST, [](AsyncWebServerRequest *req){
  //   if (req->hasParam("section", true) && req->hasParam("index", true) && req->hasParam("state", true)) {
  //     String section = req->getParam("section", true)->value();
  //     int index = req->getParam("index", true)->value().toInt();
  //     bool state = req->getParam("state", true)->value() == "on";
  //     if (section == "living") livingLights[index] = state;
  //     else if (section == "bedroom") bedroomLights[index] = state;
  //     else if (section == "ac") acState = state;
  //     req->send(200, "text/plain", "OK");
  //   } else {
  //     req->send(400, "text/plain", "Missing parameters");
  //   }
  // });

  // server.on("/api/mode", HTTP_POST, [](AsyncWebServerRequest *req){
  //   if (req->hasParam("target", true) && req->hasParam("state", true)) {
  //     String t = req->getParam("target", true)->value();
  //     bool state = req->getParam("state", true)->value() == "auto";
  //     if (t == "living") modeLiving = state;
  //     else if (t == "bedroom") modeBedroom = state;
  //     else if (t == "ac") modeAC = state;
  //     req->send(200, "text/plain", "Mode Set");
  //   } else {
  //     req->send(400, "text/plain", "Missing parameters");
  //   }
  // });

  server.begin();

  xTaskCreate(TaskDHT, "DHT", 2048, NULL, 1, NULL);
  xTaskCreate(TaskPIRLiving, "PIR_Living", 2048, NULL, 1, NULL);
  xTaskCreate(TaskPIRBedroom, "PIR_Bedroom", 2048, NULL, 1, NULL);
  xTaskCreate(TaskPIRBathroom, "PIR_Bathroom", 2048, NULL, 1, NULL);
  xTaskCreate(TaskACAuto, "AC_Auto", 2048, NULL, 1, NULL);

}

void loop() {
  // Everything is RTOS-based
}
