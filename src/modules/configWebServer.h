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

#include <array>

class ConfigServer {
   public:
    struct SsidPw_t {
        String ssid;
        String pw;
    };

    static SsidPw_t GetConfiguration(int ssidReg, int pwReg);

    static bool Configured(int configuredReg);

    ConfigServer(uint16_t const port, std::array<uint8_t, 4> const &localIp,
                 String const &hostname, String const &ssid, String const &psk);

    void serve(int configuredReg, int ssidReg, int pskReg, int ssidMaxLength,
               int pwMaxLength);

   private:
    void sendIndex();
    void onSubmit(int configuredReg, int ssidReg, int pskReg, int ssidMaxLength,
                  int pwMaxLength);

    ESP8266WebServer _server;
    volatile bool _wantServe;
};
