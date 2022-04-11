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

void planWater() {
  // Schedule watering
  Water.schedule(millis() + WATERFLOW_TIMEOUT);

  // Remember previous state
  previousState = State.get();

  // Set watering state
  State.set(WATERING);
}

void flashButtonCallback() {
  // If flash button is pressed shortly, toggle state.
  State.toggle();
}

void setup() {
  // Initialize sensors, actuators
  sensorsInit();
  actuatorsInit();

  // Boot screen
  showScreen(bootScreen);

  // Initialize WiFi communication
  communicationInit();

  // Get initial value
  AMUX.updateValues();

  // Attach flash button, short press is toggle state, long is watering
  flashButton.onPressed(flashButtonCallback);
  flashButton.onPressedFor(4000, planWater);

  if (DEBUG) {Serial.begin(9600);}
}

void loop() {
  // Poll the button
  flashButton.read();

  // Update the status LED
  State.ledUpdate();

  if(every5s.exec()) {
    if (MQTT.connected()) {
      float pres = BMP280.getPressure();
      float temp = BMP280.getTemperature();
      MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/temperature", ((String)temp).c_str());
      MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/pressure", ((String)pres).c_str());
      MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/light", ((String)AMUX.ldr).c_str());
      MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/soil", ((String)AMUX.soil).c_str());
    }
  }

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
          waterServo.write(175);
          Water.flowing = true;
        }
        // If sufficient amount of water has been given
        if ((millis() - Water.scheduleTime) >= WATERFLOW_DURATION) {
          if (Water.flowing) {
            // Close water
            waterServo.write(5);
            Water.flowing = false;
            State.set(previousState);
          }
        }
      }
      break;
  }
}
