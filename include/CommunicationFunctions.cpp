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

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ActuatorFunctions.cpp>

void noWifi(WiFiManager *myWiFiManager) {
  showScreen(bootScreenNoWifi);
}

void communicationInit() {
    WiFiManager wifiManager;
    wifiManager.setAPCallback(noWifi);
    wifiManager.setConfigPortalTimeout(180);
    wifiManager.autoConnect("Feedr Setup");
}

#endif