/**
 * @file led.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief Provides basic led functionality
 * @version 0.1
 * @date 2022-03-13
 *
 */

#pragma once

#include <util.h>

template <int Gpio, bool ActiveHigh>
class Led {
   public:
    static auto on() { setPin(Gpio, HIGH, ActiveHigh); }

    static auto off() { setPin(Gpio, LOW, ActiveHigh); }

    static auto toggle() { setPin(Gpio, !getPin(Gpio)); }

    /**
     * @brief the LED will toggle `times` times with
     * a delay time of period / 2 in ms in between.
     * @param times times to toggle led - negative values are interpreted as
     * positive
     * @param period period in ms of full on-off-cycle
     */
    static auto blink(int times, unsigned long period) {
        times = abs(times);
        for (int i = 0; i < times; i++) {
            toggle();
            delay(period / 2);
        }
    }
};