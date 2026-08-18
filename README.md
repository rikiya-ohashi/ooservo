English | [日本語](README_ja.md)

# TekuteruServo

TekuteruServo is a serial servo motor that can be used with the same feel as an SG90. It supports essentially unlimited rotation and enables high-precision position control.

With a wide range of motion of ±5.96 million rotations and an angular precision of ±1°, it also supports real-time position feedback. It also adopts an API compatible with the standard Arduino Servo library, so you can use it with the same feel as a conventional PWM servo motor.

The internal firmware is completely open, and with just one Arduino board you can freely rewrite the servo's program.

> **⚠ Important Note on Compatibility**
> This library uses a dedicated serial protocol, which differs from PWM-based servo libraries. Therefore, it does not support standard PWM servos, and TekuteruServo cannot be controlled using the standard Servo.h library either.

**You can purchase the TekuteruServo hardware [here](https://tekuteru.handcrafted.jp/items/121327019)**

If you have any questions about TekuteruServo, you can ask the AI chat in this [Gemini Notebook](https://notebooklm.google.com/notebook/272725f0-6a1c-4c52-9597-6384a2f88f91).


## Table of Contents
- [Features](#features)
- [Specifications](#specifications)
- [Python Library (Raspberry Pi)](#python-library-raspberry-pi)
- [Wiring Guide](#wiring-guide)
- [Installation (Arduino IDE)](#installation-arduino-ide)
- [Class Methods](#class-methods)
- [Usage Notes](#usage-notes)
- [Code Examples](#code-examples)
- [Firmware Customization](#firmware-customization)
- [Support & Feedback](#support--feedback)


## Features
* **High-precision multi-turn positioning:** Control across ±5.96 million rotations (-2,147,483,648° to +2,147,483,647°) with ±1° accuracy.
* **Dual-mode operation:** Supports both angle control and continuous rotation (speed control).
* **Speed adjustment:** Rotation speed can be freely set in increments of 1 deg/s.
* **Real-time position feedback:** Call `read()` at any time to get the current angle.
* **Compatible with existing parts:** Same wiring, shape, and logic voltage (3.3 V–5 V) as the SG90.
* **Same-feel API:** Uses the same `attach()` and `write()` calls as the standard Arduino Servo library.
* **Wide board support:** Works with Arduino, ESP32, Raspberry Pi Pico, and many other microcontrollers.
* **No limit on the number of units:** There is no software-side limit on how many can be connected — the practical limit depends on the board's I/O pin count and RAM capacity.
* **Freely reprogrammable:** By using an Arduino board as a programmer, you can update the servo's own firmware without needing any dedicated tools.


## Specifications
* **Operating voltage:** 4.0 V–8.4 V
* **Logic voltage:** 3.3 V–5 V
* **Max speed:** 930 deg/s (0.065 s/60°, 155 rpm) **※at 8.4 V**
* **Stall torque:** 2.1 kgf·cm **※at 8.4 V**
* **Stall current:** 1.4 A **※at 8.4 V**
* **Communication speed:** Default 9600 baud (adjustable up to 57600 baud)
* **Gear material:** Stainless steel
* **Dimensions:** 31.8 x 12 x 30.1 mm
* **Weight:** 13 g
* **Cable length:** 24 cm

### Performance Table
| Supply Voltage | Max Speed (deg/s) | (rpm) | (s/60°) | Stall Torque |
| ---: | ---: | ---: | ---: | ---: |
| **5.0 V** | 650 deg/s | 108 rpm | 0.093 s/60° | 1.5 kgf·cm |
| **6.0 V** | 760 deg/s | 126 rpm | 0.079 s/60° | 1.7 kgf·cm |
| **7.4 V** | 840 deg/s | 140 rpm | 0.072 s/60° | 2.0 kgf·cm |
| **8.4 V** | 930 deg/s | 155 rpm | 0.065 s/60° | 2.1 kgf·cm |


## Python Library (Raspberry Pi)
If you want to control TekuteruServo from Python on a Raspberry Pi, use the dedicated Python library:
[TekuteruServo-Python](https://github.com/tekuteru/TekuteruServo-Python)


## Wiring Guide
Wiring TekuteruServo is exactly the same as a standard PWM servo.

| Wire Color | Role | Connects To |
| :--- | :--- | :--- |
| Brown | GND | Arduino GND |
| Red | VCC | Arduino 5V |
| Yellow | Signal | Arduino I/O pin |

![Wiring diagram](images/wiring.png)


## Installation (Arduino IDE)
1. Open the **Arduino IDE**.
2. Open the **Library Manager**.
3. Type "**TekuteruServo**" into the search box.
4. Select the latest version and click **Install**.


## Class Methods

| Method | Overview |
| :--- | :--- |
| [`attach(pin)`](#attachpin) | Connects the servo to the specified pin |
| [`write(angle)`](#writeangle) | Rotates to the target angle at max speed (non-blocking) |
| [`write(angle, speed)`](#writeangle-speed) | Rotates to the target angle at the specified speed (deg/s) |
| [`write(angle, speed, wait)`](#writeangle-speed-wait) | Rotates with a specified speed and blocking behavior |
| [`writeRotation(speed)`](#writerotationspeed) | Continuous rotation at the specified speed (rpm) |
| [`read(&hasError)`](#readhaserror) | Gets the current angle |
| [`stop()`](#stop) | Stops immediately |
| [`wait()`](#wait) | Blocks until the current motion completes |
| [`isMoving(&hasError)`](#ismovinghaserror) | Gets whether the servo is currently rotating |
| [`setHold(hold)`](#setholdhold) | Sets the hold behavior after stopping |
| [`setZero()`](#setzero) | Saves the current angle as 0° |
| [`setSerialSpeed(baud)`](#setserialspeedbaud) | Sets the communication speed |
| [`getFirmwareVersion()`](#getfirmwareversion) | Gets the firmware version |

---

### `attach(pin)`
Connects the servo to the specified pin. It can be connected to any digital I/O pin on the board.
The return value lets you check whether the connection succeeded.
- **`pin`**: `uint8_t`
- **Return value**: `bool` — `true` if the servo is connected and responding, `false` if a communication error occurs.

> **Note:** Always call `attach()` before using any other method.

---

### `write(angle)`
Rotates the servo to the specified angle at maximum speed (non-blocking).
Immediately after power-on, the current position is mapped to the range 0°–359°. For details, see [Rotation Direction and Calibration at Startup](#2-rotation-direction-and-calibration-at-startup) in Usage Notes.
- **`angle`**: `int32_t` (range: `-2,147,483,648` to `+2,147,483,647`)

---

### `write(angle, speed)`
Rotates to the target angle at the specified speed (unit: **deg/s**).

- **`speed`**: Rotation speed (**deg/s**, `uint16_t`).
  - **`0`**: Stop
  - **`1`**: Minimum speed (1 deg/s)
  - **Upper limit**: Varies with supply voltage, as shown below:

| Supply Voltage | `speed` Upper Limit | Max Speed |
| ---: | ---: | ---: |
| **5.0 V** | **650** | 650 deg/s |
| **6.0 V** | **760** | 760 deg/s |
| **7.4 V** | **840** | 840 deg/s |
| **8.4 V** | **930** | 930 deg/s |

**Notes on speed:**
* **Under load:** If the servo slows down due to an external load while rotating, it will accelerate afterward to make up the delay, so it still reaches the target angle as expected.
* **Speed variation:** Due to individual differences, actual speed can deviate from the specified value by up to about ±5%.
* **Smoothness at low speed:** At low, no-load rotation speeds, movement can become irregular or jerky.

---

### `write(angle, speed, wait)`
Rotates to the target angle with the specified speed and blocking behavior.

- **`angle`**: Target position (degrees).
- **`speed`**: Rotation speed (**deg/s**).
- **`wait`** (`bool`): If `true`, blocks the program until the motor is within ±1° of the target position.

---

### `writeRotation(speed)`
Rotates the servo continuously at the specified speed (unit: **rpm**). Rotation continues until the next command is sent.

**Note:** The motor's movable range is `-2,147,483,648°` to `+2,147,483,647°`.

- **`speed`**: Rotation speed (**rpm**, `int16_t`).
  - **`1` to upper limit**: Forward rotation (counterclockwise).
  - **`-1` to -upper limit**: Reverse rotation (clockwise).
  - **`0`**: Stops the motor (same as `stop()`).

Max speed varies with supply voltage, as shown below:

| Supply Voltage | `speed` Upper Limit | Max Speed |
| ---: | ---: | ---: |
| **5.0 V** | **108** | 108 rpm |
| **6.0 V** | **126** | 126 rpm |
| **7.4 V** | **140** | 140 rpm |
| **8.4 V** | **155** | 155 rpm |

**Notes on speed:**
* **Under load:** If the servo slows down due to an external load while rotating, it simply maintains the set speed and does not accelerate to make up the delay.
* **Speed variation:** Due to individual differences, actual speed can deviate from the specified value by up to about ±5%.
* **Smoothness at low speed:** At low, no-load rotation speeds, movement can become irregular or jerky.

---

### `read(&hasError)`
Returns the current angle in degrees.
- **Return value**: `int32_t`
- **Error handling**: Pass a pointer to a `bool` variable as `hasError` to receive whether a communication error occurred (`true` on error, `false` if normal). Note that on a communication error the return value is always `-2,147,483,648`.

---

### `stop()`
Stops the servo immediately, in place.

---

### `wait()`
Blocks execution until the current motion completes (i.e., until the motor is within ±1° of the target position).

---

### `isMoving(&hasError)`
Returns `true` if the servo is rotating, `false` if it is stopped.
- **Return value**: `bool`
- **Error handling**: Pass a pointer to a `bool` variable as `hasError` to receive whether a communication error occurred (`true` on error, `false` if normal). Note that on a communication error the return value is always `false`.

---

### `setHold(hold)`
Sets whether the motor holds its position after reaching the target.
- **`hold`** `bool`:
  - **`true` — Active hold (default):** After the motion completes, the motor maintains its position and will resist and return to it if external force is applied.
  - **`false` — Passive mode:** Releases the holding torque so the shaft can be turned freely by hand.

---

### `setZero()`
Sets the current angle as the 0° reference point. This setting is saved to non-volatile memory (EEPROM/Flash) and is retained even after power is turned off. Running this stops any rotation that is in progress at that moment.
**Note:** Only the absolute angle (0°–359°) is saved — the accumulated rotation count is reset.

---

### `setSerialSpeed(baud)`
Sets the communication speed. It reverts to **9600 baud** when power is cycled. Running this stops any rotation that is in progress at that moment.
- **`baud`**: `uint16_t` (choose from `9600`, `19200`, `38400`, `57600`)
- **Return value**: `bool` — `true` if the change succeeds, `false` on a communication error or an invalid `baud` value.
- **Note:** Increasing the communication speed makes communication errors more likely, which can particularly affect the reliability of `read()`.

---

### `getFirmwareVersion()`
Returns the firmware version of the connected servo.

- **Latest version**: `1`
- **Return value**: `uint8_t`
- **Error handling**: Returns `0` on a communication error.


## Usage Notes

### 1. Operational Constraints and Safety
* **Heat generation:** Continuous rotation over a long period can cause the motor to heat up.
* **Magnetic interference:** Do not use near strong magnetic fields, such as large magnets or high-current cables — the built-in magnetic encoder may be affected.
* **Cable handling:** The internal wiring is delicate, so avoid pulling the cable forcefully or applying excessive strain.

### 2. Rotation Direction and Calibration at Startup
* **Reversed rotation direction issue:** Suppose that immediately after power-on, the motor is physically at 359°. Normally it would only need to advance 1° to reach 0° (=360°), but if you run `write(0)` in this state, the motor will instead **rotate 359° in the opposite direction**.
* **Solutions:** There are two approaches:
    * **Method 1: Move toward the nearer target at startup**
    Immediately after power-on, read the current position, determine whether it's closer to 0° or 360°, and target whichever is nearer.
    ```arduino
    long currentAngle = myservo.read();
    if (currentAngle > 300) {
      myservo.write(360); // Target 360° instead of 0°
    } else {
      myservo.write(0);
    }
    ```
    * **Method 2: Calibrate the origin in advance**
    Running `setZero()` once registers the motor's physical position at that time as 0°. This setting is saved to non-volatile memory, so it is retained even after power is turned off.

### 3. Pin Assignment
How many motors you can connect to a single pin depends on the function.

* **Up to 1 unit per pin:**
  * `write()` (when `wait=true`)
  * `read()`
  * `isMoving()`
  * `wait()`
  * `getFirmwareVersion()`
* **Can address multiple units on the same pin at once:**
  * `attach()` (the return value only indicates whether there was a response — it can't tell you how many servos on that pin are correctly connected)
  * `write()` (when `wait=false`)
  * `writeRotation()`
  * `stop()`
  * `setHold()`
  * `setZero()`
  * `setSerialSpeed()` (if even one unit encounters a communication error, the return value is `false`, and the communication speed of none of the servos on that pin gets changed)

### 4. Communication Characteristics and Limitations (Latency and Interrupts)
* **Interrupts are blocked during communication:** To keep signal pulse timing accurate, global interrupts are temporarily disabled (`noInterrupts()`) while sending/receiving data. At the default 9600 baud, a single command exchange can block interrupts for **several milliseconds**, and this impact grows the more frequently communication is repeated in a tight loop. This can result in effects such as:
  * Slight drift in time-measurement functions like `millis()` and `micros()`.
  * Dropped data or timing shifts in other interrupt-driven libraries, such as SoftwareSerial, I2C, or hardware timers.
* **Communication latency:** Because this is software-based serial communication, response is slower compared to PWM servos or high-performance hardware serial servos.
* **Mitigation:** For applications requiring strict real-time performance or low latency, you can shorten blocking time by increasing the communication speed with `setSerialSpeed()`.


## Code Examples

### 1. Basic Rotation
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);  // Connect to pin D2
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
  Serial.begin(9600);  // Start serial communication (set the Serial Monitor to 9600 baud)

  if (myservo.attach(2)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed");  // No servo found on pin 2
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

### 4. Waiting for Completion
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
  pinMode(LED_BUILTIN, OUTPUT);  // Set up for the LED-blink example
}

void loop() {
  // Method 1: pass true as the 3rd argument to block
  myservo.write(180, 600, true);  // Move to 180 degrees and wait until complete (within ±1°)

  // Method 2: call wait() separately
  myservo.write(-180);  // Start moving to -180 degrees
  myservo.wait();       // Wait until it reaches -180 degrees

  // Non-blocking: the program keeps running while the motor is moving
  myservo.write(720);           // Start moving to 720 degrees
  while (myservo.isMoving()) {  // Do other work while the motor is moving
    // e.g. blink an LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
```

### 5. Reading the Current Angle
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

long currentAngle;  // Use long (int32_t) to match the return type of read()

void setup() {
  Serial.begin(9600);  // Start serial communication (set the Serial Monitor to 9600 baud)

  myservo.attach(2);

  currentAngle = myservo.read();  // Read the current angle (0 ≤ angle ≤ 359)
  Serial.println(currentAngle);   // Display on the Serial Monitor
}

void loop() {
  myservo.write(360, 600, true);  // Move to 360 degrees and wait until complete
  currentAngle = myservo.read();  // Read the current angle (expected: 360±1)
  Serial.println(currentAngle);

  myservo.write(0, 600, true);    // Move to 0 degrees and wait until complete
  currentAngle = myservo.read();  // Read the current angle (expected: 0±1)
  Serial.println(currentAngle);

  myservo.write(1800);  // Start moving to 1800 degrees (non-blocking)
  delay(1000);
  currentAngle = myservo.read();  // Read the angle mid-rotation (motor is still moving)
  Serial.println(currentAngle);
  myservo.wait();  // Wait until it reaches 1800 degrees
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
  myservo.writeRotation(100);  // Forward rotation at 100 rpm
  delay(3000);

  myservo.writeRotation(-50);  // Reverse rotation at 50 rpm
  delay(3000);

  myservo.writeRotation(0);  // Stop (same as stop())
  delay(2000);
}
```

### 7. Multiple Servos
**Note:** When driving multiple servos at once, using an external power supply is recommended to keep operation stable and prevent voltage drop. Connect the power supply's GND to both the servo GND and the Arduino GND to establish a common ground.

![Wiring diagram for multiple connections](images/wiring_multiple.png)
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo1;
TekuteruServo myservo2;
// No software-side limit on the number of units — depends on available I/O pins and RAM

void setup() {
  myservo1.attach(2);
  myservo2.attach(3);
}

void loop() {
  myservo1.write(180, 600);
  myservo2.write(180, 600);
  myservo1.wait();  // Wait until myservo1 reaches 180 degrees
  myservo2.wait();  // Wait until myservo2 reaches 180 degrees

  myservo1.write(-180, 600);
  myservo2.write(-180, 300);
  myservo1.wait();
  myservo2.wait();
}
```

### 8. Setting the Zero Point
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

### 9. Setting the Serial Speed
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);

  // Valid values: 9600, 19200, 38400, 57600
  // Note: reverts to 9600 baud when power is cycled.
  myservo.setSerialSpeed(19200);
}

void loop() {
}
```


## Firmware Customization

The internal program that controls the servo motor lives at `firmware/TekuteruServo_firmware/TekuteruServo_firmware.ino`.
With just one Arduino board, you can rewrite the servo's internal firmware to suit your needs.

### Prerequisites

1. **Board Manager URL:** Open **File > Preferences** in the Arduino IDE and paste the following URL into the "Additional Boards Manager URLs" field:
   ```
   http://drazzy.com/package_drazzy.com_index.json
   ```
2. **megaTinyCore:** Install it from the Arduino IDE's Board Manager.
3. **jtag2updi:** Download the sketch from [SpenceKonde/jtag2updi](https://github.com/SpenceKonde/jtag2updi).
   * Upload the `jtag2updi` sketch to your Arduino. This turns that Arduino into a usable UPDI programmer.

### Wiring

Remove the 4 screws on the bottom of the servo and open the bottom cover to access the internal board. Connect the programmer Arduino to the servo as follows.

| Arduino Pin | Connects To (Servo Side) |
| :--- | :--- |
| **5V** | VCC |
| **GND** | GND |
| **D6** (for Arduino Uno) | Programming pad (UPDI) |

**Note:** Firmly insert the jumper pins into the programming pad and maintain stable contact. A 4.7kΩ UPDI resistor is already built in, so no external resistor is needed.

![Firmware flashing diagram](images/firmware_flash.png)

### Flashing the Firmware

1. Open `TekuteruServo_firmware.ino` (or your customized sketch) in the Arduino IDE.
2. In the **Tools** menu, set the following:
   * **Board:** `ATtiny1616`, from within `ATtiny3226/3216/1626/1616/...`
   * **Programmer:** `jtag2updi`
   * **Port:** The COM port assigned to the programmer Arduino
3. Hold the jumper pins firmly against the programming pad.
4. While maintaining that contact, click **Sketch > Upload Using Programmer** in the Arduino IDE to flash the firmware.


## Support & Feedback
* **Feedback:** For bugs or suggestions, please contact tekuterute@gmail.com.
* **API Design:** Inspired by the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library.
