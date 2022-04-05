#include <Arduino.h>
#include <DeviceState.cpp>
#include <DeviceConfig.cpp>
#include <SensorFunctions.cpp>
#include <ActuatorFunctions.cpp>
#include <CommunicationFunctions.cpp>

EventScheduler screenUpdateTimer(5000);
EventScheduler sensorValueTimer(1000);

uint8_t indexUI = 0;

uint32_t waterTime;
bool waterScheduled = false;

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
  if (Watering.scheduled) {
    if ((millis() - waterTime) > 0) {
      OLEDDisplay.clearDisplay();
      OLEDDisplay.setTextSize(2);
      OLEDDisplay.println("WATERING");
      OLEDDisplay.setTextSize(1);
      if ((millis() - Watering.scheduleTime) < 0) {
        OLEDDisplay.print((Watering.scheduleTime - millis()) / 1000, 0);
        OLEDDisplay.print(" s");
      }
      else {
        OLEDDisplay.print("In progress");
      }
      OLEDDisplay.display();
    }
  }
  if (flashButtonPress()) {
    State.toggle();
  };

  switch (State.get()) {
    case AUTOMATIC:
      digitalWrite(STATE_LED, LOW);
      break;
    case MANUAL:
      digitalWrite(STATE_LED, HIGH);
      break;
  };

  if (screenUpdateTimer.exec()) {
    OLEDDisplay.clearDisplay();
    OLEDDisplay.setTextColor(WHITE);
    switch (indexUI) {
      case 0:
        OLEDDisplay.setTextSize(1);
        OLEDDisplay.setCursor(0,0);
        OLEDDisplay.println(F("Temperature"));

        OLEDDisplay.setTextSize(2);
        OLEDDisplay.print(BMP280.getTemperature(), 1);
        OLEDDisplay.print(" ");
        OLEDDisplay.print((char)247);
        OLEDDisplay.println("C");

        OLEDDisplay.println();

        OLEDDisplay.setTextSize(1);
        OLEDDisplay.println(F("Pressure"));

        OLEDDisplay.setTextSize(2);
        OLEDDisplay.print(BMP280.getPressure()/100, 0);
        OLEDDisplay.println(" hPa");
        indexUI = 1;
        break;
      case 1:
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
        indexUI = 2;
        break;
      case 2:
        OLEDDisplay.setTextSize(1);
        OLEDDisplay.setCursor(0,0);
        OLEDDisplay.println(F("Last watering"));
        OLEDDisplay.setTextSize(2);
        uint32_t timeElapsed = (millis() - Watering.scheduleTime) / 1000;
        if (timeElapsed > 0) {
          if ((timeElapsed / 86400) >= 1.0) {
            OLEDDisplay.print((int)(timeElapsed / 86400), 0);
            OLEDDisplay.print("d ");
          }
          if ((timeElapsed / 3600) >= 1.0) {
            OLEDDisplay.print((int)(timeElapsed % 86400 / 3600), 0);
            OLEDDisplay.print("h ");
          }
          if ((timeElapsed / 60) >= 1.0) {
            OLEDDisplay.print((int)(timeElapsed % 3600 / 60), 0);
            OLEDDisplay.print("m ");
          }
          OLEDDisplay.print((int)(timeElapsed % 60), 0);
          OLEDDisplay.println("s ");
        }
        OLEDDisplay.println("ago");
        indexUI = 0;
        break;
    }
    OLEDDisplay.display();
  }

  if (sensorValueTimer.exec()) {
    BMP280.updateValues();
  }
}

