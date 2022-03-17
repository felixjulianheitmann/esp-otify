/**
 * @file configWebServer.cpp
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 *
 */

#include "configWebServer.h"

#include <EEPROM.h>
#include <modules/led.h>

#include <functional>
#include <string>

#include "../debugging.h"
#include "resources/webserver/index.h"
#include "resources/webserver/submit.h"

bool ConfigServer::Configured(int configuredReg) {
    auto eeprom = EEPROM.getConstDataPtr();
    delay(100);
    dbgln<Dbg::Eeprom>("Configured Register: " + String(eeprom[configuredReg]));
    return eeprom[configuredReg];
}

ConfigServer::SsidPw_t ConfigServer::GetConfiguration(
    Board::EEPROM::Addresses_t const& addr) {
    auto eeprom = EEPROM.getConstDataPtr();
    std::string ssid;
    std::string pw;

    auto ssidLength = eeprom[addr.WifiSsidLengthReg];
    auto pwLength   = eeprom[addr.WifiPwLengthReg];
    auto ssidIt     = &eeprom[addr.WifiSsidReg];
    auto pwIt       = &eeprom[addr.WifiPasswordReg];

    dbgln<Dbg::Eeprom>("Ssidreg at (" + String(addr.WifiSsidReg) + "): " +
                       String((char)*ssidIt) + " - L: " + String(ssidLength));
    dbgln<Dbg::Eeprom>("Pwreg at (" + String(addr.WifiPasswordReg) + "): " +
                       String((char)*pwIt) + " - L: " + String(pwLength));

    std::copy(ssidIt, ssidIt + ssidLength, std::back_inserter(ssid));
    std::copy(pwIt, pwIt + pwLength, std::back_inserter(pw));

    return {String(ssid.c_str()), String(pw.c_str())};
}

ConfigServer::ConfigServer(uint16_t const port,
                           std::array<uint8_t, 4> const& localIp,
                           String const& ssid, String const& psk)
    : _server(port) {
    WiFi.softAPConfig({localIp[0], localIp[1], localIp[2], localIp[3]},
                      {0, 0, 0, 0}, {0, 0, 0, 0});
    WiFi.softAP(ssid, psk, 1, 0, 1);  // limit to 1 connection - otherwise I
                                      // would have to encrypt html posts
    WiFi.begin();
    dbgln<Dbg::Srv>("Starting Wifi Server on: " + ssid + " (" + psk + ")");
}

void ConfigServer::serve(Board::EEPROM::Addresses_t const& addr,
                         int ssidMaxLength, int pwMaxLength) {
    _server.on("/", std::bind(&ConfigServer::sendIndex, this));
    _server.on("/network", HTTP_POST,
               std::bind(&ConfigServer::onSubmit, this, addr, ssidMaxLength,
                         pwMaxLength));
    _server.begin();
    while (true) {  // Handle until reboot
        _server.handleClient();
    }
}

// Serves the index html
void ConfigServer::sendIndex() {
    dbgln<Dbg::Srv>("Received site request");
    _server.send(200, "text/html", index_html);
}

// Processes the network selection
void ConfigServer::onSubmit(Board::EEPROM::Addresses_t const& addr,
                            int ssidMaxLength, int pwMaxLength) {
    _server.send(200, "text/html", submit_html);

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
    Connect(ssid, pw);

    dbgln<Dbg::Srv>("Connection established!");

    auto eeprom                    = EEPROM.getDataPtr();
    eeprom[addr.WifiConfiguredReg] = 1;
    eeprom[addr.WifiSsidLengthReg] = ssid.length();
    eeprom[addr.WifiPwLengthReg]   = pw.length();

    std::copy(ssid.begin(), ssid.end(), &eeprom[addr.WifiSsidReg]);
    eeprom[addr.WifiSsidReg + ssid.length()] = 0x00;

    std::copy(pw.begin(), pw.end(), &eeprom[addr.WifiPasswordReg]);
    eeprom[addr.WifiPasswordReg + pw.length()] = 0x00;

    if (!EEPROM.commit()) {
        dbgln<Dbg::Srv>("Writing network configuration to flash failed!");
        sendIndex();
        return;
    }

    dbgln<Dbg::Srv>("Connection written to EEPROM. Restarting ...");

    ESP.restart();
}