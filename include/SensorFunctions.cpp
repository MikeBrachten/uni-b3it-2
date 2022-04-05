/*
  Sensor functions
*/
#ifndef SensorFunctions_cpp
#define SensorFunctions_cpp

#include <Arduino.h>

/** 
 * BMP280 Temperature sensor
*/

#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

class BMP280Class {
    private:
        float temperature; // Celsius
        float pressure; // Pascal
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
 * General functions
 */

void sensorsInit() {
    BMP280.init();
}

#endif