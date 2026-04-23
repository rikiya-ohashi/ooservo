# TekuteruServo (Serial Servo)

TekuteruServo is a serial servo motor that feels just like a standard SG90 but offers near-infinite rotation and precise position control.

Key features include multi-turn positioning (±5.96 million rotations), ±1° angular accuracy, adjustable speeds up to 600 deg/s, and real-time position feedback. While maintaining the same physical dimensions and wiring as the SG90, it supports the same programming methods as the standard Arduino Servo library.

> **⚠ Important Compatibility Note:** > This library uses a dedicated serial protocol. It is **not** a PWM-based servo library and is **incompatible** with standard servos (like the SG90). Conversely, the standard `Servo.h` library cannot be used to control TekuteruServo.

The TekuteruServo hardware can be purchased here: [Buy TekuteruServo](https://tekuteru.handcrafted.jp/items/121327019)


## Table of Contents
- [Features](#features)
- [Mechanical Specifications](#mechanical-specifications)
- [Usage Notes & Limitations](#-usage-notes--limitations)
- [Python Support (Raspberry Pi)](#python-support-raspberry-pi)
- [Wiring Guide](#wiring-guide)
- [Installation](#installation-arduino-ide)
- [Class Methods](#class-methods)
- [Code Examples](#code-examples)
- [Support & Feedback](#support--feedback)


## Features
* **High-Precision Multi-turn Positioning:** Supports up to ±5.96 million rotations (-2,147,483,647° to +2,147,483,648°) with ±1° accuracy.
* **Familiar Interface:** It provides `attach()` and `write()` methods, ensuring **API compatibility** with the standard Arduino Servo library.
* **Universal Compatibility:** Compatible with any digital I/O pin on a wide range of microcontrollers, including **Arduino, ESP32, Raspberry Pi Pico,** and more.
* **Adjustable Dynamics:** Controlled rotation speeds (6–600 deg/s) and real-time angle feedback.
* **Dual-Mode Operation:** Supports both high-precision positioning (angle control) and continuous rotation (speed control).
* **Scalable Servo Control:** No software limit on the number of servos. You can control as many as your board's available I/O pins allow.
* **Seamless Integration:** Uses the same wiring, form factor, and logic voltage (3.3V–5V) as the SG90.


## Mechanical Specifications
* **Operating Voltage:** 3.3V - 8.4V
  * **Note:** The 3.3V output pins on boards like **Arduino Uno** or **ESP32-DevKitC** often lack sufficient current capacity, which may lead to unstable operation.
* **Logic Voltage:** 3.3V - 5V
* **Max Speed:** 700 deg/s (approx. 0.1s/60° or 100 rpm) **at 5V**
* **Angular Acceleration:** 5,000 deg/s² **at 5V**
* **Stall Torque:** 0.8 kgf·cm **at 5V**
* **Stall Current:** 800 mA
* **Communication Speed:** 9600 baud by default (Adjustable up to 57600)
* **Gear Material:** Plastic
* **Dimensions:** 32.3 x 12 x 32 mm (Compatible with SG90 standard)
* **Weight:** 11 g
* **Cable Length:** 24 cm


## ⚠ Usage Notes & Limitations
* **Note on Power-up:** While the absolute position within a single turn (0-359°) is preserved, the multi-turn rotation counter resets to zero upon power cycle.
* **Pin Assignment:** Each I/O pin is designed to control one motor. However, multiple motors can be controlled via a single pin for "broadcast" commands that do not require feedback, such as `write()` (with `wait=false`), `writeRotation()`, `stop()`, `setHold()`, and `setZero()`.
* **Power Stability:** Ensure a stable power supply. **Insufficient current can lead to unexpected malfunctions or erratic behavior.** Adding a large capacitor (e.g., 1000μF or higher) across the power lines can further improve stability, especially during high-torque movements.
* **Timing & Interrupts:** * **Interrupts:** Using hardware/software interrupts in your sketch may disrupt serial communication timing, leading to **unexpected malfunctions or erratic behavior**.
* **Magnetic Fields:** Do not use the motor near strong magnetic fields (e.g., large magnets, high-power cables), as they may interfere with the internal magnetic encoder and cause positioning errors.
* **Handle with Care:** The internal wiring is delicate; applying excessive force or tension may cause internal damage or lead to wire breakage.

## Python Support (Raspberry Pi)
For users looking to control TekuteruServo using **Python on Raspberry Pi**, please refer to the dedicated Python library:
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
Upon power-up, the current position is recognized within the 0° to 359° range.
- **`angle`**: `int32_t` (Range: `-2,147,483,648` to `2,147,483,647`)

---

### `write(angle, speed[deg/s])`
Rotates to the target angle at a specified speed value (unit: **deg/s**).

- **`speed`**: Rotation speed in **deg/s** (`uint16_t`).
  - **`0`**: Stop
  - **`1`**: Minimum speed (1 deg/s)
  - **`Max Value`**: The maximum speed depends on the supply voltage as follows:

| Supply Voltage | Input Value for `speed` | Maximum Speed |
| :--- | :--- | :--- |
| **3.3V** | **300** | 300 deg/s |
| **5.0V** | **700** | 700 deg/s |
| **7.4V** | **800** | 800 deg/s |
| **8.4V** | **900** | 900 deg/s |

**Note on Velocity Accuracy:**
* **Speed Variance:** The actual rotation speed may vary by up to ±5% from the specified value due to individual hardware variances.
* **Timing Variance:** Due to this variance and power supply stability, the time taken to reach the target angle may differ from theoretical calculations.
* **Precision Control:** For tasks requiring precise long-term speed control, it is recommended to periodically send target positions to compensate for any drift.

---

### `write(angle, speed, wait)`
Rotates to the target angle with a specified speed and blocking behavior.

- **`angle`**: Target position in degrees (`int32_t`).
- **`speed`**: Rotation speed in **deg/s** (`uint16_t`).
- **`wait`**: If `true`, the program waits until the movement is complete (within ±1°).
---

### `writeRotation(speed[rpm])`
Starts continuous rotation at a specified velocity (unit: **rpm**). The motor continues to spin until a new command is called.

- **`speed`**: Rotation speed in **rpm** (`int16_t`).
  - **`1` to `Max`**: Forward (Counter-clockwise).
  - **`-1` to `-Max`**: Reverse (Clockwise).
  - **`0`**: Stops the motor (equivalent to `stop()`).

**Maximum RPM by Voltage:**
The maximum speed depends on the supply voltage as follows:

| Supply Voltage | Max Absolute Value | Max Speed |
| :--- | :--- | :--- |
| **3.3V** | **50** | 50 rpm |
| **5.0V** | **116** | 116 rpm |
| **7.4V** | **133** | 133 rpm |
| **8.4V** | **150** | 150 rpm |

**Note on Speed Stability:**
* **Range Limit:** It cannot rotate beyond the range of `-2,147,483,648°` to `+2,147,483,647°`.
* **Load Handling:** If an external load causes the speed to drop, the motor maintains the specified output but will not accelerate beyond the set speed to catch up. It ensures the motor never exceeds the defined velocity.

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
Configures the holding behavior after the motor reaches its target position.
- **`true` (Default)**: **Active Hold.** The motor actively maintains its position and resists external forces after the movement is complete.
- **`false`**: **Passive Mode.** Disables holding torque upon completion, allowing the shaft to be rotated manually

**Note on Manual Rotation:**
When `hold` is set to `false`, manually rotating the shaft may cause the internal multi-turn rotation count to lose synchronization. To maintain accurate position tracking during manual movement, you must **periodically call `read()`** to update the internal state.

---

### `setZero()`
Sets the current absolute position (0–359°) as the 0° reference point. This is saved to non-volatile memory (EEPROM/Flash) and persists after power cycles. Ongoing rotations will stop when this is called.
**Note:** Only the absolute angle (0-359) is saved; the rotation count is reset.

---

### `setSerialSpeed(baud)`
Sets the communication speed. This must be called after `attach()`. If you are using multiple servos, you must call this method for each servo instance.
The speed resets to **9600** when the motor's power is cycled.
- **`baud`**: `uint32_t` (Select from: `9600`, `19200`, `38400`, `57600`)
- **Note:** Increasing the baud rate may cause communication errors, leading to malfunctions or failure to operate.


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
  delay(3000);

  myservo.write(-180);  // Move to -180 degrees
  delay(3000);

  myservo.write(540);  // Move to 540 degrees
  delay(3000);
}
```

### 2. Speed control
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 0);  // No rotation
  delay(3000);

  myservo.write(-180, 600);  // Move to -180 degrees with 600 [deg/s]
  delay(3000);

  myservo.write(540, 100);  // Move to 540 degrees with 100 [deg/s]
  delay(3000);
}
```

### 3. Wait for movement to complete
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 600, true);  // Move to 180 degrees, Wait for completion (within ±1°)

  myservo.write(-180, 600, true);  // Move to -180 degrees, Wait for completion (within ±1°)

  myservo.write(540, 600, false);  // Move to 540 degrees
  myservo.wait();                  // Wait for completion (until the angle is within ±1°)
}
```

### 4. Read the current angle
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

long currentAngle;  // Declare it as a long(int32_t) type.

void setup() {
  Serial.begin(9600);  // Start serial communication (Set the serial monitor to 9600 baud.)

  myservo.attach(2);

  currentAngle = myservo.read();  // Read the current angle (0 ≤ angle < 360)
  Serial.println(currentAngle);   // Display on serial monitor
}

void loop() {
  myservo.write(360, 600, true);  // Move to 360 degrees, Wait for completion
  currentAngle = myservo.read();  // Read the current angle (360±1)
  Serial.println(currentAngle);

  myservo.write(0, 600, true);    // Move to 0 degrees, Wait for completion
  currentAngle = myservo.read();  // Read the current angle (0±1)
  Serial.println(currentAngle);

  myservo.write(3600);  // Move to 3600 degrees
  delay(1000);
  currentAngle = myservo.read();  // Read the current angle
  Serial.println(currentAngle);
}
```

### 5. Multiple servos
**Note:** When operating multiple servos simultaneously, using an external power supply is highly recommended to ensure stable operation and prevent voltage drops. **When using an external power supply, ensure that the GND of the power supply is connected to both the servos and the Arduino GND to maintain a common reference voltage.**

![Wiring Diagram](images/wiring_multiple.png)
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo1, myservo2;  // There is no software limit on the number of servos

void setup() {
  myservo1.attach(2);
  myservo2.attach(3);
}

void loop() {
  myservo1.write(180);
  myservo2.write(180);
  myservo1.wait();  // Wait until myservo1 finishes rotating
  myservo2.wait();  // Wait until myservo2 finishes rotating

  myservo1.write(-180, 600);
  myservo2.write(-180, 600, true);
  myservo1.wait();

  myservo1.write(0, 600, true);

  myservo2.write(0, 600, true);
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
  myservo.writeRotation(300);  // Rotate forward with 300 [rpm]
  delay(3000);

  myservo.writeRotation(-100);  // Rotate in reverse with 100 [rpm]
  delay(3000);

  myservo.writeRotation(0);  // Stop (Same as calling stop())
  delay(2000);
}
```

### 7. Set Serial Speed & Set Zero
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600); // Baud rate for the Arduino Serial Monitor

  myservo.attach(2);
  myservo.setSerialSpeed(19200); // Set the communication speed for myservo to 19200 baud. (Options: 9600, 19200, 38400, 57600)

  myservo.setHold(false);  // the servo will not hold its position

  myservo.setZero();  // Set the current angle to 0 degrees (Multiple rotations are not saved)

  Serial.println("setZero successful");
}

void loop() {
}
```


## Support & Feedback
* **Library Design:** Inspired by the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library.
* **Feedback:** This documentation was prepared with the help of translation tools. If you encounter any technical issues or have suggestions for improving the documentation or English expressions, please contact us at: tekuteruteku@gmail.com
