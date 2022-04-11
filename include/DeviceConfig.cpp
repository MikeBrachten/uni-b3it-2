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
#define FLASH_BUTTON 0
#define STATE_LED LED_BUILTIN_AUX

#define WATERFLOW_DURATION 5000 //ms
#define WATERFLOW_TIMEOUT 5000 //ms

// Note that the DEBUG feature disables switching modes using the flash button
#define DEBUG false

#endif