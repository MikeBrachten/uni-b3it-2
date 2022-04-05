/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Arduino.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
    myservo.attach(D5);
}

void loop() {
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(1000);                       // waits 15 ms for the servo to reach the position
    myservo.write(180);              // tell servo to go to position in variable 'pos'
    delay(2000);                       // waits 15 ms for the servo to reach the position
}