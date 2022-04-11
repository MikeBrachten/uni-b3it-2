/**
 * 
 * ACTUATOR FUNCTIONS
 * File containing functionality for all actuators within the system
 * 
 * Project: Feedr for the Ineteraction Technology course of Utrecht University
 * Authors: Mike Brachten and Abdelghaffar Abd
 * 
 **/

#ifndef ActuatorFunctions_cpp
#define ActuatorFunctions_cpp

#include <Arduino.h>
#include <DeviceConfig.cpp>

/** 
 * OLED Screen
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SensorFunctions.cpp>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/** Bitmap of the Feedr logo */
const unsigned char feedroledlogo [] PROGMEM = {
	// 'feedroledlogo, 111x40px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xbf, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x7f, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x03, 0xff, 0xfe, 0x7f, 0x00, 0x00, 0x7f, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x03, 0xff, 0xfc, 0xfe, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x0c, 0x00, 0x07, 0xff, 0xf3, 0xfe, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x0c, 0x00, 0x07, 0xff, 0xc7, 0xfe, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
	0x07, 0xff, 0x0f, 0xfe, 0x00, 0x00, 0xe0, 0x01, 0xf8, 0x07, 0xf0, 0x0f, 0xcc, 0x76, 0x07, 0xfc, 
	0x3f, 0xfe, 0x00, 0x00, 0xe0, 0x03, 0xfe, 0x0f, 0xf8, 0x1f, 0xfc, 0x7e, 0x07, 0xf0, 0xff, 0xfc, 
	0x00, 0x00, 0xe0, 0x07, 0x0e, 0x1e, 0x1c, 0x38, 0x7c, 0x7c, 0x07, 0xc1, 0xff, 0xfc, 0x00, 0x00, 
	0xe0, 0x0e, 0x07, 0x18, 0x0e, 0x70, 0x1c, 0x70, 0x07, 0x07, 0xff, 0xf8, 0x00, 0x00, 0xff, 0x8c, 
	0x03, 0x38, 0x0e, 0x60, 0x1c, 0x70, 0x06, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0xff, 0x8f, 0xff, 0xbf, 
	0xfe, 0x60, 0x0c, 0x60, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0xe0, 0x0f, 0xff, 0xbf, 0xfe, 0x60, 
	0x0c, 0x60, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0xe0, 0x0f, 0xf8, 0x3b, 0xf0, 0x60, 0x0c, 0x60, 
	0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xe0, 0x0c, 0x00, 0x38, 0x00, 0x60, 0x1c, 0x60, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0xe0, 0x0e, 0x07, 0x18, 0x0e, 0x30, 0x1c, 0x60, 0x03, 0xff, 0xfe, 0x00, 
	0x00, 0x00, 0xe0, 0x07, 0x0f, 0x1e, 0x1c, 0x38, 0x7c, 0x60, 0x07, 0xbf, 0xf8, 0x00, 0x00, 0x00, 
	0xe0, 0x03, 0xfe, 0x0f, 0xf8, 0x1f, 0xfc, 0x60, 0x0f, 0x03, 0x80, 0x00, 0x00, 0x00, 0x60, 0x01, 
	0xfc, 0x07, 0xf0, 0x0f, 0xcc, 0x60, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** Possible screens the device can display 
 * @enum {number}
*/
enum screensEnum {
    bootScreen,
	watering,
	idleScreen0,
	idleScreen1,
	idleScreen2
};

#include <DeviceState.cpp>

uint32_t timeElapsed;

/** Change the screen being displayed
 * @param {screensEnum} screen - Screen to be displayed on the OLED screen
 * @param {uint32_t} waterScheduleTime - Time at which a watering is/was scheduled (optional)
*/
void showScreen(screensEnum screen, uint32_t waterScheduleTime = 0) {
	// Empty previous contents
	display.clearDisplay();
	display.setTextColor(WHITE);
	switch (screen) {
		// Boot screen with Feedr logo
		case bootScreen:
  			display.drawBitmap(
    			(display.width()  - 112 ) / 2,
    			(display.height() - 40) / 2,
    			feedroledlogo, 112, 40, 1);
			break;
		// Screen if watering is coming up or commencing
		case watering:
			display.setTextSize(2);
			display.setCursor(0,0);
			display.println("WATERING");
			display.setTextSize(1);
			if (millis() >= waterScheduleTime) {
				display.print("Now");
			}
			else {
				display.print("Soon");
			}
			break;
		// Idle screen with temperature and pressure values
		case idleScreen0:
			display.setTextSize(1);
			display.setCursor(0,0);
			display.println(F("Temperature"));
			display.setTextSize(2);
			display.print(BMP280.getTemperature(), 2);
			display.print(" ");
			display.print((char)247);
			display.println("C");
			display.println();
			display.setTextSize(1);
			display.println(F("Pressure"));
			display.setTextSize(2);
			display.print(BMP280.getPressure()/100, 0);
			display.println(" hPa");
			break;
		// Idle screen with LDR and SOIL sensors values
		case idleScreen1:
			display.setTextSize(1);
			display.setCursor(0,0);
			display.print(F("LDR"));
			display.setCursor(64, 0);
			display.println(F("SOIL"));
			display.setTextSize(2);
			display.setCursor(0, 12);
			display.print(AMUX.ldr);
			display.setCursor(64, 12);
			display.println(AMUX.soil);
			break;
		// Idle screen with last watering timer
		case idleScreen2:
			display.setTextSize(1);
			display.setCursor(0,0);
			display.println(F("Last watering"));
			display.setTextSize(2);
			timeElapsed = ((millis() - waterScheduleTime) / 1000);
			// Utilizes modulo functions and "/x > 1" to display time
			if (timeElapsed > 0) {
				// Days
				if ((timeElapsed / 86400) >= 1.0) {
					display.print((int)(timeElapsed / 86400), 0);
					display.print("d ");
				}
				// Hours
				if ((timeElapsed / 3600) >= 1.0) {
					display.print((int)(timeElapsed % 86400 / 3600), 0);
					display.print("h ");
				}
				// Minutes
				if ((timeElapsed / 60) >= 1.0) {
					display.print((int)(timeElapsed % 3600 / 60), 0);
					display.print("m ");
				}
				// Seconds
				display.print((int)(timeElapsed % 60), 0);
				display.println("s ");
			}
			display.println("ago");
			break;
	}
	// Display the contents on the sceeen
	display.display();
}

/**
 * Servo
 */

#include <Servo.h>

Servo waterServo;

/**
 * General functions
*/
void actuatorsInit() {
    // OLED
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
    }

    // Servo
    waterServo.attach(SERVO_PIN);
	waterServo.write(5);

	// Onboard status LED
	pinMode(STATE_LED, OUTPUT);
}

#endif