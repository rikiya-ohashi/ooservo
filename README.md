# TekuteruServo

TekuteruServo is a serial servo motor that feels just like a standard SG90 but offers virtually unlimited rotation and precise position control.

It delivers multi-turn positioning (±5.96 million rotations) with ±1° angular accuracy and real-time position feedback, all while providing an interface compatible with the standard Arduino Servo library.

The internal firmware is completely open to customization — you can easily update the servo's program using just a single Arduino board.

> **⚠ Important Compatibility Note:**
> This library uses a dedicated serial protocol. It is **not** a PWM-based servo library and is **incompatible** with standard PWM servos. This incompatibility goes both ways — the standard Servo.h library cannot control TekuteruServo either.

**The TekuteruServo hardware can be purchased here:** [**Buy TekuteruServo**](https://tekuteru.handcrafted.jp/items/121327019)

For questions about TekuteruServo, you can chat with an **AI assistant** via [NotebookLM](https://notebooklm.google.com/notebook/272725f0-6a1c-4c52-9597-6384a2f88f91).


## Table of Contents
- [Features](#features)
- [Mechanical Specifications](#mechanical-specifications)
- [Python Support (Raspberry Pi)](#python-support-raspberry-pi)
- [Wiring Guide](#wiring-guide)
- [Installation (Arduino IDE)](#installation-arduino-ide)
- [Class Methods](#class-methods)
- [Usage Notes](#usage-notes)
- [Code Examples](#code-examples)
- [Firmware Customization](#firmware-customization)
- [Support & Feedback](#support--feedback)


## Features
* **High-Precision Multi-Turn Positioning:** Supports ±5.96 million rotations (-2,147,483,647° to +2,147,483,647°) with ±1° accuracy.
* **Dual-Mode Operation:** Supports both angle control and continuous rotation (speed control).
* **Adjustable Speed:** Rotation speed is adjustable from 1 to 950 deg/s.
* **Real-Time Position Feedback:** Returns the current angle at any time via `read()`.
* **Seamless Integration:** Uses the same wiring, form factor, and logic voltage (3.3V-5V) as the SG90.
* **Servo-Library-Compatible Interface:** Provides `attach()` and `write()` methods that are API-compatible with the standard Arduino Servo library.
* **Universal Compatibility:** Compatible with any digital I/O pin on a wide range of microcontrollers, including Arduino, ESP32, Raspberry Pi Pico, and more.
* **Scalable Servo Control:** No arbitrary software limit on the number of servos. The practical limit is determined by your board's available I/O pins and RAM.
* **Open & Reprogrammable:** Supports direct firmware flashing using a standard Arduino board as a programmer — no dedicated hardware tools required.


## Mechanical Specifications
* **Operating Voltage:** 4.0V-8.4V
* **Logic Voltage:** 3.3V-5V
* **Max Speed:** 950 deg/s (approx. 0.063 s/60° or 158 rpm) **at 8.4V**
* **Stall Torque:** 5.0 kgf·cm **at 8.4V**
* **Stall Current:** 800 mA
* **Communication Speed:** 9600 baud by default (adjustable up to 57600 baud)
* **Gear Material:** Stainless steel
* **Dimensions:** 31.8 x 12 x 30.1 mm
* **Weight:** 11 g
* **Lead Length:** 24 cm

### Performance Chart
| Supply Voltage | Max Speed ||| Stall Torque |
| :--- | ---: | ---: | ---: | ---: |
| **5.0V** | 650 deg/s | 108 rpm | 0.0923 s/60° | 2.0 kgf·cm |
| **6.0V** | 730 deg/s | 121 rpm | 0.0822 s/60° | 3.0 kgf·cm |
| **7.4V** | 860 deg/s | 143 rpm | 0.0698 s/60° | 4.0 kgf·cm |
| **8.4V** | 950 deg/s | 158 rpm | 0.0632 s/60° | 5.0 kgf·cm |


## Python Support (Raspberry Pi)
For users who want to control TekuteruServo with Python on a Raspberry Pi, see the dedicated Python library:
[TekuteruServo-Python](https://github.com/tekuteru/TekuteruServo-Python)


## Wiring Guide
TekuteruServo uses the same physical wiring as standard PWM servos:

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
The return value can be used to confirm whether the attachment was successful.
- **`pin`**: `uint8_t`
- **Returns**: `bool` — `true` if the servo is connected and responding, `false` if a communication error occurs.

> **Note:** You must call `attach()` before using any of the other methods below.

---

### `write(angle)`
Rotates the servo to a specific target angle at maximum speed (non-blocking).
Upon power-up, the current position is mapped to the 0°-359° range. For details, see [Startup & Calibration](#2-startup--calibration-rotational-direction-at-power-up) in the Usage Notes.
- **`angle`**: `int32_t` (Range: `-2,147,483,647` to `2,147,483,647`)

---

### `write(angle, speed)`
Rotates to the target angle at a specified speed (unit: **deg/s**).

- **`speed`**: Rotation speed in **deg/s** (`uint16_t`).
  - **`0`**: Stop
  - **`1`**: Minimum speed (1 deg/s)
  - **`Max Value`**: The maximum speed depends on the supply voltage as follows:

| Supply Voltage | Max `speed` Value | Maximum Speed |
| :--- | :--- | :--- |
| **5.0V** | **650** | 650 deg/s |
| **6.0V** | **730** | 730 deg/s |
| **7.4V** | **860** | 860 deg/s |
| **8.4V** | **950** | 950 deg/s |

**Speed Behavior Notes:**
* **Load Handling:** If external load slows the motor down during rotation, it will accelerate afterward to make up for the delay, so the target angle is still reached on schedule.
* **Speed Variance:** The actual rotation speed may vary by up to ±5% from the specified value due to unit-to-unit variation.
* **Low-Speed Smoothness:** At low speeds under no load, rotation may become irregular or jerky.

---

### `write(angle, speed, wait)`
Rotates to the target angle with a specified speed and blocking behavior.

- **`angle`**: Target position in degrees.
- **`speed`**: Rotation speed in **deg/s**.
- **`wait`** (`bool`): If `true`, the program blocks until the motor reaches within ±1° of the target position.

---

### `writeRotation(speed)`
Rotates the servo continuously at a specified speed (unit: **rpm**). The motor continues to spin until a new command is issued.

**Note:** The motor cannot rotate outside the range of `-2,147,483,647°` to `+2,147,483,647°`.

- **`speed`**: Rotation speed in **rpm** (`int16_t`).
  - **`1` to `Max`**: Forward (Counterclockwise).
  - **`-1` to `-Max`**: Reverse (Clockwise).
  - **`0`**: Stops the motor (equivalent to `stop()`).

**Maximum RPM by Voltage:**
The maximum speed depends on the supply voltage as follows:

| Supply Voltage | Max `speed` Value | Max Speed |
| :--- | :--- | :--- |
| **5.0V** | **108** | 108 rpm |
| **6.0V** | **121** | 121 rpm |
| **7.4V** | **143** | 143 rpm |
| **8.4V** | **158** | 158 rpm |

**Speed Behavior Notes:**
* **Load Handling:** If external load slows the motor down during rotation, it will simply maintain the set speed afterward — it will not accelerate to compensate for the time lost due to the load.
* **Speed Variance:** The actual rotation speed may vary by up to ±5% from the specified value due to unit-to-unit variation.
* **Low-Speed Smoothness:** At low speeds under no load, rotation may become irregular or jerky.

---

### `read()`
Returns the current angle in degrees.
- **Returns**: `int32_t`
- **Error Handling**: Returns `-2,147,483,648` if a communication error occurs.

---

### `stop()`
Immediately stops the servo at its current position.

---

### `wait()`
Blocks execution until the current movement is completed (until the motor reaches within ±1° of the target position).

---

### `isMoving()`
Returns `true` if the servo is currently rotating, and `false` if it is stopped.
- **Returns**: `bool`
- **Error Handling**: Also returns `false` if a communication error occurs.

---

### `setHold(hold)`
Sets whether the motor holds its position after reaching the target.
- **`hold`** `bool`:
  - **`true` — Active Hold (Default):** The motor actively maintains its position and resists external forces after the movement is complete.
  - **`false` — Passive Mode:** Releases holding torque, allowing the shaft to be turned freely by hand.

---

### `setZero()`
Sets the current absolute position (0°-359°) as the 0° reference point. This is saved to non-volatile memory (EEPROM/Flash) and persists after power cycles. Ongoing rotations will stop when this is called.
**Note:** Only the absolute angle (0°-359°) is saved; the rotation count is reset.

---

### `setSerialSpeed(baud)`
Sets the communication speed. The speed resets to **9600 baud** after each power cycle. Ongoing rotations will stop when this is called.
- **`baud`**: `uint16_t` (Select from: `9600`, `19200`, `38400`, `57600`)
- **Note:** Increasing the baud rate may cause communication errors, particularly affecting the reliability of `read()` operations.

---

### `getFirmwareVersion()`
Returns the firmware version of the connected servo.

- **Current Version**: `1`
- **Returns**: `uint8_t`
- **Error Handling**: Returns `0` if a communication error occurs.


## Usage Notes

### 1. Operational Constraints & Safety
* **Heat Generation:** Extended periods of continuous rotation can cause the motor to heat up.
* **Magnetic Interference:** Do not use the motor near strong magnetic fields (e.g., large magnets, high-current cables), as they may interfere with the internal magnetic encoder.
* **Physical Care:** The internal wiring is delicate — avoid applying excessive force or tension to the leads.

### 2. Startup & Calibration (Rotational Direction at Power-Up)
At power-up, the servo detects its absolute position (0°-359°), but the multi-turn counter resets to zero. This can cause the motor to rotate in an unexpected direction if the home position is near the 0°/360° wraparound point.

* **The Rotation Issue:** If the motor is physically at 359° at startup and you command `write(0)`, the library targets "0° in the current revolution." To reach this, the motor will rotate **359° backward** instead of moving forward just 1°.
* **Recommended Solutions:**
    * **Startup Logic:** Read the current position immediately after power-up and move the servo to the nearest target.
    ```arduino
    long currentAngle = myservo.read();
    if (currentAngle > 300) {
      myservo.write(360); // Move forward to 360° instead of back to 0°
    } else {
      myservo.write(0);
    }
    ```
    * **Calibration:** Use `setZero()` once to calibrate your physical home position to 0°. This is saved in non-volatile memory and persists after power cycles.

### 3. Pin Assignment
The wiring configuration depends on whether you need feedback from the motor.

* **Single Motor per Pin:** Functions requiring feedback (e.g., `read()`) support only one motor per I/O pin.
* **Broadcast Control:** Commands that do not require feedback support broadcast control over a single pin:
  * `write()` (with `wait=false`)
  * `writeRotation()`
  * `stop()`
  * `setHold()`
  * `setZero()`
  * `setSerialSpeed()`
* **Verifying Connections When Multiple Motors Share a Pin:** When multiple motors share a pin, `attach()` can only confirm whether *at least one* motor is responding — it cannot determine the exact number of connected motors.

### 4. Communication Characteristics & Limitations (Latency & Interrupts)
Since TekuteruServo uses a software-based bit-banging serial protocol on a single pin, it has specific characteristics regarding communication speed and system overhead:

* **Interrupt Blocking during Communication:** To ensure precise timing for the signal pulses, global interrupts are temporarily disabled (`noInterrupts()`) during data transmission and reception. At the default baud rate of 9600 baud, sending or receiving a command may block interrupts for **several milliseconds**. Frequent communication within a tight loop will worsen this blocking. This can potentially cause:
  * Slight drifting or inaccuracies in time-tracking functions like `millis()` or `micros()`.
  * Missed data or timing issues in other interrupt-driven libraries (such as SoftwareSerial, I2C, or hardware timers).
* **Communication Latency:** Because TekuteruServo uses software-based serial communication, it introduces **response latency** compared to PWM-based servos or high-performance hardware serial servos.
* **How to Mitigate:** If your application requires strict real-time interrupt processing or lower latency, consider increasing the communication speed using `setSerialSpeed()` to reduce the **blocking duration**.


## Code Examples

### 1. Basic Rotation
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);  // Attach to digital pin 2 (D2)
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

### 2. Connection Check
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);  // Start serial communication (set the serial monitor to 9600 baud)

  if (myservo.attach(2)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed");  // No servo detected on pin 2
    while (!myservo.attach(2)) {          // Wait until connected
      delay(100);
    }
    Serial.println("Connected");
  }
}

void loop() {
}
```

### 3. Speed Control
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 600);  // Move to 180 degrees at 600 deg/s
  delay(1000);

  myservo.write(-180, 100);  // Move to -180 degrees at 100 deg/s
  delay(3000);
}
```

### 4. Wait for Movement to Complete
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
  pinMode(LED_BUILTIN, OUTPUT);  // Set up LED for blink example
}

void loop() {
  // Blocking approach 1: pass true as the third argument
  myservo.write(180, 600, true);  // Move to 180°, wait for completion (within ±1°)

  // Blocking approach 2: call wait() separately
  myservo.write(-180);  // Start moving to -180°
  myservo.wait();       // Wait until motor reaches -180°

  // Non-blocking: program continues while the motor is moving
  myservo.write(720);           // Start moving to 720°
  while (myservo.isMoving()) {  // Do other work while the motor is moving
    // Example: LED blink
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
```

### 5. Read the Current Angle
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

  myservo.write(1800);  // Start moving to 1800 degrees (non-blocking)
  delay(1000);
  currentAngle = myservo.read();  // Read angle mid-rotation (motor is still moving)
  Serial.println(currentAngle);
  myservo.wait();  // Wait until reaches 1800°
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
  myservo.writeRotation(100);  // Rotate forward at 100 rpm
  delay(3000);

  myservo.writeRotation(-50);  // Rotate in reverse at 50 rpm
  delay(3000);

  myservo.writeRotation(0);  // Stop (same as calling stop())
  delay(2000);
}
```

### 7. Multiple Servos
**Note:** When operating multiple servos simultaneously, an external power supply is highly recommended to ensure stable operation and prevent voltage drops — make sure to connect the power supply GND to both the servo GND and the Arduino GND to establish a common ground.

![Wiring Diagram](images/wiring_multiple.png)
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo1;
TekuteruServo myservo2;
// No arbitrary software limit — limited only by available I/O pins and RAM

void setup() {
  myservo1.attach(2);
  myservo2.attach(3);
}

void loop() {
  myservo1.write(180, 600);
  myservo2.write(180, 600);
  myservo1.wait();  // Wait until myservo1 reaches 180°
  myservo2.wait();  // Wait until myservo2 reaches 180°

  myservo1.write(-180, 600);
  myservo2.write(-180, 300);
  myservo1.wait();
  myservo2.wait();
}
```

### 8. Set Zero
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);

  myservo.attach(2);
  myservo.setZero();  // Set the current position as 0 degrees (saved to non-volatile memory)

  Serial.println("setZero successful");
}

void loop() {
}
```

### 9. Set Serial Speed
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);

  // Change baud rate. Valid options: 9600, 19200, 38400, 57600
  // Note: Resets to 9600 after each power cycle.
  myservo.setSerialSpeed(19200);
}

void loop() {
}
```


## Firmware Customization

The internal program that controls the servo motor is located in `firmware/TekuteruServo_firmware/TekuteruServo_firmware.ino`.
If you have a single Arduino board, you can easily rewrite and customize the servo's internal firmware.

### Prerequisites

1. **Add megaTinyCore to the Board Manager:** In the Arduino IDE, open **File > Preferences**, then paste the following URL into the **Additional Boards Manager URLs** field:

   ```
   http://drazzy.com/package_drazzy.com_index.json
   ```
2. **megaTinyCore:** Install this via the Arduino IDE Board Manager.
3. **jtag2updi:** Download this sketch from [SpenceKonde/jtag2updi](https://github.com/SpenceKonde/jtag2updi).
   * Upload the `jtag2updi` sketch to your Arduino. Your Arduino is now ready to act as a UPDI programmer.

### Wiring

Remove the four screws from the bottom of the servo and take off the lower cover to access the internal board. Connect your programmer Arduino to the servo as follows:

| Arduino Pin | Servo Connection |
| :--- | :--- |
| **5V** | VCC |
| **GND** | GND |
| **D6** (Arduino Uno) | Programming pad (UPDI) |

**Note:** Insert a jumper pin into the designated programming pad and ensure it makes firm, steady contact. The 4.7kΩ UPDI resistor is built-in, so no external resistor is needed.

![Firmware Flashing Diagram](images/firmware_flash.png)

### Flashing the Firmware

1. Open `TekuteruServo_firmware.ino` (or your customized sketch) in the Arduino IDE.
2. Configure the following settings under the **Tools** menu:
   * **Board:** `ATtiny3226/3216/1626/1616/...` > `ATtiny1616`
   * **Programmer:** `jtag2updi`
   * **Port:** Select the COM port associated with your programmer Arduino.
3. Hold the jumper pin firmly against the programming pad to ensure a secure connection.
4. While maintaining contact, click **Sketch** > **Upload Using Programmer** in the Arduino IDE to flash the firmware.


## Support & Feedback
* **Library Design:** Inspired by the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library.
* **Feedback:** If you find any errors or have suggestions, please reach out to tekuterute@gmail.com.
