#include <Arduino.h>
#include <DeviceState.cpp>
#include <DeviceConfig.cpp>
#include <SensorFunctions.cpp>
#include <ActuatorFunctions.cpp>
#include <CommunicationFunctions.cpp>

EventScheduler sensorValueTimer(2000);

bool manualMode = false;

void setup() {
  sensorsInit();
  actuatorsInit();
  communicationInit();

  pinMode(STATE_LED, OUTPUT);

  // Boot screen
  OLEDDisplay.clearDisplay();
  OLEDDisplay.drawBitmap(
    (OLEDDisplay.width()  - 112 ) / 2,
    (OLEDDisplay.height() - 40) / 2,
    feedroledlogo, 112, 40, 1);
  OLEDDisplay.display();
}

void loop() {
  switch (State.get()) {
    case AUTOMATIC:
      digitalWrite(STATE_LED, HIGH);
    case MANUAL:
      digitalWrite(STATE_LED, LOW);
  };

  if (sensorValueTimer.exec()) {
    BMP280.updateValues();
    OLEDDisplay.clearDisplay();

    OLEDDisplay.setTextColor(WHITE);

    /*
    OLEDDisplay.setTextSize(1);
    OLEDDisplay.setCursor(0,0);
    OLEDDisplay.println(F("Temperature"));

    OLEDDisplay.setTextSize(2);
    OLEDDisplay.print(BMP280.getTemperature());
    OLEDDisplay.println(" *C");

    OLEDDisplay.println();

    OLEDDisplay.setTextSize(1);
    OLEDDisplay.setCursor(0,0);
    OLEDDisplay.println(F("Pressure"));

    OLEDDisplay.setTextSize(2);
    OLEDDisplay.print(BMP280.getPressure());
    OLEDDisplay.println(" Pa");

    OLEDDisplay.println();
    */

   /*

    OLEDDisplay.setTextSize(1);
    OLEDDisplay.setCursor(0,0);
    OLEDDisplay.print(F("LDR"));
    OLEDDisplay.setCursor(64, 0);
    OLEDDisplay.println(F("SOIL"));


    OLEDDisplay.setTextSize(2);
    AMUX.select(LDR);
    OLEDDisplay.setCursor(0, 12);
    OLEDDisplay.print(AMUX.read());
    OLEDDisplay.setCursor(64, 12);
    AMUX.select(SOIL);
    OLEDDisplay.println(AMUX.read());
    AMUX.select(LDR);
    
    */

    OLEDDisplay.setTextSize(1);
    OLEDDisplay.setCursor(0,0);
    OLEDDisplay.println(F("STATE"));


    OLEDDisplay.setTextSize(2);
    OLEDDisplay.println(State.get());

    OLEDDisplay.display();
  }

  if (flashButtonPress()) {
    State.toggle();
  }
}

