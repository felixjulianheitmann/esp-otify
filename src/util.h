/**
 * @file util.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief Several small helper functions
 * @version 0.1
 * @date 2022-03-13
 */

#pragma once

#include <Arduino.h>

inline bool getPin(int pin, bool activeHigh = true) {
    if (activeHigh) {
        return digitalRead(pin);
    } else {
        return !digitalRead(pin);
    }
}

inline void setPin(int pin, bool val, bool activeHigh = true) {
    pinMode(pin, OUTPUT);
    if (activeHigh) {
        digitalWrite(pin, val);
    } else {
        digitalWrite(pin, !val);
    }
}