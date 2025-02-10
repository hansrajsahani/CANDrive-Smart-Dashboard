#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#define READ_PIN 23  
// Function Prototypes
void setupWiFi();
void setupMQTT();
void setupUART();
void processUARTData();
void publishData(uint8_t speed, uint16_t distance, uint8_t doorStatus, float temperature);
void interpretUARTData(const uint8_t* data, uint8_t* speed, uint16_t* distance, uint8_t* doorStatus, float* temperature);

// Global Variables
WiFiClient espClient; // WiFi client for network communication
PubSubClient client(espClient); // MQTT client using the WiFi client
DynamicJsonDocument sensorDataPayload(256); // JSON document for sensor data
char sensorDataBuffer[256]; // Buffer to hold serialized JSON data

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  setupWiFi(); // Connect to WiFi network
  setupMQTT(); // Set up MQTT client
  setupUART(); // Initialize UART communication
  pinMode(READ_PIN, INPUT);  // Set pin 23 as input

}

void loop() {
  // Ensure the MQTT client is connected
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Attempting MQTT connection...");
      // Attempt to connect to the MQTT broker
      if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, NULL)) {
        Serial.println("Connected to MQTT broker");
      } else {
        Serial.print("Failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000); // Wait before retrying
      }
    }
  }
  client.loop(); // Maintain MQTT connection
   if (digitalRead(READ_PIN) == HIGH) {
    processUARTData(); // Process incoming UART data
  }
}

void setupWiFi() {
  WiFi.mode(WIFI_STA); // Set WiFi to station mode
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi network
  // Wait until connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP()); // Display the IP address
}

void setupMQTT() {
  client.setServer(MQTT_BROKER, MQTT_PORT); // Configure MQTT broker settings
}

void setupUART() {
  // Initialize UART1 with specified baud rate and pins
  Serial1.begin(115200, SERIAL_8N1, RXD_PIN, TXD_PIN);
}

void processUARTData() {
  const int expectedLength = 5; // Expected length of the UART data frame
  uint8_t data[expectedLength]; // Buffer to store received UART data
  // Check if the expected number of bytes is available
  if (Serial1.available() >= expectedLength) {
    Serial1.readBytes(data, expectedLength); // Read bytes into buffer
    uint8_t speed = 0;
    long distance = 0;
    uint8_t doorStatus = 0;
    float temperature = 0.0;

    // Interpret the received UART data
    interpretUARTData(data, &speed, &distance, &doorStatus, &temperature);
    // Publish the interpreted data via MQTT
    publishData(speed, distance, doorStatus, temperature);
  }
}

void interpretUARTData(const uint8_t* data, uint8_t* speed, long* distance, uint8_t* doorStatus, float* temperature) {
  if (data == NULL) {
    return; // Exit if data is null
  }

  // Extract Speed from byte 0
  *speed = data[0];

  // Extract Distance from bytes 1 and 2
  *distance = (data[1] << 8) | data[2];

  // Extract Temperature from bytes 3 and 4 (first 12 bits)
 uint16_t tempRaw = (data[3] << 8)  | (data[4] & 0xF0);
  tempRaw = tempRaw >> 4;
  *temperature = tempRaw * 0.0625; // Convert to actual temperature

  // Extract Door Status from the least significant bit of byte 4
  *doorStatus = data[4] & 0x01;
}

void publishData(uint8_t speed, long distance, uint8_t doorStatus, float temperature) {
  // Populate JSON document with sensor data
  sensorDataPayload["speed"] = speed;
  sensorDataPayload["distance"] = distance;
  sensorDataPayload["door_status"] = doorStatus;
  sensorDataPayload["temperature"] = temperature;

  Serial.println("speed");
  Serial.println(speed);
  Serial.println("distance");
  Serial.println(distance);
  Serial.println("doorstatus");
  Serial.println(doorStatus);
    Serial.println("temp");
  Serial.println(temperature);
  // Serialize JSON document to buffer
  serializeJson(sensorDataPayload, sensorDataBuffer);
  // Publish serialized JSON data to MQTT topic
  client.publish(MQTT_TOPIC_PUBLISH, sensorDataBuffer);
  Serial.println("Data published to MQTT broker");
}
