# Pinout

## Arduino Nano ESP32

```
             +-----+
 +-----------| USB |-----------+
 |           +-----+           |
 | [ ]D13/SCK      MISO/D12[ ] |
 | [ ]3.3V         MOSI/D11[ ]~|
 | [ ]V.ref          SS/D10[ ]~|
 | [I]A0                 D9[M]~|
 | [I]A1                 D8[H] |
 | [I]A2                 D7[R] |
 | [O]A3                 D6[R]~|
 | [O]A4                 D5[R]~|
 | [O]A5/SDA             D4[L] |
 | [ ]A6/SCL        INT1/D3[L]~|
 | [ ]A7            INT0/D2[L] |
 | [ ]5V                GND[ ] |
 | [ ]RST               RST[ ] |
 | [ ]GND  5V MOSI GND  TX1[ ] |
 | [ ]Vin  [ ] [ ] [ ]  RX1[ ] |
 |         [ ] [ ] [ ]         |
 |        MISO SCK RST         |
 +-NANO-ESP32------------------+
```

### Left Motor (L)

-   D2 - enable pin
-   D3 - direction pin
-   D4 - step pin

### Right Motor (R)

-   D5 - enable pin
-   D6 - direction pin
-   D7 - step pin

### HBot (H)

-   D8 - calibration switch pin

### Magnet (M)

-   D9 - up magnet pin

### Detection Matrix Shift In (I) / Shift Out (O)

-   A0 - shift in data
-   A1 - shift in clock
-   A2 - shift in latch
-   A3 - shift out clock
-   A4 - shift out latch
-   A5 - shift out data
