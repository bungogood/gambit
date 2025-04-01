# Gambit
[![Build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

Gambit is an open-source robotic chessboard that moves pieces using magnets and detects their positions with reed switches. It uses a T-Bot mechanism with an electromagnet for precise piece movement and a reed switch matrix for accurate position detection. The board is powered by an Arduino Nano ESP32, which allows for future enhancements like Bluetooth and Wi-Fi connectivity.

![Gambit](https://github.com/bungogood/gambit-board/assets/49326541/d9cc9700-85d9-4ce3-9050-ed555c96e772)

## Features

- **Simple Hardware**: Arduino Nano ESP32 with simple components available on Amazon
- **Easy Movement**: T-Bot mechanism with off-the-shelf parts
- **Built-in Chess Engine**: Runs a Nibble Chess derived ches engine directly on the ESP32
- **Chess Notation Support**: FEN, LAN support for compatibility (Working on PGN)
- **Modern Development**: PlatformIO for better code organization and testing
- **Wireless Control**: Bluetooth connectivity for app integration

## Goals

- **Affordable**: Uses widely available, low-cost components
- **Compact**: Competition-sized board (approximately 60cm x 60cm)
- **Open Source**: All designs and code freely available to modify and improve
- **Reproducible**: Simple enough for hobbyists to build
- **Standalone**: No external computer required to play

## Documentation

For build instructions and demonstrations, visit our [Hackaday project page](https://hackaday.io/project/192559-gambit).

## References
- [Hackaday Project](https://hackaday.io/project/192559-gambit)
- [Nibble Chess](https://github.com/maksimKorzh/nibble-chess) by Maksim Korzh
- [Micro-Max](https://www.chessprogramming.org/Micro-Max) by Harm Geert Muller
