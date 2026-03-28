#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 255, true);  // Move to 180 degrees, Wait for completion

  myservo.write(-180, 255, true);  // Move to -180 degrees, Wait for completion

  myservo.write(540, 255, false);  // Move to 540 degrees
  myservo.wait();                  // Wait until myservo finishes rotating
}