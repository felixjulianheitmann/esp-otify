/**
 * @file board_config.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief This file contains basic configuration constants for the board
 * @version 0.1
 * @date 2022-03-13
 */

#pragma once

#include "setup.h"

namespace Board {

namespace Led {

constexpr int Pin         = 2;
constexpr bool ActiveHigh = false;

}  // namespace Led

namespace EEPROM {

// The reset pin must be held for at least that amount of time during boot to
// clear the EEPROM and enable new network configuration
constexpr int ResetPin              = 2;
constexpr unsigned long ResetTimeMs = 3000;
constexpr bool ResetPinActiveHigh   = false;

constexpr ptrdiff_t WifiConfiguredReg = 0x00000000;
constexpr ptrdiff_t WifiSsidReg       = WifiConfiguredReg + 8;
constexpr ptrdiff_t WifiPasswordReg = WifiSsidReg + Setup::Wifi::SsidBufferSize;

constexpr ptrdiff_t Size =
    8 + Setup::Wifi::SsidBufferSize + Setup::Wifi::SsidBufferSize;

}  // namespace EEPROM

namespace Serial {

constexpr int BaudRate = 115200;

}  // namespace Serial

}  // namespace Board
