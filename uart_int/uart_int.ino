#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

// Include ESP32-specific headers for UART configuration
#include "driver/uart.h"
#include "soc/uart_struct.h"

// Uncomment the following line to enable debug prints
#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// Function Prototypes
void setupWiFi();
void setupMQTT();
void setupUART();
void spoofMACAddress();
void processUARTData();
void publishData(float speed, float distance, int doorStatus, float temperature);
void interpretUARTData(const uint8_t* data, float* speed, float* distance, int* doorStatus, float* temperature);

// Global Variables
WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonDocument sensorDataPayload(1024);
char sensorDataBuffer[1024];
volatile bool dataReady = false; // Flag to indicate when data is ready for processing

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  spoofMACAddress();    // Optionally change the MAC address
  setupWiFi();          // Connect to Wi-Fi
  setupMQTT();          // Initialize MQTT client
  setupUART();          // Configure UART for data reception
}

void loop() {
  // Ensure the MQTT client is connected
  if (!client.connected()) {
    while (!client.connected()) {
      DEBUG_PRINTLN("Attempting MQTT connection...");
      if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, NULL)) {
        DEBUG_PRINTLN("Connected to MQTT broker");
      } else {
        DEBUG_PRINT("Failed, rc=");
        DEBUG_PRINT(client.state());
        DEBUG_PRINTLN(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
  client.loop(); // Maintain MQTT connection

  // Process UART data if available
  if (dataReady) {
    processUARTData();
    dataReady = false; // Reset the flag
  }
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);             // Set Wi-Fi to station mode
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to the specified Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    DEBUG_PRINTLN("Connecting to WiFi...");
  }
  DEBUG_PRINTLN("Connected to WiFi");
  DEBUG_PRINT("ESP32 IP Address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}

void setupMQTT() {
  client.setServer(MQTT_BROKER, MQTT_PORT); // Set MQTT broker address and port
}

void setupUART() {
  const uart_config_t uartConfig = {
    .baud_rate = 115200,                // Set baud rate to 115200
    .data_bits = UART_DATA_8_BITS,      // 8 data bits
    .parity = UART_PARITY_DISABLE,      // No parity
    .stop_bits = UART_STOP_BITS_1,      // 1 stop bit
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE // No hardware flow control
  };
  uart_param_config(UART_NUM_1, &uartConfig); // Configure UART parameters
  uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // Set UART pins
  uart_driver_install(UART_NUM_1, RX_BUF_SIZE, 0, 0, NULL, 0); // Install UART driver

  // Attach interrupt to UART receive pin
  attachInterrupt(digitalPinToInterrupt(RXD_PIN), uartInterruptHandler, FALLING);
}

void spoofMACAddress() {
  #ifdef NEW_MAC_ADDRESS
    uint8_t newMac[6] = NEW_MAC_ADDRESS;
    esp_err_t result = esp_wifi_set_mac(WIFI_IF_STA, newMac);
    if (result == ESP_OK) {
      DEBUG_PRINT("MAC Address spoofed to: ");
      DEBUG_PRINTLN(WiFi.macAddress());
    } else {
      DEBUG_PRINTLN("Failed to spoof MAC Address");
    }
  #else
    DEBUG_PRINTLN("No MAC Address defined for spoofing.");
  #endif
}

void uartInterruptHandler() {
  // Set the flag to indicate that data is ready for processing
  dataReady = true;
}

void processUARTData() {
  const int expectedLength = 5; // Expected length of the UART data frame
  uint8_t data[expectedLength]; // Buffer to store received UART data

  // Read bytes from UART
  int len = uart_read_bytes(UART_NUM_1, data, expectedLength, 20 / portTICK_RATE_MS);
  if (len == expectedLength) {
    // Debug: Print the received data
    DEBUG_PRINT("Received UART data: ");
    for (int i = 0; i < expectedLength; i++) {
      DEBUG_PRINT(data[i]);
      DEBUG_PRINT(" ");
    }
    DEBUG_PRINTLN("");

    float speed = 0.0;
    float distance = 0.0;
    int doorStatus = 0;
    float temperature = 0.0;

    // Interpret the received UART data
    interpretUARTData(data, &speed, &distance, &doorStatus, &temperature);

    // Debug: Print interpreted values
    DEBUG_PRINT("Interpreted Speed: ");
    DEBUG_PRINTLN(speed);
    DEBUG_PRINT("Interpreted Distance: ");
    DEBUG_PRINTLN(distance);
    DEBUG_PRINT("Interpreted Door Status: ");
    DEBUG_PRINTLN(doorStatus);
    DEBUG_PRINT("Interpreted Temperature: ");
    DEBUG_PRINTLN(temperature);

    // Publish the interpreted data via MQTT
    publishData(speed, distance, doorStatus, temperature);
  } else {
    DEBUG_PRINTLN("Error: Incomplete UART data received.");
  }
}

void interpretUARTData(const uint8_t* data, float* speed, float* distance, int* doorStatus, float* temperature) {
  // Byte 0: Speed
  *speed = static_cast<float>(data[0]);

  // Bytes 1-2: Distance (16-bit value, big-endian)
  *distance = static_cast<float>((data[1] << 8) | data[2]);

  // Bytes 3-4: Temperature and Door Status
  // Extract the 12-bit temperature value (first 4 bits of byte 3 and all 8 bits of byte 4)
  uint16_t tempRaw = (data[3] << 8)  | (data[4] & 0xF0);
  tempRaw = tempRaw >> 4;
  *temperature = tempRaw * 0.625f;

  // Extract the door status (last 4 bits of byte 3)
  *doorStatus = data[3] & 0x01;
}

void publishData(float speed, float distance, int doorStatus, float temperature) {
  // Populate JSON document with sensor data
  sensorDataPayload["speed"] = speed;
  sensorDataPayload["distance"] = distance;
  sensorDataPayload["door_status"] = doorStatus;
  sensorDataPayload["temperature"] = temperature;

  // Serialize JSON document to buffer
  serializeJson(sensorDataPayload, sensorDataBuffer);

  // Publish serialized JSON to MQTT topic
  client.publish(MQTT_TOPIC_PUBLISH, sensorDataBuffer);
  DEBUG_PRINTLN("Data published to MQTT broker");
}
