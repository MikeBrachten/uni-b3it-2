/**
 * 
 * COMMUNICATION FUNCTIONS
 * File containing all external communication functions (like WiFi)
 * 
 * Project: Feedr for the Ineteraction Technology course of Utrecht University
 * Authors: Mike Brachten and Abdelghaffar Abd
 * 
 **/

#ifndef CommunicationFunctions_cpp
#define CommunicationFunctions_cpp

#include <Arduino.h>

#include <ESP8266WiFi.h>

/**
 * WiFi
 */

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ActuatorFunctions.cpp>

void noWifi(WiFiManager *myWiFiManager) {
  showScreen(bootScreenNoWifi);
}

WiFiManager wifiManager;

/**
 * MQTT
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "mqtt.uu.nl";
const char* mqtt_clientid = "Freshr1";
const char* mqtt_user = "";
const char* mqtt_pass = "";

WiFiClient espClient;
PubSubClient MQTT(espClient);

/**
 * General
 */

// Connect the NodeMCU to internet using AP or saved settings
void connectWiFi() {
  wifiManager.setAPCallback(noWifi);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("Feedr Setup");
}

#include <ActuatorFunctions.cpp>

// Send updated sensor values to MQTT
void updateMQTT() {
  if (MQTT.connected()) {
    float pres = BMP280.getPressure();
    float temp = BMP280.getTemperature();
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/temperature", ((String)temp).c_str());
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/pressure", ((String)(pres/100)).c_str());
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/light", ((String)AMUX.ldr).c_str());
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/soil", ((String)AMUX.soil).c_str());
  }
}

// Handle MQTT messages
void mqttCallback(String topic, byte* payload, unsigned int length) {
  // Commands
  if (topic == "infob3it/091/DEN307/BG/LivingRoom/Yucca/commands") {
    if (payload[0] == 'w') {
      // Water now
      planWater();
    }
    else if (payload[0] == 'u') {
      // Update values
      updateValues();
      updateMQTT();
    }
  }
  // Mode switches
  else if (topic == "infob3it/091/DEN307/BG/LivingRoom/Yucca/mode") {
    if (payload[0] == 'a') {
      State.set(AUTOMATIC);
    }
    else if (payload[0] == 'm') {
      State.set(MANUAL);
    }
  }
}

// Connect to MQTT
void connectMQTT() {
  MQTT.setServer("mqtt.uu.nl", 1883);
  MQTT.setCallback(mqttCallback);
  while(!MQTT.connect(mqtt_clientid, mqtt_user, mqtt_pass, "infob3it/091/DEN307/BG/LivingRoom/Yucca/status", 1, true, "offline")) {
    // Wait for MQTT Connection
  }
  MQTT.subscribe("infob3it/091/DEN307/BG/LivingRoom/Yucca/commands", 0);
  MQTT.subscribe("infob3it/091/DEN307/BG/LivingRoom/Yucca/mode", 0);
  MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/status", "online", true);
  MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/mode", "a", true);
}

// Initiate WiFi and MQTT
void communicationInit() {
  // Connect to WiFi
  connectWiFi();

  // Connect to MQTT
  connectMQTT();
}

#endif