#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char* ssid = "realme 9 Pro+";         // Replace with your WiFi SSID
const char* password = "Code Red"; // Replace with your WiFi password

// MQTT Broker details
const char* mqttBroker = "mqtt.thingsboard.cloud";
const int mqttPort = 1883;
const char* token = "HOyN3h8E1xMmfGw06dC7"; // Device token from ThingsBoard
const char* mqttTopic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublishTime = 0;
unsigned long lastDoorStatusChangeTime = 0;
int door_status = 0; // Door status: 0 (closed), 1 (open)

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  client.setServer(mqttBroker, mqttPort);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  // Change door status every 10 seconds
  if (currentMillis - lastDoorStatusChangeTime >= 10000) {
    door_status = random(0, 2); // Randomly set door status to 0 or 1
    lastDoorStatusChangeTime = currentMillis;
  }

  // Publish telemetry data every 5 seconds
  if (currentMillis - lastPublishTime >= 5000) {
    publishTelemetry();
    lastPublishTime = currentMillis;
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Client", token, NULL)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishTelemetry() {
  // Generate random telemetry data
  float speed = random(0, 121);       // Speed in km/h
  float distance = random(0, 501);    // Distance in km
  float temperature = random(20, 41); // Temperature in °C

  // Create JSON payload
  DynamicJsonDocument telemetry(256);
  telemetry["speed"] = speed;
  telemetry["distance"] = distance;
  telemetry["door_status"] = door_status;
  telemetry["temperature"] = temperature;

  char payload[256];
  serializeJson(telemetry, payload);

  // Publish telemetry data
  client.publish(mqttTopic, payload);
  Serial.println("Published telemetry: " + String(payload));
}
