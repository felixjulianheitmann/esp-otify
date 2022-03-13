#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <modules/configWebServer.h>
#include <modules/led.h>

#include "../board_config.h"

#include <EEPROM.h>

bool resetDesired(){
    delay(1000);
    pinMode(Board::EEPROM::ResetPin, INPUT);
    if(getPin(Board::EEPROM::ResetPin, Board::EEPROM::ResetPinActiveHigh)){
        delay(Board::EEPROM::ResetTimeMs);
        if(getPin(Board::EEPROM::ResetPin, Board::EEPROM::ResetPinActiveHigh)){
            return true;
        }
    }
    return false;
}

void setup() {
    Serial.begin(Board::Serial::BaudRate);
    delay(100);
    EEPROM.begin(Board::EEPROM::Size);
    if(resetDesired()){
        Serial.println("EEPROM reset desired: true");
        for(int i = 0; i < Board::EEPROM::Size; ++i) {
            EEPROM.write(i, 0);
        }
        EEPROM.commit();
        delay(100);
        Serial.println("EEPROM Erase complete");
    }

    Serial.println("\nStarting...");
    if(Setup::Wifi::StartWithWebConfig && !ConfigServer::Configured(Board::EEPROM::WifiConfiguredReg)) {
        auto server = ConfigServer(
        Setup::ConfigServer::Port,
        Setup::ConfigServer::LocalIp, Setup::ConfigServer::Hostname, Setup::ConfigServer::Ssid, Setup::ConfigServer::Password);
        server.serve(Board::EEPROM::WifiConfiguredReg, Board::EEPROM::WifiSsidReg, Board::EEPROM::WifiPasswordReg, Setup::Wifi::SsidBufferSize, Setup::Wifi::PasswordBufferSize);
    } else {
        for(auto status = WiFi.begin(Setup::Wifi::Ssid, Setup::Wifi::Password); status != WL_CONNECTED; status = WiFi.begin(Setup::Wifi::Ssid, Setup::Wifi::Password))
        {
            Serial.println("Status: " + String(status));
            WiFi.printDiag(Serial);
            delay(1000);
        }
    }

    // auto [ssid, pw] = ConfigServer::GetConfiguration(Board::EEPROM::WifiSsidReg, Board::EEPROM::WifiPasswordReg);


}

void loop() { }
