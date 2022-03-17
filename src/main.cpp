#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <modules/configWebServer.h>
#include <modules/led.h>

#include "../board_config.h"
#include "../debugging.h"

bool resetDesired();
void select_and_connect_wifi();
void clearEEPROM();

void setup() {
    Serial.begin(Board::Serial::BaudRate);
    EEPROM.begin(Board::EEPROM::Size);
    delay(100);

    if (resetDesired()) {
        clearEEPROM();
    }

    Serial.println("\nStarting...");

    select_and_connect_wifi();

    Serial.println("Setup finished! Entering loop...");
}

void loop() {}

// -------------------------------------------------
// Utility functions
// -------------------------------------------------

bool resetDesired() {
    delay(1000);
    pinMode(Board::EEPROM::ResetPin, INPUT);
    if (getPin(Board::EEPROM::ResetPin, Board::EEPROM::ResetPinActiveHigh)) {
        delay(Board::EEPROM::ResetTimeMs);
        if (getPin(Board::EEPROM::ResetPin,
                   Board::EEPROM::ResetPinActiveHigh)) {
            return true;
        }
    }
    return false;
}

void select_and_connect_wifi() {
    using namespace Setup::ConfigServer;
    using namespace Board;

    if (Setup::Wifi::StartWithWebConfig &&
        !ConfigServer::Configured(EEPROM::Addresses.WifiConfiguredReg)) {
        dbgln<Dbg::Srv>("Selecting WiFi connection through webserver.");

        auto server = ConfigServer(Port, LocalIp, Ssid, Password);
        server.serve(EEPROM::Addresses, Setup::Wifi::SsidBufferSize,
                     Setup::Wifi::PasswordBufferSize);

    } else if (Setup::Wifi::StartWithWebConfig &&
               ConfigServer::Configured(EEPROM::Addresses.WifiConfiguredReg)) {
        dbgln<Dbg::Srv>("Selecting WiFi connection from EEPROM.");
        delay(1000);
        auto [ssid, pw] = ConfigServer::GetConfiguration(EEPROM::Addresses);
        ConfigServer::Connect<Board::Led::Pin, Board::Led::ActiveHigh>(ssid,
                                                                       pw);

    } else {
        dbgln<Dbg::Srv>("Selecting WiFi connection from Setup.h.");

        ConfigServer::Connect<Board::Led::Pin, Board::Led::ActiveHigh>(
            Setup::Wifi::Ssid, Setup::Wifi::Password);
    }
}

void clearEEPROM() {
    Serial.println("EEPROM reset desired!");
    for (int i = 0; i < Board::EEPROM::Size; ++i) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    delay(100);
    Serial.println("EEPROM Erase complete");
}