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

// Update sensor values
void updateValues() {
  BMP280.updateValues();
  AMUX.updateValues();
}

// Send updated sensor values to MQTT
void updateMQTT() {
  if (MQTT.connected()) {
    float pres = BMP280.getPressure();
    float temp = BMP280.getTemperature();
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/temperature", ((String)temp).c_str());
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/pressure", ((String)pres).c_str());
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/light", ((String)AMUX.ldr).c_str());
    MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/soil", ((String)AMUX.soil).c_str());
  }
}

// Connect to MQTT
void connectMQTT() {
  MQTT.setServer("mqtt.uu.nl", 1883);
  MQTT.setCallback(mqttCallback);
  while(!MQTT.connect(mqtt_clientid, mqtt_user, mqtt_pass, "infob3it/091/DEN307/BG/LivingRoom/Yucca/status", 1, true, "offline")) {
    // Wait for MQTT Connection
  }
  MQTT.subscribe("infob3it/091/DEN307/BG/LivingRoom/Yucca/commands", 0);
  MQTT.publish("infob3it/091/DEN307/BG/LivingRoom/Yucca/status", "online", true);
}

// Handle MQTT messages
void mqttCallback(String topic, byte* payload, unsigned int length) {
  // Commands
  if (topic == "infob3it/091/DEN307/BG/LivingRoom/Yucca/commands") {
    if (payload[0] == 'w') {
      // Water now
      planWater();
    }
    else if (payload[0] == 'u') {
      // Update values
      updateValues();
      updateMQTT();
    }
  }
  // Mode switches
  else if (topic == "infob3it/091/DEN307/BG/LivingRoom/Yucca/mode") {
    if (payload[0] == 'a') {
      if (State.get() == MANUAL) {
        State.set(AUTOMATIC);
      }
    }
    else if (payload[0] == 'm') {
      if (State.get() == AUTOMATIC) {
        State.set(MANUAL);
      }
    }
  }
}

void setup() {
  // Initialize sensors, actuators
  sensorsInit();
  actuatorsInit();

  // Boot screen
  showScreen(bootScreen);

  // These couldn't be in communicationInit() :(
  wifiManager.setAPCallback(noWifi);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("Feedr Setup");

  connectMQTT();

  // Get initial value
  AMUX.updateValues();

  // Attach flash button, short press is toggle state, long is watering
  flashButton.onPressed(flashButtonCallback);
  flashButton.onPressedFor(4000, planWater);

  if (DEBUG) {Serial.begin(9600);}
  Serial.begin(9600);
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
