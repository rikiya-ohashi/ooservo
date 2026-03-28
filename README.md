# TekuteruServo (Serial Servo)

TekuteruServo is a high-performance serial servo motor designed as a drop-in replacement for standard hobby servos like the SG90. 

It features advanced capabilities including multi-turn positioning (±5.96 million rotations), ±1° angular accuracy, adjustable speeds up to 600 deg/s, and real-time position feedback. While maintaining the same physical dimensions and wiring as the SG90, it supports **the same programming methods as the standard Arduino Servo library**. 

**Note:** Although the programming logic is identical, the standard Arduino Servo library cannot be used with this hardware; you must use this dedicated TekuteruServo library. This library is **not compatible** with standard PWM hobby servos (e.g., SG90, MG996R).

The TekuteruServo hardware can be purchased here: [Buy TekuteruServo](https://tekuteru.handcrafted.jp/items/121327019)


## Features
* **High-Precision Multi-turn Positioning:** Supports ±5,965,232 full rotations (-2,147,483,647° to +2,147,483,648°) with ±1° accuracy.
* **Familiar Interface:** Includes `attach()` and `write()` methods, ensuring **API compatibility** with the standard Arduino Servo library.
* **Adjustable Dynamics:** Controlled rotation speeds (6–600 deg/s) and real-time angle feedback.
* **Dual-Mode Operation:** Supports both high-precision positioning (angle control) and continuous rotation (speed control).
* **Seamless Integration:** Uses the same wiring, form factor, and logic voltage (3.3V–5V) as the SG90.
* **Volatile Rotation Count:** While the absolute angle within a single turn (0–359°) is preserved, the multi-turn rotation count resets to zero upon power-up.


## Mechanical Specifications
* **Supply Voltage:** 5V
* **Logic Voltage:** 3.3V - 5V
* **Max Speed:** 600 deg/s (approx. 0.1s/60° or 100 rpm)
* **Angular Acceleration:** 5,000 deg/s²
* **Stall Torque:** 1.8 kgf·cm
* **Gear Material:** Plastic
* **Weight:** 15 g
* **Cable Length:** 20 cm


## Usage Notes & Limitations

### ⚠ Control & Connectivity
* **Pin Assignment:** Each I/O pin is designed to control one motor. However, multiple motors can be controlled via a single pin for "broadcast" commands that do not require feedback, such as `write()` (with `wait=false`), `stop()`, `setZero()`, and `setHold()`.
* **Hardware Interference:** Using hardware interrupts in your sketch may cause jitter. Avoid using the motor near strong magnetic fields or with unstable power sources.

### ⚠ Operational Risks
* **Stall & Impact:** Exceeding stall torque or applying strong impacts will **damage the plastic gears**.
* **Power Stability:** Ensure a stable power supply to prevent unstable or erratic motor behavior. Adding a large capacitor (e.g., 1000μF or higher) across the power lines can further improve stability, especially during high-torque movements.

### ⚠ Physical Handling
* **Cable Care:** **Internal connections** are fragile and may break if handled roughly.


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


## Class Methods

### `attach(pin)`
Attaches the servo to the specified pin.
- **`pin`**: `uint8_t`

---

### `write(angle)`
Rotates to the target angle at maximum speed (600 deg/s).  
Upon power-up, the initial position is initialized within the 0° to 359° range.
- **`angle`**: `int32_t` (Range: `-2,147,483,648` to `2,147,483,647`)

---

### `write(angle, speedValue)`
Rotates to the target angle at a specified speed value (0–255).
- **0**: Stop
- **1**: Minimum speed (6 deg/s)
- **255**: Maximum speed (600 deg/s)

To map a specific angular velocity (6–600 deg/s) to the speed argument:  
`speed = map(angularVelocity, 6, 600, 1, 255);`

| Speed Value (0–255) | Angular Velocity [deg/s] |
|---------------------|--------------------------|
| 0                   | 0 (Stop)                 |
| 1                   | 6 (Min)                  |
| 127                 | 303                      |
| 255                 | 600 (Max)                |

**Note on Velocity Accuracy:**
* **Speed Variance:** The actual rotation speed may vary by up to ±5% from the specified value. 
* **Timing Variance:** Due to this speed variance, the time taken to reach the target angle may differ from the theoretical calculation, especially during long-duration or multi-turn movements.
* **Precision Control:** For tasks requiring precise long-term speed control, it is recommended to periodically send (target positions) to compensate for any drift.


---

### `write(angle, speed, wait)`
Rotates to the target angle.
- **`angle`**: Target position in degrees (`int32_t`).
- **`speed`**: Rotation speed from `1` (6 deg/s) to `255` (600 deg/s). `0` stops the motor.
- **`wait`**: If `true`, the function blocks until the motor reaches the target position (within ±1°).

---

### `writeRotation(speed)`
Starts continuous rotation at a specified velocity. The motor will spin indefinitely until `stop()` or a new `write()` command is called.

`speed`: `int16_t` (Range: `-255` to `255`)

- **`1` to `255`**: Forward rotation (Counter-clockwise). `255` corresponds to approx. 600 deg/s.
- **`-1` to `-255`**: Reverse rotation (Clockwise).
- **`0`**: Stops the motor (equivalent to `stop()`).

**Note:**
- **Velocity:** The actual rotation speed for a given value is the same as described in `write(angle, speedValue)`.
- **Range Limit:** It cannot rotate beyond the range of `-2,147,483,648°` to `+2,147,483,647°`.

---

### `read()`
Returns the current angle in degrees (rounded to the nearest integer).  
Returns `-2,147,483,648` in case of a communication error.
- **Returns**: `int32_t`

---

### `stop()`
Immediately stops the servo at its current position.

---

### `wait()`
Blocks execution until the current movement is completed.

---

### `isMoving()`
Returns `true` if the servo is currently rotating. Returns `false` if stopped or if a communication error occurs.
- **Returns**: `bool`

---

### `setHold(hold)`
Configures holding behavior.  
- **`true` (Default)**: Actively maintains its position against external force.
- **`false`**: "Free move" state; allows manual rotation, though angular accuracy may decrease.

---

### `setZero()`
Sets the current absolute position (0–359°) as the 0° reference point. This is saved to non-volatile memory (EEPROM/Flash) and persists after power cycles. Ongoing rotations will stop when this is called.
**Note:** Only the absolute angle (0-359) is saved; the rotation count is reset.


## Code Examples

### 1. Basic Rotation
```
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
```
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
```

### 3. Wait for movement to complete
```
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
```

### 4. Read the current angle
```
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
```

### 5. Multiple servos
**Note:** When operating multiple servos simultaneously, using an external power supply is highly recommended to ensure stable operation and prevent voltage drops. **When using an external power supply, ensure that the GND of the power supply is connected to both the servos and the Arduino GND to maintain a common reference voltage.**

![Wiring Diagram](images/wiring_multiple.png)
```
#include <TekuteruServo.h>

TekuteruServo myservo1, myservo2;  // There is no software limit on the number of servos

void setup() {
  myservo1.attach(2);
  myservo2.attach(3);
}

void loop() {
  myservo1.write(180);
  myservo2.write(-90);
  myservo1.wait();  // Wait until myservo1 finishes rotating
  myservo2.wait();  // Wait until myservo2 finishes rotating

  myservo1.write(-90, 255, true);

  myservo2.write(180, 255, true);

  myservo1.write(-180, 255);
  myservo2.write(-180, 255, true);
  myservo1.wait();
}
```

### 6. Continuous Rotation
```
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
```

### 7. Set Zero
```
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
```


## Support & Feedback
* **Library Design:** Inspired by the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library.
* **Disclaimer:** TekuteruServo is an independent product and is not affiliated with Tower Pro.
* **Feedback:** This documentation was prepared with the help of translation tools. If you encounter any technical errors or have suggestions for improving the English expressions, please contact us at: abc@gmail.com