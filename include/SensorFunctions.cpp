/*
  Sensor functions
*/
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

/** Inputs of the AMUX Board 
 * @enum {number}
*/
enum AMUXInputEnum {
    LDR,
    SOIL
};

class AMUXBoard {
    private:
        AMUXInputEnum input = LDR;
    public:
        void init() {
            pinMode(AMUX_SEL_PIN, OUTPUT);
            digitalWrite(AMUX_SEL_PIN, LOW);
            pinMode(AMUX_AOUT_PIN, INPUT);
        }
        int read() {
            return analogRead(AMUX_AOUT_PIN);
        }
        void select(AMUXInputEnum input) {
            if (input == SOIL) {
                digitalWrite(AMUX_SEL_PIN, HIGH);
            }
            else {
                digitalWrite(AMUX_SEL_PIN, LOW);
            }
        }
};

AMUXBoard AMUX;

/**
 * Flash button
 */

uint32_t previousButtonPress;

bool flashButtonPress() {
    if (digitalRead(D3) == LOW && (millis() - previousButtonPress) < 500) {
        return true;
        previousButtonPress = millis();
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
}

#endif