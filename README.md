# CustomKeypad

An Arduino/PlatformIO library for matrix keypads with **external pull-down resistors**.  
This library is a drop-in replacement for the popular `Keypad.h` but optimized for hardware where rows are pulled down to GND and columns are driven HIGH.  

## Features
- Works with **external pull-down resistors** (rows idle LOW, columns HIGH).
- Reliable **hold detection** `setHoldTime()`.
- Event listener support `addEventListener()`.
- `getKeys()` for **multi-key detection**.
- Backward-compatible with Arduino Keypad API style.

## Installation

### PlatformIO
Add this line to your `platformio.ini`:
```bash 
lib_deps = https://github.com/Sheikh-Araf/CustomKeypad.git
```


### Arduino IDE
1. Download ZIP from GitHub.
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library…**
3. Select the downloaded file.

---

## Example

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