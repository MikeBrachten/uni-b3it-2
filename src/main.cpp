#include <Arduino.h>
#include <BMP280Sensor.cpp>

#include <OLED.cpp>

BMP280Class BMP280;

void setup() {
  BMP280.init();
  OLEDinit();
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - 112 ) / 2,
    (display.height() - 40) / 2,
    feedroledlogo, 112, 40, 1);
  display.display();
  delay(2000);
}

void loop() {
  BMP280.updateValues();
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Temperature"));

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(BMP280.getTemperature());

  display.display();
  delay(2000);
}

/*

#include <BMP280Sensor.cpp>

BMP280Class BMP280;

void setup() {
  Serial.begin(9600);
  BMP280.init();
}

void loop() {
  BMP280.updateValues();
  Serial.print("Temperature: ");
  Serial.println(BMP280.getTemperature());
  Serial.print("Pressure: ");
  Serial.println(BMP280.getPressure());
  delay(2000);
}

*/