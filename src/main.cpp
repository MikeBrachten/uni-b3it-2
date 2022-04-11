/**
 *
 *                        .-J:                                                                      
 *                  ..:^!?Y55J                                                                      
 *            .^!7?JY555555555~                                                                     
 *          ~?55555555555JY5557                                                                     
 *        :Y5555555555Y??Y55557            .?JJJJJJ:                               :BJ              
 *       .Y5555555J?!!7J555555^            .@B^~~~~.   ^~!!^      :~!!~.     .^!!~.:@5  :^ ^!.      
 *       ^5555J7^^~7J555555557             .&P       !GP?7?5B?  :5GJ77YG5.  ?#PJ?J5P@Y  P@P5Y:      
 *       .5J~..~?Y55555555557              .&#Y555: ^@G~^^^^5@! G@!^^^^7@P ?@7     ?@Y  P@~         
 *        ..^?5555555555P5Y~               .&P      ~@P7?????Y~ #&??????Y? Y@^     ~@Y  P&.         
 *       .!Y5Y55555555YJ7^                 .&G       Y#?^::!GY  !#5~::~5G^ .G#?^^~?G@Y  P@.         
 *      ~Y5?: :^^~~~^:.                    .J7        ^?JYYJ~    .7JYYJ7.    ~JYYJ!:J!  7J.         
 *    .?55!                                                                                         
 *    ?55Y                                                                                          
 *   .555Y                                                                                          
 *    7??7                                                                                          
 * 
 * Feedr Plant Feeder
 * Project for the Ineteraction Technology course of Utrecht University
 * 
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
EventScheduler every5s(5000);
EventScheduler every1s(1000);
EventScheduler sensorValueTimer(1000);

uint8_t indexUI = 0;
uint8_t secondsUI = 0;

deviceStateEnum previousState;

void flashButtonCallback() {
  State.toggle();
}

void setup() {
  // Initialize sensors, actuators and WiFi communication
  sensorsInit();
  actuatorsInit();
  communicationInit();

  // Boot screen
  showScreen(bootScreen);

  // Get initial value
  AMUX.updateValues();

  // Attach flash button
  flashButton.onPressed(flashButtonCallback);

  if (DEBUG) {Serial.begin(9600);}
}

void loop() {
  // Poll the button
  flashButton.read();

  // Update the status LED
  State.ledUpdate();

  if (sensorValueTimer.exec()) {
    // Update sensor values
    BMP280.updateValues();
    AMUX.updateValues();

    // DEBUG information
    if (DEBUG) {
      Serial.print("Time:" );
      Serial.println(millis());
      Serial.print("State: ");
      Serial.println(State.get());
      Serial.print("Water.scheduleTime:");
      Serial.println(Water.scheduleTime);
      Serial.print("millis() - Water.scheduleTime: ");
      Serial.println(millis() - Water.scheduleTime);
      Serial.print("timeElapsed: ");
      Serial.println((millis() - Water.scheduleTime) / 1000);
      Serial.print("LDR: ");
      Serial.println(AMUX.ldr);
      Serial.print("Soil: ");
      Serial.println(AMUX.soil);
      Serial.print("Temp: ");
      Serial.println(BMP280.getTemperature());
      Serial.print("Pressure: ");
      Serial.println(BMP280.getPressure());
      Serial.println();
      Serial.println();
    }
  }

  switch (State.get()) {
    // Both Manual & Automatic
    case AUTOMATIC:
      if (AMUX.soil < 350 && millis() >= (Water.scheduleTime + 60000)) {
        Water.schedule(millis() + WATERFLOW_TIMEOUT);
        previousState = State.get();
        State.set(WATERING);
      }
    case MANUAL:
      if (every1s.exec()) {
        secondsUI++;
        // Update OLED Screen values
        switch (indexUI) {
          case 0:
            showScreen(idleScreen0);
            if (secondsUI >= 5) {
              indexUI = 1;
              secondsUI = 0;
            }
            break;
          case 1:
            showScreen(idleScreen1);
            if (Water.scheduleTime) {
              if (secondsUI >= 5) {
                indexUI = 2;
                secondsUI = 0;
              }
            }
            else {
              if (secondsUI >= 5) {
                indexUI = 0;
                secondsUI = 0;
              }
            }
            break;
          case 2:
            showScreen(idleScreen2, Water.scheduleTime);
            if (secondsUI >= 5) {
              indexUI = 0;
              secondsUI = 0;
            }
            break;
        }
      }
      break;
    case WATERING:
      if (every1s.exec()) {
        showScreen(watering, Water.scheduleTime);
      }
      if (millis() >= Water.scheduleTime) {
        if (Water.flowing == false) {
            waterServo.write(175);
            Water.flowing = true;
        }
        if ((millis() - Water.scheduleTime) >= WATERFLOW_DURATION) {
          if (Water.flowing) {
              waterServo.write(5);
              Water.flowing = false;
              State.set(previousState);
          }
        }
      }
      break;
  }
}
