#pragma once

#include <Arduino.h>

namespace Board {

namespace EEPROM {

struct Addresses_t {
    ptrdiff_t WifiConfiguredReg;
    ptrdiff_t WifiSsidLengthReg;
    ptrdiff_t WifiPwLengthReg;
    ptrdiff_t WifiSsidReg;
    ptrdiff_t WifiPasswordReg;
};

}  // namespace EEPROM

}  // namespace Board
