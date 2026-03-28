#ifndef TekuteruServo_h
#define TekuteruServo_h
#include "Arduino.h"

class TekuteruServo {
public:
  TekuteruServo();

  // Initialize the servo on a specific pin
  void attach(uint8_t pin);

  // Move to a target angle
  void write(int32_t angle, uint8_t speed = 255, bool wait = false);

  // Read the current angle
  int32_t read();

  // 
  void writeRotation(int16_t speed);

  // Stop the motor immediately
  void stop();

  // Wait until the current movement is completed
  void wait();

  // Check if the motor is currently moving
  bool isMoving();

  // Enable or disable position holding
  void setHold(bool hold);

  // Reset the current position to zero
  void setZero();

private:
  uint8_t _pin;
  void sendByte(uint8_t data);
};

#endif