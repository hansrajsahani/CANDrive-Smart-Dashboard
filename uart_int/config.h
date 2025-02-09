#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
const char* WIFI_SSID = "realme 9 Pro+";
const char* WIFI_PASSWORD = "Code Red";

// MQTT Broker Configuration
const char* MQTT_BROKER = "mqtt.thingsboard.cloud";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_Client";
const char* MQTT_USERNAME = "HOyN3h8E1xMmfGw06dC7"; // Device token from ThingsBoard

// MQTT Topics
const char* MQTT_TOPIC_PUBLISH = "v1/devices/me/telemetry";

// MAC Address for WiFi Spoofing (Uncomment and set if needed)
// #define NEW_MAC_ADDRESS {0xC8, 0xB2, 0x9B, 0x70, 0x8A, 0xEB}

// UART Configuration
#define UART_NUM UART_NUM_1
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define RX_BUF_SIZE 1024

// UART Interrupt Configuration
#define UART_RX_BUF_SIZE 1024
#define UART_RX_PIN GPIO_NUM_16
#define UART_TX_PIN GPIO_NUM_17
#define UART_BAUD_RATE 115200

#endif // CONFIG_H
