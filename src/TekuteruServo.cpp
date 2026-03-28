#define CMD_START_BYTE     0xAA
#define CMD_WRITE          0x00
#define CMD_WRITE_WAIT     0x01
#define CMD_READ           0x02
#define CMD_STOP           0x03
#define CMD_IS_MOVING      0x04
#define CMD_SET_ZERO       0x05
#define CMD_SET_HOLD_FALSE 0x06
#define CMD_SET_HOLD_TRUE  0x07

#define BIT_DURATION_US    104  // 9600 baud
#define POS_ERROR_VALUE    -2147483648
#define COMM_TIMEOUT_MS    50

#include "TekuteruServo.h"

TekuteruServo::TekuteruServo():_pin(255) {}

void TekuteruServo::sendByte(uint8_t data) {
  noInterrupts();
  uint32_t start = micros();
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  while (micros() - start < BIT_DURATION_US);
  start += BIT_DURATION_US;
  for (uint8_t i = 0; i < 8; i++) {
    if (data & (1 << i)) {
      pinMode(_pin, INPUT);
    } else {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
    }
    while (micros() - start < BIT_DURATION_US);
    start += BIT_DURATION_US;
  }
  pinMode(_pin, INPUT);
  while (micros() - start < BIT_DURATION_US);
  interrupts();
}

void TekuteruServo::attach(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, INPUT);
}

void TekuteruServo::write(int32_t angle, uint8_t speed, bool wait) {
  if (_pin == 255) return;
  if (speed == 0) {
    stop();
    return;
  }
  uint8_t packet[6];
  packet[0] = wait ? CMD_WRITE_WAIT : CMD_WRITE;
  for (uint8_t i = 0; i < 4; i++) packet[i + 1] = ((uint32_t)angle >> (24 - i * 8)) & 0xFF;
  packet[5] = speed;
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < 6; i++) checksum += packet[i];
  sendByte(CMD_START_BYTE);
  for (uint8_t i = 0; i < 6; i++) sendByte(packet[i]);
  sendByte(~checksum);
  if (wait) {
    while (digitalRead(_pin));
    while (!digitalRead(_pin));
    delayMicroseconds(100);
  }
}

void TekuteruServo::writeRotation(int16_t speed) {
  speed = constrain(speed, -255, 255);
  if (speed > 0) {
    write(2147483647, speed);
  } else if (speed < 0) {
    write(-2147483648, -speed);
  } else {
    stop();
  }
}

int32_t TekuteruServo::read() {
  if (_pin == 255) return POS_ERROR_VALUE;
  sendByte(CMD_START_BYTE);
  sendByte(CMD_READ);
  uint32_t tempU32 = 0;
  for (uint8_t i = 0; i < 4; i++) {
    uint32_t timeoutStart = millis();
    while (digitalRead(_pin)) {
      if (millis() - timeoutStart > COMM_TIMEOUT_MS) {

        pinMode(13, OUTPUT);
        while (1) {
          digitalWrite(13, HIGH);
          delay(100);
          digitalWrite(13, LOW);
          delay(100);
        }

        return POS_ERROR_VALUE;
      }
    }
    noInterrupts();
    uint32_t start = micros();
    uint8_t receivedByte = 0;
    while (micros() - start < (BIT_DURATION_US + (BIT_DURATION_US / 2)));
    start += (BIT_DURATION_US + (BIT_DURATION_US / 2));
    for (uint8_t j = 0; j < 8; j++) {
      if (digitalRead(_pin)) receivedByte |= (1 << j);
      while (micros() - start < BIT_DURATION_US);
      start += BIT_DURATION_US;
    }
    interrupts();
    tempU32 |= ((uint32_t)receivedByte << (24 - i * 8));
  }
  delayMicroseconds(100 + BIT_DURATION_US);
  int32_t angle;
  memcpy(&angle, &tempU32, sizeof(angle));
  return angle;
}

void TekuteruServo::stop() {
  if (_pin == 255) return;
  sendByte(CMD_START_BYTE);
  sendByte(CMD_STOP);
}

void TekuteruServo::wait() {
  while (isMoving()) delay(1);
}

bool TekuteruServo::isMoving() {
  if (_pin == 255) return false;
  sendByte(CMD_START_BYTE);
  sendByte(CMD_IS_MOVING);
  uint32_t timeoutStart = millis();
  while (digitalRead(_pin)) {
    if (millis() - timeoutStart > COMM_TIMEOUT_MS) {

      pinMode(13, OUTPUT);
      while (1) {
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        delay(1000);
      }

      return false;
    }
  }
  noInterrupts();
  delayMicroseconds(BIT_DURATION_US + (BIT_DURATION_US / 2));
  bool moving = digitalRead(_pin);
  interrupts();
  delayMicroseconds(100 + (moving ? -1 : 1) * (BIT_DURATION_US / 2));
  return moving;
}

void TekuteruServo::setHold(bool hold) {
  if (_pin == 255) return;
  sendByte(CMD_START_BYTE);
  sendByte(hold ? CMD_SET_HOLD_TRUE : CMD_SET_HOLD_FALSE);
}

void TekuteruServo::setZero() {
  if (_pin == 255) return;
  sendByte(CMD_START_BYTE);
  sendByte(CMD_SET_ZERO);
  delay(5);
}