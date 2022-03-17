/**
 * @file configWebServer.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief Serves a web server with different configuration options
 * @version 0.1
 * @date 2022-03-13
 *
 */

#pragma once

#include <ESP8266WebServer.h>
#include <modules/led.h>

#include <array>

#include "../debugging.h"
#include "../types.h"

class ConfigServer {
   public:
    struct SsidPw_t {
        String ssid;
        String pw;
    };

    static SsidPw_t GetConfiguration(Board::EEPROM::Addresses_t const &addr);

    static bool Configured(int configuredReg);

    template <int LED = -1, bool ActiveHigh = true>
    static void Connect(String const &ssid, String const &pw) {
        dbgln<Dbg::Srv>("Connecting to '" + ssid + "' (" + pw + ")");
        WiFi.mode(WIFI_STA);
        for (auto status = WiFi.begin(ssid, pw); status != WL_CONNECTED;
             status      = WiFi.status()) {
            if constexpr (LED >= 0) {
                Led<LED, ActiveHigh>::toggle();
            }
            Serial.print(".");
            delay(1000);
        }
        if constexpr (LED >= 0) {
            Led<LED, ActiveHigh>::on();
        }
    }

    ConfigServer(uint16_t const port, std::array<uint8_t, 4> const &localIp,
                 String const &ssid, String const &psk);

    void serve(Board::EEPROM::Addresses_t const &addr, int ssidMaxLength,
               int pwMaxLength);

   private:
    void sendIndex();
    void onSubmit(Board::EEPROM::Addresses_t const &addr, int ssidMaxLength,
                  int pwMaxLength);

    ESP8266WebServer _server;
};
