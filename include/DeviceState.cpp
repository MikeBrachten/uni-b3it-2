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
        bool waterOpen = false;
    public:
        /** Returns whether a watering is scheduled soon */
        bool scheduled = false;

        /** Returns the time at which watering is/was scheduled */
        uint32_t scheduleTime = 0;

        /** Executes watering routine using servo */
        void exec() {
            if ((millis() - scheduleTime) >= 0 && scheduled == true) {
                if (waterOpen == false) {
                    waterServo.write(175);
                    waterOpen = true;
                }
            }
            if ((millis() - scheduleTime) >= duration) {
                if (waterOpen == true) {
                    waterServo.write(5);
                    waterOpen = false;
                }
                scheduled = false;
            }
        }

        /** Schedule a watering 
         * @param {uint8_t} delaySec - How long from now watering should occur
        */
        void schedule(uint8_t delaySec) {
            scheduleTime = millis() + delaySec*1000;
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