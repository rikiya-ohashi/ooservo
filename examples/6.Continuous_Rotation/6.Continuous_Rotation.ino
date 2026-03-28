#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.writeRotation(100);  // Rotate forward at medium speed
  delay(3000);

  myservo.writeRotation(-255);  // Rotate backward at max speed
  delay(3000);

  myservo.writeRotation(0);  // Stop
  delay(2000);
}