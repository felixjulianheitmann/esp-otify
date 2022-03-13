#include <Arduino.h>
#include <modules/led.h>

#include "../board_config.h"

auto led = Led<board::Led_Pin, board::Led_Active_High>();

void setup() {}

void loop() { led.blink(4, 1000); }