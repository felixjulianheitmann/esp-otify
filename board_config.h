/**
 * @file board_config.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief This file contains basic configuration constants for the board
 * @version 0.1
 * @date 2022-03-13
 */

#pragma once

#include "setup.h"
#include "types.h"
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

constexpr Addresses_t Addresses{
    .WifiConfiguredReg = 0x0000,
    .WifiSsidLengthReg = 0x0004,
    .WifiPwLengthReg   = 0x0006,
    .WifiSsidReg       = 0x0008,
    .WifiPasswordReg   = 0x0008 + Setup::Wifi::SsidBufferSize,
};

constexpr ptrdiff_t Size =
    8 + Setup::Wifi::SsidBufferSize + Setup::Wifi::SsidBufferSize;

}  // namespace EEPROM

namespace Serial {

constexpr int BaudRate = 115200;

}  // namespace Serial

}  // namespace Board
