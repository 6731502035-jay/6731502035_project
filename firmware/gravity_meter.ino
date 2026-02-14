#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h> // Make sure to install ArduinoJson library

// =============================================================
// CONFIGURATION
// =============================================================
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Hardware Pins
const int POT_PIN = 34;    // Analog pin for Soil Moisture Sensor
const int PUMP_PIN = 2;    // GPIO 2 (Built-in LED often, or connect Relay here)

// Calibration
const int AIR_VALUE = 4095;   // Dry (Max ADC value)
const int WATER_VALUE = 1500; // Wet (Min ADC value)

WebServer server(80);

// Pump State
bool isPumping = false;
unsigned long pumpStartTime = 0;
const unsigned long PUMP_DURATION = 5000; // 5 seconds

// =============================================================
// FUNCTIONS
// =============================================================

int getMoisturePercentage() {
  int raw = analogRead(POT_PIN);
  // Map raw value to 0-100%
  // Constrain range to avoid negative numbers or >100
  int constrained = constrain(raw, WATER_VALUE, AIR_VALUE);
  int percentage = map(constrained, AIR_VALUE, WATER_VALUE, 0, 100); 
  return percentage;
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

void handleStatus() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  StaticJsonDocument<200> doc;
  doc["moisture"] = getMoisturePercentage();
  doc["pump"] = isPumping;
  doc["raw_adc"] = analogRead(POT_PIN); // Debug info

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleWater() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  if (!isPumping) {
    isPumping = true;
    pumpStartTime = millis();
    digitalWrite(PUMP_PIN, HIGH); // Turn ON (Assume Active HIGH)
    Serial.println("Pump Activated!");
    server.send(200, "application/json", "{\"status\":\"pumping\"}");
  } else {
    server.send(409, "application/json", "{\"status\":\"already_pumping\"}");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Pin Modes
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); // Off initially

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Define Routes
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/water", HTTP_POST, handleWater);
  
  // Handle CORS for all routes (Preflight)
  server.on("/status", HTTP_OPTIONS, handleOptions);
  server.on("/water", HTTP_OPTIONS, handleOptions);
  
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      handleOptions();
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();

  // Non-blocking timer for Pump
  if (isPumping && (millis() - pumpStartTime > PUMP_DURATION)) {
    isPumping = false;
    digitalWrite(PUMP_PIN, LOW); // Turn OFF
    Serial.println("Pump Deactivated.");
  }
  
  delay(10); // Small delay for stability
}
