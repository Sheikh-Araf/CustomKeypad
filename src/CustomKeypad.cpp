#include "CustomKeypad.h"

CustomKeypad::CustomKeypad(char **userKeymap, byte *rowPins, byte *colPins, byte numRows, byte numCols)
{
    _keymap  = userKeymap;
    _rows    = rowPins;
    _cols    = colPins;
    _numRows = numRows;
    _numCols = numCols;
}

void CustomKeypad::begin()
{
    for (byte c = 0; c<_numCols; c++)
    {
        pinMode(_cols[c], OUTPUT);
        digitalWrite(_cols[c], HIGH);
    }
    for (byte r = 0; r<_numRows; r++)
    {
        pinMode(_rows[r], INPUT);
    }
}



char CustomKeypad::scanKeypad()
{
    for(byte r = 0; r < _numRows; r++)
    {
        if(digitalRead(_rows[r]) == HIGH)
        {
            for(byte c = 0; c < _numCols; c++)
            {
                return _keymap[r][c];
            }
        }
    }
    return 0;
}

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

byte CustomKeypad::getKeys(char *keysBuffer, byte maxKeys) {

    byte count = 0;
    for (byte r = 0; r < _numRows; r++) {
        if (digitalRead(_rows[r]) == HIGH) { 
            for (byte c = 0; c < _numCols; c++) 
            {
                if (count < maxKeys) {keysBuffer[count++] = _keymap[r][c];}
            }
        }
    }

    return count;
}

void CustomKeypad::transitionTo(char newState)
{
    if(_keyState != newState)
    {
        _keyState = newState;
        if(_eventListener && _lastKey)
        {
            _eventListener(_lastKey);
        }
    }
}

void CustomKeypad::setDebounceTime(unsigned int debounceTime)       {_debounceTime = debounceTime;}
void CustomKeypad::setHoldTime(unsigned int holdTime)               {_holdTime = holdTime;}
void CustomKeypad::addEventListener(KeypadEventListener listener)   {_eventListener = listener;}
char CustomKeypad::getKeyState()                                    {return _keyState;}
bool CustomKeypad::isPressed(char key)                              {return (_lastKey == key);}