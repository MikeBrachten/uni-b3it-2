#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void OLEDinit() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
}