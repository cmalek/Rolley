#include <NewPing.h>
#include <Servo.h>
#include <Bounce2.h>
#include <Rolley.h>
#include <LSM303.h>
#include <Wire.h>
#include <Rolley.h>

Servo servo;
NewPing sonar(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE);
rolley::Rolley robot = rolley::Rolley();

void setup() {
  robot.setup(&servo, &sonar);
  Serial.begin(9600);
}

void loop() {
  robot.sensor_test();
}
