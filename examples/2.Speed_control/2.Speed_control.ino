#include <TekuteruServo.h>

TekuteruServo myservo;

int angularVelocity;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 0);  // No rotation
  delay(3000);

  myservo.write(-180, 100);  // Move to -180 degrees with speed value 100
  delay(3000);

  myservo.write(540, 255);  // Move to 540 degrees with speed value 255 (max speed)
  delay(3000);


  angularVelocity = 300;
  myservo.write(-180, map(angularVelocity, 6, 600, 1, 255));  // Move to -180 degrees at approximately 300 [deg/s]
  delay(3000);

  angularVelocity = 600;
  myservo.write(540, map(angularVelocity, 6, 600, 1, 255));  // Move to 540 degrees at 600 [deg/s] (max speed)
  delay(3000);
}