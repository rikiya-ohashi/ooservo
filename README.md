English | [日本語](README_ja.md)

# TekuteruServo

TekuteruServo is a serial servo motor that handles just like an SG90. It supports effectively unlimited rotation and offers high-precision position control.

It features a wide range of motion of ±5.96 million rotations with ±1° angular accuracy, and also supports real-time position feedback. It also adopts an API compatible with the standard Arduino Servo library, so it can be used just like a conventional PWM servo motor.

The internal firmware is completely open, so with just a single Arduino board you can freely rewrite the servo's program.

> **⚠ Important Compatibility Note**
> This library uses a dedicated serial protocol and differs from PWM-based servo libraries. As a result, it does not support standard PWM servos, and you cannot control TekuteruServo using the standard Servo.h library either.

**TekuteruServo hardware can be purchased [here](https://tekuteru.handcrafted.jp/items/121327019)**

If you have any questions about TekuteruServo, you can ask in the AI chat on the [Gemini Notebook](https://notebooklm.google.com/notebook/272725f0-6a1c-4c52-9597-6384a2f88f91).


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
* **High-precision multi-turn positioning:** Control ±5.96 million rotations (-2,147,483,648° to +2,147,483,647°) with ±1° precision.
* **Dual-mode operation:** Supports both angle control and continuous rotation (speed control).
* **Speed adjustment:** Rotation speed can be freely set in increments of 1 deg/s.
* **Real-time position feedback:** Call `read()` at any time to get the current angle.
* **Compatibility with existing parts:** Same wiring, form factor, and logic voltage (3.3 V to 5 V) as the SG90.
* **Familiar API:** Use the same `attach()` and `write()` methods as the standard Arduino Servo library.
* **Wide board support:** Works with a variety of microcontrollers, including Arduino, ESP32, and Raspberry Pi Pico.
* **No limit on the number of connected units:** There is no software-side limit on how many servos can be connected; the practical limit depends on the number of I/O pins and the amount of RAM on your board.
* **Freely reprogrammable:** By using an Arduino board as a programmer, you can update the servo's own firmware without needing any dedicated tools.


## Specifications
* **Operating voltage:** 4.0 V to 8.4 V
* **Logic voltage:** 3.3 V to 5 V
* **Maximum speed:** 930 deg/s (0.065 s/60°, 155 rpm) **(at 8.4 V)**
* **Stall torque:** 2.1 kgf·cm **(at 8.4 V)**
* **Stall current:** 1.4 A **(at 8.4 V)**
* **Communication speed:** 9600 baud by default (can be changed up to 57600 baud)
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
If you want to control TekuteruServo from Python on a Raspberry Pi, please use the dedicated Python library:
[TekuteruServo-Python](https://github.com/tekuteru/TekuteruServo-Python)


## Wiring Guide
TekuteruServo wiring is exactly the same as a standard PWM servo.

| Wire Color | Role | Connects To |
| :--- | :--- | :--- |
| Brown | GND | Arduino GND |
| Red | VCC | Arduino 5V |
| Yellow | Signal | Arduino I/O pin |

![Wiring Diagram](images/wiring.png)


## Installation (Arduino IDE)
1. Open the **Arduino IDE**.
2. Open the **Library Manager**.
3. Type "**TekuteruServo**" into the search box.
4. Select the latest version and click **Install**.


## Class Methods

| Method | Description |
| :--- | :--- |
| [`attach(pin)`](#attachpin) | Attach the servo to the specified pin |
| [`write(angle)`](#writeangle) | Rotate to the target angle at maximum speed (non-blocking) |
| [`write(angle, speed)`](#writeangle-speed) | Rotate to the target angle at the specified speed (deg/s) |
| [`write(angle, speed, wait)`](#writeangle-speed-wait) | Rotate with a specified speed and blocking behavior |
| [`writeRotation(speed)`](#writerotationspeed) | Continuously rotate at the specified speed (rpm) |
| [`read(&hasError)`](#readhaserror) | Get the current angle |
| [`stop()`](#stop) | Stop immediately |
| [`wait()`](#wait) | Block until the current motion is complete |
| [`isMoving(&hasError)`](#ismovinghaserror) | Get whether the servo is currently rotating |
| [`setHold(hold)`](#setholdhold) | Set the hold behavior after stopping |
| [`setZero()`](#setzero) | Save the current angle as 0° |
| [`setSerialSpeed(baud)`](#setserialspeedbaud) | Set the communication speed |
| [`getFirmwareVersion()`](#getfirmwareversion) | Get the firmware version |

---

### `attach(pin)`
Attaches the servo to the specified pin. It can be connected to any digital I/O pin on the board.
The return value tells you whether the connection succeeded.
- **`pin`**: `uint8_t`
- **Return value**: `bool` — `true` if the servo is connected and responding, `false` if a communication error occurs.

> **Note:** Always call `attach()` before using any other method.

---

### `write(angle)`
Rotates the servo to the specified angle at maximum speed (non-blocking).
Immediately after power-on, the current position is mapped to the range 0° to 359°. For details, see [Startup Calibration](#2-startup-rotation-direction-and-calibration) in Usage Notes.
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
* **Under load:** If external load slows the servo down while rotating, it will accelerate afterward to make up the difference, so it still reaches the target angle as scheduled.
* **Speed variation:** Due to individual unit variation, actual speed may deviate from the specified value by up to about ±5%.
* **Smoothness at low speed:** At low, unloaded rotation speeds, movement can become irregular or jerky.

---

### `write(angle, speed, wait)`
Rotates to the target angle with a specified speed and blocking behavior.

- **`angle`**: Target position (degrees).
- **`speed`**: Rotation speed (**deg/s**).
- **`wait`** (`bool`): If `true`, blocks the program until the motor reaches within ±1° of the target position.

---

### `writeRotation(speed)`
Continuously rotates the servo at the specified speed (unit: **rpm**). Rotation continues until the next command is sent.

**Note:** The motor's range of motion is `-2,147,483,648°` to `+2,147,483,647°`.

- **`speed`**: Rotation speed (**rpm**, `int16_t`).
  - **`1` to upper limit**: Forward rotation (counterclockwise).
  - **`-1` to -upper limit**: Reverse rotation (clockwise).
  - **`0`**: Stop the motor (same as `stop()`).

Maximum speed varies with supply voltage, as shown below:

| Supply Voltage | `speed` Upper Limit | Max Speed |
| ---: | ---: | ---: |
| **5.0 V** | **108** | 108 rpm |
| **6.0 V** | **126** | 126 rpm |
| **7.4 V** | **140** | 140 rpm |
| **8.4 V** | **155** | 155 rpm |

**Notes on speed:**
* **Under load:** If external load slows the servo down while rotating, it simply maintains the set speed and does not accelerate to make up the difference.
* **Speed variation:** Due to individual unit variation, actual speed may deviate from the specified value by up to about ±5%.
* **Smoothness at low speed:** At low, unloaded rotation speeds, movement can become irregular or jerky.

---

### `read(&hasError)`
Returns the current angle, in degrees.
- **Return value**: `int32_t`
- **Error handling**: Pass a pointer to a `bool` variable as `hasError` to receive whether a communication error occurred (`true` on error, `false` if normal). Note that the return value is fixed at `-2,147,483,648` when a communication error occurs.

---

### `stop()`
Stops the servo immediately, in place.

---

### `wait()`
Blocks execution until the current motion is complete (until the servo is within ±1° of the target position).

---

### `isMoving(&hasError)`
Returns `true` if the servo is rotating, or `false` if it is stopped.
- **Return value**: `bool`
- **Error handling**: Pass a pointer to a `bool` variable as `hasError` to receive whether a communication error occurred (`true` on error, `false` if normal). Note that the return value is fixed at `false` when a communication error occurs.

---

### `setHold(hold)`
Sets whether the motor holds its position after reaching the target.
- **`hold`** `bool`:
  - **`true` — Active hold (default):** After motion completes, the motor maintains its position and will resist external force, trying to return to that position.
  - **`false` — Passive mode:** Releases holding torque, allowing the shaft to be turned freely by hand.

---

### `setZero()`
Sets the current angle as the 0° reference point. This setting is saved to non-volatile memory (EEPROM/Flash) and is not lost when power is turned off. Running this stops any in-progress rotation immediately.
**Note:** Only the absolute angle (0° to 359°) is saved; the accumulated rotation count is reset.

---

### `setSerialSpeed(baud)`
Sets the communication speed. It reverts to **9600 baud** when power is cycled. Running this stops any in-progress rotation immediately.
- **`baud`**: `uint16_t` (choose from `9600`, `19200`, `38400`, `57600`)
- **Return value**: `bool` — `true` if the change succeeded, `false` on communication error or an invalid `baud` value.
- **Note:** Raising the communication speed makes communication errors more likely, and this can particularly affect the reliability of `read()`.

---

### `getFirmwareVersion()`
Returns the firmware version of the connected servo.

- **Latest version**: `1`
- **Return value**: `uint8_t`
- **Error handling**: Returns `0` on communication error.


## Usage Notes

### 1. Operational Constraints and Safety
* **Heat:** Continuous rotation for extended periods can cause the motor to heat up.
* **Magnetic interference:** Do not use near strong magnetic fields, such as large magnets or high-current cables. The built-in magnetic encoder may be affected.
* **Wiring care:** The internal wiring is delicate, so avoid pulling the cable forcefully or applying excessive strain.

### 2. Startup Rotation Direction and Calibration
* **The reverse-rotation issue:** Suppose that right after power-on, the motor is physically at the 359° position. Normally, it would only need to advance 1° to reach 0° (=360°), but if you run `write(0)` in this state, the motor will instead **rotate 359° in the opposite direction**.
* **Solutions:** There are two approaches:
    * **Method 1: Move toward whichever target is closer at startup**
    Immediately after power-on, read the current position, determine whether it's closer to 0° or 360°, and target whichever is closer.
    ```arduino
    long currentAngle = myservo.read();
    if (currentAngle > 300) {
      myservo.write(360); // Target 360° instead of 0°
    } else {
      myservo.write(0);
    }
    ```
    * **Method 2: Calibrate the origin in advance**
    Running `setZero()` once registers the motor's physical position at that moment as 0°. This setting is saved to non-volatile memory, so it is not lost when the power is turned off.

### 3. Pin Assignment
How many motors can be connected to a single pin depends on the function being used.

* **Up to 1 unit per pin:**
  * `write()` (when `wait=true`)
  * `read()`
  * `isMoving()`
  * `wait()`
  * `getFirmwareVersion()`
* **Can send to multiple units on the same pin simultaneously:**
  * `attach()` (the return value only indicates whether there was a response; it cannot tell you how many servos on that pin are actually connected correctly)
  * `write()` (when `wait=false`)
  * `writeRotation()`
  * `stop()`
  * `setHold()`
  * `setZero()`
  * `setSerialSpeed()` (if even one unit causes a communication error, the return value is `false`, and the communication speed is not changed for any servo connected to that pin)

### 4. Communication Characteristics and Limitations (Latency and Interrupts)
* **Interrupts are blocked during communication:** To keep the timing of signal pulses accurate, global interrupts are temporarily disabled (`noInterrupts()`) while data is being sent or received. At the default 9600 baud, a single command exchange can block interrupts for **a few milliseconds**, and this effect grows the more communication is repeated in a tight loop. As a result, you may see effects such as:
  * Slight drift in timing functions such as `millis()` and `micros()`.
  * Dropped data or timing shifts in other interrupt-driven libraries, such as SoftwareSerial, I2C, or hardware timers.
* **Communication latency:** Because this is software-based serial communication, response is slower compared to PWM servos or high-performance hardware-serial servos.
* **Mitigation:** For applications that require strict real-time behavior or low latency, you can shorten the blocking time by increasing the communication speed with `setSerialSpeed()`.


## Code Examples

### 1. Basic Rotation
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);  // Attach to pin D2
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
    Serial.println("Connection failed");  // No servo was found on pin 2
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

### 4. Waiting for Motion to Complete
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
  pinMode(LED_BUILTIN, OUTPUT);  // Set up for the LED-blink example
}

void loop() {
  // Method 1: pass true as the 3rd argument to make it blocking
  myservo.write(180, 600, true);  // Move to 180 degrees and wait until complete (within ±1°)

  // Method 2: call wait() separately
  myservo.write(-180);  // Start moving to -180 degrees
  myservo.wait();       // Wait until -180 degrees is reached

  // Non-blocking: the program keeps running while the motor is moving
  myservo.write(720);           // Start moving to 720 degrees
  while (myservo.isMoving()) {  // Do other work while the motor is moving
    // Example: blink the LED
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
  currentAngle = myservo.read();  // Read the angle while rotating (the motor is still moving)
  Serial.println(currentAngle);
  myservo.wait();  // Wait until 1800 degrees is reached
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

  // Allowed values: 9600, 19200, 38400, 57600
  // Note: Reverts to 9600 baud when power is cycled.
  myservo.setSerialSpeed(19200);
}

void loop() {
}
```


## Firmware Customization

The internal program that controls the servo motor is located at `firmware/TekuteruServo_firmware/TekuteruServo_firmware.ino`.
With just a single Arduino board, you can rewrite the servo's internal firmware however you like.

### Prerequisites

1. **Board Manager URL:** Open **File > Preferences** in the Arduino IDE, and paste the following URL into the "Additional Boards Manager URLs" field:
   ```
   http://drazzy.com/package_drazzy.com_index.json
   ```
2. **megaTinyCore:** Install it from the Arduino IDE's Boards Manager.
3. **jtag2updi:** Download the sketch from [SpenceKonde/jtag2updi](https://github.com/SpenceKonde/jtag2updi).
   * Flash the `jtag2updi` sketch onto an Arduino. This turns that Arduino into a usable UPDI programmer.

### Wiring

Remove the 4 screws on the bottom of the servo and open the bottom cover to access the internal circuit board. Connect the programmer Arduino to the servo as follows.

| Arduino Pin | Connects To (Servo Side) |
| :--- | :--- |
| **5V** | VCC |
| **GND** | GND |
| Programming pin (see table below) | Programming pad (UPDI) |

The programming pin depends on the type of Arduino you're using.

| Arduino Type | Arduino Pin |
| :--- | :--- |
| Uno R3 | D6 |
| Nano | D6 |
| Pro Mini | D6 |
| Mega 2560 | D18 |

**Note:** Firmly insert the jumper pin into the programming pad and keep contact stable. A 4.7 kΩ UPDI resistor is already built in, so no external resistor is needed.

![Firmware flashing diagram](images/firmware_flash.png)

### Flashing the Firmware

1. In the Arduino IDE, open `TekuteruServo_firmware.ino` (or your customized sketch).
2. In the **Tools** menu, set the following:
   * **Board:** `ATtiny1616`, from within `ATtiny3226/3216/1626/1616/...`
   * **Programmer:** `jtag2updi`
   * **Port:** the COM port assigned to the programmer Arduino
3. Keep the jumper pin firmly pressed against the programming pad.
4. While holding that in place, click **Sketch > Upload Using Programmer** in the Arduino IDE to flash the firmware.


## Support & Feedback
* **Feedback:** If you find a bug or have suggestions for improvement, please contact tekuterute@gmail.com.
* **API design:** Inspired by the [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) library.
