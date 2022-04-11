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
const char* mqtt_user = "student091";
const char* mqtt_pass = "bZXjFr7W";
const char* mqtt_topic_prefix = "infob3it/091/";

WiFiClient espClient;
PubSubClient MQTT(espClient);

/**
 * General
 */

void communicationInit() {
  wifiManager.setAPCallback(noWifi);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("Feedr Setup");

  MQTT.setServer("mqtt.uu.nl", 1883);
  //MQTT.setCallback(function);
  while(!MQTT.connect(mqtt_clientid, mqtt_user, mqtt_pass)) {
    // Wait for MQTT Connection
  }
}

#endif