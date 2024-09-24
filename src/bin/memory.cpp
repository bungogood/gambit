#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>

#include "indicator.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);
Preferences preferences;
const bool VERBOSE = true;

struct Config {
    String name;
    String ssid;
    String password;
    int chess_engine_moves_ahead;
};

Config config;

Config initConfig() {
    Config config;
    config.name = "Gambit";
    config.ssid = "06906f58-19b5-4be9-bdf4-48e5e26b225e";
    config.password = "12345678";
    config.chess_engine_moves_ahead = 3;
    return config;
}

void writeConfig(Config config) {
    indicator.set(Color::BLUE);
    preferences.begin(config.name.c_str(), false);
    JsonDocument doc;
    doc["name"] = config.name;
    doc["ssid"] = config.ssid;
    doc["password"] = config.password;
    doc["chess-engine-moves-ahead"] = config.chess_engine_moves_ahead;

    String json;
    serializeJson(doc, json);

    preferences.putString("config", json);
    preferences.end();
    indicator.set(Color::GREEN);
}

void readConfig(String name) {
    preferences.begin(name.c_str(), true);
    String configString = preferences.getString("config", "{}");
    preferences.end();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, configString);
    if (error) {
        Serial.println("Failed to read preferences");
        indicator.set(Color::RED);
    } else {
        Serial.println("Preferences read");
        indicator.set(Color::GREEN);
    }

    config.name = doc["name"].as<String>();
    config.ssid = doc["ssid"].as<String>();
    config.password = doc["password"].as<String>();
    config.chess_engine_moves_ahead = doc["chess-engine-moves-ahead"].as<int>();

    if (VERBOSE) {
        Serial.println("Config:");
        Serial.println("Name: " + config.name);
        Serial.println("SSID: " + config.ssid);
        Serial.println("Password: " + config.password);
        Serial.println(config.chess_engine_moves_ahead);
    }
}

void setup() {
    Serial.begin(115200);
    indicator.init();
    indicator.set(Color::BLUE);

    config = initConfig();
    delay(5000);
    writeConfig(config);
    config.name = "blank";
    config.ssid = "blank";
    w config.password = "blank";
    config.chess_engine_moves_ahead = 0;
    delay(5000);
    readConfig("Gambit");
}

void loop() {}