#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);

  myservo.attach(2);

  myservo.setHold(false);  // the servo will not hold its position

  myservo.setZero();  // Set the current angle to 0 degrees (Multiple rotations are not saved)

  Serial.println("setZero successful");
}

void loop() {
}