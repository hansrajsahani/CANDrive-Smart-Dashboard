#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "realme 9 pro+";   // Replace with your WiFi SSID
const char* password = "Code Red"; // Replace with your WiFi password

const char* mqttBroker = "mqtt.thingsboard.cloud";
const int mqttPort = 1883;
const char* clientID = "ESP32_Client";
const char* token = "HOyN3h8E1xMmfGw06dC7";  // Device token from ThingsBoard
const char* mqttTopic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);
long lastReconnectAttempt = 0;

boolean reconnect() {
  if (client.connect(clientID, token, NULL)) {
    Serial.println("Connected to ThingsBoard MQTT broker");
  }
  return client.connected();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttBroker, mqttPort);
  lastReconnectAttempt = 0;
}

void loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
    
    // Generate random telemetry data
    float temperature = random(20, 40);
    int door_status = random(0, 2);
    float speed = random(0, 120);
    float distance = random(0, 500);

    // Create JSON payload
    String payload = "{";
    payload += "\"temperature\":" + String(temperature) + ",";
    payload += "\"door_status\":" + String(door_status) + ",";
    payload += "\"speed\":" + String(speed) + ",";
    payload += "\"distance\":" + String(distance);
    payload += "}";

    Serial.println("Publishing telemetry: " + payload);
    
    // Publish telemetry data
    client.publish(mqttTopic, payload.c_str());
    delay(5000); // Publish every 5 seconds
  }
}
