/**
 * 
 * SENSOR FUNCTIONS
 * File containing functionality for all sensors within the system
 * 
 * Project: Feedr for the Ineteraction Technology course of Utrecht University
 * Authors: Mike Brachten and Abdelghaffar Abd
 * 
 **/

#ifndef SensorFunctions_cpp
#define SensorFunctions_cpp

#include <Arduino.h>
#include <DeviceConfig.cpp>

/** 
 * BMP280 Temperature sensor
*/

#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

class BMP280Class {
    private:
        float temperature;
        float pressure;
    public:
        void init() {
            bmp.begin(0x76, 0x58);
            bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X2, Adafruit_BMP280::SAMPLING_X16, Adafruit_BMP280::FILTER_X16, Adafruit_BMP280::STANDBY_MS_500);
        }
        void updateValues() {
            temperature = bmp.readTemperature();
            pressure = bmp.readPressure();
        }
        float getTemperature() {
            return temperature;
        }
        float getPressure() {
            return pressure;
        }
};

BMP280Class BMP280;

/**
 * AMUX, LDR and Soil
 */

class AMUXBoard {
    public:
        uint16_t ldr() {
            digitalWrite(AMUX_SEL_PIN, LOW);
            return analogRead(AMUX_AOUT_PIN);
        }
        uint16_t soil() {
            digitalWrite(AMUX_SEL_PIN, HIGH);
            return analogRead(AMUX_AOUT_PIN);
            digitalWrite(AMUX_SEL_PIN, LOW);
        };
        void init() {
            pinMode(AMUX_SEL_PIN, OUTPUT);
            digitalWrite(AMUX_SEL_PIN, LOW);
            pinMode(AMUX_AOUT_PIN, INPUT);
        }
};

AMUXBoard AMUX;

/**
 * Flash button
 */

uint32_t previousButtonPress = 0;

bool flashButtonPress() {
    if (digitalRead(FLASH_BUTTON) == LOW && (millis() - previousButtonPress) > 500) {
        previousButtonPress = millis();
        return true;
    }
    else {
        return false;
    }
}

/**
 * General functions
 */

void sensorsInit() {
    // BMP280 temperature sensor
    BMP280.init();

    // AMUX board - LDR / Soil
    AMUX.init();
    
    // Button
    pinMode(FLASH_BUTTON, INPUT_PULLUP);

    
}

#endif