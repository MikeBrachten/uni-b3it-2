/**
 * 
 * DEVICE CONFIGURATION
 * File for specifying all pins used within the system.
 * 
 * Project: Feedr for the Ineteraction Technology course of Utrecht University
 * Authors: Mike Brachten and Abdelghaffar Abd
 * 
 **/

#ifndef DeviceConfig_cpp
#define DeviceConfig_cpp

#define AMUX_SEL_PIN D4
#define AMUX_AOUT_PIN A0
#define I2C_SCL_PIN D1
#define I2C_SDA_PIN D2
#define SERVO_PIN D5
#define FLASH_BUTTON D3
#define STATE_LED LED_BUILTIN_AUX

#define SOIL_TRESHOLD 350   // Resistance AD converter value

#endif