/**
 * @file CustomKeypad.cpp
 * @brief Implementation of the CustomKeypad class for handling matrix keypad input.
 * @author Sheikh Fardin Hossen Araf, Embedded System Engineer
 * @version 1.0.0
 */

#include "CustomKeypad.h"

/**
 * @brief Constructs a CustomKeypad object with the specified keymap and pin configuration.
 *
 * Initializes the keypad by setting the user-defined keymap, row and column pins, and the number
 * of rows and columns in the keypad matrix.
 *
 * @param userKeymap 2D array representing the keypad character map.
 * @param rowPins Array of pin numbers for keypad rows.
 * @param colPins Array of pin numbers for keypad columns.
 * @param numRows Number of rows in the keypad matrix.
 * @param numCols Number of columns in the keypad matrix.
 * @return None
 */
CustomKeypad::CustomKeypad(char **userKeymap, byte *rowPins, byte *colPins, byte numRows, byte numCols)
{
    _keymap  = userKeymap;
    _rows    = rowPins;
    _cols    = colPins;
    _numRows = numRows;
    _numCols = numCols;
}

/**
 * @brief Initializes the keypad by configuring pin modes for rows and columns.
 *
 * Sets column pins as outputs and initializes them to LOW. Sets row pins as inputs for
 * detecting key presses.
 *
 * @param None
 * @return None
 * @note Uses Arduino `pinMode` and `digitalWrite` functions.
 */
void CustomKeypad::begin()
{
    for (byte c = 0; c < _numCols; c++) {
        pinMode(_cols[c], OUTPUT);
        digitalWrite(_cols[c], LOW);
    }
    
    for (byte r = 0; r < _numRows; r++) {
        pinMode(_rows[r], INPUT);
    }
}

/**
 * @brief Scans the keypad matrix to detect a single key press.
 *
 * Activates each column in sequence, checks for HIGH signals on row pins to detect a key press,
 * and returns the corresponding character from the keymap. Returns 0 if no key is pressed.
 *
 * @param None
 * @return char The character of the pressed key, or 0 if no key is pressed.
 * @note Uses Arduino `digitalWrite`, `digitalRead`, and `delayMicroseconds` functions.
 */
char CustomKeypad::scanKeypad()
{
    for (byte c = 0; c < _numCols; c++) {
        digitalWrite(_cols[c], HIGH);
        delayMicroseconds(10); // settle

        for (byte r = 0; r < _numRows; r++) {
            if (digitalRead(_rows[r]) == HIGH) {
                digitalWrite(_cols[c], LOW);   // restore before returning
                return _keymap[r][c];
            }
        }

        digitalWrite(_cols[c], LOW);
    }
    return 0; 
}

/**
 * @brief Retrieves the current key with debouncing and hold detection.
 *
 * Scans the keypad for a key press, applies debouncing to filter noise, and detects hold events
 * based on the configured hold time. Notifies the event listener if registered.
 *
 * @param None
 * @return char The character of the currently pressed key, or 0 if no key is pressed.
 * @note Uses Arduino `millis` function for timing, and relies on member variables `_lastKey`,
 *       `_lastChange`, `_pressStart`, `_holding`, `_keyState`, `_debounceTime`, `_holdTime`,
 *       and `_eventListener`.
 */
char CustomKeypad::getKey()
{
    char key = scanKeypad();
    unsigned long now = millis();

    if (key != _lastKey) {
        if (now - _lastChange > _debounceTime) {
            _lastChange = now;
            _pressStart = now;
            _holding = false;
            _keyState = (key ? KEY_PRESSED : KEY_RELEASED);
            if (_eventListener) _eventListener(key);
        }
    }
    else if (key && !_holding && (now - _pressStart >= _holdTime)) {
        _keyState = KEY_HOLD;
        _holding = true;
        if (_eventListener) _eventListener(key);
    }

    _lastKey = key;
    return key;
}

/**
 * @brief Scans the keypad for multiple key presses and stores them in a buffer.
 *
 * Activates each column in sequence, checks for HIGH signals on row pins, and stores the
 * corresponding characters in the provided buffer up to the specified maximum.
 *
 * @param keysBuffer Buffer to store the characters of pressed keys.
 * @param maxKeys Maximum number of keys to store in the buffer.
 * @return byte The number of keys detected and stored in the buffer.
 * @note Uses Arduino `digitalWrite`, `digitalRead`, and `delayMicroseconds` functions.
 */
byte CustomKeypad::getKeys(char *keysBuffer, byte maxKeys)
{
    byte count = 0;

    for (byte c = 0; c < _numCols; c++) {
        digitalWrite(_cols[c], HIGH);
        delayMicroseconds(10);

        for (byte r = 0; r < _numRows; r++) {
            if (digitalRead(_rows[r]) == HIGH) {
                if (count < maxKeys) {
                    keysBuffer[count++] = _keymap[r][c];
                }
            }
        }

        digitalWrite(_cols[c], LOW);
    }

    return count;
}

/**
 * @brief Transitions the keypad to a new state and notifies the event listener.
 *
 * Changes the keypad state if different from the current state and calls the event listener
 * with the last pressed key, if registered.
 *
 * @param newState The new state to transition to (e.g., KEY_PRESSED, KEY_RELEASED, KEY_HOLD).
 * @return None
 * @note Relies on member variables `_keyState`, `_eventListener`, and `_lastKey`.
 */
void CustomKeypad::transitionTo(char newState)
{
    if (_keyState != newState)
    {
        _keyState = newState;
        if (_eventListener && _lastKey)
        {
            _eventListener(_lastKey);
        }
    }
}

/**
 * @brief Sets the debounce time for key presses.
 *
 * Configures the time interval used to filter out noise from key presses.
 *
 * @param debounceTime The debounce time in milliseconds.
 * @return None
 * @note Updates the member variable `_debounceTime`.
 */
void CustomKeypad::setDebounceTime(unsigned int debounceTime)
{
    _debounceTime = debounceTime;
}

/**
 * @brief Sets the hold time for detecting a key hold event.
 *
 * Configures the time interval required for a key press to be considered a hold event.
 *
 * @param holdTime The hold time in milliseconds.
 * @return None
 * @note Updates the member variable `_holdTime`.
 */
void CustomKeypad::setHoldTime(unsigned int holdTime)
{
    _holdTime = holdTime;
}

/**
 * @brief Registers an event listener for keypad events.
 *
 * Sets the callback function to be invoked when keypad events occur (e.g., key press, release, or hold).
 *
 * @param listener The callback function to handle keypad events.
 * @return None
 * @note Updates the member variable `_eventListener`.
 */
void CustomKeypad::addEventListener(KeypadEventListener listener)
{
    _eventListener = listener;
}

/**
 * @brief Retrieves the current state of the keypad.
 *
 * Returns the current state of the keypad, such as pressed, released, or held.
 *
 * @param None
 * @return char The current keypad state (e.g., KEY_PRESSED, KEY_RELEASED, KEY_HOLD).
 * @note Returns the member variable `_keyState`.
 */
char CustomKeypad::getKeyState()
{
    return _keyState;
}

/**
 * @brief Checks if a specific key is currently pressed.
 *
 * Compares the provided key with the last detected key to determine if it is pressed.
 *
 * @param key The key to check.
 * @return bool True if the specified key is pressed, false otherwise.
 * @note Relies on the member variable `_lastKey`.
 */
bool CustomKeypad::isPressed(char key)
{
    return (_lastKey == key);
}