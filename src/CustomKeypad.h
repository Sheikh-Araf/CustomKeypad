#pragma once
#include <Arduino.h>


#define KEY_RELEASED    0
#define KEY_PRESSED     1
#define KEY_HOLD        2

typedef char KeypadEvent;
typedef void (*KeypadEventListener)(KeypadEvent);

class CustomKeypad {
    public:
        CustomKeypad(char **keymap, byte *rows, byte *cols, byte numRows, byte numCols);

        void begin();
        char getKey();
        byte getKeys(char *keysBuffer, byte maxKeys);
        char getKeyState();
        bool isPressed(char key);
        void setDebounceTime(unsigned int debounceTime);
        void setHoldTime(unsigned int holdTime);
        void addEventListener(KeypadEventListener listener);

    private:
        byte *_rows;
        byte *_cols;
        byte _numRows;
        byte _numCols;
        char **_keymap;
        
        unsigned int _debounceTime = 50;
        unsigned int _holdTime = 1000;

        unsigned long _lastChange = 0;
        unsigned long _pressStart = 0;
        char _lastKey = 0;
        char _keyState = KEY_RELEASED;
        bool _holding = false;

        KeypadEventListener _eventListener = nullptr;

        char scanKeypad();
        void transitionTo(char newState);
};