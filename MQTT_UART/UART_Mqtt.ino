#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include "config.h" 

DynamicJsonDocument sensor_data_payload(1024);
char sensor_data_format_for_mqtt_publish[1024];

int receivedNumber = 0;  // Variable to store the received number

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

long lastReconnectAttempt = 0;

// Function to handle reconnection to the MQTT broker
boolean reconnect() {
  if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println("Connected to MQTT broker");
  }
  return client.connected();
}

void setup() {
  Serial.begin(115200);  // Debugging via USB Serial
  Serial.println("ESP32 is ready. Please enter a number:");
  
  Serial.println("Attempting to connect to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Couldn't connect to WiFi.");
  }
  Serial.print("ESP32 IP ADDRESS: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(MQTT_BROKER, MQTT_PORT); // Connect to broker
  lastReconnectAttempt = 0;
}

void loop() {
  // Handle MQTT connection
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) { // Try to reconnect every 5 seconds
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
    Serial.println("Connected to Broker --- !!");
  }
  
  // Check if data is available in the UART buffer (waiting for user input)
  if (Serial.available()) {  
    char incomingChar = Serial.read();  // Read one character at a time
    Serial.print("Received: ");
    Serial.println(incomingChar);

    if (incomingChar >= '0' && incomingChar <= '9') {
      receivedNumber = receivedNumber * 10 + (incomingChar - '0');
    } 
    else if (incomingChar == '\n' || incomingChar == '\r') {
      Serial.print("You sent the number: ");
      Serial.println(receivedNumber);
      receivedNumber = 0; // Clear buffer
    }
  }

  // Generate random sensor data
  float humidity = random(0, 100);
  float temperature = random(-16, 56);

  // Populate the JSON payload with sensor data
  sensor_data_payload["temperature"] = temperature;
  sensor_data_payload["humidity"] = humidity;

  // Serialize JSON payload to a char array
  serializeJson(sensor_data_payload, sensor_data_format_for_mqtt_publish);

  Serial.println("Humidity: " + String(humidity));
  Serial.println("Temperature: " + String(temperature));
  client.publish(MQTT_TOPIC_PUBLISH, sensor_data_format_for_mqtt_publish);  // Publish sensor data to MQTT topic
  Serial.println("Sensor data sent to broker");
  delay(5000);
}
