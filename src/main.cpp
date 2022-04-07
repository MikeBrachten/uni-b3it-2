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

uint8_t indexUI = 0;

deviceStateEnum previousState;

void setup() {
  // Initialize sensors, actuators and WiFi communication
  sensorsInit();
  actuatorsInit();
  communicationInit();

  // Boot screen
  showScreen(bootScreen);

  // Get initial value
  AMUX.updateValues();
}

void loop() {
  // Update the status LED
  State.ledUpdate();

  if (every1s.exec()) {
    // Update sensor values
    BMP280.updateValues();
    AMUX.updateValues();
  }

  switch (State.get()) {
    // Both Manual & Automatic
    case AUTOMATIC:
      if (AMUX.soil < 350) {
        Water.schedule(millis() + WATERFLOW_TIMEOUT);
        previousState = State.get();
        State.set(WATERING);
      }
    case MANUAL:
      // Toggle manual/automatic mode if flash button is pressed
      if (flashButtonPress()) {
        State.toggle();
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
            if (Water.scheduleTime) {
              indexUI = 2;
            }
            else {
              indexUI = 0;
            }
            break;
          case 2:
            showScreen(idleScreen2);
            indexUI = 0;
            break;
        }
      }
      break;
    case WATERING:
      if (every1s.exec()) {
        showScreen(watering, Water.scheduleTime);
      }
      if ((millis() - Water.scheduleTime) >= 0) {
        if (Water.flowing == false) {
            waterServo.write(175);
            Water.flowing = true;
        }
      }
      if ((millis() - Water.scheduleTime) >= WATERFLOW_DURATION) {
        if (Water.flowing) {
            waterServo.write(5);
            Water.flowing = false;
            State.set(previousState);
        }
      }
      break;
  }
}
