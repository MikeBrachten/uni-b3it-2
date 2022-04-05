#include <Arduino.h>
#include <DeviceState.cpp>
#include <DeviceConfig.cpp>
#include <SensorFunctions.cpp>
#include <ActuatorFunctions.cpp>
#include <CommunicationFunctions.cpp>

EventScheduler screenUpdateTimer(4000);
EventScheduler sensorValueTimer(1000);

uint8_t indexUI = 0;

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
  //State.waterTime = 10000;
  //State.wateringMode = true;
}

void loop() {
  if (State.wateringMode) {
    if ((millis() - State.waterTime) > 0) {
      OLEDDisplay.clearDisplay();
      OLEDDisplay.setTextSize(2);
      OLEDDisplay.println("WATERING");
      OLEDDisplay.setTextSize(1);
      OLEDDisplay.print("Now");
      OLEDDisplay.display();
      waterServo.write(180);
      delay(5000);
      waterServo.write(0);
      State.wateringMode = false;
    }
    else {
      OLEDDisplay.clearDisplay();
      OLEDDisplay.setTextSize(2);
      OLEDDisplay.println("WATERING");
      OLEDDisplay.setTextSize(1);
      OLEDDisplay.print("Soon (");
      OLEDDisplay.print((millis() - State.waterTime) / 1000, 0);
      OLEDDisplay.println(" sec)");
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

  if (screenUpdateTimer.exec() && !State.wateringMode) {
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
        indexUI = 0;
        break;
    }
    OLEDDisplay.display();
  }

  if (sensorValueTimer.exec()) {
    BMP280.updateValues();
  }
}

