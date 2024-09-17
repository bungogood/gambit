#include <Arduino.h>
#include "indicator.hpp"
#include "bluetooth.hpp"

Indicator indicator(LED_RED, LED_GREEN, LED_BLUE);


//   a  b  c  d  e  f  g   h
// 8 .  .  .  .  .  .  .   .  8 D8
                       
// 7 .  .  .  .  .  .  .   .  7 D7
                       
// 6 .  .  .  .  .  .  .   .  6 D6
                       
// 5 .  .  .  .  .  .  .   .  5 D5
                       
// 4 .  .  .  .  .  .  .   .  4 D4
                       
// 3 .  .  .  .  .  .  .   .  3 D3
                       
// 2 .  .  .  .  .  .  .   .  2 D2
                       
// 1 .  .  .  .  .  .  .   .  1 D1 (TX1)
//  15  1  2  3  4  5  6   7 (595)

const int ONE_EIGHT_PINS[8] = {D1, D2, D3, D4, D5, D6, D7, D8};
byte A_H_ACTIVATION_NUMS[8];
const bool VERBOSE = true;
const bool SHIFT_IN = true;
int boardState[8][8];
BluetoothManager ble;


//74HC595 pins
const int shiftOutDataPin = A0;
const int shiftOutClockPin = A1;
const int shiftOutLatchPin = A2;



//74HC165 pins
const int shiftInClockEnablePin = D9;
const int shiftInDataPin = D10;
const int shiftInClockPin = D11;
const int shiftInLoadPin = D12;


//shift in consts
const int PULSE_WIDTH_USEC = 5;


byte shiftOutData;
byte inputData;



void setup() {
    Serial.begin(9600);

    ble.begin("Gambit");

    indicator.init();
    indicator.set(Color::BLUE);

    // Shift out set up

    for (int i = 0; i < 8; i++) {
        A_H_ACTIVATION_NUMS[i] = 1 << i;
    }

    pinMode(shiftOutLatchPin, OUTPUT);
    pinMode(shiftOutClockPin, OUTPUT);
    pinMode(shiftOutDataPin, OUTPUT);

    // Shift in set up
    if (SHIFT_IN) {
        pinMode(shiftInLoadPin, OUTPUT);
        pinMode(shiftInClockPin, OUTPUT);
        pinMode(shiftInDataPin, INPUT);
        pinMode(shiftInClockEnablePin, OUTPUT);
    } else{
        for (int i = 0; i < 8; i++) {
            pinMode(ONE_EIGHT_PINS[i], INPUT_PULLDOWN);
        }
    }

}

byte shiftIn(int dataPin, int clockPin, int bitOrder){
    byte value = 0;
    long bitValue;
    for (int i =0; i < 8; i++){
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(10);
        bitValue = digitalRead(dataPin);

        if (bitOrder == LSBFIRST){
            value |= (bitValue << i);
        } else {
            value |= (bitValue << (7 - i));
        }
        digitalWrite(clockPin, LOW);
        delayMicroseconds(10);
    }
    return value;
}

void shiftOut(int dataPin, int clockPin, byte dataOut){
    int i=0;
    int pinState;
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    for (i = 7; i >= 0; i--)  {
        digitalWrite(clockPin, LOW);
        if ( dataOut & (1 << i) ) {
            pinState = HIGH;
        }
        else {
            pinState = LOW;
        }
        digitalWrite(dataPin, pinState);
        digitalWrite(clockPin, HIGH);
        digitalWrite(dataPin, LOW);
    }
    digitalWrite(clockPin, LOW);
}

void loop(){
    for (int col = 0; col < 8; col++) {
        shiftOutData = A_H_ACTIVATION_NUMS[col];
        digitalWrite(shiftOutLatchPin, LOW);
        shiftOut(shiftOutDataPin, shiftOutClockPin, shiftOutData);
        digitalWrite(shiftOutLatchPin, HIGH);
        delayMicroseconds(5);


        if (SHIFT_IN){
            // load shiftin register
            digitalWrite(shiftInLoadPin, LOW);
            delayMicroseconds(50);
            digitalWrite(shiftInLoadPin, HIGH);


            inputData = shiftIn(shiftInDataPin, shiftInClockPin, MSBFIRST);


            for (int row = 0; row < 8; row++) {
                boardState[row][col] = (inputData >> row) & 1;
            }
        }
        else{
            for (int row = 0; row < 8; row++) {
                boardState[row][col] = digitalRead(ONE_EIGHT_PINS[row]);
            }
        }
        delay(5);
    }

    // write board low
    shiftOutData = 0;
    digitalWrite(shiftOutLatchPin, LOW);
    shiftOut(shiftOutDataPin, shiftOutClockPin, shiftOutData);
    digitalWrite(shiftOutLatchPin, HIGH);
    delayMicroseconds(5);

    // print the board state from 8 to 1
    if (VERBOSE) {
        Serial.println("Board State:");
        Serial.println("  a b c d e f g h");
        for (int row = 7; row >= 0; row--) {
            Serial.print(row);
            for (int col = 0; col < 8; col++) {
                Serial.print(" ");
                Serial.print(boardState[row][col]);
            }
            Serial.println();
        }
        Serial.println("  a b c d e f g h");
    }

    delay(500);
    // uint64_t boardStateU64 = 0;
    // for (int row = 0; row < 8; row++) {
    //     for (int col = 0; col < 8; col++) {
    //         boardStateU64 |= (uint64_t)boardState[row][col] << (row * 8 + col);
    //     }
    // }
    // ble.setReedSwitchValue(boardStateU64);
}