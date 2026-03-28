#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180);  // Move to 180 degrees
  delay(3000);

  myservo.write(-180);  // Move to -180 degrees
  delay(3000);

  myservo.write(540);  // Move to 540 degrees
  delay(3000);
}