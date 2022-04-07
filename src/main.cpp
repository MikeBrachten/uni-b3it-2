/**
 * 
 * COMMUNICATION FUNCTIONS
 * File containing all external communication functions (like WiFi)
 * 
 * Project: Feedr for the Ineteraction Technology course of Utrecht University
 * Authors: Mike Brachten and Abdelghaffar Abd
 * 
 **/


#include <Arduino.h>
#include <DeviceState.cpp>
#include <DeviceConfig.cpp>
#include <SensorFunctions.cpp>
#include <ActuatorFunctions.cpp>
#include <CommunicationFunctions.cpp>

// Set timers
EventScheduler everySecond(1000);

void setup() {
  Serial.begin(9600);

  // Initialize sensors, actuators and WiFi communication
  sensorsInit();
  actuatorsInit();
  communicationInit();

  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - 112 ) / 2,
    (display.height() - 40) / 2,
    feedroledlogo, 112, 40, 1);
  display.display();

  delay(2000);

  // Boot screen
  showScreen(bootScreen);

  delay(2000);
}

void loop() {
  // If watering will commence soon, display on screen
  if (Water.scheduled) {
    showScreen(watering, Water.scheduleTime);
  }

  // Toggle manual/automatic mode if flash button is pressed
  if (flashButtonPress()) {
    State.toggle();
  };

<<<<<<< Updated upstream
  // Update the onboard state LED
  State.ledUpdate();

  if (everySecond.exec()) {
=======
  switch (State.get()) {
    case AUTOMATIC:
      digitalWrite(STATE_LED, LOW);

      /* Bug: scherm werkt niet! 

      // If soil is low, water
      if (AMUX.soil() <= SOIL_TRESHOLD) {
        Water.schedule(5);
      }

      // If past schedule time and not yet watered, water now
      Water.exec();
      */

      break;
    case MANUAL:
      digitalWrite(STATE_LED, HIGH);
      break;
  };

  if (every5s.exec()) {
    // Update OLED Screen values
    switch (indexUI) {
      case 0:
        showScreen(idleScreen0);
        indexUI = 1;
        break;
      case 1:
        showScreen(idleScreen1);
        indexUI = 2;
        break;
      case 2:
        showScreen(idleScreen2);
        indexUI = 0;
        break;
    }
    display.display();
  }


  if (every1s.exec()) {
>>>>>>> Stashed changes
    // Update sensor values
    BMP280.updateValues();

    // Update OLED Screen values (if not watering soom)
    if (!Water.scheduled) {
      uint32_t seconds = millis() / 1000;
      if (seconds % 15 <= 5) {showScreen(idleScreen0);}
      else if (seconds % 15 <= 10) {showScreen(idleScreen1);}
      else if (seconds % 15 <= 15) {showScreen(idleScreen2, Water.scheduleTime);}
    }
  }
}