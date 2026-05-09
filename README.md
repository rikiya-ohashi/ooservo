# TekuteruServo

TekuteruServo is a serial servo motor that feels just like a standard SG90 but offers virtually unlimited rotation and precise position control.

Key features include multi-turn positioning (±5.96 million rotations), ±1° angular accuracy, and real-time position feedback. It maintains the same physical dimensions, wiring, and programming interface as the standard Arduino Servo library.

**The TekuteruServo hardware can be purchased here:** [**Buy TekuteruServo**](https://tekuteru.handcrafted.jp/items/121327019)

For questions about this library, you can chat with an AI assistant via [Notebook LM](https://notebooklm.google.com/notebook/272725f0-6a1c-4c52-9597-6384a2f88f91).


> **⚠ Important Compatibility Note:**
> This library uses a dedicated serial protocol. It is **not** a PWM-based servo library and is **incompatible** with standard servos (like the SG90). This incompatibility goes both ways — the standard Servo.h library cannot control TekuteruServo either.


## Table of Contents
- [Features](#features)
- [Mechanical Specifications](#mechanical-specifications)
- [Usage Notes](#-usage-notes)
- [Python Support (Raspberry Pi)](#python-support-raspberry-pi)
- [Wiring Guide](#wiring-guide)
- [Installation](#installation-arduino-ide)
- [Class Methods](#class-methods)
- [Code Examples](#code-examples)


## Features
* **High-Precision Multi-turn Positioning:** Supports up to ±5.96 million rotations (-2,147,483,648° to +2,147,483,647°) with ±1° accuracy.
* **Servo-Library-Compatible Interface:** Provides `attach()` and `write()` methods that are API-compatible with the standard Arduino Servo library.
* **Universal Compatibility:** Compatible with any digital I/O pin on a wide range of microcontrollers, including **Arduino, ESP32, Raspberry Pi Pico,** and more.
* **Adjustable Dynamics:** Controlled rotation speeds (1–900 deg/s) and real-time angle feedback.
* **Dual-Mode Operation:** Supports both high-precision positioning (angle control) and continuous rotation (speed control).
* **Scalable Servo Control:** No software limit on the number of servos. You can control as many as your board's available I/O pins allow.
* **Seamless Integration:** Uses the same wiring, form factor, and logic voltage (3.3V–5V) as the SG90.


## Mechanical Specifications
* **Operating Voltage:** 3.3V - 8.4V
  * **Note:** For 3.3V usage, see [Power & Signal Integrity](#3-power--signal-integrity).
* **Logic Voltage:** 3.3V - 5V
* **Max Speed:** 900 deg/s (approx. 0.067s/60° or 150 rpm) **at 8.4V**
* **Stall Torque:** 1.2 kgf·cm **at 8.4V**
* **Stall Current:** 800 mA
* **Communication Speed:** 9600 baud by default (adjustable up to 57600 baud)
* **Gear Material:** Stainless steel
* **Dimensions:** 31.8 x 12 x 30.1 mm
* **Weight:** 11 g
* **Lead Length:** 24 cm

### Performance Chart
| Supply Voltage | Max Speed (deg/s) | Max Speed (rpm) | Stall Torque |
| :--- | :--- | :--- | :--- |
| **3.3V** | 600 deg/s | 70 rpm | 0.5 kgf·cm |
| **5.0V** | 700 deg/s | 116 rpm | 0.8 kgf·cm |
| **7.4V** | 800 deg/s | 133 rpm | 1.0 kgf·cm |
| **8.4V** | 900 deg/s | 150 rpm | 1.2 kgf·cm |


## ⚠ Usage Notes

### 1. Startup & Calibration (Rotational Direction at Power-up)
At power-up, the servo detects its absolute position (0°-359°), but the multi-turn counter resets to zero. This can cause the motor to rotate in an unexpected direction if the home position is near the 0°/360° wraparound point.

* **The Rotation Issue:** If the motor is physically at 359° at startup and you command `write(0)`, the library targets "0° in the *current* cycle." To reach this, the motor will rotate **359° backward** instead of moving forward just 1°.
* **Recommended Solutions:**
    * **Calibration:** Use `setZero()` once to calibrate your physical home position to 0°. This is saved in non-volatile memory and persists after power cycles.
    * **Startup Logic:** Read the current position immediately after power-up and move the servo to the nearest target.
    ```arduino
    long currentPos = myservo.read();
    if (currentPos > 300) {
      myservo.write(360); // Move forward to 360° instead of back to 0°
    } else {
      myservo.write(0);
    }
    ```

### 2. Pin Assignment
The wiring configuration depends on whether you need feedback from the motor.

* **Single Motor per Pin:** Functions requiring feedback (e.g., `read()` or `wait()`) support only one motor per I/O pin.
* **Broadcast Control:** For commands that do not require feedback, multiple motors can be controlled via a single pin. This applies to:
  * `write()` (with `wait=false`)
  * `writeRotation()`
  * `stop()`
  * `setHold()`
  * `setZero()`
  * `setSerialSpeed()`

### 3. Communication Latency
TekuteruServo communicates at 9600 baud by default, which introduces some **response latency** compared to PWM-based servos or high-performance serial servos. For latency-sensitive applications, increasing the baud rate via `setSerialSpeed()` can help, though it may affect communication reliability.

### 4. Power & Signal Integrity
* **Power Supply:** The 3.3V output pins on boards like Arduino Uno or ESP32 often lack sufficient current capacity. Always use a stable external power source.
* **Noise Reduction:** Adding a large capacitor (e.g., 1000μF or higher) across the power lines can further improve stability.

### 5. Operational Constraints & Safety
* **No Interrupts:** Using hardware/software interrupts in your sketch may disrupt serial communication timing, leading to unexpected malfunctions.
* **Magnetic Interference:** Do not use the motor near strong magnetic fields (e.g., large magnets, high-power cables), as they may interfere with the internal magnetic encoder.
* **Physical Care:** The internal wiring is delicate; applying excessive force or tension may result in broken or damaged wires.


## Python Support (Raspberry Pi)
For users who want to control TekuteruServo with Python on a Raspberry Pi, see the dedicated Python library:
[TekuteruServo-Python](https://github.com/tekuteru/TekuteruServo-Python)


## Wiring Guide
TekuteruServo follows the standard SG90 wiring convention:

| Wire Color | Function | Connection      |
|------------|----------|-----------------|
| Brown      | GND      | Arduino GND     |
| Red        | VCC      | 5V Power Supply |
| Yellow     | Signal   | Arduino I/O pin |

![Wiring Diagram](images/wiring.png)


## Installation (Arduino IDE)
1. Open the **Arduino IDE**.
2. Open the **Library Manager**.
3. In the search bar, type **"TekuteruServo"**.
4. Select the latest version and click **Install**.


## Class Methods

### `attach(pin)`
Attaches the servo to the specified pin. You can attach a servo to any available digital I/O pin on your board.
- **`pin`**: `uint8_t`

---

### `write(angle)`
Rotates the servo to a specific target angle at maximum speed.
Upon power-up, the current position is mapped to the 0°–359° range. For details, see [Startup & Calibration](#1-startup--calibration-rotational-direction-at-power-up) in the Usage Notes.
- **`angle`**: `int32_t` (Range: `-2,147,483,648` to `2,147,483,647`)

---

### `write(angle, speed)`
Rotates to the target angle at a specified speed value (unit: **deg/s**).

- **`speed`**: Rotation speed in **deg/s** (`uint16_t`).
  - **`0`**: Stop
  - **`1`**: Minimum speed (1 deg/s)
  - **`Max Value`**: The maximum speed depends on the supply voltage as follows:

| Supply Voltage | Max `speed` Value | Maximum Speed |
| :--- | :--- | :--- |
| **3.3V** | **600** | 600 deg/s |
| **5.0V** | **700** | 700 deg/s |
| **7.4V** | **800** | 800 deg/s |
| **8.4V** | **900** | 900 deg/s |

**Note on Velocity Accuracy:**
* **Speed Variance:** The actual rotation speed may vary by up to ±5% from the specified value due to unit-to-unit variation.
* **Timing Variance:** Due to this variance and power supply stability, the time taken to reach the target angle may differ from theoretical calculations.
* **Precision Control:** For tasks requiring precise long-term speed control, it is recommended to periodically send target positions to compensate for any drift.

---

### `write(angle, speed, wait)`
Rotates to the target angle with a specified speed and blocking behavior.

- **`angle`**: Target position in degrees (`int32_t`).
- **`speed`**: Rotation speed in **deg/s** (`uint16_t`).
- **`wait`**: If `true`, the program blocks until the motor reaches within ±1° of the target position.

---

### `writeRotation(speed)`
Rotates the servo continuously at a specified speed (unit: **rpm**). The motor continues to spin until a new command is issued.

- **`speed`**: Rotation speed in **rpm** (`int16_t`).
  - **`1` to `Max`**: Forward (Counter-clockwise).
  - **`-1` to `-Max`**: Reverse (Clockwise).
  - **`0`**: Stops the motor (equivalent to `stop()`).

**Maximum RPM by Voltage:**
The maximum speed depends on the supply voltage as follows:

| Supply Voltage | Max `speed` Value | Max Speed |
| :--- | :--- | :--- |
| **3.3V** | **50** | 50 rpm |
| **5.0V** | **116** | 116 rpm |
| **7.4V** | **133** | 133 rpm |
| **8.4V** | **150** | 150 rpm |

**Note on Speed Stability:**
* **Range Limit:** The motor cannot rotate beyond the range of `-2,147,483,648°` to `+2,147,483,647°`.
* **Speed Variance:** The actual rotation speed may vary by up to ±5% from the specified value due to individual hardware variances.
* **Load Handling:** If an external load causes the speed to drop, the motor will not compensate by accelerating beyond the set speed.

---

### `read()`
Returns the current angle in degrees.
- **Returns**: `int32_t`
- **Error Handling**: Returns `-2,147,483,648` if a communication error occurs (i.e., no response from the motor within 50ms).

---

### `stop()`
Immediately stops the servo at its current position.

---

### `wait()`
Blocks program execution until the current movement is completed (until the motor reaches within ±1° of the target position).

---

### `isMoving()`
Returns `true` if the servo is currently rotating, and `false` if it is stopped.
- **Returns**: `bool`
- **Error Handling**: Also returns `false` if a communication error occurs (timeout after 50ms without a response).

---

### `setHold(hold)`
Sets whether the motor holds its position after reaching the target.
- **`true` (Default)**: **Active Hold.** The motor actively maintains its position and resists external forces after the movement is complete.
- **`false`**: **Passive Mode.** Releases holding torque, allowing the shaft to be turned freely by hand.

---

### `setZero()`
Sets the current absolute position (0°-359°) as the 0° reference point. This is saved to non-volatile memory (EEPROM/Flash) and persists after power cycles. Ongoing rotations will stop when this is called.
**Note:** Only the absolute angle (0°-359°) is saved; the rotation count is reset.

---

### `setSerialSpeed(baud)`
Sets the communication speed. This must be called after `attach()`. If you are using multiple servos, you must call this method for each servo instance. Ongoing rotations will stop when this is called.
The speed resets to **9600** when the motor's power is cycled.
- **`baud`**: `uint16_t` (Select from: `9600`, `19200`, `38400`, `57600`)
- **Note:** Increasing the baud rate may cause communication errors, particularly affecting the reliability of `read()` operations.


## Code Examples

### 1. Basic Rotation
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180);  // Move to 180 degrees
  delay(1000);

  myservo.write(-180);  // Move to -180 degrees
  delay(2000);

  myservo.write(720);  // Move to 720 degrees
  delay(3000);
}
```

### 2. Speed Control
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 600);   // Move to 180 degrees at 600 deg/s
  delay(1000);

  myservo.write(-180, 100);  // Move to -180 degrees at 100 deg/s
  delay(3000);
}
```

### 3. Wait for Movement to Complete
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  // Blocking: program waits here until the move is done
  myservo.write(180, 600, true);   // Move to 180 degrees, wait for completion (within ±1°)
  myservo.write(-180, 600, true);  // Move to -180 degrees, wait for completion (within ±1°)

  // Non-blocking: program continues while the motor is moving
  myservo.write(360, 600, false);  // Start moving to 360 degrees (same as calling myservo.write(360, 600);)
  delay(500);                      // Do other work here while the motor moves
  myservo.wait();                  // Then wait for completion (within ±1°)
}
```

### 4. Read the Current Angle
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

long currentAngle;  // Use long (int32_t) to match the return type of read()

void setup() {
  Serial.begin(9600);  // Start serial communication (set the serial monitor to 9600 baud)

  myservo.attach(2);

  currentAngle = myservo.read();  // Read the current angle (0 ≤ angle ≤ 359)
  Serial.println(currentAngle);   // Display on serial monitor
}

void loop() {
  myservo.write(360, 600, true);  // Move to 360 degrees, wait for completion
  currentAngle = myservo.read();  // Read the current angle (expected: 360±1)
  Serial.println(currentAngle);

  myservo.write(0, 600, true);    // Move to 0 degrees, wait for completion
  currentAngle = myservo.read();  // Read the current angle (expected: 0±1)
  Serial.println(currentAngle);

  myservo.write(3600);            // Start moving to 3600 degrees (non-blocking)
  delay(1000);
  currentAngle = myservo.read();  // Read angle mid-rotation (motor is still moving)
  Serial.println(currentAngle);
}
```

### 5. Multiple Servos
**Note:** When operating multiple servos simultaneously, using an external power supply is highly recommended to ensure stable operation and prevent voltage drops. **When using an external power supply, ensure the power supply GND is connected to both the servo GND and the Arduino GND to establish a common ground.**

![Wiring Diagram](images/wiring_multiple.png)
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo1, myservo2;  // There is no software limit on the number of servos

void setup() {
  myservo1.attach(2);
  myservo2.attach(3);
}

void loop() {
  // Move both servos simultaneously, then wait for both to finish
  myservo1.write(180);
  myservo2.write(180);
  myservo1.wait();  // Wait until myservo1 reaches 180°
  myservo2.wait();  // Wait until myservo2 reaches 180°

  // Move both simultaneously at the same speed; they finish at roughly the same time
  myservo1.write(-180, 600);
  myservo2.write(-180, 600);
  myservo1.wait();
  myservo2.wait();

  // Move both simultaneously at different speeds
  myservo1.write(0, 600, true);   // Fast: wait here until myservo1 finishes
  myservo2.write(0, 100, true);   // Slow: wait here until myservo2 finishes
}
```

### 6. Continuous Rotation
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.writeRotation(100);   // Rotate forward at 100 rpm
  delay(3000);

  myservo.writeRotation(-50);  // Rotate in reverse at 50 rpm
  delay(3000);

  myservo.writeRotation(0);    // Stop (same as calling stop())
  delay(2000);
}
```

### 7. Set Zero
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);

  myservo.attach(2);

  myservo.setHold(false);         // Disable hold torque — shaft can be rotated manually
  delay(3000);

  myservo.setZero();  // Set the current position as 0 degrees (saved to non-volatile memory)

  Serial.println("setZero successful");
}

void loop() {
}
```

### 8. Set Serial Speed
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
  myservo.setSerialSpeed(19200);  // Change communication speed to 19200 baud (options: 9600, 19200, 38400, 57600)
                                  // Note: resets to 9600 on power cycle
}

void loop() {
}
```


## Support & Feedback
* **Library Design:** Inspired by the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library.
* **Feedback:** If you find any errors or have suggestions, please reach out to tekuterute@gmail.com.
