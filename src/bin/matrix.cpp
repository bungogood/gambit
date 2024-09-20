#include <Arduino.h>

#include "bluetooth.hpp"
#include "indicator.hpp"
#include "reed.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);
BluetoothManager ble;
ReedMatrix reed(A3, A5, A4, A1, A0, A2);

void setup() {
    Serial.begin(9600);
    indicator.init();
    indicator.set(Color::BLUE);
    reed.init();
    ble.begin("Gambit");
}

void loop() {
    uint64_t board = reed.read();
    ble.setReedSwitchValue(board);
    delay(200);
}
