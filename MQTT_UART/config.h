// config.h
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
const char* WIFI_SSID = "acts";
const char* WIFI_PASSWORD = "";

// MQTT Broker Configuration
//const char* MQTT_BROKER = "192.168.76.167";
const char* MQTT_BROKER = "demo.thingsboard.io";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "";
//mosquitto_pub -d -q 1 -h demo.thingsboard.io -p 1883 -t v1/devices/me/telemetry -u "AXNfCIOAJcIcHjTMmJPA" -m "{temperature:25}"
// MQTT Broker Credentials
const char* MQTT_USERNAME = "AXNfCIOAJcIcHjTMmJPA";
const char* MQTT_PASSWORD = "";

// MQTT Topics
const char* MQTT_TOPIC_PUBLISH = "v1/devices/me/telemetry";
const char* MQTT_TOPIC_SUBSCRIBE = "cdac/desd/led/control";

// MAC Address for WiFi Spoofing
const uint8_t NEW_MAC_ADDRESS[] = {0xc8, 0xb2, 0x9b, 0x70, 0x8a, 0xeb};

#endif
