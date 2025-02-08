#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include "config.h"

// Function Prototypes
void setupWiFi();
void setupMQTT();
void setupUART();
void spoofMACAddress();
void processUARTData();
void publishData(float speed, float distance, int doorStatus, float temperature);

// Global Variables
WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonDocument sensorDataPayload(1024);
char sensorDataBuffer[1024];

void setup() {
  Serial.begin(115200);
  spoofMACAddress();
  setupWiFi();
  setupMQTT();
  setupUART();
}

void loop() {
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Attempting MQTT connection...");
      if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, NULL)) {
        Serial.println("Connected to MQTT broker");
      } else {
        Serial.print("Failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
  client.loop();
  processUARTData();
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupMQTT() {
  client.setServer(MQTT_BROKER, MQTT_PORT);
}

void setupUART() {
  const uart_config_t uartConfig = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM, &uartConfig);
  uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM, RX_BUF_SIZE, 0, 0, NULL, 0);
}

void spoofMACAddress() {
  #ifdef NEW_MAC_ADDRESS
    uint8_t newMac[6] = NEW_MAC_ADDRESS;
    esp_err_t result = esp_wifi_set_mac(WIFI_IF_STA, newMac);
    if (result == ESP_OK) {
      Serial.print("MAC Address spoofed to: ");
      Serial.println(WiFi.macAddress());
    } else {
      Serial.println("Failed to spoof MAC Address");
    }
  #else
    Serial.println("No MAC Address defined for spoofing.");
  #endif
}

void processUARTData() {
  uint8_t data[RX_BUF_SIZE];
  int len = uart_read_bytes(UART_NUM, data, RX_BUF_SIZE, 20 / portTICK_RATE_MS);
  if (len > 0) {
    // Interpret the received data to extract Speed, Distance, Door Status, and Temperature
    float speed = 0.0;       // Placeholder for extracted speed
    float distance = 0.0;    // Placeholder for extracted distance
    int doorStatus = 0;      // Placeholder for extracted door status
    float temperature = 0.0; // Placeholder for extracted temperature

    // TODO: Add logic to interpret 'data' and extract the above values

    publishData(speed, distance, doorStatus, temperature);
  }
}

void publishData(float speed, float distance, int doorStatus, float temperature) {
  sensorDataPayload["speed"] = speed;
  sensorDataPayload["distance"] = distance;
  sensorDataPayload["door_status"] = doorStatus;
  sensorDataPayload["temperature"] = temperature;

  serializeJson(sensorDataPayload, sensorDataBuffer);
  client.publish(MQTT_TOPIC_PUBLISH, sensorDataBuffer);
  Serial.println("Data published to MQTT broker");
}
