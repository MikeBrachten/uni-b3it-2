/*
  Device state
  Global state and session handling functions
*/
#ifndef DeviceState_cpp
#define DeviceState_cpp

#include <Arduino.h>

/** Possible states of the device 
 * @enum {number}
*/
enum deviceStateEnum {
    MANUAL,
    AUTOMATIC
};

class DeviceState
{
private:
    bool stateChange = true;
    deviceStateEnum state = AUTOMATIC;
public:
    /** Returns the current state */
    deviceStateEnum get() {
        return state;
    }

    /** Returns true if state has been changed since last loop */
    bool changed() {
        return stateChange;
    }

    void toggle() {
        if (state == AUTOMATIC) {
            set(MANUAL);
        }
        else if (state == MANUAL) {
            set(AUTOMATIC);
        }
    }

    /** 
     * Change state of device
     * @param {deviceStateEnum} newState - State to which the device should be changed
     * */
    void set(deviceStateEnum newState) {
        digitalWrite(LED_BUILTIN, newState);
        state = newState;
        stateChange = true;
    }
};

class EventScheduler
{
private:
    const uint16_t interval;
    uint32_t lastTrigger;
public:
    EventScheduler(uint16_t a) : interval(a){ /* Constructor */ }

    /** Returns true if the event needs to trigger */
    bool exec() {
        if ((millis() - lastTrigger) >= interval) {
            lastTrigger = millis();
            return true;
        }
        else {
            return false;
        }
    }
};

DeviceState State;

#endif