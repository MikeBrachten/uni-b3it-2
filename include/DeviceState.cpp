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
    deviceStateEnum state = AUTOMATIC;
public:
    /** Returns the current state */
    deviceStateEnum get() {
        return state;
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
        state = newState;
    }
};

DeviceState State;

#include <ActuatorFunctions.cpp>

class WateringClass {
    private:
        uint16_t duration = 5000;
    public:
        bool scheduled;
        uint32_t scheduleTime = 0;
        void exec() {
            if ((millis() - scheduleTime) >= 0) {
                waterServo.write(175);
            }
            if ((millis() - scheduleTime) >= duration) {
                waterServo.write(5);
                scheduled = false;
            }
        }
        void schedule(uint32_t time) {
            scheduleTime = time;
            scheduled = true;
        }
};

WateringClass Watering;

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