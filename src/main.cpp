#include <Arduino.h>
#include <DeviceState.cpp>
#include <DeviceConfig.cpp>
#include <SensorFunctions.cpp>
#include <ActuatorFunctions.cpp>
#include <CommunicationFunctions.cpp>

EventScheduler sensorValueTimer(2000);

bool manualMode = false;

void setup() {
  digitalWrite(LED_BUILTIN, State.get());
  pinMode(D3, INPUT_PULLUP);
  sensorsInit();
  actuatorsInit();
  communicationInit();
  OLEDDisplay.clearDisplay();
  OLEDDisplay.drawBitmap(
    (OLEDDisplay.width()  - 112 ) / 2,
    (OLEDDisplay.height() - 40) / 2,
    feedroledlogo, 112, 40, 1);
  OLEDDisplay.display();
}

void loop() {
  if (sensorValueTimer.exec()) {
    BMP280.updateValues();
    OLEDDisplay.clearDisplay();

    OLEDDisplay.setTextSize(1);
    OLEDDisplay.setTextColor(SSD1306_WHITE);
    OLEDDisplay.setCursor(0,0);
    OLEDDisplay.println(F("Temperature"));

    OLEDDisplay.setTextSize(2);
    OLEDDisplay.setTextColor(SSD1306_WHITE);
    OLEDDisplay.println(BMP280.getTemperature());

    OLEDDisplay.display();
  }
  if (flashButtonPress()) {
    State.toggle();
  }
}

