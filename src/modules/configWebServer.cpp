/**
 * @file configWebServer.cpp
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 *
 */

#include "configWebServer.h"

#include <EEPROM.h>

#include <functional>
#include <string>

#include "../debugging.h"
#include "resources/webserver/index.h"

bool ConfigServer::Configured(int configuredReg) {
    auto eeprom = EEPROM.getConstDataPtr();
    delay(100);
    dbgln<Dbg::Srv>("Configured Register: " + String(eeprom[configuredReg]));
    return eeprom[configuredReg];
}

ConfigServer::SsidPw_t ConfigServer::GetConfiguration(int ssidReg, int pwReg) {
    auto eeprom = EEPROM.getConstDataPtr();
    std::string ssid;
    std::string pw;

    auto ssidIt = &eeprom[ssidReg];
    auto pwIt   = &eeprom[pwReg];
    while (ssidIt != 0x00) {
        ssid.push_back(*ssidIt);
        ++ssidIt;
    }
    while (pwIt != 0x00) {
        pw.push_back(*pwIt);
        ++pwIt;
    }
    return {String(ssid.c_str()), String(pw.c_str())};
}

ConfigServer::ConfigServer(uint16_t const port,
                           std::array<uint8_t, 4> const &localIp,
                           String const &hostname, String const &ssid,
                           String const &psk)
    : _server(port) {
    WiFi.softAPConfig({localIp[0], localIp[1], localIp[2], localIp[3]},
                      {0, 0, 0, 0}, {0, 0, 0, 0});
    WiFi.setHostname(hostname.c_str());
    WiFi.softAP(ssid, psk, 1, 0, 1);  // limit to 1 connection - otherwise I
                                      // would have to encrypt html posts
    WiFi.begin();
    dbgln<Dbg::Srv>("Starting Wifi Server on: " + ssid + " (" + psk + ")");
}

void ConfigServer::serve(int configuredReg, int ssidReg, int pskReg,
                         int ssidMaxLength, int pwMaxLength) {
    _server.on("/", std::bind(&ConfigServer::sendIndex, this));
    _server.on("/network", HTTP_POST,
               std::bind(&ConfigServer::onSubmit, this, configuredReg, ssidReg,
                         pskReg, ssidMaxLength, pwMaxLength));
    _server.begin();
    _wantServe = true;
    while (_wantServe) {
        _server.handleClient();
    }
}

// Serves the index html
void ConfigServer::sendIndex() {
    dbgln<Dbg::Srv>("Received site request");
    _server.send(200, "text/html", index_html);
}

// Processes the network selection
void ConfigServer::onSubmit(int configuredReg, int ssidReg, int pskReg,
                            int ssidMaxLength, int pwMaxLength) {
    auto ssid = _server.arg("ssid");
    auto pw   = _server.arg("psk");
    dbgln<Dbg::Srv>("Received submit request for SSID: " + ssid + " (" + pw +
                    ")");
    if (ssid.isEmpty()) {
        dbgln<Dbg::Srv>("Empty SSID. Returning to home page.");
        sendIndex();
        return;
    } else if (ssid.length() >= (size_t)ssidMaxLength) {
        dbgln<Dbg::Srv>("SSID too long. Cannot be stored in EEPROM");
        sendIndex();
        return;
    } else if (pw.length() >= (size_t)pwMaxLength) {
        dbgln<Dbg::Srv>("Password too long. Cannot be stored in EEPROM");
        sendIndex();
        return;
    }

    dbgln<Dbg::Srv>("Trying to connect to external WiFi ...");
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);
    dbgln<Dbg::Srv>("Wifi mode: " + String(WiFi.getMode()));
    for (wl_status_t status = WiFi.begin(ssid, pw); status != WL_CONNECTED;
         status             = WiFi.begin(ssid, pw)) {
        dbgln<Dbg::Srv>("Connection failed to SSID: '" + ssid + "' (" + pw +
                        "). Error: " + String(status) + " Retrying ...");
        dbgln<Dbg::Srv>("Wifi mode: " + String(WiFi.getMode()));
        delay(1000);
    }

    dbgln<Dbg::Srv>("Connection established!");

    auto eeprom           = EEPROM.getDataPtr();
    eeprom[configuredReg] = 1;
    for (size_t i = 0; i < ssid.length(); ++i) {
        eeprom[ssidReg + i] = ssid[i];
    }
    eeprom[ssidReg + ssid.length()] = 0x00;
    for (size_t i = 0; i < pw.length(); ++i) {
        eeprom[pskReg + i] = pw[i];
    }
    eeprom[pskReg + pw.length()] = 0x00;
    if (!EEPROM.commit()) {
        dbgln<Dbg::Srv>("Writing network configuration to flash failed!");
        sendIndex();
        return;
    }

    dbgln<Dbg::Srv>("Connection written to EEPROM. Restarting ...");

    ESP.restart();
}