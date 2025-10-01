# CustomKeypad

CustomKeypad is an Arduino and PlatformIO compatible library designed as a drop-in replacement for the widely used [Keypad.h](https://playground.arduino.cc/Code/Keypad/) library.

It is optimized for hardware using external **pull-down resistors (rows idle LOW, columns driven HIGH)** and provides more robust hold detection, multi-key scanning, and event handling while retaining full backward compatibility with existing [Keypad.h](https://playground.arduino.cc/Code/Keypad/)-style APIs.

<div style="background-color: #fff3cd; border-left: 5px solid #ffc107; padding: 10px; margin: 15px 0;">
  <p style="font-weight: bold; color: #664d03; margin-top: 0;">IMPORTANT</p>
  <p style="margin-bottom: 0;">Original concept and API design are based on the excellent work by Mark Stanley and Alexander Brevig, the authors of <a href="https://playground.arduino.cc/Code/Keypad/">Keypad.h</a>. This library extends their work to support additional hardware configurations and advanced input handling.</p>
</div>

## Key Features
- Works with **external pull-down resistors** (rows idle LOW, columns HIGH).
- Reliable **hold detection** `setHoldTime()`.
- Event listener support `addEventListener()`.
- `getKeys()` for **multi-key detection**.
- Backward-compatible with Arduino Keypad API style.
- Compatible with Arduino, ESP32, STM32, and other platforms

## Installation

### PlatformIO
Add this line to your `platformio.ini`:
```bash 
lib_deps = Sheikh-Araf/CustomKeypad@^1.0.0
```

(Or use the GitHub URL directly)

```bash 
lib_deps = https://github.com/Sheikh-Araf/CustomKeypad.git
```


### Arduino IDE
1. Download ZIP from GitHub or [Releases](https://github.com/Sheikh-Araf/CustomKeypad/releases).
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library…**
3. Select the downloaded file.

---

## Example
##### Basic Usage Example
```cpp
#include <CustomKeypad.h>

#define ROWS 4
#define COLS 4

// Row and column connections (adjust for your wiring)
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Standard 4x4 keypad layout
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

char *keymap[ROWS] = {
  keys[0], keys[1], keys[2], keys[3]
};

// Create keypad object
CustomKeypad keypad(keymap, rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  keypad.begin();
  keypad.setHoldTime(1500); // 1.5 seconds for HOLD
  Serial.println("CustomKeypad Test - 4x4 Matrix");
}

void loop() {
  // Get single key
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);
  }

  // Check for hold
  if (keypad.getKeyState() == KEY_HOLD) {
    Serial.print("Key held: ");
    Serial.println(key);
  }

  // Multi-key check
  char pressed[5]; // up to 5 simultaneous
  byte numPressed = keypad.getKeys(pressed, 5);
  if (numPressed > 1) { // more than one key pressed
    Serial.print("Multiple keys pressed: ");
    for (byte i = 0; i < numPressed; i++) {
      Serial.print(pressed[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

```
##### Event Listener Example

```cpp

#include <CustomKeypad.h>

void keypadEvent(KeypadEvent key) {
  Serial.print("Event detected: ");
  Serial.println(key);
}

#define ROWS 4
#define COLS 4
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

char* keymap[ROWS] = { keys[0], keys[1], keys[2], keys[3] };
CustomKeypad keypad(keymap, rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  keypad.begin();
  keypad.addEventListener(keypadEvent);
}

void loop() {
  keypad.getKey(); // Triggers events internally
}


```

## License
This library is distributed under the [MIT License](./LICENSE).

Credit for the original implementation of [Keypad.h](https://playground.arduino.cc/Code/Keypad/) goes to Mark Stanley and Alexander Brevig.

## Contribution

**Contributions are welcome!**
Feel free to open issues or pull requests on [GitHub](https://github.com/Sheikh-Araf/CustomKeypad)