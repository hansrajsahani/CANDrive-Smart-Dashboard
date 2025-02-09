// config.h
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
const char* WIFI_SSID = "realme 7";
const char* WIFI_PASSWORD = "ajaysingh";

// MQTT Broker Configuration
//const char* MQTT_BROKER = "192.168.76.167";
const char* MQTT_BROKER = "mqtt.thingsboard.cloud";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "";

// MQTT Broker Credentials
const char* MQTT_USERNAME = "PJJRS5lGnjREqHLUKRpF";
const char* MQTT_PASSWORD = "";

// MQTT Topics
const char* MQTT_TOPIC_PUBLISH = "v1/devices/me/telemetry";
const char* MQTT_TOPIC_SUBSCRIBE = "cdac/desd/led/control";

// MAC Address for WiFi Spoofing
//const uint8_t NEW_MAC_ADDRESS[] = {0xc8, 0xb2, 0x9b, 0x70, 0x8a, 0xeb};

#endif
