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
    AUTOMATIC
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
    public:
        /** Returns whether a watering is scheduled soon */
        bool scheduled = false;

        /** Returns the time at which watering is/was scheduled */
        uint32_t scheduleTime = 0;

        /** Executes watering directly using servo */
        void exec() {
            if ((millis() - scheduleTime) >= 0) {
                waterServo.write(175);
            }
            if ((millis() - scheduleTime) >= duration) {
                waterServo.write(5);
                scheduled = false;
            }
        }

        /** Schedule a watering 
         * @param {uint32_t} time - Time at which the plant should be watered
        */
        void schedule(uint32_t time) {
            scheduleTime = time;
            scheduled = true;
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