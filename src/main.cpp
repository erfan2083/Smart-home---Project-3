#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <DHT.h>
#include <htmlPage.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ------------------------ Pin Definitions ------------------------
#define DHTPIN 21
#define DHTTYPE DHT22
#define PIR_LIVING 12
#define PIR_BEDROOM 13
#define PIR_BATHROOM 0
#define LDR_SENSOR 36

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

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char sunIcon [] PROGMEM = {
  0b00000100, 0b00000000,
  0b00000100, 0b00000000,
  0b10100101, 0b01000000,
  0b01011010, 0b10000000,
  0b00111111, 0b00000000,
  0b00111111, 0b00000000,
  0b01011010, 0b10000000,
  0b10100101, 0b01000000,
  0b00000100, 0b00000000,
  0b00000100, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char moonIcon [] PROGMEM = {
  0b00000111, 0b00000000,
  0b00011111, 0b10000000,
  0b00111111, 0b11000000,
  0b00111100, 0b00000000,
  0b01111000, 0b00000000,
  0b01110000, 0b00000000,
  0b01110000, 0b00000000,
  0b01110000, 0b00000000,
  0b01111000, 0b00000000,
  0b00111100, 0b00000000,
  0b00111111, 0b11000000,
  0b00011111, 0b10000000,
  0b00000111, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};


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
void TaskDisplay(void *param) {
  while (true) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Header
    display.setCursor(0, 0);
    display.println("EFN Smart Home");

    // Icons
    if (isNight())
      display.drawBitmap(108, 0, moonIcon, 16, 16, WHITE);
    else
      display.drawBitmap(108, 0, sunIcon, 16, 16, WHITE);

    // Living Room
    display.setCursor(0, 16);
    display.print("Living:");
    display.print(" L1:"); display.print(digitalRead(2));
    display.print(" L2:"); display.print(digitalRead(4));
    display.print(" L3:"); display.println(digitalRead(8));

    // Bedroom
    display.print("Bedroom:");
    display.print(" L1:"); display.print(digitalRead(7));
    display.print(" L2:"); display.println(digitalRead(32));

    // Bathroom & AC
    display.print("Bath PIR: "); display.println(digitalRead(0));
    display.print("AC: "); display.println(digitalRead(27));

    // Temperature / Humidity
    extern float temperature, humidity;
    display.printf("T:%.1fC H:%.1f%%\n", temperature, humidity);

    display.display();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskDHT(void *param) {
  while (true) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) temperature = t;
    if (!isnan(h)) humidity = h;
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

void TaskPIRLiving(void *param) {
  while (true) {
    pirLiving = digitalRead(PIR_LIVING);
    if (modeLiving) {
      if((digitalRead(LDR_SENSOR)) == HIGH){
        digitalWrite(LED_LIVING_1, pirLiving);
        livingLights[0] = pirLiving;
        digitalWrite(LED_LIVING_2, pirLiving);
        livingLights[1] = pirLiving;
        digitalWrite(LED_LIVING_3, pirLiving);
        livingLights[2] = pirLiving;
      }
    } else {
      digitalWrite(LED_LIVING_1, livingLights[0]);
      digitalWrite(LED_LIVING_2, livingLights[1]);
      digitalWrite(LED_LIVING_3, livingLights[2]);
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void TaskPIRBedroom(void *param) {
  while (true) {
    pirBedroom = digitalRead(PIR_BEDROOM);
    if (modeBedroom) {
      if((digitalRead(LDR_SENSOR)) == HIGH){
        digitalWrite(LED_BEDROOM_1, pirBedroom);
        bedroomLights[0] = pirBedroom;
        bedroomLights[1] = bedroomLights[0];
        digitalWrite(LED_BEDROOM_2, pirBedroom);
      } 
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
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskACAuto(void *param) {
  while (true) {
    if (modeAC) {
      if (temperature >= 27) acState = true;
      else if (temperature <= 22) acState = false;
    }
    digitalWrite(LED_AC, acState);
    vTaskDelay(1050 / portTICK_PERIOD_MS);
  }
}


// ------------------------ Setup ------------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIR_LIVING, INPUT);
  pinMode(PIR_BEDROOM, INPUT);
  pinMode(PIR_BATHROOM, INPUT);
  pinMode(LDR_SENSOR, INPUT);
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

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    for(;;);
  }
  display.clearDisplay();
  display.display();

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
    else {
      Serial.println("no light params");
      req->send(400, "text/plain", "missing light params");
    } 

    req->send(200, "text/plain", "light done");
  });

  server.on("/api/ac/light", HTTP_GET, [](AsyncWebServerRequest *req){
    acState = !acState;
    digitalWrite(LED_AC, acState);
    
    req->send(200, "text/plain", "ac done");
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

  server.on("/api/auto/bathroom", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"state\": " + String(pirBathroom);
    json += "}";
    request->send(200, "application/json", json);
  });

  server.on("/api/auto/other", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("bedRoomMode") && request->hasParam("livingRoomLightMode") && request->hasParam("livingRoomAcMode")){
      modeBedroom = request->getParam("bedRoomMode")->value().toInt();
      modeLiving = request->getParam("livingRoomLightMode")->value().toInt();
      modeAC = request->getParam("livingRoomAcMode")->value().toInt();

      String json = "{";
      json += "\"livingRoom\": " + String((pirLiving == (digitalRead(LDR_SENSOR)) && (digitalRead(LDR_SENSOR)) == 1)) + ",";
      json += "\"bedRoom\": " + String((pirBedroom == (digitalRead(LDR_SENSOR)) && (digitalRead(LDR_SENSOR)) == 1)) + ",";
      json += "\"ac\": " + String(acState);
      json += "}";
      request->send(200, "application/json", json);
    }
    else {
      Serial.println("no auto params");
      request->send(400, "text/plain", "missing auto light params");
    } 
  });

  server.begin();


  xTaskCreate(TaskDHT, "DHT", 4096, NULL, 1, NULL);
  xTaskCreate(TaskPIRLiving, "PIR_Living", 4096, NULL, 1, NULL);
  xTaskCreate(TaskPIRBedroom, "PIR_Bedroom", 4096, NULL, 1, NULL);
  xTaskCreate(TaskPIRBathroom, "PIR_Bathroom", 4096, NULL, 1, NULL);
  xTaskCreate(TaskACAuto, "AC_Auto", 4096, NULL, 1, NULL);
  xTaskCreate(TaskDisplay, "OLED", 4096, NULL, 1, NULL);
  
  //vTaskStartScheduler();
}

void loop() {
  // Everything is RTOS-based
}