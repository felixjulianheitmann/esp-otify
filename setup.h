/**
 * @file setup.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief This file contains configuration options for the software setup
 * @version 0.1
 * @date 2022-03-13
 */

#pragma once

#include <array>
#include <string>
namespace Setup {

namespace Wifi {

// Acitvating this, starts a web server on boot, that allows to configure a
// network connection. These changes are written and persistently saved to the
// EEPROM. The device can be reset by pinning the EEPROM_Reset_Pin to low during
// boot and holding it for at least `EEPROM_reset_time` seconds.
// If set to false, the values of `SSID` and `Password` will be used to connect
// to a local network.
constexpr bool StartWithWebConfig = false;
constexpr int SsidBufferSize      = 64;
constexpr int PasswordBufferSize  = 128;

constexpr char Ssid[]     = "";
constexpr char Password[] = "";

}  // namespace Wifi

namespace ConfigServer {

constexpr uint16_t Port                  = 80;
constexpr std::array<uint8_t, 4> LocalIp = {192, 168, 0, 1};
constexpr char Hostname[]                = "config.local";
constexpr char Ssid[]                    = "ESP-01 Server";
constexpr char Password[]                = "";

}  // namespace ConfigServer

}  // namespace Setup
