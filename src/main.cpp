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
 * 
 *              +---------------------------------------------------------------------+                          
 *              | Feedr Plant Feeder                                                  |
 *              | Project for the Interaction Technology course of Utrecht University |
 *              |                                                                     |
 *              | Authors: Mike Brachten and Abdelghaffar Abd                         |
 *              +---------------------------------------------------------------------+ 
 * 
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

void flashButtonCallback() {
  // If flash button is pressed shortly, toggle state.
  State.toggle();

  // Update state in MQTT upon changing
  if (MQTT.connected()) {
    if (State.get() == MANUAL) {
      MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/mode", "m", true);
    }
    else if (State.get() == AUTOMATIC) {
      MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/mode", "a", true);
    }
  }
}

void setup() {
  // Initialize sensors, actuators
  sensorsInit();
  actuatorsInit();

  // Boot screen
  showScreen(bootScreen);

  // Initialize WiFi and MQTT
  communicationInit();

  // Get initial values
  AMUX.updateValues();
  updateValues();

  // Attach flash button, short press is toggle state, long is watering
  flashButton.onPressed(flashButtonCallback);
  flashButton.onPressedFor(4000, planWater);
}

void loop() {
  // Poll the button
  flashButton.read();

  // Update the status LED
  State.ledUpdate();

  // MQTT Loop
  MQTT.loop();

  if(every5s.exec()) {
    // If connection lost, reconnect
    if(!MQTT.connected()) {
      connectMQTT();
    }

    updateMQTT();
  }

  // Update sensor values in time
  if (sensorValueTimer.exec()) {
    updateValues();
  }

  switch (State.get()) {
    // Both Manual & Automatic
    case AUTOMATIC:
      // If the soil is dry and last watering was longer than 30s ago
      if (AMUX.soil < 350 && millis() >= (Water.scheduleTime + 30000)) {
        planWater();
      }
    case MANUAL:
      if (every1s.exec()) {
        // Another second passed, register.
        secondsUI++;

        // Update OLED Screen values
        switch (indexUI) {
          case 0:
            // Show screen 0
            showScreen(idleScreen0);

            // On 5th second, go to next screen
            if (secondsUI >= 5) {
              indexUI = 1;
              secondsUI = 0;
            }
            break;
          case 1:
            showScreen(idleScreen1);

            // In case watered before, show last watering screen
            if (Water.scheduleTime) {
              if (secondsUI >= 5) {
                indexUI = 2;
                secondsUI = 0;
              }
            }
            // If not watered before, dont display last watering screen
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
        // Show/refresh watering screen
        showScreen(watering, Water.scheduleTime);
      }
      // If later than scheduled watering time
      if (millis() >= Water.scheduleTime) {
        // Water should flow now, open servo.
        if (Water.flowing == false) {
          waterServo.write(0);
          Water.flowing = true;
        }
        // If sufficient amount of water has been given
        if ((millis() - Water.scheduleTime) >= WATERFLOW_DURATION) {
          if (Water.flowing) {
            // Close water
            waterServo.write(180);
            Water.flowing = false;
            State.set(previousState);
          }
        }
      }
      break;
  }
}
