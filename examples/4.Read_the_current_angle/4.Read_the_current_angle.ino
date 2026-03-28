#include <TekuteruServo.h>

TekuteruServo myservo;

long currentAngle;  // Declare it as a long type.

void setup() {
  Serial.begin(9600);  // Start serial communication (Set the serial monitor to 9600 baud.)

  myservo.attach(2);

  currentAngle = myservo.read();  // Read the current angle (0 ≤ angle < 360)
  Serial.println(currentAngle);   // Display on serial monitor
}

void loop() {
  myservo.write(360, 255, true);  // Move to 360 degrees, Wait for completion
  currentAngle = myservo.read();  // Read the current angle (360±1)
  Serial.println(currentAngle);

  myservo.write(0, 255, true);    // Move to 0 degrees, Wait for completion
  currentAngle = myservo.read();  // Read the current angle (0±1)
  Serial.println(currentAngle);

  myservo.write(3600);  // Move to 3600 degrees
  delay(1000);
  currentAngle = myservo.read();  // Read the current angle
  Serial.println(currentAngle);
}