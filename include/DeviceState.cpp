/**
 * 
 * DEVICE STATE
 * Global state, device property and handling functions
 * 
 * Project: Feedr for the Ineteraction Technology course of Utrecht University
 * Authors: Mike Brachten and Abdelghaffar Abd
 * 
 **/

#ifndef DeviceState_cpp
#define DeviceState_cpp

#include <Arduino.h>
#include <DeviceConfig.cpp>

/** Possible states of the device 
 * @enum {number}
*/
enum deviceStateEnum {
    MANUAL,
    AUTOMATIC,
    WATERING
};

class DeviceState
{
private:
    deviceStateEnum state = AUTOMATIC;
public:
    /** Returns the current state */
    deviceStateEnum get() {
        return state;
    }

    /** Toggles the state between manual and automatic mode */
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
        state = newState;
    }

    /** Update the onboard LED according to device state */
    void ledUpdate() {
        switch (state) {
            case AUTOMATIC:
            case WATERING:
                digitalWrite(STATE_LED, LOW);
                break;
            case MANUAL:
                digitalWrite(STATE_LED, HIGH);
                break;
        };
    }
};

DeviceState State;

#include <ActuatorFunctions.cpp>

class WateringClass {
    private:
        uint16_t duration = 5000;
        deviceStateEnum previousState;
    public:
        bool flowing = false;
        /** Returns the time at which watering is/was scheduled */
        unsigned long scheduleTime;

        /** Schedule a watering 
         * @param {uint8_t} delaySec - Delay in seconds before watering begins
        */
        void schedule(long time) {
            scheduleTime = time;
        }
};

WateringClass Water;

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

#endif